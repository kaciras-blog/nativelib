const binding = require("..");

it('should throw on pass invalid object', () => {
	expect(() => binding.xxHash3_128()).toThrowError();
	expect(() => binding.xxHash3_128(0)).toThrowError();
});

it('should generate correct value', () => {
	expect(binding.xxHash3_128("xxhash", "hex")).toBe("9c8b437c78cac00a376072e24bfdf4d2");
});
