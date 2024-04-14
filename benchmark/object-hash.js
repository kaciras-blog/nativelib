import { readFileSync } from "fs";
import { performance } from "perf_hooks";
import yaml from "js-yaml";
import hashSum from "hash-sum";

// import xxHash from "../lib";
// import packageJson from "../package.json";
const packageLock = yaml.load(readFileSync("../pnpm-lock.yaml", "utf8"));

export function hashSumMyImpl(value) {
	let hi = 0;
	let lo = 0;

	const seen = new Map();
	foldValue(value);

	const buf = new Uint32Array(2);
	buf[0] = hi;
	buf[1] = lo;
	return Buffer.from(buf.buffer, buf.byteOffset, 8);

	function foldString(text) {
		for (let i = 0; i < text.length; i++) {
			const chr = text.charCodeAt(i);
			lo = ((lo << 5) - lo) + chr;
		}
	}

	function foldObject(obj) {
		if (Array.isArray(value)) {
			hi ^= 0x3A9268;
		}
		for (const key of Object.keys(obj).sort()) {
			foldString(key);
			foldValue(obj[key]);
		}
	}

	function foldValue(value) {
		const type = typeof value;

		if (type === "function" || type === "symbol") {
			throw new TypeError("value is not hash-able");
		}

		if (value === null) {
			hi ^= 0x13F76B4F7FCE;
		} else if (type === "object") {
			const id = seen.get(value);
			if (id === undefined) {
				seen.set(value, seen.size);
				foldObject(value);
			} else {
				hi ^= id;
			}
		} else if (type === "number") {
			hi ^= 0x55667788;
			lo ^= value;
		} else {
			foldString(type);
			foldString(String(value));
		}
	}
}

// 结论：hash-sum 最快，本地实现只略比 JS 快 10%，不值得。

function run(func) {
	// warm up
	for (let i = 0; i < 100; i++) func();

	const start = performance.now();
	for (let i = 0; i < 100; i++) func();
	const end = performance.now();

	console.log(func.name);
	// console.log(func());
	console.log(`${(end - start).toFixed(3)} ms\n`);
}

function sum() {
	return hashSum(packageLock);
}

function myImpl() {
	return hashSumMyImpl(packageLock).toString("base64url");
}

function jsonSerialize() {
	return JSON.stringify(packageLock);
}

run(jsonSerialize);
run(sum);
run(myImpl);
