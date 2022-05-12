#include "xxhash3_128.h"

using namespace Napi;

Object RegisterModule(Env env, Object exports) {
	XXHash3_128::Register(env, exports);
	XXH3_128ObjectHasher::Register(env, exports);
	return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, RegisterModule);
