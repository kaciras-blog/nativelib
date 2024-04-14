/*
 * 提供对构建好的二进制文件的打包和从 GitHub 下载功能，该脚本需要配合 CI 使用。
 *
 * 【为什么不用 https://github.com/prebuild/prebuild】
 * 1）prebuild 无法正确处理包名里的斜杠。
 * 2）prebuild 不支持压缩率更高的 brotli 算法。
 * 3）通过与 CI 整合，使得实现非常简单，prebuild 功能太多反而不好用。
 */
import { execSync } from "child_process";
import { dirname } from "path";
import { createWriteStream, mkdirSync, rmSync } from "fs";
import { Readable } from "stream";
import { createBrotliCompress, createBrotliDecompress } from "zlib";
import tar from "tar-fs";
import packageJson from "../package.json" with { type: "json" };

// 定死工作目录为项目根目录，免得老是去组装路径
process.chdir(dirname(import.meta.dirname));

/**
 * 获取当前环境下的压缩包名，跟 prebuild 的一致。
 */
function getPackageName() {
	const name = packageJson.name.split("/").pop();
	const { version } = packageJson;
	const { platform, arch } = process;

	return `${name}-${version}-${platform}-${arch}.tar.br`;
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
	rmSync("prebuilds", { recursive: true, force: true });
	mkdirSync("prebuilds");

	const pack = tar.pack(".", {
		entries: ["build/Release/nativelib.node"],
	});

	const file = `prebuilds/${getPackageName()}`;
	console.log(`Packing files to ${file}`);

	pack.pipe(createBrotliCompress()).pipe(createWriteStream(file));
}

/**
 * 从 GitHub Release 上下载预编译好的文件并解压。
 * 如果下载失败或没有预构建，则尝试编译。
 */
async function download() {
	const url = `${getGithubRelease()}/${getPackageName()}`;
	let response;

	try {
		response = await fetch(url);
	} catch (e) {
		return handleInstallError(e);
	}

	switch (response.status) {
		case 404:
			return handleInstallError(`没有合适的预编译文件：${url}`);
		case 200:
			return Readable
				.fromWeb(response.body)
				.pipe(createBrotliDecompress())
				.pipe(tar.extract("."));
	}

	handleInstallError(`下载失败（${response.status}）：${url}`);
}

function handleInstallError(error) {
	console.warn(error);
	const cmd = "node-gyp rebuild --ensure";

	try {
		execSync(cmd, { stdio: "inherit" });
	} catch (e) {
		process.exit(e.status);
	}
}

const { NO_PREBUILD } = process.env;
const [, , verb] = process.argv;

if (verb === "install") {
	if (!NO_PREBUILD) {
		download();
	}
} else if (verb === "pack") {
	pack();
} else {
	console.error("Argument required: install or pack");
	process.exit(1);
}
