#include <sstream>
#include <xxhash_class.h>

namespace XXHash {
	using namespace std;

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

	bool CheckSecretSize(size_t size) {
		if (size >= XXH3_SECRET_SIZE_MIN) {
			return true;
		}
		ostringstream message;
		message << "secret must be at least " << XXH3_SECRET_SIZE_MIN << " bytes";
		Nan::ThrowError(message.str().c_str());
		return false;
	}

	// ========================== XXHash 32bit ==========================

	XXHash32Wrapper::XXHash32Wrapper(XXHash32Wrapper* src) {
		XXH32_copyState(state, src->state);
	}

	XXHash32Wrapper::~XXHash32Wrapper() {
		XXH32_freeState(state);
	}

	void XXHash32Wrapper::Init() {
		XXH32_reset(state, 0);
	}

	Maybe<void> XXHash32Wrapper::Init(Local<Context> context, Local<Value> seed) {
		if (!seed->IsUint32()) {
			Nan::ThrowTypeError("seed must be an integer");
			return v8::Nothing<void>();
		}
		XXH32_reset(state, seed->Uint32Value(context).FromJust());
		return v8::JustVoid();
	}

	void XXHash32Wrapper::Update(InputData* input) {
		XXH32_update(state, input->Buffer, input->Length);
	}

	XXH32_canonical_t XXHash32Wrapper::Digest() {
		return Canonical32(XXH32_digest(state));
	}

	XXH32_canonical_t XXHash32Wrapper::Digest(InputData* input) {
		return Canonical32(XXH32(input->Buffer, input->Length, 0));
	}

	Maybe<XXH32_canonical_t> XXHash32Wrapper::Digest(InputData* input, Local<Context> context, Local<Value> seed) {
		XXH64_hash_t hash;

		if (seed->IsUint32()) {
			auto seedVal = seed->Uint32Value(context).FromJust();
			hash = XXH32(input->Buffer, input->Length, seedVal);
		}
		else {
			Nan::ThrowTypeError("seed must be an integer");
			return v8::Nothing<XXH32_canonical_t>();
		}

		return v8::Just(Canonical32(hash));
	}

	// ========================== XXHash 64bit ==========================

	XXHash64Wrapper::XXHash64Wrapper(XXHash64Wrapper* src) {
		XXH64_copyState(state, src->state);
	}

	XXHash64Wrapper::~XXHash64Wrapper() {
		XXH64_freeState(state);
	}

	void XXHash64Wrapper::Init() {
		XXH64_reset(state, 0);
	}

	Maybe<void> XXHash64Wrapper::Init(Local<Context> context, Local<Value> seed) {
		if (!seed->IsUint32()) {
			Nan::ThrowTypeError("seed must be an integer");
			return v8::Nothing<void>();
		}
		XXH64_reset(state, seed->Uint32Value(context).FromJust());
		return v8::JustVoid();
	}

	void XXHash64Wrapper::Update(InputData* input) {
		XXH64_update(state, input->Buffer, input->Length);
	}

	XXH64_canonical_t XXHash64Wrapper::Digest() {
		return Canonical64(XXH64_digest(state));
	}

	XXH64_canonical_t XXHash64Wrapper::Digest(InputData* input) {
		return Canonical64(XXH64(input->Buffer, input->Length, 0));
	}

	Maybe<XXH64_canonical_t> XXHash64Wrapper::Digest(InputData* input, Local<Context> context, Local<Value> seed) {
		XXH64_hash_t hash;

		if (seed->IsUint32()) {
			auto seedVal = seed->Uint32Value(context).FromJust();
			hash = XXH64(input->Buffer, input->Length, seedVal);
		}
		else {
			Nan::ThrowTypeError("seed must be an integer");
			return v8::Nothing<XXH64_canonical_t>();
		}

		return v8::Just(Canonical64(hash));
	}

	// ========================= XXHash 3 64bit =========================


	XXHash3_64Wrapper::XXHash3_64Wrapper(XXHash3_64Wrapper* src) {
		XXH3_copyState(state, src->state);
	}

	XXHash3_64Wrapper::~XXHash3_64Wrapper() {
		XXH3_freeState(state);
	}

	void XXHash3_64Wrapper::Init() {
		XXH3_64bits_reset(state);
	}

