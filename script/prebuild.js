/*
 * 提供对构建好的二进制文件的打包和从 GitHub 下载功能，该脚本需要配合 Travis CI 使用。
 *
 * 【为什么不用 https://github.com/prebuild/prebuild】
 * 1）prebuild 把 @xxx/xxx 这样的包名直接用做文件名导致路径错乱。
 * 2）prebuild 不支持压缩率更高的 brotli 算法。
 * 3）通过与 CI 整合，自己实现一个也不难，prebuild 功能太多不好用。
 */
const { execSync } = require("child_process");
const { join } = require("path");
const fs = require("fs");
const zlib = require("zlib");
const { https } = require("follow-redirects");
const tar = require("tar-fs");
const packageJson = require("../package.json");

// 定死工作目录为项目根目录，免得下面老是去组装路径
process.chdir(join(__dirname, ".."));

const [, , COMMAND, ...ARGS] = process.argv;

/**
 * 获取当前环境下的压缩包名，跟 prebuild 的一致。
 */
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

/**
 * 打包编译好的二进制文件，并使用 brotli 算法压缩（brotli 比 gzip 压缩率高20%）。
 * 注意本命令只打包，不负责编译和上传，这两个步骤需要 CI 来完成。
 */
function pack() {
	fs.rmSync("prebuilds", { recursive: true, force: true });
	fs.mkdirSync("prebuilds");

	const pack = tar.pack(".", {
		entries: ["build/Release/binding.node"]
	});

	const file = `prebuilds/${getPackageName()}`;
	console.log(`Packing files to ${file}`);

	pack.pipe(zlib.createBrotliCompress()).pipe(fs.createWriteStream(file));
}

/**
 * 从 GitHub Release 上下载预编译好的文件并解压。
 */
function download() {
	const url = `${getGithubRelease()}/${getPackageName()}`;
	const request = https.get(url);

	request.on("response", response => {
		const { statusCode } = response;

		if (statusCode === 404) {
			return handleInstallError(`没有合适的预编译文件：${url}`);
		} else if (statusCode !== 200) {
			return handleInstallError(`下载失败（${statusCode}）：${url}`);
		}

		response
			.pipe(zlib.createBrotliDecompress())
			.pipe(tar.extract("."));
	});

	request.on("error", handleInstallError).end();
}

function handleInstallError(error) {
	console.warn(error);

	try {
		const cmd = "node-gyp rebuild --ensure";
		execSync(cmd, { stdio: "inherit" });
	} catch (e) {
		process.exit(e.status);
	}
}

if (process.env.NO_PREBUILD) {
	// skip install prebuilt on CI
} else if (COMMAND === "install") {
	download();
} else if (COMMAND === "pack") {
	pack();
} else {
	console.error("Argument required: install or pack");
	process.exit(1);
}
