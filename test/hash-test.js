const binding = require("..");

// 测试 Hash 结果是否正确

test.each([
	["xxhash", "9a95b70e"],
	["", "02cc5d05"]
])("xxHash32 - %#", (input, hex) =>{
	expect(binding.xxHash32(input, "hex")).toBe(hex);
	expect(binding.createXXH32().update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", "32dd38952c4bc720"],
	["", "ef46db3751d8e999"]
])("xxHash64 - %#", (input, hex) =>{
	expect(binding.xxHash64(input, "hex")).toBe(hex);
	expect(binding.createXXH64().update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", "aa4c2b42ae6b13de"],
	["", "2d06800538d394c2"]
])("xxHash3_64 - %#", (input, hex) =>{
	expect(binding.xxHash3_64(input, "hex")).toBe(hex);
	expect(binding.createXXH3_64().update(input).digest("hex")).toBe(hex);
});

test.each([
	["xxhash", "9c8b437c78cac00a376072e24bfdf4d2"],
	["", "99aa06d3014798d86001c324468d497f"]
])("xxHash3_128 - %#", (input, hex) =>{
	expect(binding.xxHash3_128(input, "hex")).toBe(hex);
	expect(binding.createXXH3_128().update(input).digest("hex")).toBe(hex);
});
