#pragma once

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>
#include <encoding.h>

// 把 xxHash 的函数封装成类，避免跟 v8 相关的代码混在一起。
// 另一个原因是 xxHash 每个变体都有至少9个相关的函数，全写进模板太长了难看，用类封装下就只用写两个。
namespace XXHash {

	class XXHash32Wrapper {
	private:
		XXH32_state_t* state = XXH32_createState();
	public:
		typedef XXH32_canonical_t Sum;

		explicit XXHash32Wrapper(XXHash32Wrapper* src);
		XXHash32Wrapper();
		XXHash32Wrapper(XXH32_hash_t seed);
		~XXHash32Wrapper();

		void update(InputData input);
		Sum digest();

		static Sum digest(InputData input);
	};

	class XXHash64Wrapper {
	private:
		XXH64_state_t* state = XXH64_createState();
	public:
		typedef XXH64_canonical_t Sum;

		explicit XXHash64Wrapper(XXHash64Wrapper* src);
		XXHash64Wrapper();
		XXHash64Wrapper(XXH64_hash_t seed);
		~XXHash64Wrapper();

		void update(InputData input);
		Sum digest();

		static Sum digest(InputData input);
	};

	class XXHash3_64Wrapper {
	private:
		XXH3_state_t* state = XXH3_createState();
	public:
		typedef XXH64_canonical_t Sum;

		explicit XXHash3_64Wrapper(XXHash3_64Wrapper* src);
		XXHash3_64Wrapper();
		XXHash3_64Wrapper(XXH64_hash_t seed);
		~XXHash3_64Wrapper();

		void update(InputData input);
		Sum digest();

		static Sum digest(InputData input);
	};

	class XXHash3_128Wrapper {
	private:
		XXH3_state_t* state = XXH3_createState();
	public:
		typedef XXH128_canonical_t Sum;

		explicit XXHash3_128Wrapper(XXHash3_128Wrapper* src);
		XXHash3_128Wrapper();
		XXHash3_128Wrapper(XXH64_hash_t seed);
		~XXHash3_128Wrapper();

		void update(InputData input);
		Sum digest();

		static Sum digest(InputData input);
	};
}
