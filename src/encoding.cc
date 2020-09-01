#include <encoding.h>

namespace XXHash {
	using node::encoding;

	static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	/*
	 * 从 NodeJS 抄的代码
	 * https://github.com/nodejs/node/blob/master/src/base64.h
	 */
	Local<String> UrlSafeBase64(const char* src, size_t slen) {
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

	InputData::InputData(const char* buffer, size_t len, bool owned) {
		Buffer = buffer;
		Length = len;
		IsOwned = owned;
	}

	InputData::~InputData() {
		if (IsOwned) {
			delete[] Buffer;
		}
	}

	Maybe<shared_ptr<InputData>> ParseInput(Isolate* isolate, Local<Value> input, Local<Value> encVal) {
		auto enc = node::ParseEncoding(isolate, encVal, static_cast<encoding>(-1));
		if (enc == -1) {
			Nan::ThrowError("Invalid encoding");
			return v8::Nothing<shared_ptr<InputData>>();
		}
		return ParseInput(isolate, input, enc);
	}

	Maybe<shared_ptr<InputData>> ParseInput(Isolate* isolate, Local<Value> input, encoding enc) {
		shared_ptr<InputData> data;

		// Buffer::Data 返回底层数据，无复制，所以用完也不能删
		if (Buffer::HasInstance(input)) {
			auto buffer = Buffer::Data(input);
			auto len = Buffer::Length(input);
			data = make_shared<InputData>(buffer, len, false);
		}
		else if (input->IsString()) {
			auto len = node::DecodeBytes(isolate, input, enc);
			auto buffer = new char[len];
			node::DecodeWrite(isolate, buffer, len, input, enc);
			data = make_shared<InputData>(buffer, len, true);
		}
		else {
			Nan::ThrowTypeError("data must be string or buffer");
			return v8::Nothing<shared_ptr<InputData>>();
		}

		return v8::Just<shared_ptr<InputData>>(data);
	}

	/*
	 * 将 char* 转换为JS的字符串，支持 "latin1" | "hex" | "base64" | "base64u"
	 *
	 * base64u 是 base64 算法替换了 '/' 和 '+' 的变体。
	 * https://tools.ietf.org/html/rfc4648#section-5
	 */
	Local<Value> EncodeDigest(const char* digest, size_t size, Local<String> outType) {
		Local<Value> result;
		auto length = outType->Length();

		// 这是所有支持的选项里最长的，比这个更长肯定是错误
		auto type = new char[sizeof("base64u")];

		// 注意 sizeof("xxx") 包含了字符串末尾的 \0
		if (length >= sizeof(type)) {
			type[0] = 0;
		}
		else {
			type[Nan::DecodeWrite(type, length, outType)] = 0;
		}

		if (strcmp(type, "base64u") == 0) {
			result = UrlSafeBase64(digest, size);
		}
		else if (strcmp(type, "base64") == 0) {
			result = Nan::Encode(digest, size, Nan::BASE64);
		}
		else if (strcmp(type, "hex") == 0) {
			result = Nan::Encode(digest, size, Nan::HEX);
		}
		else if (strcmp(type, "latin1") == 0) {
			result = Nan::Encode(digest, size, Nan::BINARY);
		}
		else {
			// TODO: 把参数值也带上？
			Nan::ThrowError("Unsupported output encoding");
		}

		delete[] type;
		return result;
	}
}