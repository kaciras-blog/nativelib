Kaciras 博客的 Node 本地扩展，包含了一些需要在底层实现的功能。

## xxHash

[xxHash](https://github.com/Cyan4973/xxHash) 是一个非加密 Hash 函数系列，拥有很快的运算速度。

本项包含 xxHash 算法的Node扩展，支持（XXH32, XXH64, XXH3, XXH128）。

# 安装

安装前需要配置编译环境，详见 [https://github.com/nodejs/node-gyp#installation](https://github.com/nodejs/node-gyp#installation)

```shell script
yarn add git://github.com/kaciras-blog/nativelib#v0.1.1
```

# 用法

## xxHash

使用跟 crypto 模块相似的 API：

```javascript
const { createXXH3_128 } = require("xxhash-native");

console.log(createXXH3_128().update("xxhash").digest("hex"));
// 9c8b437c78cac00a376072e24bfdf4d2
```

使用快捷函数：

```javascript
const { xxHash3_128 } = require("xxhash-native");

// base64u 是 Base 64 Encoding with URL and Filename Safe Alphabet
console.log(xxHash3_128("xxhash", "base64u"));
// nItDfHjKwAo3YHLiS_300g==
```
