#pragma once

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>
#include <encoding.h>

// 把 xxHash 的函数封装成类，避免跟 v8 相关的代码混在一起。
// 另一个原因是 xxHash 每个变体都有至少9个相关的函数，全写进模板太长了难看，用类封装下就只用写两个。
namespace XXHash {
	using v8::Context;

	class XXHash32Wrapper {
	private:
		XXH32_state_t* state = XXH32_createState();
	public:
		typedef XXH32_canonical_t Sum;

		XXHash32Wrapper() = default;
		explicit XXHash32Wrapper(XXHash32Wrapper* src);
		~XXHash32Wrapper();

		void Init();
		Maybe<void> Init(Local<Context> context, Local<Value> seed);

		void Update(InputData* input);
		XXH32_canonical_t Digest();

		static XXH32_canonical_t Digest(InputData* input);
		static Maybe<XXH32_canonical_t> Digest(InputData* input, Local<Context> context, Local<Value> seed);
	};

	class XXHash64Wrapper {
	private:
		XXH64_state_t* state = XXH64_createState();
	public:
		typedef XXH64_canonical_t Sum;

		XXHash64Wrapper() = default;
		explicit XXHash64Wrapper(XXHash64Wrapper* src);
		~XXHash64Wrapper();

		void Init();
		Maybe<void> Init(Local<Context> context, Local<Value> seed);

		void Update(InputData* input);
		XXH64_canonical_t Digest();

		static XXH64_canonical_t Digest(InputData* input);
		static Maybe<XXH64_canonical_t> Digest(InputData* input, Local<Context> context, Local<Value> seed);
	};

	class XXHash3_64Wrapper {
	private:
		XXH3_state_t* state = XXH3_createState();
	public:
		typedef XXH64_canonical_t Sum;

		XXHash3_64Wrapper() = default;
		explicit XXHash3_64Wrapper(XXHash3_64Wrapper* src);
		~XXHash3_64Wrapper();

		void Init();
		Maybe<void> Init(Local<Context> context, Local<Value> seed);

		void Update(InputData* input);
		XXH64_canonical_t Digest();

		static XXH64_canonical_t Digest(InputData* input);
		static Maybe<XXH64_canonical_t> Digest(InputData* input, Local<Context> context, Local<Value> seed);
	};

	class XXHash3_128Wrapper {
	private:
		XXH3_state_t* state = XXH3_createState();
	public:
		typedef XXH128_canonical_t Sum;

		XXHash3_128Wrapper() = default;
		explicit XXHash3_128Wrapper(XXHash3_128Wrapper* src);
		~XXHash3_128Wrapper();

		void Init();
		Maybe<void> Init(Local<Context> context, Local<Value> seed);

		void Update(InputData* input);
		XXH128_canonical_t Digest();

		static XXH128_canonical_t Digest(InputData* input);
		static Maybe<XXH128_canonical_t> Digest(InputData* input, Local<Context> context, Local<Value> seed);
	};
}
