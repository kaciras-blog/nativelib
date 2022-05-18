#include "xxhash3_128.h"

// 该文件只用到了 napi.h 一个 C++ 库，省掉命名空间也不会导致混乱。
using namespace Napi;

/*
 * 将 XXHash 的摘要输出转换为 Node 的 Buffer 类型。
 *
 * 【实现注意】
 * 不要使用 Buffer::New，该函数创建的 Buffer 在 GC 时不释放内存。
 * 正确的做法是用 Buffer::Copy 复制。
 * https://github.com/nodejs/node-addon-api/blob/main/doc/buffer.md#new-1
 */
Value ToCanonicalBuffer(const CallbackInfo& info, XXH128_hash_t hash) {
	XXH128_canonical_t sum;
	XXH128_canonicalFromHash(&sum, hash);

	auto env = info.Env();
	return Buffer<uint8_t>::Copy(env, sum.digest, sizeof(sum.digest));
}

/*
 * 检查一下 Secret 的长度是否满足要求，如果太短则抛出异常。
 */
void CheckSecret(const CallbackInfo& info, Buffer<uint8_t> buffer) {
	if (buffer.Length() >= XXH3_SECRET_SIZE_MIN) {
		return;
	}
	throw Error::New(info.Env(), "Secret must be at least 136 bytes");
}

void XXHash3_128::Register(Napi::Env env, Object exports) {
	auto t = DefineClass(env, "XXHash3_128Core", {
		StaticMethod<&XXHash3_128::Hash>("hash"),
		InstanceMethod<&XXHash3_128::Update>("updateBytes"),
		InstanceMethod<&XXHash3_128::Digest>("digestBytes"),
	});

	auto constructor = new FunctionReference();
	*constructor = Napi::Persistent(t);

	exports.Set("XXHash3_128Core", t);
	env.SetInstanceData<FunctionReference>(constructor);
}

XXHash3_128::XXHash3_128(const CallbackInfo& info) : ObjectWrap<XXHash3_128>(info) {
	auto argument = info[0];

	if (argument.IsUndefined())
	{
		// 不用任何参数调用构造方法，在这里也会接收到一个 undefined？
		XXH3_128bits_reset(state);
	}
	else if (argument.IsNumber())
	{
		// 以整数类型的种子初始化。
		auto seed = argument.As<Number>().Int64Value();
		XXH3_128bits_reset_withSeed(state, seed);
	}
	else if (argument.IsBuffer())
	{
		// 以 Buffer 数据来初始化。
		auto secret = argument.As<Buffer<uint8_t>>();
		CheckSecret(info, secret);
		XXH3_128bits_reset_withSecret(state, secret.Data(), secret.Length());
	}
	else if (argument.IsObject())
	{
		// 参数为对象时复制状态，懒得单独写个方法所以放这里了。
		// 因为 Buffer 也是 object 所以要放在后面。
		// 如果对象不是 XXHash3_128 的实例则抛出异常 Invalid argument。
		auto from = ObjectWrap<XXHash3_128>::Unwrap(argument.ToObject());
		XXH3_copyState(state, from->state);
	}
	else
	{
		auto env = info.Env();
		throw TypeError::New(env, "Seed must be a number or buffer");
	}
}

XXHash3_128::~XXHash3_128() {
	XXH3_freeState(state);
}

Value XXHash3_128::Update(const CallbackInfo& info) {
	auto input = info[0];

	if (input.IsBuffer())
	{
		auto buffer = input.As<Buffer<uint8_t>>();
		XXH3_128bits_update(state, buffer.Data(), buffer.Length());
		return info.This();
	}
	else
	{
		auto env = info.Env();
		throw TypeError::New(env, "updateBytes() only accept buffer");
	}
}

Value XXHash3_128::Digest(const CallbackInfo& info) {
	return ToCanonicalBuffer(info, XXH3_128bits_digest(state));
}

Value XXHash3_128::Hash(const CallbackInfo& info) {
	auto input = info[0].As<Buffer<uint8_t>>();
	auto seed = info[1];

	if (seed.IsUndefined())
	{
		auto hash = XXH3_128bits(input.Data(), input.Length());
		return ToCanonicalBuffer(info, hash);
	}

	if (seed.IsBuffer())
	{
		auto secret = seed.As<Buffer<uint8_t>>();
		CheckSecret(info, secret);

		auto hash = XXH3_128bits_withSecret(
			input.Data(), input.Length(),
			secret.Data(), secret.Length()
		);

		return ToCanonicalBuffer(info, hash);
	}

	if (seed.IsNumber())
	{
		auto value = seed.As<Number>().Int32Value();
		auto hash = XXH3_128bits_withSeed(input.Data(), input.Length(), value);
		return ToCanonicalBuffer(info, hash);
	}

	auto env = info.Env();
	throw TypeError::New(env, "Seed must be a number or buffer");
}

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
