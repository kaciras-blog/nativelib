declare module "*nativelib.node" {

	/**
	 * XXHash 的核心实现，仅支持 Buffer 输入输出，详见 src/xxhash3_128.h
	 */
	class XXHash3_128Core {
		constructor(copyOrSeed?: XXHash3_128Core | number | Buffer);

		protected updateBytes(buffer: Buffer): this;
		protected digestBytes(): Buffer;

		static hash(input: Buffer, seed?: number | Buffer): Buffer;
	}
}
