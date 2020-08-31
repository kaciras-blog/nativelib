const binding = require("..");

test.each([
	undefined,
	null,
	"string",
	{},
])
('should throw error on invalid seed %#', (seed) => {
	expect(() => binding.createXXH3_64(seed)).toThrowError();
});

it('should throw error on update without data', () => {
	expect(() => binding.createXXH3_64().update()).toThrowError();
});

it('should throw error on digest without data', () => {
	expect(() => binding.xxHash3_64()).toThrowError();
});

const invalidDataTest = test.each([undefined, null, 123456, {}]);

invalidDataTest('should throw error on update with %s', data => {
	expect(() => binding.createXXH3_64().update(data)).toThrowError();
});

invalidDataTest('should throw error on digest with %s', data => {
	expect(() => binding.xxHash3_64(data)).toThrowError();
});

it('should decode string in utf8', () => {
	const a = binding.xxHash3_64("我好帅");
	const b = binding.xxHash3_64(Buffer.from("我好帅", "utf8"));
	expect(a).toStrictEqual(b);
});
