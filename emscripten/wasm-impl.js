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

module.exports.ready = ready;
module.exports.xxHash3_128 = xxHash3_128;
