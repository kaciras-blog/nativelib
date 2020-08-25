#include <encoding.h>

#include <sstream>
#include <node_api.h>
#include <node.h>
#include <nan.h>
#include <node_buffer.h>

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

namespace XXHash {
	using v8::ObjectTemplate;
	using v8::FunctionTemplate;
	using v8::Context;
	using v8::Function;

	class XXHash3_128Object {

	public:

		static void Init(Local<Object> exports) {
			auto isolate = exports->GetIsolate();
			auto context = isolate->GetCurrentContext();
			auto handle_scope(isolate);

			// 搞个对象来保存构造方法，相当于闭包上下文？
			auto objectTpl = ObjectTemplate::New(isolate);
			objectTpl->SetInternalFieldCount(1);
			auto closure = objectTpl->NewInstance(context).ToLocalChecked();

			// 定义一个类（也是函数），使用空函数体
			auto clazz = FunctionTemplate::New(isolate);
			auto name = String::NewFromUtf8(isolate, "XXH3_128", NewStringType::kInternalized).ToLocalChecked();
			clazz->SetClassName(name);
			clazz->InstanceTemplate()->SetInternalFieldCount(1);

			// 在这个类的原型上定义几个方法
			NODE_SET_PROTOTYPE_METHOD(clazz, "copy", Copy);
			NODE_SET_PROTOTYPE_METHOD(clazz, "update", Update);
			NODE_SET_PROTOTYPE_METHOD(clazz, "digest", Digest);

			// 把类的构造函数加入闭包
			closure->SetInternalField(0, clazz->GetFunction(context).ToLocalChecked());

			// 定义 createXXH3_128() 函数并导出
			auto createHash = FunctionTemplate::New(isolate, New, closure);
			name = String::NewFromUtf8(isolate, "createXXH3_128", NewStringType::kInternalized).ToLocalChecked();
			createHash->SetClassName(name);
			exports->Set(context, name, createHash->GetFunction(context).ToLocalChecked()).Check();

			// 再导出一个快捷 Hash 的函数
			NODE_SET_METHOD(exports, "xxHash3_128", DirectHash);
		}

	private:

		XXH3_state_t* state;

		~XXHash3_128Object() {
			XXH3_freeState(state);
		}

		static void New(const FunctionCallbackInfo<Value>& args) {
			auto context = args.GetIsolate()->GetCurrentContext();
			auto constructor = args.Data().As<Object>()->GetInternalField(0).As<Function>();

			auto data = new XXHash3_128Object();
			data->state = XXH3_createState();
			XXH3_128bits_reset(data->state);

			auto instance = constructor->NewInstance(context).ToLocalChecked();
			instance->SetAlignedPointerInInternalField(0, data);
			args.GetReturnValue().Set(instance);
		}

		static void Copy(const FunctionCallbackInfo<Value>& args) {
			auto context = args.GetIsolate()->GetCurrentContext();

			auto field = args.This()->GetAlignedPointerFromInternalField(0);
			auto data = static_cast<XXHash3_128Object*>(field);

			auto dataCopy = new XXHash3_128Object();
			*dataCopy->state = *data->state;

			
			
			auto function = args.Data().As<Object>()->GetInternalField(0).As<Function>();
			auto instance = function->NewInstance(context).ToLocalChecked();

			auto field2 = instance->GetAlignedPointerFromInternalField(0);
			auto t2 = static_cast<XXHash3_128Object*>(field2);
			
			args.GetReturnValue().Set(instance);
		}

		static void Update(const FunctionCallbackInfo<Value>& args) {
			if (args.Length() == 0) {
				return Nan::ThrowError("Argument required");
			}

			auto isolate = args.GetIsolate();

			auto inputData = ParseInput(args[0]);
			if (inputData.isInvalid()) {
				return;
			}

			auto field = args.This()->GetAlignedPointerFromInternalField(0);
			auto thisArg = static_cast<XXHash3_128Object*>(field);

			XXH3_128bits_update(thisArg->state, inputData.Buffer, inputData.Length);

			if (inputData.IsOwned) {
				delete[] inputData.Buffer;
			}

			args.GetReturnValue().Set(args.This());
		}

		static void Digest(const FunctionCallbackInfo<Value>& args) {
			auto field = args.This()->GetAlignedPointerFromInternalField(0);
			auto thisArg = static_cast<XXHash3_128Object*>(field);

			auto hash = XXH3_128bits_digest(thisArg->state);
			XXH128_canonical_t canonical_sum;
			XXH128_canonicalFromHash(&canonical_sum, hash);

			SetDigestOutput(canonical_sum, args, 0);
		}

		static void DirectHash(const FunctionCallbackInfo<Value>& args) {
			if (args.Length() == 0) {
				return Nan::ThrowError("Argument required");
			}

			auto isolate = args.GetIsolate();
			auto inputData = ParseInput(args[0]);

			if (inputData.Buffer == NULL) {
				return; // 输入参数有错误
			}

			auto hash = XXH3_128bits(inputData.Buffer, inputData.Length);
			XXH128_canonical_t canonical_sum;
			XXH128_canonicalFromHash(&canonical_sum, hash);

			if (inputData.IsOwned) {
				delete[] inputData.Buffer;
			}

			SetDigestOutput(canonical_sum, args, 1);
		}

		static void SetDigestOutput(
			XXH128_canonical_t sum,
			const FunctionCallbackInfo<Value>& args,
			int i
		){
			Local<Value> rv;

			if (args.Length() == i) {
				rv = Buffer::New(args.GetIsolate(), sizeof(sum)).ToLocalChecked();
				*reinterpret_cast<XXH128_canonical_t*>(node::Buffer::Data(rv)) = sum;
			}
			else if (args[0]->IsString()) {
				auto output = reinterpret_cast<char*>(sum.digest);
				rv = EncodeOutput(output, sizeof(sum), args[0].As<String>());
			}
			else {
				 return Nan::ThrowTypeError("encoding argument must be an string");
			}

			args.GetReturnValue().Set(rv);
		}
	};

	void Initialize(Local<Object> exports) {
		XXHash3_128Object::Init(exports);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}
