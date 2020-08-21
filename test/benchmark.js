/*
 * 因为要对上传的文件做Hash，故测试了常用的一些Hash函数性能。
 *
 * md5              - 57.62 ms
 * sha3_256         - 125.29 ms
 * sha2_256         - 94.65 ms
 * murmurHash3_sync - 5.73 ms
 */
const crypto = require("crypto");
const { performance } = require("perf_hooks");
const { murmurHash128x64 } = require("murmurhash-native");
const binding = require("../build/Release/binding");

const buffer = crypto.randomBytes(4 * 1024 * 1024);

function md5() {
	return crypto.createHash("md5").update(buffer).digest("base64");
}

function sha3_256() {
	return crypto.createHash("sha3-256").update(buffer).digest("base64");
}

function sha2_256() {
	return crypto.createHash("sha256").update(buffer).digest("base64");
}

function murmurHash3_sync() {
	return murmurHash128x64(buffer, "base64");
}

function xxHash3_128() {
	return binding.base64F(buffer);
}

async function test(func) {
	const start = performance.now();
	let result;
	for (let i = 0; i < 100; i++) {
		result = func();
	}
	const end = performance.now();

	console.log("\n" + func.name);
	console.log(result);
	console.log(`${(end - start).toFixed(2)} ms`);
}

test(xxHash3_128);
test(md5);
test(murmurHash3_sync);
test(sha2_256);
test(sha3_256);
