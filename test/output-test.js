// 测试输出方法返回的结果是否正确，以及各种输出编码
const binding = require("..");

test.each([
	undefined,
	null,
	"",
	"foo",
	"loooooooooooooooooong",
])("should throw error on unsupported encoding %#", encoding => {
	expect(() => binding.xxHash3_128("", encoding)).toThrowError();
});

test.each([
	["latin1", "latin1"],
	["base64", "base64"],
	["hex", "hex"],
	["base64u", "base64"], // UrlSafe 变体可以直接解码
])("should return %s string", (enc, dec) => {
	const hash = binding.createXXH3_128().update("xxhash");
	expect(Buffer.from(hash.digest(enc), dec)).toStrictEqual(hash.digest());
});

