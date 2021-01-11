#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL

#include <emscripten/emscripten.h>
#include <xxhash.h>

EMSCRIPTEN_KEEPALIVE
XXH32_state_t* XXH32_createState();

EMSCRIPTEN_KEEPALIVE
XXH64_state_t* Create_64() {
	return XXH64_createState();
}

EMSCRIPTEN_KEEPALIVE
XXH3_state_t* Create3() {
	return XXH3_createState();
}

EMSCRIPTEN_KEEPALIVE
uint8_t* Canonical_32(XXH32_hash_t hash) {
	const size_t SIZE = sizeof(XXH32_canonical_t);
	XXH32_canonical_t* canonical_sum;

	canonical_sum = (XXH32_canonical_t*)malloc(SIZE);
	XXH32_canonicalFromHash(canonical_sum, hash);

	return canonical_sum->digest;
}

EMSCRIPTEN_KEEPALIVE
uint8_t* Canonical_64(XXH64_hash_t hash) {
	const size_t SIZE = sizeof(XXH64_canonical_t);
	XXH64_canonical_t* canonical_sum;

	canonical_sum = (XXH64_canonical_t*)malloc(SIZE);
	XXH64_canonicalFromHash(canonical_sum, hash);

	return canonical_sum->digest;
}

EMSCRIPTEN_KEEPALIVE
uint8_t* Canonical_128(XXH128_hash_t hash) {
	const size_t SIZE = sizeof(XXH128_canonical_t);
	XXH128_canonical_t* canonical_sum;

	canonical_sum = (XXH128_canonical_t*)malloc(SIZE);
	XXH128_canonicalFromHash(canonical_sum, hash);

	return canonical_sum->digest;
}
