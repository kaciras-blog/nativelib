#include "object_hash.h"

// 拿内存地址来做 Hash，性能更好，但这是 v8 内部 API，还是不要用了。
//bool XXH3_128ObjectHasher::CheckSeen(const Value value) {
//	v8::Local<v8::Value> local;
//	napi_value v = value;
//	memcpy(static_cast<void*>(&local), &v, sizeof(v));
//
//	auto context = v8::Isolate::GetCurrent()->GetCurrentContext();
//	auto s = *local->ToObject(context).ToLocalChecked();
//	auto ad = reinterpret_cast<v8::internal::Address*>(s);
//
//	if (!seen.emplace(*ad).second) {
//		XXH3_128bits_update(state, "[Circular]", 10);
//		return true;
//	}
//	return false;
//}

XXH3_128ObjectHasher::XXH3_128ObjectHasher(Env env) {
	XXH3_128bits_reset(state);

	auto setClass = env.Global().Get("Set").As<Function>();
	auto setProto = setClass.Get("prototype").As<Object>();

	seen = setClass.New({});
	setHas = setProto.Get("has").As<Function>();
	setAdd = setProto.Get("add").As<Function>();
}

bool XXH3_128ObjectHasher::CheckSeen(Value value) {
	bool isSeen = setHas.Call(seen, { value }).As<Boolean>();

	if (isSeen) {
		XXH3_128bits_update(state, "[Circular]", 10);
	}
	else {
		setAdd.Call(seen, { value });
	}

	return isSeen;
}

void XXH3_128ObjectHasher::FoldValue(Value value) {
	if (value.IsString())
	{
		auto key = value.As<String>().Utf16Value();
		XXH3_128bits_update(state, key.c_str(), key.size() * 2);
	}
	else if (value.IsNumber())
	{
		auto d = value.As<Number>().DoubleValue();
		XXH3_128bits_update(state, &d, sizeof(d));
	}
	else if (value.IsBoolean())
	{
		int b = value.As<Boolean>() ? 0x4EB6440D : 0xC3C4FF67;
		XXH3_128bits_update(state, &b, sizeof(b));
	}
	else if (value.IsArray())
	{
		if (CheckSeen(value)) {
			return;
		}
		auto array = value.As<Array>();
		for (int i = array.Length() - 1; i >= 0; i--)
		{
			FoldValue(array[i]);
			XXH3_128bits_update(state, &i, sizeof(i));
		}
	}
	else if (value.IsObject())
	{
		if (CheckSeen(value)) {
			return;
		}
		auto obj = value.As<Object>();
		std::vector<std::pair<std::string, Value>> keys;

		for (const auto& e : value.As<Object>()) {
			auto key = e.first.As<String>().Utf8Value();
			keys.push_back(std::make_pair(key, e.second));
		}

		std::sort(keys.begin(), keys.end(), [](auto& left, auto& right) {
			return left.first < right.first;
		});

		for (const auto& e : keys) {
			FoldValue(e.second);
			auto& key = e.first;
			XXH3_128bits_update(state, key.c_str(), key.size() * 2);
		}
	}
	else if (value.IsUndefined())
	{
		XXH3_128bits_update(state, "[Undefined]", 11);
	}
	else if (value.IsNull())
	{
		XXH3_128bits_update(state, "[Null]", 6);
	}
	else
	{
		// function, symbol
		throw TypeError::New(value.Env(), "value is not hash-able");
	}
}

Value XXH3_128ObjectHasher::Fold(const CallbackInfo& info) {
	auto hash = XXH3_128ObjectHasher(info.Env());
	hash.FoldValue(info[0]);
	return ToCanonicalBuffer(info, XXH3_128bits_digest(hash.state));
}

void XXH3_128ObjectHasher::Register(Napi::Env env, Object exports) {
	exports.Set("ObjectHash", Function::New(env, XXH3_128ObjectHasher::Fold));
}
