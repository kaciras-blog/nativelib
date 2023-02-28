[![Build Status](https://travis-ci.org/kaciras-blog/nativelib.svg?branch=master)](https://travis-ci.org/kaciras-blog/nativelib)
[![codecov](https://codecov.io/gh/kaciras-blog/nativelib/branch/master/graph/badge.svg?token=XBI5620UXW)](https://codecov.io/gh/kaciras-blog/nativelib)

Kaciras 博客的 Node 本地扩展，包含了一些需要在底层实现的功能。

## xxHash

[xxHash](https://github.com/Cyan4973/xxHash) 是一个非加密 Hash 函数系列，拥有很快的运算速度。本项包含 xxHash3_128 算法的 Node 扩展。

# 安装

```shell script
npm i @kaciras-blog/nativelib
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

console.log(xxHash3_128("xxhash", "base64url")); // nItDfHjKwAo3YHLiS_300g
```

xxHash 算法非常快，故没有提供异步的版本。

## hashSum

计算一个对象的 Hash，底层使用 xxHash3_128 算法，返回 16 字节的 buffer 对象。该函数可以作为 [hash-sum](https://github.com/bevacqua/hash-sum) 的替代。

```javascript
const { hashSum } = require("@kaciras-blog/nativelib");

const hash = hashSum({
	a: 12345,
	b: null,
	c: ["foo", "bar"],
	d: { foo: true },
});

console.log(hash.toString("base64url")); // n2mK3P2WyGed560H01IaTA
```
