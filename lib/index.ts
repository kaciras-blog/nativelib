import { BinaryToTextEncoding, Encoding } from "crypto";
import { XXHash3_128Core } from "../build/Release/nativelib.node";

/**
 * 跟 NodeJS 的 crypto 模块差不多的 API，不过没有继承 stream
 */
class XXHash3_128 extends XXHash3_128Core {

	copy() {
		return new XXHash3_128(this);
	}

	update(data: string | Buffer, encoding?: Encoding) {
		if (typeof data === "string") {
			data = Buffer.from(data, encoding);
		}
		return super.updateBytes(data) as this;
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
 * @throws 如果种子是 Buffer 类型且长度不足 136 字节
 */
export function createXXH3_128(seed?: number | Buffer) {
	return new XXHash3_128(seed);
}

/**
 * 无状态函数，直接计算结果，仅支持 Buffer 类型参数和输出。
 */
export const xxHash3_128 = XXHash3_128Core.hash;
