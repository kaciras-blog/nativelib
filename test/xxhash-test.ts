import { describe, expect, it } from "@jest/globals";
import { createXXH3_128, xxHash3_128 } from "../lib/index.ts";

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

	it.each(invalidSeeds)("should throw error on invalid seed %#", (seed) => {
		expect(() => createXXH3_128(seed)).toThrow();
	});

	it("should throw error on update without data", () => {
		// @ts-expect-error
		expect(() => createXXH3_128().update()).toThrow();
	});

	it.each(invalidInputs)("should throw on update with %s", data => {
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

	it.each(data)("digest - %#", (input, expected) => {
		const digest = createXXH3_128()
			.update(input)
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});

	it.each(data)("digest buffer - %#", (input, expected) => {
		const digest = createXXH3_128()
			.update(Buffer.from(input))
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});

	it.each(dataWithSeed)("digest with seed - %#", (input, expected) => {
		const digest = createXXH3_128(SEED)
			.update(input)
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});

	it.each(dataWithSecret)("digest with secret - %#", (input, expected) => {
		const digest = createXXH3_128(SECRET)
			.update(input)
			.digest("hex");

		expect(digest).toStrictEqual(expected);
	});
});

describe("xxHash3_128", () => {

	it("should throw error on digest without data", () => {
		// @ts-expect-error
		expect(() => xxHash3_128()).toThrow();
	});

	it.each(invalidSeeds)("should throw error on invalid seed %#", (seed) => {
		expect(() => xxHash3_128(EMPTY, seed)).toThrow();
	});

	it.each(data)("digest buffer - %#", (input, expected) => {
		const digest = xxHash3_128(Buffer.from(input));
		expect(digest.toString("hex")).toStrictEqual(expected);
	});

	it.each(dataWithSeed)("digest with seed - %#", (input, expected) => {
		const digest = xxHash3_128(Buffer.from(input), SEED);
		expect(digest.toString("hex")).toStrictEqual(expected);
	});

	it.each(dataWithSecret)("digest with secret - %#", (input, expected) => {
		const digest = xxHash3_128(Buffer.from(input), SECRET);
		expect(digest.toString("hex")).toStrictEqual(expected);
	});
});
