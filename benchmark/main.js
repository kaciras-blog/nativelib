import { performance } from "perf_hooks";
import crypto from "crypto";
import xxhashjs from "xxhashjs";
import { murmurHash128x64 } from "murmurhash-native";
import webpackImpl from "../deps/webpack/xxh64.js";
import nativelib from "../lib/index.js";
import wasm from "../build/wasm.js";

function wasmImpl(buffer) {
	const pInput = wasm._createBuffer(buffer.length);
	wasm.HEAP8.set(buffer, pInput);

	const pHash = wasm._xxHash3_128(pInput, buffer.length);
	const hash = Buffer.from(wasm.HEAP8.buffer, pHash, 16);

	wasm._freeIO(pInput, pHash);
	return hash;
}

/*
 * 测试一下各种算法，以及两种不同的版本（WASM，Native）的性能。
 *
 * 【结论】
 * xxHash3_128 比 MurMurHash128x64 快 2 倍。
 * WASM 版 xxHash3_128 比二进制版慢 2 倍。
 * 加密 Hash 比非加密版慢很多，都是 18 倍以上。
 */

const buffer = crypto.randomBytes(10 * 1024 * 1024);

function xxHash3_128() {
	return nativelib.createXXH3_128().update(buffer).digest();
}

function quickXXHash3_128() {
	return nativelib.xxHash3_128(buffer);
}

function xxHash3_128Wasm() {
	return wasmImpl(buffer);
}

function murmurHash3() {
	return murmurHash128x64(buffer);
}

function xxHash64_webpack() {
	return webpackImpl().update(buffer).digest();
}

function xxhashjs32() {
	return xxhashjs.h32(buffer, 0);
}

function xxhashjs64() {
	return xxhashjs.h64(buffer, 0);
}

// 箭头函数的 name 不能改，所以用了点骚操作。
function nodeCrypto(name) {
	return { [name]: () => crypto.createHash(name).update(buffer).digest() }[name];
}

function run(func) {
	// warm up
	for (let i = 0; i < 10; i++) func();

	const start = performance.now();
	for (let i = 0; i < 10; i++) func();
	const end = performance.now();

	console.log(func.name);
	console.log(func());
	console.log(`${(end - start).toFixed(3)} ms\n`);
}

wasm.onRuntimeInitialized = () => {
	run(xxHash3_128);
	run(quickXXHash3_128);

	run(xxHash3_128Wasm);
	run(xxHash64_webpack);

	run(murmurHash3);

	run(xxhashjs32);
	run(xxhashjs64);

	try {
		crypto.createHash("md4");
		run(nodeCrypto("md4"));
	} catch {
		console.log("当前的 Node 不支持 MD4\n");
	}

	run(nodeCrypto("md5"));
	run(nodeCrypto("sha3-256"));
	run(nodeCrypto("sha256"));
};
