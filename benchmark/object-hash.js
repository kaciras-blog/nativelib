const { readFileSync } = require("fs");
const { performance } = require("perf_hooks");
const yaml = require("js-yaml");
const hashSum = require("hash-sum");

const xxHash = require("../lib");
const packageJson = require("../package.json");
const packageLock = yaml.load(readFileSync("../pnpm-lock.yaml", "utf8"));

function run(func) {
	// warm up
	for (let i = 0; i < 10; i++) func();

	const start = performance.now();
	for (let i = 0; i < 10; i++) func();
	const end = performance.now();

	console.log(func.name);
	console.log(func());
	console.log(`${(end - start).toFixed(3)} ms\n`);
}

function myImpl() {
	return xxHash.hashSum(packageLock);
}

function sum() {
	return hashSum(packageLock);
}

run(sum);
run(myImpl);
