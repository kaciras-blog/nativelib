const binding = require("../build/Release/nativelib.node");

export const XXHash3_128Core = binding.XXHash3_128Core as typeof XXHash3_128Native;

/**
 * XXHash 的核心实现，仅支持 Buffer 输入输出，详见 src/xxhash3_128.h
 */
declare class XXHash3_128Native {

	constructor(value?: XXHash3_128Native | number | Buffer);

	protected updateBytes(buffer: Buffer): this;

	protected digestBytes(): Buffer;

	static hash(input: Buffer, seed?: number | Buffer): Buffer;
}
