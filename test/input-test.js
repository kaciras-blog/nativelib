// 测试输入方法（createXX_, update, xxHash_）在各种参数类型和编码下的正确性
const binding = require("..");

test.each([
	undefined,
	null,
	"string",
	{},
])("should throw error on invalid seed %#", (seed) => {
	expect(() => binding.createXXH3_64(seed)).toThrow();
});

it("should throw error on update without data", () => {
	expect(() => binding.createXXH3_64().update()).toThrow();
});

test.each([
	["xxhash", "ascii"],
	["(￣ε(#￣)☆", "utf8",],
	["(￣ε(#￣)☆", "utf-8",],
	["(￣ε(#￣)☆", "utf16le"],
	["(￣ε(#￣)☆", "ucs2"],
	["(￣ε(#￣)☆", "ucs-2"],
	["KO+/o861KCPvv6Mp4piG4pWw4pWubyjvv6Pnmr/vv6MvLy8p", "base64"],
	["KO-/o861KCPvv6Mp4piG4pWw4pWubyjvv6Pnmr_vv6MvLy8p", "base64"],
	["xxhash", "latin1"],
	["xxhash", "binary"],
	["28efbfa3ceb52823efbfa329e29886", "hex"],
])("should update %s with encoding %s", (text, encoding) => {
	const fromString = binding.createXXH3_64().update(text, encoding);
	const fromBuffer = binding.createXXH3_64().update(Buffer.from(text, encoding));
	expect(fromString).toStrictEqual(fromBuffer);
});

it("should throw error on digest without data", () => {
	expect(() => binding.xxHash3_64()).toThrow();
});

const invalidData = [
	{},
	undefined,
	null,
	123456,
];

test.each(invalidData)("should throw error on update with %s", data => {
	expect(() => binding.createXXH3_64().update(data)).toThrow();
});

test.each(invalidData)("should throw error on digest with %s", data => {
	expect(() => binding.xxHash3_64(data)).toThrow();
});

it("should decode string in utf8", () => {
	const a = binding.xxHash3_64("我好帅");
	const b = binding.xxHash3_64(Buffer.from("我好帅", "utf8"));
	expect(a).toStrictEqual(b);
});
