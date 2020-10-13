#include <emscripten/val.h>
#include <emscripten/bind.h>
#include <sanitizer/lsan_interface.h>

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

using namespace emscripten;

using std::string;


class XXHash3_128 {

	XXH3_state_s* state = XXH3_createState();

public:

	XXHash3_128() {
		XXH3_128bits_reset(state);
	}

	~XXHash3_128() {
		XXH3_freeState(state);
	}

	void Update(string buffer) {
		XXH3_128bits_update(state, buffer.data(), buffer.length());
	}

	val Digest() {
		auto hash = XXH3_128bits_digest(state);

		XXH128_canonical_t canonical_sum;
		XXH128_canonicalFromHash(&canonical_sum, hash);

		return val(typed_memory_view(16, canonical_sum.digest));
	}
};

val Canonical_128(XXH128_hash_t hash) {
	const size_t SIZE = sizeof(XXH128_canonical_t);

	XXH128_canonical_t canonical_sum;
	XXH128_canonicalFromHash(&canonical_sum, hash);

	return val(typed_memory_view(SIZE, canonical_sum.digest));
}

val CalcXXHash3_128(string buffer) {
	auto hash = XXH3_128bits(buffer.data(), buffer.length());
	return Canonical_128(hash);
}

val CalcXXHash3_128(string buffer, int32_t seed) {
	auto hash = XXH3_128bits_withSeed(buffer.data(), buffer.length(), seed);

	XXH128_canonical_t canonical_sum;
	XXH128_canonicalFromHash(&canonical_sum, hash);

	const size_t size = sizeof(canonical_sum.digest);
	return val(typed_memory_view(size, canonical_sum.digest));
}

val CalcXXHash3_128(string buffer, string secret) {
	if (secret.length() < XXH3_SECRET_SIZE_MIN) {

	}
	auto hash = XXH3_128bits_withSecret(buffer.data(), buffer.length(), secret.data(), secret.length());

	XXH128_canonical_t canonical_sum;
	XXH128_canonicalFromHash(&canonical_sum, hash);

	const size_t size = sizeof(canonical_sum.digest);
	return val(typed_memory_view(size, canonical_sum.digest));
}

EMSCRIPTEN_BINDINGS(xxhash_module) {
	function("xxHash3_128", select_overload<val(string)>(&CalcXXHash3_128));
	function("xxHash3_128_Seed", select_overload<val(string, int32_t)>(&CalcXXHash3_128));
	function("xxHash3_128_Secret", select_overload<val(string, string)>(&CalcXXHash3_128));

	class_<XXHash3_128>("XXHash3_128")
		.constructor()
		.function("Update", &XXHash3_128::Update)
		.function("Digest", &XXHash3_128::Digest);

	/*
	 * Sanitizer 是编译器的一个功能，在编译期插入一些代码，用来检测各种运行时 BUG。
	 * emcc 使用 -fsanitize=... 来启用该功能。
	 *
	 * 当使用 -fsanitize=address 时，导出以下两个函数用来检测内存泄漏。
	 * 这俩函数区别是 doLeakCheckError 会导致程序异常，而 doLeakCheck 仅打印错误信息。
	 *
	 * 【详情见】
	 * https://emscripten.org/docs/debugging/Sanitizers.html
	 * https://web.dev/webassembly-memory-debugging/
	 */
#if __has_feature(address_sanitizer)
	function("doLeakCheckError", &__lsan_do_leak_check);
	function("doLeakCheck", &__lsan_do_recoverable_leak_check);
#endif
}
