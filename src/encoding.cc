#include <encoding.h>

namespace XXHash {

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

	bool InputData::isInvalid() const {
		return Buffer == NULL;
	}

	/*
	 * 解析输入的JS值，返回需要传递给 hash 函数的数据。
	 *
	 * 搞个智能指针玩玩比毕竟以前没用过，其实不用也行。
	 */
	shared_ptr<InputData> ParseInput(const Local<Value> input) {
		char* buffer;
		size_t len;
		bool owned;

		// Buffer::Data 返回底层数据，无复制，所以用完也不能删
		if (Buffer::HasInstance(input)) {
			owned = false;
			len = Buffer::Length(input);
			buffer = Buffer::Data(input);
		}
		else if (input->IsString()) {
			owned = true;
			len = Nan::DecodeBytes(input, Nan::UTF8);
			buffer = new char[len];
			Nan::DecodeWrite(buffer, len, input, Nan::UTF8);
		}
		else {
			owned = false;
			len = 0;
			buffer = NULL;
		}

		return make_shared<InputData>(buffer, len, owned);
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