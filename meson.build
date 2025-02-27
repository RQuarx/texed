project(
    'texed',
    'cpp',
    version: 'lazy.ass.too.lazy.to.set.it',
    default_options: ['cpp_std=c++23']
)

compile_flags = [
    '-Wall',
    '-Wextra',
    '-Wno-unused-parameter'
]
dependencies = [
   dependency('SDL3_ttf'),
   dependency('SDL3'),
]

src_dir = 'src/'
src_files = files(
    src_dir + 'input_handler.cpp',
    src_dir + 'file_handler.cpp',
    src_dir + 'decoration.cpp',
    src_dir + 'arg_parse.cpp',
    src_dir + 'movement.cpp',
    src_dir + 'cursor.cpp',
    src_dir + 'editor.cpp',
    src_dir + 'utils.cpp',
    src_dir + 'main.cpp'
)

executable(
    'texed',
    src_files,
    cpp_args: compile_flags,
    dependencies: dependencies
)