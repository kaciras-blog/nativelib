import { BinaryToTextEncoding, Encoding } from "crypto";

const binding = require("../build/Release/nativelib.node");

// 扩展的类型定义不是很好办，只能这样搞个变量然后设类型。
const XXHash3_128Core = binding.XXHash3_128Core as typeof XXHash3_128CoreType;

/** xxHash3 的种子类型，如果是 Buffer 则还有长度限制 */
type Seed = number | Buffer;

/**
 * XXHash 的核心实现，仅支持 Buffer 输入输出，详见 src/xxhash3_128.h
 */
declare class XXHash3_128CoreType {

	constructor(value?: XXHash3_128CoreType | Seed);

	protected updateBytes(buffer: Buffer | number): this;

	protected digestBytes(): Buffer;

	static hash(input: Buffer, seed?: Seed): Buffer;
}

/**
 * 跟 NodeJS 的 crypto 模块差不多的 API，不过没有继承 stream
 */
export class XXHash3_128 extends XXHash3_128Core {

	copy() {
		return new XXHash3_128(this);
	}

	/**
	 * 跟 crypto.Hash 的签名一致，不过 encoding 参数可以省略。
	 *
	 * <h2>为什么不用重载</h2>
	 * 因为 TS 内部实现问题，重载函数无法处理联合类型的参数，所以没办法这么写：
	 *
	 * @example
	 * update(data: Buffer): this;
	 * update(data: number): this;
	 * update(data: string, encoding?: Encoding): this;
	 *
	 * 不用重载的话会多出 (Buffer|number) + Encoding 的无效组合。
	 * 相关 Issue 6 年都没有解决：
	 * https://github.com/microsoft/TypeScript/issues/1805
	 *
	 * @param data 要更新的数据
	 * @param encoding 数据的编码，仅 data 为 string 时有效，默认为 utf8
	 */
	update(data: string | Buffer | number, encoding?: Encoding) {
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
 * @throws 如果种子是 Buffer 类型且长度不足 136 字节
 */
export function createXXH3_128(seed?: Seed) {
	return new XXHash3_128(seed);
}

/**
 * 无状态函数，直接计算结果，仅支持 Buffer 类型参数和输出。
 */
export const xxHash3_128 = XXHash3_128Core.hash;

/**
 * 计算一个对象的 Hash，使用 xxHash3_128 算法，返回 base64url 编码的字符串。
 * 对象可以是
 */
export function hashSum(value: any, seed?: Seed) {
	const hash = createXXH3_128(seed);
	const seen = new Map<unknown, number>();
	foldValue(value);
	return hash.digest();

	function foldObject(obj: any) {
		for (const key of Object.keys(obj).sort()) {
			hash.update(key);
			foldValue(obj[key]);
		}
	}

	function foldValue(value: any) {
		const type = typeof value;

		if (type === "function" || type === "symbol") {
			throw new TypeError("value is not hash-able");
		}

		if (value === null) {
			hash.update(0x13F76B4F7FCE);
		} else if (type === "object") {
			const id = seen.get(value);
			if (id === undefined) {
				seen.set(value, seen.size);
				foldObject(value);
			} else {
				hash.update(id);
				hash.update(0x4EB6440D);
			}
		} else if (type === "number") {
			hash.update(value);
		} else {
			hash.update(type).update(String(value));
		}
	}
}
