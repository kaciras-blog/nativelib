const binding = require("./build/Debug/binding");

// console.log(binding.xxHash3_128("xxhash", "base64"));
// console.log(binding.xxHash3_128("xxhash", "base64-u"));

// console.log(binding.xxHash3_128("xxhash", "hex"));
// console.log(binding.xxHash3_128(Buffer.from("xxhash"), "hex"));

const hash = binding.createXXH3_128();
hash.update("xxhash");
console.log(hash.digest().toString("hex"));
console.log(hash.digest("hex"));

// let h2 = binding.createXXH3_128();
// h2.update("xx");
// h2 = h2.copy().update("hash");
//
global.gc();
// console.log(h2.digest("hex"));

module.exports = binding;
