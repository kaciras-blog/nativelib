#pragma once

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL

#include <xxhash.h>
#include <napi.h>

class XXH3_128ObjectHasher {
private:
	XXH3_state_t* state = XXH3_createState();
	Napi::Object seen;

	Napi::Function setHas;
	Napi::Function setAdd;

	XXH3_128ObjectHasher(Napi::Env env);

	bool CheckSeen(const Napi::Value value);
	void FoldValue(const Napi::Value value);

	static Napi::Value Fold(const Napi::CallbackInfo& info);

public:
	static void Register(Napi::Env env, Napi::Object exports);
};
