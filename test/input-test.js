const binding = require("../build/Release/binding");

it('should throw on pass invalid object', () => {
	expect(() => binding.base64F()).toThrowError();
	expect(() => binding.base64F(0)).toThrowError();
});
