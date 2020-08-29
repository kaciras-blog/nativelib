#include <assert.h>
#include <xxhash_class.h>

namespace XXHash {

	// ========================== XXHash 32bit ==========================

	XXHash32Wrapper::XXHash32Wrapper() {
		XXH32_reset(state, 0);
	}

	XXHash32Wrapper::XXHash32Wrapper(XXH32_hash_t seed) {
		XXH32_reset(state, seed);
	}

	XXHash32Wrapper::XXHash32Wrapper(XXHash32Wrapper* src) {
		XXH32_copyState(state, src->state);
	}

	XXHash32Wrapper::~XXHash32Wrapper() {
		XXH32_freeState(state);
	}

	void XXHash32Wrapper::update(const void* data, size_t len) {
		XXH32_update(state, data, len);
	}

	XXH32_canonical_t XXHash32Wrapper::digest() {
		auto hash = XXH32_digest(state);
		XXH32_canonical_t canonical_sum;
		XXH32_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	XXH32_canonical_t XXHash32Wrapper::digest(const void* data, size_t len) {
		auto hash = XXH32(data, len, 0);
		XXH32_canonical_t canonical_sum;
		XXH32_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	// ========================== XXHash 64bit ==========================

	XXHash64Wrapper::XXHash64Wrapper() {
		XXH64_reset(state, 0);
	}

	XXHash64Wrapper::XXHash64Wrapper(XXH64_hash_t seed) {
		XXH64_reset(state, seed);
	}

	XXHash64Wrapper::XXHash64Wrapper(XXHash64Wrapper* src) {
		XXH64_copyState(state, src->state);
	}

	XXHash64Wrapper::~XXHash64Wrapper() {
		XXH64_freeState(state);
	}

	void XXHash64Wrapper::update(const void* data, size_t len) {
		XXH64_update(state, data, len);
	}

	XXH64_canonical_t XXHash64Wrapper::digest() {
		auto hash = XXH64_digest(state);
		XXH64_canonical_t canonical_sum;
		XXH64_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	XXH64_canonical_t XXHash64Wrapper::digest(const void* data, size_t len) {
		auto hash = XXH64(data, len, 0);
		XXH64_canonical_t canonical_sum;
		XXH64_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	// ========================= XXHash 3 64bit =========================

	XXHash3_64Wrapper::XXHash3_64Wrapper() {
		XXH3_64bits_reset(state);
	}

	XXHash3_64Wrapper::XXHash3_64Wrapper(XXH64_hash_t seed) {
		XXH3_64bits_reset_withSeed(state, seed);
	}

	XXHash3_64Wrapper::XXHash3_64Wrapper(const void* secret, size_t size) {
		XXH3_64bits_reset_withSecret(state, secret, size);
	}

	XXHash3_64Wrapper::XXHash3_64Wrapper(XXHash3_64Wrapper* src) {
		XXH3_copyState(state, src->state);
	}

	XXHash3_64Wrapper::~XXHash3_64Wrapper() {
		XXH3_freeState(state);
	}

	void XXHash3_64Wrapper::update(const void* data, size_t len) {
		XXH3_64bits_update(state, data, len);
	}

	XXH64_canonical_t XXHash3_64Wrapper::digest() {
		auto hash = XXH3_64bits_digest(state);
		XXH64_canonical_t canonical_sum;
		XXH64_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	XXH64_canonical_t XXHash3_64Wrapper::digest(const void* data, size_t len) {
		auto hash = XXH3_64bits(data, len);
		XXH64_canonical_t canonical_sum;
		XXH64_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	// ========================= XXHash 3 128bit =========================

	XXHash3_128Wrapper::XXHash3_128Wrapper() {
		XXH3_128bits_reset(state);
	}

	XXHash3_128Wrapper::XXHash3_128Wrapper(XXH64_hash_t seed) {
		XXH3_128bits_reset_withSeed(state, seed);
	}

	XXHash3_128Wrapper::XXHash3_128Wrapper(const void* secret, size_t size) {
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
