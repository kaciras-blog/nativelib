const { performance } = require("perf_hooks");
const crypto = require("crypto");
const { murmurHash128x64 } = require("murmurhash-native");
const binding = require(".");

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
	return binding.xxHash3_128(buffer, "base64u");
}

async function test(func) {
	const start = performance.now();
	let result;
	for (let i = 0; i < 10; i++) {
		result = func();
	}
	const end = performance.now();

	console.log("\n" + func.name);
	console.log(result);
	console.log(`${(end - start).toFixed(3)} ms`);
}

test(sha2_256);
test(xxHash3_128);
test(md5);
test(murmurHash3_sync);
test(sha3_256);
