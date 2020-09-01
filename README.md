Kaciras 博客的 Node 本地扩展，包含：

- [xxHash](https://github.com/Cyan4973/xxHash) 算法的Node扩展，支持（XXH32, XXH64, XXH3, XXH128）

# 安装

安装前需要配置编译环境，详见 [https://github.com/nodejs/node-gyp#installation](https://github.com/nodejs/node-gyp#installation)

```shell script
yarn add  
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
