module.exports = {
	extends: ["@kaciras/core"],
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
