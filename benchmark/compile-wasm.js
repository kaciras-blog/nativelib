/*
 * emcc 的命令有点长，单独写一个脚本来组装。
 *
 * 【用法】
 * node script/compile-wasm.js [-release] [-safe-heap]
 * 默认会使用调试模式编译。
 */
import { execSync } from "child_process";
import { dirname } from "path";

process.chdir(dirname(import.meta.dirname));

const ARGS = [
	"-o build/wasm.js",
	"-I deps/xxHash",
];

const SWITCHES = {
	ALLOW_MEMORY_GROWTH: 1,
	WASM: 1,
};

const SOURCE_FILES = [
	"benchmark/xxhash.c",
	"deps/xxHash/xxhash.c",
];

if (process.argv.includes("-release")) {
	ARGS.push("-O3");
} else {
	// SAFE_HEAP 与 sanitizer 不兼容
	if (process.argv.includes("-safe-heap")) {
		SWITCHES.SAFE_HEAP = 1;
	} else {
		ARGS.push("-fsanitize=address");

		// fix wasm-ld error
		SWITCHES.TOTAL_MEMORY = 335544320;
	}
	SWITCHES.ASSERTIONS = 1;
	ARGS.push("-g4");
	ARGS.push("--source-map-base build");
}

// =================================================

const switchArgs = Object.entries(SWITCHES)
	.map(([name, value]) => `-s ${name}=${value}`);

const command = ["emcc"];
command.push.apply(command, switchArgs);
command.push.apply(command, ARGS);
command.push.apply(command, SOURCE_FILES);

try {
	execSync(command.join(" "), { stdio: "inherit" });
} catch (e) {
	process.exit(e.status);
}
