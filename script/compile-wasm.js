/*
 * emcc 的命令有点长，单独写一个脚本来组装。
 *
 * 【用法】
 * node script/compile-wasm.js [-release]
 * 默认不加 -release 参数会使用调试模式编译。
 */
const { execSync } = require("child_process");
const { join } = require("path");

process.chdir(join(__dirname, ".."));

const ARGS = [
	"--bind",
	"-o build/wasm.js",
	"-I deps/xxHash",
];

const FLAGS = {
	ALLOW_MEMORY_GROWTH: true,
	MODULARIZE: true,
};

const SOURCE = [
	"emscripten/xxhash.cpp",
	"deps/xxHash/xxhash.c",
];

if (process.argv.includes("-release")) {
	ARGS.push("-O2");
	ARGS.push("--closure 1");
} else {
	ARGS.push("-g4");
	ARGS.push("--source-map-base build");
	FLAGS.ASSERTIONS = true;
	FLAGS.SAFE_HEAP = true;
}

// =================================================

const flagCommands = Object.keys(FLAGS)
	.map(flag => `-s ${flag}=${FLAGS[flag] ? 1 : 0}`);

const command = ["emcc"];
command.push.apply(command, ARGS);
command.push.apply(command, flagCommands);
command.push.apply(command, SOURCE);

try {
	execSync(command.join(" "), { stdio: "inherit" });
} catch (e) {
	process.exit(e.status);
}
