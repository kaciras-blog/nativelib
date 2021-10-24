// noinspection ES6PreferShortImport

import { BinaryToTextEncoding, Encoding } from "node:crypto";
import { XXHash3_128Core } from "../build/Release/binding.node";

export type InputData = string | Buffer;

/**
 * 无状态函数，直接计算结果
 */
export const xxHash3_128 = XXHash3_128Core.hash;

/**
 * 跟 NodeJS 的 crypto 模块差不多的 API，不过没有继承 stream
 */
class XXHash3_128 extends XXHash3_128Core {

	copy() {
		return new XXHash3_128(this);
	}

	/**
	 * 用字符串更新状态，支持所有 Buffer.from 的编码。
	 *
	 * 注：base64 就可以解码 base64u。
	 *
	 * @param data 字符串数据
	 * @param encoding 编码，省略则为UTF-8
	 */
	update(data: InputData, encoding?: Encoding) {
		if (typeof data === "string") {
			data = Buffer.from(data, encoding);
		}
		return super.updateBytes(data);
	}

	digest(encoding?: BinaryToTextEncoding) {
		const buffer = super.digestBytes();
		return encoding ? buffer.toString(encoding) : buffer;
	}
}

/**
 * 创建一个新的 XXHash3_128 实例，可以指定初始种子。
 *
 * 如果种子是 Buffer 则至少要有 136 bytes。
 *
 * @param seed 种子
 * @throws 如果密钥的长度不足 136 字节
 */
export function createXXH3_128(seed?: number | Buffer) {
	return new XXHash3_128(seed);
}
