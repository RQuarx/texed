project(
    'texed',
    'cpp',
    version: 'lazy.ass.too.lazy.to.set.it',
    default_options: ['cpp_std=c++23']
)

compile_flags = [
    '-O3',
    '-Wall',
    '-Wextra',
    '-Wno-unused-parameter'
]

inc_dir = include_directories('inc')
src_dir = 'src/'
src_files = files(
    src_dir + 'input_handler.cpp',
    src_dir + 'file_handler.cpp',
    src_dir + 'decoration.cpp',
    src_dir + 'arg_parse.cpp',
    src_dir + 'log_utils.cpp',
    src_dir + 'movement.cpp',
    src_dir + 'cursor.cpp',
    src_dir + 'editor.cpp',
    src_dir + 'utils.cpp',
    src_dir + 'main.cpp'
)

dependencies = [
   dependency('SDL3_ttf'),
   dependency('SDL3'),
   dependency('fmt'),
]

executable(
    'texed',
    src_files,
    include_directories: inc_dir,
    cpp_args: compile_flags,
    dependencies: dependencies
)