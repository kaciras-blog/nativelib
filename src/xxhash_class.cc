#include <xxhash_class.h>

namespace XXHash {

	XXHash3_128Wrapper::XXHash3_128Wrapper() {
		state = XXH3_createState();
		XXH3_128bits_reset(state);
	}

	XXHash3_128Wrapper::XXHash3_128Wrapper(XXH64_hash_t seed) {
		state = XXH3_createState();
		XXH3_128bits_reset_withSeed(state, seed);
	}

	XXHash3_128Wrapper::XXHash3_128Wrapper(const void* secret, size_t size) {
		state = XXH3_createState();
		XXH3_128bits_reset_withSecret(state, secret, size);
	}

	XXHash3_128Wrapper::XXHash3_128Wrapper(XXHash3_128Wrapper* src) {
		XXH3_copyState(state, src->state);
	}

	XXHash3_128Wrapper::~XXHash3_128Wrapper() {
		XXH3_freeState(state);
	}

	void XXHash3_128Wrapper::update(const void* data, size_t len) {
		XXH3_128bits_update(state, data, len);
	}

	XXH128_canonical_t XXHash3_128Wrapper::digest() {
		auto hash = XXH3_128bits_digest(state);
		XXH128_canonical_t canonical_sum;
		XXH128_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	XXH128_canonical_t XXHash3_128Wrapper::digest(const void* data, size_t len) {
		auto hash = XXH3_128bits(data, len);
		XXH128_canonical_t canonical_sum;
		XXH128_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}
}
