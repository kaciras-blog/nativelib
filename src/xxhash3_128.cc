#include <assert.h>
#include "xxhash3_128.h"

Napi::Value Canonical128(const Napi::CallbackInfo& info, XXH128_hash_t hash) {
	XXH128_canonical_t canonical_sum;
	XXH128_canonicalFromHash(&canonical_sum, hash);

	auto data = reinterpret_cast<const char*>(&canonical_sum);

	auto env = info.Env();
	return Napi::Buffer<char>::Copy(env, data, sizeof(XXH128_canonical_t));
}

void CheckSecret(const Napi::CallbackInfo& info, Napi::Buffer<char> buffer){
	if (buffer.Length() >= XXH3_SECRET_SIZE_MIN) {
		return;
	}
	throw Napi::Error::New(info.Env(), "Secret must be at least 136 bytes");
}

void XXHash3_128::Register(Napi::Env env, Napi::Object exports) {
	Napi::HandleScope scope(env);

	Napi::Function t = DefineClass(env, "XXHash3_128Core", {
		StaticMethod<&XXHash3_128::Hash>("hash"),
		InstanceMethod<&XXHash3_128::Update>("update"),
		InstanceMethod<&XXHash3_128::Digest>("digest"),
		});

	Napi::FunctionReference* constructor = new Napi::FunctionReference();
	*constructor = Napi::Persistent(t);

	exports.Set("XXHash3_128Core", t);
	env.SetInstanceData<Napi::FunctionReference>(constructor);
}

XXHash3_128::XXHash3_128(const Napi::CallbackInfo& info) : Napi::ObjectWrap<XXHash3_128>(info) {
	auto env = info.Env();
	auto secret = info[0];

	if (secret.IsUndefined())
	{
		XXH3_128bits_reset(state);
	} else if (secret.IsObject())
	{
		auto from = Napi::ObjectWrap<XXHash3_128>::Unwrap(secret.ToObject());
		XXH3_copyState(state, from->state);
	} else if (secret.IsNumber())
	{
		auto seed = secret.As<Napi::Number>().Int32Value();
		XXH3_128bits_reset_withSeed(state, seed);
	} else if (secret.IsBuffer())
	{
		auto buffer = secret.As<Napi::Buffer<char>>();
		CheckSecret(info, buffer);

		XXH3_128bits_reset_withSecret(state, buffer.Data(), buffer.Length());
	} else
	{
		throw Napi::TypeError::New(env, "Secret must be a number or buffer");
	}
}

XXHash3_128::~XXHash3_128() {
	XXH3_freeState(state);
}

Napi::Value XXHash3_128::Update(const Napi::CallbackInfo& info){
	auto value = info[0];

	if (value.IsBuffer())
	{
		auto buffer = value.As<Napi::Buffer<char>>();
		XXH3_128bits_update(state, buffer.Data(), buffer.Length());
		return info.This();
	} else
	{
		auto env = info.Env();
		throw Napi::Error::New(env, "update() only accept buffer");
	}
}

Napi::Value XXHash3_128::Digest(const Napi::CallbackInfo& info){
	return Canonical128(info, XXH3_128bits_digest(state));
}

Napi::Value XXHash3_128::Hash(const Napi::CallbackInfo& info) {
	auto input = info[0].As<Napi::Buffer<char>>();
	auto secret = info[1];

	if (secret.IsUndefined())
	{
		auto hash = XXH3_128bits(input.Data(), input.Length());
		return Canonical128(info, hash);
	}

	if (secret.IsBuffer())
	{
		auto buffer = secret.As<Napi::Buffer<char>>();
		CheckSecret(info, buffer);

		auto hash = XXH3_128bits_withSecret(
			input.Data(), input.Length(),
			buffer.Data(), buffer.Length()
		);

		return Canonical128(info, hash);
	}

	if (secret.IsNumber())
	{
		auto seed = secret.As<Napi::Number>().Int32Value();
		auto hash = XXH3_128bits_withSeed(input.Data(), input.Length(), seed);
		return Canonical128(info, hash);
	}

	auto env = info.Env();
	throw Napi::TypeError::New(env, "Secret must be a number or buffer");
}
