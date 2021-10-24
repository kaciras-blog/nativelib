{
	"targets": [{
		"target_name": "nativelib",
		"include_dirs": [
			'<!(node -p "require(`node-addon-api`).include_dir")',
			"src",
			"deps/xxHash"
		],
		"sources": [
			"src/binding.cc",
			"src/xxhash3_128.cc"
		],
		'cflags!': ['-fno-exceptions'],
		'cflags_cc!': ['-fno-exceptions'],
		'conditions': [
			["OS=='win'", {
				"defines": ["_HAS_EXCEPTIONS=1"],
				"msvs_settings": {
					"VCCLCompilerTool": {
						"ExceptionHandling": 1
					},
				},
			}],
			["OS=='mac'", {
				'xcode_settings': {
					'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
					'CLANG_CXX_LIBRARY': 'libc++',
					'MACOSX_DEPLOYMENT_TARGET': '10.7',
				},
			}],
		],
	}
]}
