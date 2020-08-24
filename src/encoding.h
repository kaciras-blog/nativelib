#pragma once
#include <node.h>
#include <nan.h>
#include <node_buffer.h>

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

	struct InputData {
		char* Buffer;
		size_t Length;
		bool IsOwned;
	};

	InputData ConvertInput(const Local<Value>& input);

	Local<Value> ConvertOutput(const char* digest, size_t size, const Local<String>& outType);
}