#include <encoding.h>

#include <sstream>
#include <node.h>
#include <nan.h>
#include <node_buffer.h>

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

namespace XXHash {

	void XXHash(const FunctionCallbackInfo<Value>& args) {
		if (args.Length() == 0) {
			Nan::ThrowError("Argument required");
		}
		auto isolate = args.GetIsolate();
		auto inputData = ConvertInput(args[0]);

		if (inputData.Buffer == NULL) {
			return; // 输入参数有错误
		}

		auto hash = XXH3_128bits(inputData.Buffer, inputData.Length);
		XXH128_canonical_t canonical_sum;
		XXH128_canonicalFromHash(&canonical_sum, hash);

		if (inputData.IsOwned) {
			delete[] inputData.Buffer;
		}

		auto output = (char*)(canonical_sum.digest);
		Local<Value> returnValue;

		if (args.Length() == 1) {
			returnValue = Buffer::New(isolate, output, sizeof(canonical_sum)).ToLocalChecked();
		}
		else if (args[1]->IsString()) {
			returnValue = ConvertOutput(output, sizeof(canonical_sum), args[1].As<String>());
		}
		else {
			Nan::ThrowTypeError("encoding argument must be an string");
			return;
		}

		args.GetReturnValue().Set(returnValue);
	}

	void Initialize(Local<Object> exports) {
		NODE_SET_METHOD(exports, "xxHash3_128", XXHash);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}
