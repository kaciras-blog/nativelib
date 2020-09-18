const Native = require("../a.out");

module.exports.ready = () => new Promise(resolve => Native.onRuntimeInitialized = resolve);

function xxHash3_128(buffer) {

	const inputPointer = Native._createBuffer(buffer.length);
	Native.HEAP8.set(buffer, inputPointer);

	const pointer = Native._xxHash3_128(inputPointer, buffer.length);
	const rv = Buffer.from(Native.HEAP8.buffer, pointer, 16);

	Native._freeIO(inputPointer, pointer);
	return rv;
}

function test() {
	const buf = Buffer.from("xxhash");
	const result = xxHash3_128(buf);
	console.log(result.toString("hex")); // 9c8b437c78cac00a376072e24bfdf4d2
}

// (async function (){
// 	await module.exports.ready();
// 	test();
// 	test();
// })();

module.exports.xxHash3_128 = xxHash3_128;
