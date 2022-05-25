const { readFileSync } = require("fs");
const { performance } = require("perf_hooks");
const yaml = require("js-yaml");
const hashSum = require("hash-sum");

const xxHash = require("../lib");
const packageJson = require("../package.json");
const packageLock = yaml.load(readFileSync("../pnpm-lock.yaml", "utf8"));

// 结论：hash-sum 最快，本地实现只略比 JS 快 10%，不值得。

function run(func) {
	// warm up
	for (let i = 0; i < 100; i++) func();

	const start = performance.now();
	for (let i = 0; i < 100; i++) func();
	const end = performance.now();

	console.log(func.name);
	console.log(func());
	console.log(`${(end - start).toFixed(3)} ms\n`);
}

function sum() {
	return hashSum(packageLock);
}

function myImpl() {
	return xxHash.hashSum(packageLock).toString("base64url");
}

run(sum);
run(myImpl);
