#include <encoding.h>
#include <xxhash_class.h>

#include <sstream>
#include <node_api.h>
#include <node.h>
#include <nan.h>
#include <node_buffer.h>

namespace XXHash {
	using v8::ObjectTemplate;
	using v8::FunctionTemplate;
	using v8::Number;
	using v8::Function;
	using v8::Context;

	Local<String> GetName(Isolate* isolate, const char* type, const char* version) {
		ostringstream stream;
		stream << type << version;
		auto name = String::NewFromUtf8(isolate, stream.str().c_str(), NewStringType::kInternalized);
		return name.ToLocalChecked();
	}

	template<typename XXHashClass>
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
			clazz->SetClassName(GetName(isolate, "XXHash", version));
			clazz->InstanceTemplate()->SetInternalFieldCount(1);

			// 在这个类的原型上定义几个方法
			NODE_SET_PROTOTYPE_METHOD(clazz, "copy", Copy);
			NODE_SET_PROTOTYPE_METHOD(clazz, "update", Update);
			NODE_SET_PROTOTYPE_METHOD(clazz, "digest", Digest);

			// 把类的构造函数加入闭包
			closure->SetInternalField(0, clazz->GetFunction(context).ToLocalChecked());

			// 定义 createXXH3_128() 函数并导出
			auto createHash = FunctionTemplate::New(isolate, New, closure);
			auto name = GetName(isolate, "createXXH", version);
			createHash->SetClassName(name);
			createHash->SetLength(1);
			exports->Set(context, name, createHash->GetFunction(context).ToLocalChecked()).Check();

			// 再导出一个快捷 Hash 的函数
			ostringstream result;
			result << "xxHash" << version;
			NODE_SET_METHOD(exports, result.str().c_str(), QuickHash);
		}

	private:

		static XXHashClass* GetState(const FunctionCallbackInfo<Value>& args) {
			return static_cast<XXHashClass*>(args.This()->GetAlignedPointerFromInternalField(0));
		}

		static void New(const FunctionCallbackInfo<Value>& args) {
			auto context = args.GetIsolate()->GetCurrentContext();
			auto constructor = args.Data().As<Object>()->GetInternalField(0).As<Function>();

			XXHashClass* state;

			if (args.Length() == 0) {
				state = new XXHashClass();
			}
			else if (args[0]->IsNumber()) {
				auto seed = args[0]->Uint32Value(context).FromJust();
				state = new XXHashClass(seed);
			}
			else {
				return Nan::ThrowTypeError("Invalid argument");
			}

			auto instance = constructor->NewInstance(context).ToLocalChecked();
			instance->SetAlignedPointerInInternalField(0, state);
			args.GetReturnValue().Set(instance);
		}

		static void Copy(const FunctionCallbackInfo<Value>& args) {
			auto isolate = args.GetIsolate();
			auto context = isolate->GetCurrentContext();

			auto stateCopy = new XXHashClass(GetState(args));

			auto name = String::NewFromUtf8(isolate, "constructor").ToLocalChecked();
			auto constructor = args.This()->Get(context, name).ToLocalChecked().As<Function>();

			auto instance = constructor->NewInstance(context).ToLocalChecked();
			instance->SetAlignedPointerInInternalField(0, stateCopy);

			args.GetReturnValue().Set(instance);
		}

		static void Update(const FunctionCallbackInfo<Value>& args) {
			if (args.Length() == 0) {
				return Nan::ThrowError("Argument required");
			}

			auto data = ParseInput(args[0]);
			if (data->isInvalid()) {
				return Nan::ThrowTypeError("data must be string or buffer");
			}

			GetState(args)->update(data.get());

			args.GetReturnValue().Set(args.This());
		}

		static void Digest(const FunctionCallbackInfo<Value>& args) {
			SetDigestOutput(GetState(args)->digest(), args, 0);
		}

		static void QuickHash(const FunctionCallbackInfo<Value>& args) {
			if (args.Length() == 0) {
				return Nan::ThrowError("data required");
			}

			auto data = ParseInput(args[0]);
			if (data->isInvalid()) {
				return Nan::ThrowTypeError("data must be string or buffer");
			}

			SetDigestOutput(XXHashClass::digest(data.get()), args, 1);
		}

		static void SetDigestOutput(
			typename XXHashClass::Sum sum,
			const FunctionCallbackInfo<Value>& args,
			int i
		) {
			Local<Value> rv;

			if (args.Length() == i) {
				rv = Buffer::New(args.GetIsolate(), sizeof(sum)).ToLocalChecked();
				*reinterpret_cast<typename XXHashClass::Sum*>(node::Buffer::Data(rv)) = sum;
			}
			else if (args[i]->IsString()) {
				auto output = reinterpret_cast<char*>(sum.digest);
				rv = EncodeDigest(output, sizeof(sum), args[i].As<String>());
			}
			else {
				return Nan::ThrowTypeError("encoding argument must be an string");
			}

			args.GetReturnValue().Set(rv);
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
