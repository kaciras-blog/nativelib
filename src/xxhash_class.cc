#include <assert.h>
#include <xxhash_class.h>

namespace XXHash {

	XXH32_canonical_t Canonical32(XXH32_hash_t hash) {
		XXH32_canonical_t canonical_sum;
		XXH32_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	XXH64_canonical_t Canonical64(XXH64_hash_t hash) {
		XXH64_canonical_t canonical_sum;
		XXH64_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

	XXH128_canonical_t Canonical128(XXH128_hash_t hash) {
		XXH128_canonical_t canonical_sum;
		XXH128_canonicalFromHash(&canonical_sum, hash);
		return canonical_sum;
	}

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

	void XXHash32Wrapper::update(InputData input) {
		XXH32_update(state, input.Buffer, input.Length);
	}

	XXH32_canonical_t XXHash32Wrapper::digest() {
		return Canonical32(XXH32_digest(state));
	}

	XXH32_canonical_t XXHash32Wrapper::digest(InputData input) {
		return Canonical32(XXH32(input.Buffer, input.Length, 0));
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

	void XXHash64Wrapper::update(InputData input) {
		XXH64_update(state, input.Buffer, input.Length);
	}

	XXH64_canonical_t XXHash64Wrapper::digest() {
		return Canonical64(XXH64_digest(state));
	}

	XXH64_canonical_t XXHash64Wrapper::digest(InputData input) {
		return  Canonical64(XXH64(input.Buffer, input.Length, 0));
	}

	// ========================= XXHash 3 64bit =========================

	XXHash3_64Wrapper::XXHash3_64Wrapper() {
		XXH3_64bits_reset(state);
	}

	XXHash3_64Wrapper::XXHash3_64Wrapper(XXH64_hash_t seed) {
		XXH3_64bits_reset_withSeed(state, seed);
	}

	XXHash3_64Wrapper::XXHash3_64Wrapper(XXHash3_64Wrapper* src) {
		XXH3_copyState(state, src->state);
	}

	XXHash3_64Wrapper::~XXHash3_64Wrapper() {
		XXH3_freeState(state);
	}

	void XXHash3_64Wrapper::update(InputData input) {
		XXH3_64bits_update(state, input.Buffer, input.Length);
	}

	XXH64_canonical_t XXHash3_64Wrapper::digest() {
		return Canonical64(XXH3_64bits_digest(state));
	}

	XXH64_canonical_t XXHash3_64Wrapper::digest(InputData input) {
		return Canonical64(XXH3_64bits(input.Buffer, input.Length));
	}

	// ========================= XXHash 3 128bit =========================

	XXHash3_128Wrapper::XXHash3_128Wrapper() {
		XXH3_128bits_reset(state);
	}

	XXHash3_128Wrapper::XXHash3_128Wrapper(XXH64_hash_t seed) {
		XXH3_128bits_reset_withSeed(state, seed);
	}

	XXHash3_128Wrapper::XXHash3_128Wrapper(XXHash3_128Wrapper* src) {
		XXH3_copyState(state, src->state);
	}

	XXHash3_128Wrapper::~XXHash3_128Wrapper() {
		XXH3_freeState(state);
	}

	void XXHash3_128Wrapper::update(InputData input) {
		XXH3_128bits_update(state, input.Buffer, input.Length);
	}

	XXH128_canonical_t XXHash3_128Wrapper::digest() {
		return Canonical128(XXH3_128bits_digest(state));
	}

	XXH128_canonical_t XXHash3_128Wrapper::digest(InputData input) {
		return Canonical128(XXH3_128bits(input.Buffer, input.Length));
	}
}
