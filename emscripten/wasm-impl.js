const loadWasmModule = require("../build/wasm.js");

let xxhashWasm;

async function ready() {
	xxhashWasm = await loadWasmModule();
}

function xxHash3_128(buffer) {
	return Buffer.from(xxhashWasm.xxHash3_128(buffer));
}

function xxHash3_128Seed(buffer) {
	return Buffer.from(xxhashWasm.xxHash3_128_Seed(buffer, 1048573));
}

function test() {
	const buf = Buffer.from("xxhash");
	const result = xxHash3_128(buf);
	const result2 = xxHash3_128Seed(buf);

	console.log(result.toString("hex"));
	// 9c8b437c78cac00a376072e24bfdf4d2

	console.log(result2.toString("hex"));
}

(async function () {
	await ready();
	test();
	test();
})();

module.exports.ready = ready;
module.exports.xxHash3_128 = xxHash3_128;
