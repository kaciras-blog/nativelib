/*
 * 提供对构建好的二进制文件的打包和从 GitHub 下载功能，该脚本需要配合 Travis CI 使用。
 *
 * prebuild（https://github.com/prebuild/prebuild）用的我各种难受索性自己写一个算了。
 */
const { join } = require("path");
const fs = require("fs");
const zlib = require("zlib");
const { https } = require("follow-redirects");
const tar = require("tar-fs");
const packageJson = require("../package.json");

// 定死工作目录为项目根目录，免得下面老是去组装路径
process.chdir(join(__dirname, ".."));

function handleError(error) {
	console.error(error);
	process.exit(2);
}

function getPackageName() {
	const name = packageJson.name.split("/").pop();
	const { version } = packageJson;
	const runtime = "node";
	const abi = process.versions.modules;
	const { platform, arch } = process;

	return `${name}-v${version}-${runtime}-v${abi}-${platform}-${arch}.tar.br`;
}

function getGithubRelease() {
	const re = new RegExp('github.com/([^/"]+)/([^/"]+)');
	const { version, repository } = packageJson;

	const match = re.exec(JSON.stringify(repository));
	if (!match) {
		throw new Error("找不到 GitHub 地址，请设置 package.json 的 repository 属性");
	}

	return `https://${match[0]}/releases/download/v${version}`.replace(/\.git$/, "");
}

function pack() {
	fs.rmdirSync("prebuilds", { recursive: true });
	fs.mkdirSync("prebuilds");

	const pack = tar.pack(".", {
		entries: ["build/Release/binding.node"]
	});

	pack.pipe(zlib.createBrotliCompress())
		.pipe(fs.createWriteStream(`prebuilds/${getPackageName()}`));
}

function download() {
	const url = `${getGithubRelease()}/${getPackageName()}`;
	const request = https.get(url);

	request.on("response", response => {
		if (response.statusCode !== 200) {
			console.error(`无法从 GitHub 下载预编译的文件：${url}`);
			process.exit(3);
		}
		response
			.pipe(zlib.createBrotliDecompress())
			.pipe(tar.extract("."));
	});

	request.on("error", handleError).end();
}

if (process.argv.includes("--no-prebuild")) {
	// skip install prebuild on CI
} else if (process.argv.includes("install")) {
	download();
} else if (process.argv.includes("pack")) {
	pack();
} else {
	console.error("Argument required: -install or -pack");
	process.exit(1);
}
