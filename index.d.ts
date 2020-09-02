export type InputData = string | Buffer;

/**
 * 输出的字符串编码，兼容 crypto 的 HexBase64Latin1Encoding.
 *
 * base64u 是 base64 算法替换了 '/' 和 '+' 的变体。
 * @see https://tools.ietf.org/html/rfc4648#section-5
 */
export type OutputEncoding = "latin1" | "hex" | "base64" | "base64u";

/**
 * 跟 NodeJS 的 crypto 模块差不多的 API，不过没有继承 stream
 */
export interface XXHash {

	copy(): XXHash;

	/**
	 * 用字符串更新状态，支持所有 Buffer.from 的编码。
	 *
	 * 注：base64 就可以解码 base64u。
	 *
	 * @param data 字符串数据
	 * @param encoding 编码，省略则为UTF-8
	 */
	update(data: string, encoding?: BufferEncoding): XXHash;

	update(data: Buffer): XXHash;

	digest(): Buffer;

	digest(encoding: OutputEncoding): string;
}

interface XXHashFactory {

	/**
	 * 创建一个 XXHash 的实例，使用默认的种子。
	 */
	(): XXHash;

	/**
	 * 使用种子来初始化 XXHash。
	 *
	 * @param seed 种子
	 */
	(seed: number): XXHash;
}

interface SecretXXHashFactory extends XXHashFactory {

	/**
	 * 使用一个密钥作为种子初始化 xxHash。
	 *
	 * xxHash3 要求密钥至少有 136 bytes。
	 *
	 * @param secret 密钥
	 */
	(secret: Buffer): XXHash;
}

export const createXXH32: XXHashFactory;
export const createXXH64: XXHashFactory;
export const createXXH3_64: SecretXXHashFactory;
export const createXXH3_128: SecretXXHashFactory;

// =============================================================================
//                       下面是无状态函数，直接计算结果
// =============================================================================


interface XXHashFunction {

	/**
	 * 不支持指定输入编码因为会跟 XXHashOutEncoding 混淆，若是 string 则使用 utf8 编码。
	 * 若要需要不同的编码可以使用 Buffer.from(str, "utf16")。
	 */
	(data: InputData): Buffer;

	(data: InputData, encoding: OutputEncoding): string;

	(data: InputData, seed: number): Buffer;

	(data: InputData, seed: number, encoding: OutputEncoding): string;
}

interface SecretXXHashFunction extends XXHashFunction {

	(data: InputData, secret: Buffer): Buffer;

	(data: InputData, secret: Buffer, encoding: OutputEncoding): string;
}

export const xxHash32: XXHashFunction;
export const xxHash64: XXHashFunction;
export const xxHash3_64: SecretXXHashFunction;
export const xxHash3_128: SecretXXHashFunction;