	Maybe<void> XXHash3_64Wrapper::Init(Local<Context> context, Local<Value> seed) {
		if (seed->IsUint32()) {
			auto seedVal = seed->Uint32Value(context).FromJust();
			XXH3_64bits_reset_withSeed(state, seedVal);
		}
		else if (Buffer::HasInstance(seed)) {
			auto secret = Buffer::Data(seed);
			auto len = Buffer::Length(seed);

			if (!CheckSecretSize(len)) {
				return v8::Nothing<void>();
			}

			XXH3_64bits_reset_withSecret(state, secret, len);
		}
		else {
			Nan::ThrowTypeError("seed must be an integer and secret must be a Buffer");
			return v8::Nothing<void>();
		}
		return v8::JustVoid();
	}

	void XXHash3_64Wrapper::Update(InputData* input) {
		XXH3_64bits_update(state, input->Buffer, input->Length);
	}

	XXH64_canonical_t XXHash3_64Wrapper::Digest() {
		return Canonical64(XXH3_64bits_digest(state));
	}

	XXH64_canonical_t XXHash3_64Wrapper::Digest(InputData* input) {
		return Canonical64(XXH3_64bits(input->Buffer, input->Length));
	}

	Maybe<XXH64_canonical_t> XXHash3_64Wrapper::Digest(InputData* input, Local<Context> context, Local<Value> seed) {
		XXH64_hash_t hash;

		if (seed->IsUint32()) {
			auto seedVal = seed->Uint32Value(context).FromJust();
			hash = XXH3_64bits_withSeed(input->Buffer, input->Length, seedVal);
		}
		else if (Buffer::HasInstance(seed)) {
			auto secret = Buffer::Data(seed);
			auto len = Buffer::Length(seed);

			if (!CheckSecretSize(len)) {
				return v8::Nothing<XXH64_canonical_t>();
			}

			hash = XXH3_64bits_withSecret(input->Buffer, input->Length, secret, len);
		}
		else {
			Nan::ThrowTypeError("seed must be an integer and secret must be a Buffer");
			return v8::Nothing<XXH64_canonical_t>();
		}

		return v8::Just(Canonical64(hash));
	}

	// ========================= XXHash 3 128bit =========================


	XXHash3_128Wrapper::XXHash3_128Wrapper(XXHash3_128Wrapper* src) {
		XXH3_copyState(state, src->state);
	}

	XXHash3_128Wrapper::~XXHash3_128Wrapper() {
		XXH3_freeState(state);
	}

	void XXHash3_128Wrapper::Init() {
		XXH3_128bits_reset(state);
	}

	Maybe<void> XXHash3_128Wrapper::Init(Local<Context> context, Local<Value> seed) {
		if (seed->IsUint32()) {
			auto seedVal = seed->Uint32Value(context).FromJust();
			XXH3_128bits_reset_withSeed(state, seedVal);
		}
		else if (Buffer::HasInstance(seed)) {
			auto secret = Buffer::Data(seed);
			auto len = Buffer::Length(seed);

			if (!CheckSecretSize(len)) {
				return v8::Nothing<void>();
			}

			XXH3_128bits_reset_withSecret(state, secret, len);
		}
		else {
			Nan::ThrowTypeError("seed must be an integer and secret must be a Buffer");
			return v8::Nothing<void>();
		}
		return v8::JustVoid();
	}

	void XXHash3_128Wrapper::Update(InputData* input) {
		XXH3_128bits_update(state, input->Buffer, input->Length);
	}

	XXH128_canonical_t XXHash3_128Wrapper::Digest() {
		return Canonical128(XXH3_128bits_digest(state));
	}

	XXH128_canonical_t XXHash3_128Wrapper::Digest(InputData* input) {
		return Canonical128(XXH3_128bits(input->Buffer, input->Length));
	}

	Maybe<XXH128_canonical_t> XXHash3_128Wrapper::Digest(InputData* input, Local<Context> context, Local<Value> seed) {
		XXH128_hash_t hash;

		if (seed->IsUint32()) {
			auto seedVal = seed->Uint32Value(context).FromJust();
			hash = XXH3_128bits_withSeed(input->Buffer, input->Length, seedVal);
		}
		else if (Buffer::HasInstance(seed)) {
			auto secret = Buffer::Data(seed);
			auto len = Buffer::Length(seed);

			if (!CheckSecretSize(len)) {
				return v8::Nothing<XXH128_canonical_t>();
			}

			hash = XXH3_128bits_withSecret(input->Buffer, input->Length, secret, len);
		}
		else {
			Nan::ThrowTypeError("seed must be int number and secret must be Buffer");
			return v8::Nothing<XXH128_canonical_t>();
		}

		return v8::Just(Canonical128(hash));
	}
}
