module.exports = {
	testMatch: [
		"**/test/*-test.ts",
	],
	moduleFileExtensions: [
		"ts", "js", "json",
	],
	preset: "ts-jest",
	clearMocks: true,
	coverageDirectory: "coverage",
	coverageProvider: "v8",
};
