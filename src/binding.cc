#include <encoding.h>
#include <xxhash_class.h>

#include <sstream>
#include <node.h>
#include <nan.h>
#include <node_buffer.h>
#include <utils.h>

namespace XXHash {
	using v8::ObjectTemplate;
	using v8::FunctionTemplate;
	using v8::Number;
	using v8::Function;
	using v8::Context;
	using v8::Persistent;
	using v8::External;

	template<class XXHashClass>
	struct XXHashBinding {
		XXHashClass* state;
		Persistent<Object> handle;
	};

	template<class XXHashClass>
	class XXHashTemplate {
	public:

		static void Init(Local<Object> exports, const char* version) {
			auto isolate = exports->GetIsolate();
			auto context = isolate->GetCurrentContext();

			// 这一句创建了一个新堆栈，用于垃圾回收
			auto handle_scope(isolate);

			// 搞个对象来保存构造方法，相当于闭包上下文？
			auto objectTpl = ObjectTemplate::New(isolate);
			objectTpl->SetInternalFieldCount(1);
			auto closure = objectTpl->NewInstance(context).ToLocalChecked();

			// 定义一个类（也是函数），使用空函数体
			auto clazz = FunctionTemplate::New(isolate);
			clazz->SetClassName(JSName(isolate, "XXHash", version));
			clazz->InstanceTemplate()->SetInternalFieldCount(1);

			// 在这个类的原型上定义几个方法
			NODE_SET_PROTOTYPE_METHOD(clazz, "copy", Copy);
			NODE_SET_PROTOTYPE_METHOD(clazz, "update", Update);
			NODE_SET_PROTOTYPE_METHOD(clazz, "digest", Digest);

			// 把类的构造函数加入闭包
			closure->SetInternalField(0, clazz->GetFunction(context).ToLocalChecked());

			// 定义 createXXH<verion>() 函数并导出
			auto createHash = FunctionTemplate::New(isolate, New, closure);
			auto name = JSName(isolate, "createXXH", version);
			createHash->SetClassName(name);
			createHash->SetLength(1);
			exports->Set(context, name, createHash->GetFunction(context).ToLocalChecked()).Check();

			// 再导出一个快捷 Hash 的函数
			ostringstream result;
			result << "xxHash" << version;
			NODE_SET_METHOD(exports, result.str().c_str(), QuickHash);
		}

		static void CreateStateClass(Local<Context> context) {
			auto isolate = context->GetIsolate();
		}

		static void DisposeState(const v8::WeakCallbackInfo<XXHashBinding<XXHashClass>>& data) {
			auto binding = data.GetParameter();
			delete binding->state;
			binding->handle.Reset();
		}

		static void SetState(Local<Object> object, XXHashClass* state) {
			auto isolate = object->GetIsolate();

			object->SetAlignedPointerInInternalField(0, state);

			auto binding = new XXHashBinding<XXHashClass>();
			binding->state = state;
			binding->handle.Reset(isolate, object);
			binding->handle.SetWeak(binding, DisposeState, v8::WeakCallbackType::kParameter);
		}

		static XXHashClass* GetState(const FunctionCallbackInfo<Value>& args) {
			return static_cast<XXHashClass*>(args.This()->GetAlignedPointerFromInternalField(0));
		}

		// =========================== Enter Points ===========================

		static void New(const FunctionCallbackInfo<Value>& args) {
			auto context = args.GetIsolate()->GetCurrentContext();
			auto constructor = args.Data().As<Object>()->GetInternalField(0).As<Function>();

			auto state = new XXHashClass();

			if (args.Length() == 0) {
				state->Init();
			}
			else if (state->Init(context, args[0]).IsNothing()) {
				return;
			}

			auto instance = constructor->NewInstance(context).ToLocalChecked();
			SetState(instance, state);
			args.GetReturnValue().Set(instance);
		}

