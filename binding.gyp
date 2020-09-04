{
	"targets": [
		{
			"target_name": "binding",
			"sources": [
				"src/binding.cc",
				"src/xxhash_class.cc",
				"src/encoding.cc"
			],
			"include_dirs": [
				"deps/xxHash",
				"src",
				"<!(node -e \"require('nan')\")"
			]
		}
	]
}
