import core from "@kaciras/eslint-config-core";
import typescript from "@kaciras/eslint-config-typescript";
import jest from "@kaciras/eslint-config-jest";
import jestConfig from "./jest.config.js";

export default [
	{ ignores: ["{build,deps,coverage}/**", "{lib,test}/*.js"] },
	...core,
	...typescript,
	...jest.map(config => ({ ...config, files: jestConfig.testMatch })),
]
