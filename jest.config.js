// For a detailed explanation regarding each configuration property, visit:
// https://jestjs.io/docs/en/configuration.html

module.exports = {
   clearMocks: true,
  coverageDirectory: "coverage",
  coverageProvider: "v8",

  // The root directory that Jest should scan for tests and modules within
  // rootDir: undefined,

  // A list of paths to directories that Jest should use to search for files in
  // roots: [
  //   "<rootDir>"
  // ],

  // The paths to modules that run some code to configure or set up the testing environment before each test
  // setupFiles: [],

  testEnvironment: "node",

  // The glob patterns Jest uses to detect test files
  testMatch: [
    "**/test/**/*-test.[jt]s?(x)",
  ],
};
