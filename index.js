const binding = require("./build/Debug/binding");

console.log(binding.base64F("xxhash"));
console.log(binding.base64F(Buffer.from("xxhash")));
// 9c8b437c78cac00a376072e24bfdf4d2