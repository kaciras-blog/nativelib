const binding = require("./build/Debug/binding");

// console.log(binding.xxHash3_128("xxhash", "base64"));
// console.log(binding.xxHash3_128("xxhash", "base64-u"));
//
// console.log(binding.xxHash3_128("xxhash", "hex"));
// console.log(binding.xxHash3_128(Buffer.from("xxhash"), "hex"));

module.exports = binding;
