import { createXXH3_128, hashSum, xxHash3_128 } from "../lib";

const EMPTY = Buffer.alloc(0);
const SEED = 1048573;
const SECRET = Buffer.from(
	"xxHash is an extremely fast non-cryptographic hash algorithm," +
	" working at RAM speed limit." +
	" It is proposed in four flavors (XXH32, XXH64, XXH3_64bits and XXH3_128bits)",
);

const invalidSeeds: any[] = [null, "string", {}, [], xxHash3_128, true];
const invalidInputs: any[] = [undefined, null, {}, [], xxHash3_128, true];

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
	const uniqueHashes = new Set<string>();
	const obj1 = ["foo"];
	const obj2 = ["bar"];

	test.each([
		[0, 1, 2, 3],
		{ 0: 0, 1: 1, 2: 2, 3: 3 },
		{ 0: 0, 1: 1, 2: 2, 3: 3, length: 4 },
		{ url: 12 },
		{ headers: 12 },
		{ headers: 122 },
		{ headers: "122" },
		{ headers: { accept: "text/plain" } },
		{ payload: [0, 1, 2, 3], headers: [{ a: "b" }] },
		"", "''",
		"null", "'null'",
		"false", "'false'",
		"true", "'true'",
		"0", "'0'",
		"1", "'1'",
		"void 0", "'void 0'",
		"undefined", "'undefined'",
		null,
		false,
		true,
		Infinity,
		-Infinity,
		NaN,
		0,
		1,
		void 0,
		{ a: obj1, b: obj2, c: obj1 },
		{ a: obj1, b: obj2, c: obj2 },
		{},
		[],
		[{}],
		{ a: {}, b: {} },
	])("creates unique hashes %#", value => {
		const hash = hashSum(value).toString("base64url");
		expect(uniqueHashes).not.toContain(hash);
		uniqueHashes.add(hash);
	});

	test.each([
		[{ a: "1", b: 2 }, { b: 2, a: "1" }],
		[{}, Object.create(null)],
		[{ a: "1" }, { a: "1" }],
	])("hashes clash if same properties %#", (a, b) => {
		expect(hashSum(a)).toStrictEqual(hashSum(b));
	});
});

