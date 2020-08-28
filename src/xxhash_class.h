#pragma once

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

namespace XXHash {

	class XXHash3_128Wrapper {
	private:
		XXH3_state_t* state;
	public:
		XXHash3_128Wrapper();
		XXHash3_128Wrapper(XXH64_hash_t seed);
		XXHash3_128Wrapper(const void* secret, size_t size);

		explicit XXHash3_128Wrapper(XXHash3_128Wrapper* src);
		~XXHash3_128Wrapper();

		void update(const void* data, size_t len);
		XXH128_canonical_t digest();

		static XXH128_canonical_t digest(const void* data, size_t len);
	};
}
