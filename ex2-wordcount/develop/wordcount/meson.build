project('expalgo-wordcount', 'cpp')

omp = dependency('openmp')
incdir = include_directories('/usr/local/opt/llvm/include/', '/usr/local/opt/libomp/')
executable('wordcount',
	'main.cpp',
	override_options: ['cpp_std=c++17'],
	include_directories : incdir,
	dependencies: omp,
	install: true)
