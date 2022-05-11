import { createXXH3_128, hashSum, xxHash3_128 } from "../lib";

const EMPTY = Buffer.alloc(0);
const SEED = 1048573;
const SECRET = Buffer.from(
	"xxHash is an extremely fast non-cryptographic hash algorithm," +
	" working at RAM speed limit." +
	" It is proposed in four flavors (XXH32, XXH64, XXH3_64bits and XXH3_128bits)",
);

const invalidSeeds: any[] = [null, "string", {}, true];
const invalidInputs: any[] = [undefined, null, {}, 123456, true];

const data = [
	["xxhash", "9c8b437c78cac00a376072e24bfdf4d2"],
	["", "99aa06d3014798d86001c324468d497f"],
];

const dataWithSeed = [
	["xxhash", "cb6ecb7a68b24de13c38fd1df6beaa6d"],
	["", "e511155e2e2cc9f448d6b520e2bb5a6c"],
];

const dataWithSecret = [
	["xxhash", "0ed3ac9d7bf477a459c6cc598b479f67"],
	["", "49667c25447e15aa5da4ef2a97dec1c9"],
];

describe("createXXH3_128", () => {

	it("should throw error with too short secret", () => {
		expect(() => createXXH3_128(EMPTY)).toThrow();
	});

	test.each(invalidSeeds)("should throw error on invalid seed %#", (seed) => {
		expect(() => createXXH3_128(seed)).toThrow();
	});

	it("should throw error on update without data", () => {
		// @ts-ignore Test for invalid call
		expect(() => createXXH3_128().update()).toThrow();
	});

	test.each(invalidInputs)("should throw on update with %s", data => {
		expect(() => createXXH3_128().update(data)).toThrow();
	});

	it("should copy state", () => {
		const hash = createXXH3_128().update("xxh");

		const copy = hash.copy().update("ash");
		const original = hash.update("ash");

		expect(copy.digest()).toStrictEqual(original.digest());
	});

	it("should isolate instance", () => {
		const hash = createXXH3_128().update("xxh");
		const copy = hash.copy().update("ash");

		expect(hash.digest()).not.toStrictEqual(copy.digest());
	});

	test.each(data)("digest - %#", (input, expected) => {
		const digest = createXXH3_128()
			.update(input)
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});

	test.each(data)("digest buffer - %#", (input, expected) => {
		const digest = createXXH3_128()
			.update(Buffer.from(input))
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});

	test.each(dataWithSeed)("digest with seed - %#", (input, expected) => {
		const digest = createXXH3_128(SEED)
			.update(input)
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});

	test.each(dataWithSecret)("digest with secret - %#", (input, expected) => {
		const digest = createXXH3_128(SECRET)
			.update(input)
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});
});

describe("xxHash3_128", () => {

	it("should throw error on digest without data", () => {
		// @ts-ignore Test for invalid call
		expect(() => xxHash3_128()).toThrow();
	});

	test.each(invalidSeeds)("should throw error on invalid seed %#", (seed) => {
		expect(() => xxHash3_128(EMPTY, seed)).toThrow();
	});

	test.each(data)("digest buffer - %#", (input, expected) => {
		const digest = xxHash3_128(Buffer.from(input));
		expect(digest.toString("hex")).toStrictEqual(expected);
	});

	test.each(dataWithSeed)("digest with seed - %#", (input, expected) => {
		const digest = xxHash3_128(Buffer.from(input), SEED);
		expect(digest.toString("hex")).toStrictEqual(expected);
	});

	test.each(dataWithSecret)("digest with secret - %#", (input, expected) => {
		const digest = xxHash3_128(Buffer.from(input), SECRET);
		expect(digest.toString("hex")).toStrictEqual(expected);
	});
});

describe("hashSum", () => {

	test("creates unique hashes", () => {
		const cases = [];

		function test_case(value: any, name?: string) {
			const hash = hashSum(value);
			cases.push({ value, hash });
			console.log("%s from:", hash, name || value);
		}

		test_case([0, 1, 2, 3]);
		test_case({ 0: 0, 1: 1, 2: 2, 3: 3 });
		test_case({ 0: 0, 1: 1, 2: 2, 3: 3, length: 4 });
		test_case({ url: 12 });
		test_case({ headers: 12 });
		test_case({ headers: 122 });
		test_case({ headers: "122" });
		test_case({ headers: { accept: "text/plain" } });
		test_case({ payload: [0, 1, 2, 3], headers: [{ a: "b" }] });
		test_case("", "''");
		test_case("null", "'null'");
		test_case("false", "'false'");
		test_case("true", "'true'");
		test_case("0", "'0'");
		test_case("1", "'1'");
		test_case("void 0", "'void 0'");
		test_case("undefined", "'undefined'");
		test_case(null);
		test_case(false);
		test_case(true);
		test_case(Infinity);
		test_case(-Infinity);
		test_case(NaN);
		test_case(0);
		test_case(1);
		test_case(void 0);
		test_case({});
		test_case({ a: {}, b: {} });
		test_case([]);
		test_case(new Date());
		test_case(new Date(2019, 5, 28));
		test_case(new Date(1988, 5, 9));
	});

	test.each([
		[{ a: "1", b: 2 }, { b: 2, a: "1" }],
		[{}, {}],
		[{ a: "1" }, { a: "1" }],
	])("hashes clash if same properties %#", (a, b) => {
		expect(hashSum(a)).toBe(hashSum(b));
	});
});
