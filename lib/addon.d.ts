declare module "*binding.node" {

	/**
	 * XXHash 的核心实现，仅支持 Buffer 输入输出，详情见 xxhash3_128.cc
	 */
	class XXHash3_128Core {
		constructor(copy?: XXHash3_128Core | number | Buffer);

		protected updateBytes(buffer: Buffer): this;
		protected digestBytes(): Buffer;

		static hash(input: Buffer, secret?: number | Buffer): Buffer;
	}
}
