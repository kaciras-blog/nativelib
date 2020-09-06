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

function xxHash32() {
	return binding.createXXH32().update(buffer).digest("base64u");
}

function xxHash64() {
	return binding.createXXH64().update(buffer).digest("base64u");
}

function xxHash3_64() {
	return binding.createXXH3_64().update(buffer).digest("base64u");
}

function xxHash3_128() {
	return binding.createXXH3_128().update(buffer).digest("base64u");
}

function quickXXHash3_128() {
	return binding.xxHash3_128(buffer, "base64u");
}

async function test(func) {
	let result;

	// warm up
	for (let i = 0; i < 100; i++) func();

	const start = performance.now();
	for (let i = 0; i < 100; i++) {
		result = func();
	}
	const end = performance.now();

	console.log("\n" + func.name);
	console.log(result);
	console.log(`${(end - start).toFixed(3)} ms`);
}

test(murmurHash3_sync);

test(xxHash32);
test(xxHash64);
test(xxHash3_64);
test(xxHash3_128);
test(quickXXHash3_128);

test(sha2_256);
test(md5);
test(sha3_256);
