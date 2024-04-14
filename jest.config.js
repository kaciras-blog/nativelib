export default {
	testMatch: [
		"**/test/*-test.ts",
	],
	moduleFileExtensions: [
		"ts", "js", "json",
	],
	preset: "ts-jest/presets/default-esm",
	clearMocks: true,
	coverageDirectory: "coverage",
	coverageProvider: "v8",
};