		static void QuickHash(const FunctionCallbackInfo<Value>& args) {
			auto isolate = args.GetIsolate();
			auto context = isolate->GetCurrentContext();

			if (args.Length() == 0) {
				return Nan::ThrowError("data required");
			}

			auto maybeData = ParseInput(isolate, args[0], encoding::UTF8);
			if (maybeData.IsNothing()) {
				return;
			}

			auto data = maybeData.FromJust().get();
			Local<Value> returnValue;

			if (args.Length() == 1) {
				auto sum = XXHashClass::Digest(data);
				returnValue = ToBuffer(isolate, sum);
			}
			else if (args.Length() > 2) {
				auto sum = XXHashClass::Digest(data, context, args[1]);
				if (sum.IsNothing()) {
					return;
				}
				returnValue = ToString(sum.FromJust(), args[2]);
			}
			else if (args[1]->IsString()) {
				auto sum = XXHashClass::Digest(data);
				returnValue = ToString(sum, args[1]);
			}
			else {
				auto sum = XXHashClass::Digest(data, context, args[1]);
				if (sum.IsNothing()) {
					return;
				}
				returnValue = ToBuffer(isolate, sum.FromJust());
			}

			args.GetReturnValue().Set(returnValue);
		}

		// =========================== interface XXHash ===========================

		static void Copy(const FunctionCallbackInfo<Value>& args) {
			auto isolate = args.GetIsolate();
			auto context = isolate->GetCurrentContext();

			auto stateCopy = new XXHashClass(GetState(args));

			auto name = String::NewFromUtf8(isolate, "constructor").ToLocalChecked();
			auto constructor = args.This()->Get(context, name).ToLocalChecked().As<Function>();

			auto instance = constructor->NewInstance(context).ToLocalChecked();
			SetState(instance, stateCopy);
			args.GetReturnValue().Set(instance);
		}

		static void Update(const FunctionCallbackInfo<Value>& args) {
			auto isolate = args.GetIsolate();
			auto data = v8::Nothing<shared_ptr<InputData>>();

			if (args.Length() == 0) {
				return Nan::ThrowError("Argument required");
			}

			if (args.Length() > 1) {
				data = ParseInput(isolate, args[0], args[1]);
			}
			else {
				data = ParseInput(isolate, args[0], encoding::UTF8);
			}

			if (data.IsNothing()) {
				return;
			}
			GetState(args)->Update(data.FromJust().get());
			args.GetReturnValue().Set(args.This());
		}

		static void Digest(const FunctionCallbackInfo<Value>& args) {
			auto sum = GetState(args)->Digest();
			Local<Value> returnVal;

			if (args.Length() == 0) {
				returnVal = ToBuffer(args.GetIsolate(), sum);
			}
			else if (args[0]->IsString()) {
				returnVal = ToString(sum, args[0]);
			}
			else {
				return Nan::ThrowTypeError("encoding argument must be an string");
			}

			args.GetReturnValue().Set(returnVal);
		}

		// ========================= Helper Functions =========================

		static Local<Value> ToBuffer(Isolate* isolate, typename XXHashClass::Sum sum) {
			auto buffer = new char[sizeof(sum)];
			*reinterpret_cast<typename XXHashClass::Sum*>(buffer) = sum;
			return Buffer::New(isolate, buffer, sizeof(sum)).ToLocalChecked();
		}

		static Local<Value> ToString(typename XXHashClass::Sum sum, Local<Value> encVal) {
			auto output = reinterpret_cast<char*>(sum.digest);
			return EncodeDigest(output, sizeof(sum), encVal.As<String>());
		}
	};

	void Initialize(Local<Object> exports) {
		XXHashTemplate<XXHash32Wrapper>::Init(exports, "32");
		XXHashTemplate<XXHash64Wrapper>::Init(exports, "64");
		XXHashTemplate<XXHash3_64Wrapper>::Init(exports, "3_64");
		XXHashTemplate<XXHash3_128Wrapper>::Init(exports, "3_128");
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}
