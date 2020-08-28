export type XXHashInputData = string | Buffer;

export type XXHashOutEncoding = "latin1" | "hex" | "base64" | "base64u";

/** 跟 NodeJS 的 crypto 模块差不多的API，不过没有继承 stream */
export interface XXHash {

	copy(): XXHash;

	update(data: XXHashInputData): XXHash;

	digest(): Buffer;

	digest(encoding: XXHashOutEncoding): string;
}

interface XXHashFactory {
	(): XXHash;

	/**
	 * 使用一个密钥来初始化 XXHash，
	 *
	 * @param secret 密钥
	 */
	(secret: Buffer): XXHash;

	(seed: number): XXHash;
}

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

export const xxHash3_128: SeedHashFunction;
