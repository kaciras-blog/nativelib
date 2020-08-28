module.exports = {
	testMatch: [
		"**/test/**/*-test.[jt]s?(x)",
	],
	testEnvironment: "node",
	clearMocks: true,
	coverageDirectory: "coverage",
	coverageProvider: "v8",
};
