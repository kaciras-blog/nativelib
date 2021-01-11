Kaciras 博客的 Node 本地扩展，包含了一些需要在底层实现的功能。

[![Build Status](https://travis-ci.org/kaciras-blog/nativelib.svg?branch=master)](https://travis-ci.org/kaciras-blog/nativelib)

## xxHash

[xxHash](https://github.com/Cyan4973/xxHash) 是一个非加密 Hash 函数系列，拥有很快的运算速度。本项包含 xxHash 算法的 Node 扩展，支持（XXH32, XXH64, XXH3, XXH128）。

# 安装

```shell script
yarn add @kaciras-blog/nativelib [--no-prebuild]
```

默认情况将从 GitHub Release 上下载编译好的二进制文件，如果需要自己编译请添加`--no-prebuild`参数。

# 用法

## xxHash

使用跟 crypto 模块相似的 API：

```javascript
const { createXXH3_128 } = require("@kaciras-blog/nativelib");

console.log(createXXH3_128().update("xxhash").digest("hex"));
// 9c8b437c78cac00a376072e24bfdf4d2
```

使用快捷函数：

```javascript
const { xxHash3_128 } = require("@kaciras-blog/nativelib");

// base64u 是 Base 64 Encoding with URL and Filename Safe Alphabet
console.log(xxHash3_128("xxhash", "base64u"));
// nItDfHjKwAo3YHLiS_300g==
```

xxHash 算法非常快，故没有提供异步的版本。
