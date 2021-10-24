module.exports = {
	extends: [
		"@kaciras/core",
		"@kaciras/typescript",
	],
	env: {
		node: true,
	},
	overrides: [
		{
			files: require("./jest.config").testMatch,
			extends: ["@kaciras/jest"],
		},
	],
};
