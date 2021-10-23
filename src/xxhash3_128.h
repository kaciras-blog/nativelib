#pragma once

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL

#include <xxhash.h>
#include <napi.h>

class XXHash3_128 : public Napi::ObjectWrap<XXHash3_128> {
private:

	XXH3_state_t* state = XXH3_createState();

	//explicit XXHash3_128(XXHash3_128* src);

	Napi::Value Update(const Napi::CallbackInfo& info);
	Napi::Value Digest(const Napi::CallbackInfo& info);

	static Napi::Value Hash(const Napi::CallbackInfo& info);

public:

	XXHash3_128(const Napi::CallbackInfo& info);
	~XXHash3_128();

	static void Register(Napi::Env env, Napi::Object exports);
};
