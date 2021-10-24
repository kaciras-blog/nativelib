#pragma once

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL

#include <xxhash.h>
#include <napi.h>

/*
 * 对 XXHash 的最小封装，该类不对应 Node 的 Hash 类型，部分功能将在 JS 层继承实现。
 *
 * 之所以这样做，是因为字符串编码在 JS 里可以简单地 Buffer.from，但在扩展里就很难处理，
 * 如果使用基于 v8 API 的 node::DecodeBytes 则会导致 napi 的兼容性失效。
 */
class XXHash3_128 : public Napi::ObjectWrap<XXHash3_128> {
private:

	XXH3_state_t* state = XXH3_createState();

	Napi::Value Update(const Napi::CallbackInfo& info);
	Napi::Value Digest(const Napi::CallbackInfo& info);

	static Napi::Value Hash(const Napi::CallbackInfo& info);

public:

	XXHash3_128(const Napi::CallbackInfo& info);

	// 析构函数在 ObjectWrap 默认的
	~XXHash3_128();

	// 注册该类到扩展
	static void Register(Napi::Env env, Napi::Object exports);
};
