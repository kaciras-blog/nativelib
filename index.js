// 包含扩展名防止某些加载器不能自动识别
const { XXHash3_128Core } = require("./build/Debug/binding.node");

class XXHash3_128 extends XXHash3_128Core {

	copy() {
		return new XXHash3_128(this);
	}

	update(value, encoding) {
		if (typeof value === "string") {
			value = Buffer.from(value, encoding);
		}
		return super.update(value);
	}

	digest(encoding) {
		const buffer = super.digest();
		return encoding ? buffer.toString(encoding) : buffer;
	}
}

module.exports.createXXH3_128 = (seed) => new XXHash3_128(seed);

module.exports.xxHash3_128 = XXHash3_128Core.hash;
