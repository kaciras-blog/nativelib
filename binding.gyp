{
	"targets": [
		{
			"target_name": "binding",
			"sources": [
				"src/binding.cc",
				"src/encoding.cc"
			],
			"include_dirs": [
				"xxHash",
				"src",
				"<!(node -e \"require('nan')\")"
			]
		}
	]
}