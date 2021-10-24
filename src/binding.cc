#include <napi.h>
#include "xxhash3_128.h"

Napi::Object RegisterModule(Napi::Env env, Napi::Object exports) {
	XXHash3_128::Register(env, exports);
	return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, RegisterModule);
