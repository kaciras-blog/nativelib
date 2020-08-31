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

	/*
	 * 表示从输入（Buffer, string）读取的原始字节数据
	 */
	class InputData {
	public:
		const char* Buffer;
		size_t Length;

		InputData(const char* buffer, size_t len, bool owned);
		~InputData();
	private:
		bool IsOwned;
	};

	/*
	 * 解析输入的JS字符串，返回需要传递给 hash 函数的数据。
	 *
	 * 搞个智能指针玩玩比毕竟以前没用过，其实不用也行。
	 */
	shared_ptr<InputData> ParseString(Isolate* isolate, Local<Value> input, encoding encoding);

	/*
	 * 从 Buffer 对象获取数据，注意调用前先用 Buffer::HasInstance 检查
	 */
	shared_ptr<InputData> ParseBuffer(Local<Value> input);

	Local<Value> EncodeDigest(const char* digest, size_t size, Local<String> outType);
}
