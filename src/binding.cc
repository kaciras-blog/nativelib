#include <node.h>
#include <nan.h>
#include <node_buffer.h>

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

namespace XXHash {
	using namespace node;

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;
	using v8::MaybeLocal;
	using v8::NewStringType;

	static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	void Base64FWithoutPadding(const char* src, unsigned n, char* dst) {

	}

	MaybeLocal<String> Base64F(Isolate* isolate, const char* src, size_t slen) {
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

		auto result = String::NewFromUtf8(isolate, dst, NewStringType::kNormal, dlen);
		delete[] dst;

		return result;
	}

	void JSBase64F(const FunctionCallbackInfo<Value>& args) {
		auto isolate = args.GetIsolate();
		auto input = args[0];

		if (input->IsString()) {
			auto slen = Nan::DecodeBytes(input, Nan::UTF8);
			auto src = new char[slen];
			Nan::DecodeWrite(src, slen, input, Nan::UTF8);

			XXH128_canonical_t canonical_sum;
			auto hash = XXH3_128bits(src, slen);
			XXH128_canonicalFromHash(&canonical_sum, hash);

			auto st = Nan::Encode(canonical_sum.digest, sizeof(canonical_sum), Nan::HEX);
			//auto st = Base64F(isolate, (char*)canonical_sum.digest, sizeof(canonical_sum));

			args.GetReturnValue().Set(st);
			delete[] src;
		}
		else if (Buffer::HasInstance(input)) {
			auto slen = Buffer::Length(input);
			auto src = Buffer::Data(input);

			args.GetReturnValue().Set(Base64F(isolate, src, slen).ToLocalChecked());
			delete[] src;
		}
		else {
			Nan::ThrowTypeError("data must be string or buffer");
		}
	}

	void Initialize(Local <Object> exports) {
		NODE_SET_METHOD(exports, "base64F", JSBase64F);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}
