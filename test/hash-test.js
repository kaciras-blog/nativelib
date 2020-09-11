/*
 * 测试 Hash 的结果是否正确
 */
const binding = require("..");

const EMPTY = Buffer.alloc(0);

test.each([
	["xxhash", "9a95b70e"],
	[EMPTY, "02cc5d05"]
])("xxHash32 - %#", (input, hex) => {
	expect(binding.xxHash32(input, "hex")).toBe(hex);
	expect(binding.createXXH32().update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", "32dd38952c4bc720"],
	[EMPTY, "ef46db3751d8e999"]
])("xxHash64 - %#", (input, hex) => {
	expect(binding.xxHash64(input, "hex")).toBe(hex);
	expect(binding.createXXH64().update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", "aa4c2b42ae6b13de"],
	[EMPTY, "2d06800538d394c2"]
])("xxHash3_64 - %#", (input, hex) => {
	expect(binding.xxHash3_64(input, "hex")).toBe(hex);
	expect(binding.createXXH3_64().update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", "9c8b437c78cac00a376072e24bfdf4d2"],
	[EMPTY, "99aa06d3014798d86001c324468d497f"]
])("xxHash3_128 - %#", (input, hex) => {
	expect(binding.xxHash3_128(input, "hex")).toBe(hex);
	expect(binding.createXXH3_128().update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", 1048573, "8c07ce30"],
	[EMPTY, 1048573, "f0c7624b"]
])("xxHash32 with seed - %#", (input, seed, hex) => {
	expect(binding.xxHash32(input, seed, "hex")).toBe(hex);
	expect(binding.createXXH32(seed).update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", 1048573, "15791517e13e1fc1"],
	[EMPTY, 1048573, "86e58a770464f3e7"]
])("xxHash64 with seed - %#", (input, seed, hex) => {
	expect(binding.xxHash64(input, seed, "hex")).toBe(hex);
	expect(binding.createXXH64(seed).update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", 1048573, "60b348f3ad2d9ddf"],
	[EMPTY, 1048573, "83c22612d8847b7f"]
])("xxHash3_64 with seed - %#", (input, seed, hex) => {
	expect(binding.xxHash3_64(input, seed, "hex")).toBe(hex);
	expect(binding.createXXH3_64(seed).update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", 1048573, "cb6ecb7a68b24de13c38fd1df6beaa6d"],
	[EMPTY, 1048573, "e511155e2e2cc9f448d6b520e2bb5a6c"]
])("xxHash3_128 with seed - %#", (input, seed, hex) => {
	expect(binding.xxHash3_128(input, seed, "hex")).toBe(hex);
	expect(binding.createXXH3_128(seed).update(input).digest("hex")).toBe(hex);
});
