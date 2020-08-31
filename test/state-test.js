// 测试 XXHash 能否正确的更新和复制
const binding = require("..");

it('should copy state', () => {
	const hash = binding.createXXH3_128().update("xxh");
	const digest = hash.copy().update("ash").digest();

	expect(digest).toStrictEqual(binding.xxHash3_128("xxhash"));
});

it('should isolate instance', () => {
	const hash = binding.createXXH3_128().update("xxh");
	const copy = hash.copy();

	expect(hash.update("ash").digest()).not.toStrictEqual(copy.digest());
});
