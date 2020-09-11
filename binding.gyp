{
	"targets": [
		{
			"target_name": "binding",
			"sources": [
				"src/binding.cc",
				"src/encoding.cc",
				"src/xxhash_api.cc",
				"src/xxhashes.cc"
			],
			"include_dirs": [
				"deps/xxHash",
				"src",
				"<!(node -e \"require('nan')\")"
			]
		}
	]
}
