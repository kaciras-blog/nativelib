#pragma once

#include <sstream>

using namespace std;

using v8::String;
using v8::NewStringType;
using v8::Isolate;
using v8::Local;

/*
 * 由两个字符串来组装函数的名字，返回 internalized string。
 */
Local<String> JSName(Isolate* isolate, const char* type, const char* version) {
	ostringstream stream;
	stream << type << version;

	// 不能写成 stream.str().c_str()，因为这会导致 stream.str() 提前释放
	auto name = stream.str();

	return String::NewFromUtf8(isolate, name.c_str(), NewStringType::kInternalized).ToLocalChecked();
}
