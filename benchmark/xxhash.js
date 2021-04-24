const { performance } = require("perf_hooks");
const crypto = require("crypto");
const xxhashjs = require("xxhashjs");
const native = require("..");
// const wasm = require("../emscripten/wasm-impl");

/*
 * 测试一下各种算法，以及两种不同的版本（WASM，Native）的性能。
 *
 * 【结论】
 * xxHash3_128 比 MurMurHash128x64 快 2 倍。
 * WASM 版 xxHash3_128 比本地二进制版慢 5 倍。
 * 加密 Hash 比非加密版慢很多，都是 20 倍以上。
 */

const buffer = crypto.randomBytes(10 * 1024 * 1024);

function md4() {
	return crypto.createHash("md4").update(buffer).digest("base64");
}

function md5() {
	return crypto.createHash("md5").update(buffer).digest("base64");
}

function sha3_256() {
	return crypto.createHash("sha3-256").update(buffer).digest("base64");
}

function sha2_256() {
	return crypto.createHash("sha256").update(buffer).digest("base64");
}

function xxHash32() {
	return native.createXXH32().update(buffer).digest("base64u");
}

function xxHash64() {
	return native.createXXH64().update(buffer).digest("base64u");
}

function xxHash3_64() {
	return native.createXXH3_64().update(buffer).digest("base64u");
}

function xxHash3_128() {
	return native.createXXH3_128().update(buffer).digest("base64u");
}

function quickXXHash3_128() {
	return native.xxHash3_128(buffer, "base64u");
}

// function xxHash3_128Wasm() {
// 	return wasm.xxHash3_128(buffer).toString("base64");
// }

function xxhashjs32() {
	return xxhashjs.h32(buffer, 0).toString(16);
}

function xxhashjs64() {
	return xxhashjs.h64(buffer, 0).toString(16);
}

function run(func) {
	// warm up
	for (let i = 0; i < 10; i++) func();

	const start = performance.now();
	for (let i = 0; i < 10; i++) func();
	const end = performance.now();

	console.log("\n" + func.name);
	console.log(func());
	console.log(`${(end - start).toFixed(3)} ms`);
}

async function runBenchmarks() {
	// await wasm.ready();

	run(xxHash32);
	run(xxHash64);
	run(xxHash3_64);
	run(xxHash3_128);
	run(quickXXHash3_128);
	// run(xxHash3_128Wasm);

	run(xxhashjs32);
	run(xxhashjs64);

	run(sha2_256);
	run(md4);
	run(md5);
	run(sha3_256);
}

runBenchmarks().catch(e => console.error(e));
