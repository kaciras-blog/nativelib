export default {
	preset: "ts-jest/presets/default-esm",
	testMatch: ["**/test/*-test.ts"],
	clearMocks: true,
	coverageDirectory: "coverage",
	coverageProvider: "v8",
	moduleFileExtensions: ["ts", "js", "json"],
};
