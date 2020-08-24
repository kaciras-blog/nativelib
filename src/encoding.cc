#include <sstream>
#include <encoding.h>

namespace XXHash {

	static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	Local<String> Base64F(const char* src, size_t slen) {
		unsigned dlen = (slen + 2) / 3 * 4;
		auto dst = new char[dlen];

		unsigned a;
		unsigned b;
		unsigned c;

		unsigned i = 0;
		unsigned k = 0;
		unsigned n = slen / 3 * 3;

		while (i < n) {
			a = src[i + 0] & 0xff;
			b = src[i + 1] & 0xff;
			c = src[i + 2] & 0xff;

			dst[k + 0] = table[a >> 2];
			dst[k + 1] = table[((a & 3) << 4) | (b >> 4)];
			dst[k + 2] = table[((b & 0x0f) << 2) | (c >> 6)];
			dst[k + 3] = table[c & 0x3f];

			i += 3;
			k += 4;
		}

		switch (slen - n) {
		case 1:
			a = src[i + 0] & 0xff;
			dst[k + 0] = table[a >> 2];
			dst[k + 1] = table[(a & 3) << 4];
			dst[k + 2] = '=';
			dst[k + 3] = '=';
			break;
		case 2:
			a = src[i + 0] & 0xff;
			b = src[i + 1] & 0xff;
			dst[k + 0] = table[a >> 2];
			dst[k + 1] = table[((a & 3) << 4) | (b >> 4)];
			dst[k + 2] = table[(b & 0x0f) << 2];
			dst[k + 3] = '=';
			break;
		}

		auto result = String::NewFromUtf8(Isolate::GetCurrent(), dst, NewStringType::kNormal, dlen);
		delete[] dst;

		return result.ToLocalChecked();
	}

	InputData ConvertInput(const Local<Value>& input) {
		InputData data;

		if (Buffer::HasInstance(input)) {
			data.Length = Buffer::Length(input);
			data.Buffer = Buffer::Data(input);
			data.IsOwned = false;
		}
		else if (input->IsString()) {
			data.Length = Nan::DecodeBytes(input, Nan::UTF8);
			data.Buffer = new char[data.Length];
			Nan::DecodeWrite(data.Buffer, data.Length, input, Nan::UTF8);
			data.IsOwned = true;
		}
		else {
			data.IsOwned = false;
			data.Length = 0;
			data.Buffer = NULL;
			Nan::ThrowTypeError("data must be string or buffer");
		}

		return data;
	}

	/*
	 * "hex" | "base64" | "base64-u";
	 */
	Local<Value> ConvertOutput(const char* digest, size_t size, const Local<String>& outType) {
		Local<Value> result;
		auto length = outType->Length();

		// 这是所有支持的选项里最长的，比这个更长肯定是错误
		auto type = new char[sizeof("base64-u")];

		if (length > sizeof(type)) {
			type[0] = 0;
		}
		else {
			type[Nan::DecodeWrite(type, length, outType)] = 0;
		}

		if (strcmp(type, "base64-u") == 0) {
			result = Base64F(digest, size);
		}
		else if (strcmp(type, "base64") == 0) {
			result = Nan::Encode(digest, size, Nan::BASE64);
		}
		else if (strcmp(type, "hex") == 0) {
			result = Nan::Encode(digest, size, Nan::HEX);
		}
		else {
			std::stringstream ss;
			ss << "Unknown output encoding: " << type;
			Nan::ThrowError(ss.str().c_str());
		}

		delete[] type;
		return result;
	}
}