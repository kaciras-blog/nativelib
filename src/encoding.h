#pragma once

#include <node.h>
#include <nan.h>
#include <node_buffer.h>

namespace XXHash {
	using namespace node;
	using namespace std;

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;
	using v8::MaybeLocal;
	using v8::NewStringType;

	class InputData {
	public:
		size_t Length;
		const char* Buffer;

		InputData(const char* buffer, size_t len, bool owned);
		~InputData();
		bool isInvalid() const;

	private:
		bool IsOwned;
	};

	shared_ptr<InputData> ParseInput(Local<Value> input);

	Local<Value> EncodeDigest(const char* digest, size_t size, Local<String> outType);
}
