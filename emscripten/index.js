const loadWasm = require("../build/wasm.js");

let wasmModule;

class XXHashObject {

	constructor() {

	}

	close() {

	}

	copy() {

	}

	update(buffer) {

	}

	digest() {

	}
}

module.exports.init = (url) => {
	const promise = url ? loadWasm({ locateFile: () => url }) : loadWasm();
	return promise.then(instance => { wasmModule = instance; });
};

// module.exports.createXXH32 =
// module.exports.createXXH64 =
// module.exports.createXXH3_64=
// module.exports.createXXH3_128 =
