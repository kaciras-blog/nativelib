module.exports = {
	env: {
		node: true,
		commonjs: true,
		es2021: true
	},
	extends: "eslint:recommended",
	parserOptions: {
		ecmaVersion: 12
	},
	rules: {
		"indent": ["error", "tab"],
		"quotes": ["error", "double", { avoidEscape: true }],
		"semi": ["error", "always"],
		"no-unused-vars": ["off"],
		"linebreak-style": ["error", "unix"],
	}
};
