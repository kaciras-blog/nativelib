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
	private:
		bool IsOwned;
	};

	/*
	 * 解析输入的JS值，返回需要传递给 hash 函数的数据。
	 *
	 * 搞个智能指针玩玩比毕竟以前没用过，其实不用也行。
	 */
	shared_ptr<InputData> ParseString(Isolate* isolate, Local<Value> input, encoding encoding);

	shared_ptr<InputData> ParseBuffer(Local<Value> input);

	Local<Value> EncodeDigest(const char* digest, size_t size, Local<String> outType);
}
