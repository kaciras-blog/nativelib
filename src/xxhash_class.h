#pragma once

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

namespace XXHash {

	class XXHash32Wrapper {
	private:
		XXH32_state_t* state;
	public:
		XXHash32Wrapper(XXH32_hash_t seed);

		explicit XXHash32Wrapper(XXHash32Wrapper* src);
		~XXHash32Wrapper();

		void update(const void* data, size_t len);
		XXH32_canonical_t digest();

		static XXH32_canonical_t digest(const void* data, size_t len);
	};

	class XXHash64Wrapper {
	private:
		XXH64_state_t* state;
	public:
		XXHash64Wrapper(XXH64_hash_t seed);

		explicit XXHash64Wrapper(XXHash64Wrapper* src);
		~XXHash64Wrapper();

		void update(const void* data, size_t len);
		XXH64_canonical_t digest();

		static XXH64_canonical_t digest(const void* data, size_t len);
	};

	class XXHash3_64Wrapper {
	private:
		XXH3_state_t* state;
	public:
		XXHash3_64Wrapper();
		XXHash3_64Wrapper(XXH64_hash_t seed);
		XXHash3_64Wrapper(const void* secret, size_t size);

		explicit XXHash3_64Wrapper(XXHash3_64Wrapper* src);
		~XXHash3_64Wrapper();

		void update(const void* data, size_t len);
		XXH64_canonical_t digest();

		static XXH64_canonical_t digest(const void* data, size_t len);
	};

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
