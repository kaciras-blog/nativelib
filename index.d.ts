export type XXHashInputData = string | Buffer;

export type XXHashOutEncoding = "latin1" | "hex" | "base64" | "base64u";

/**
 * 跟 NodeJS 的 crypto 模块差不多的API，不过没有继承 stream
 */
export interface XXHash {

	copy(): XXHash;

	update(data: XXHashInputData): XXHash;

	digest(): Buffer;

	digest(encoding: XXHashOutEncoding): string;
}

interface XXHashFactory {

	/** 创建一个 XXHash 的实例 */
	(): XXHash;

	/**
	 * 使用一个密钥来初始化 XXHash。
	 *
	 * @param secret 密钥
	 * @throws 若密钥小于最低长度
	 */
	(secret: Buffer): XXHash;

	/**
	 * 使用种子来初始化 XXHash。
	 *
	 * @param seed 种子
	 */
	(seed: number): XXHash;
}

export const createXXH32: XXHashFactory;
export const createXXH64: XXHashFactory;
export const createXXH3_64: XXHashFactory;
export const createXXH3_128: XXHashFactory;

// =============================================================================
//                       下面是无状态函数，直接计算结果
// =============================================================================

interface XXHashFunction {

	(data: XXHashInputData): Buffer;

	(data: XXHashInputData, encoding: XXHashOutEncoding): string;
}

interface SeedHashFunction extends XXHashFunction {
	(data: XXHashInputData, seed: number): Buffer;

	(data: XXHashInputData, seed: number, encoding: XXHashOutEncoding): string;

	(data: XXHashInputData, secret: Buffer): Buffer;

	(data: XXHashInputData, secret: Buffer, encoding: XXHashOutEncoding): string;
}

export const xxHash32: SeedHashFunction;
export const xxHash64: SeedHashFunction;
export const xxHash3_64: SeedHashFunction;
export const xxHash3_128: SeedHashFunction;
