#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL

#include <emscripten/emscripten.h>
#include <xxhash.h>

EMSCRIPTEN_KEEPALIVE
uint8_t* createBuffer(size_t length) {
	return (uint8_t*) malloc(length);
}

EMSCRIPTEN_KEEPALIVE
void freeIO(uint8_t* input, uint8_t* output) {
	free(input);
	free(output);
}

EMSCRIPTEN_KEEPALIVE
uint8_t* xxHash3_128(uint8_t* buffer, size_t length) {
    XXH128_hash_t hash;
    XXH128_canonical_t canonical_sum;

    hash = XXH3_128bits(buffer, length);
    XXH128_canonicalFromHash(&canonical_sum, hash);

	uint8_t* result = (uint8_t*) malloc(sizeof(XXH128_canonical_t));
	memcpy(result, canonical_sum.digest, sizeof(XXH128_canonical_t));

    return result;
}
