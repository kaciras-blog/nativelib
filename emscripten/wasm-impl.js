const Native = require("../build/wasm.js");

module.exports.ready = () => new Promise(resolve => Native.onRuntimeInitialized = resolve);

function xxHash3_128(buffer) {
	const pInput = Native._createBuffer(buffer.length);
	Native.HEAP8.set(buffer, pInput);

	const pHash = Native._xxHash3_128(pInput, buffer.length);
	const hash = Buffer.from(Native.HEAP8.buffer, pHash, 16);

	Native._freeIO(pInput, pHash);
	return hash;
}

// function test() {
// 	const buf = Buffer.from("xxhash");
// 	const result = xxHash3_128(buf);
//
// 	console.log(result.toString("hex"));
// 	// 9c8b437c78cac00a376072e24bfdf4d2
// }
//
// (async function (){
// 	await module.exports.ready();
// 	test();
// 	test();
// })();

module.exports.xxHash3_128 = xxHash3_128;
