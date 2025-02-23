project(
    'texed',
    'cpp',
    version: 'Der_Kuchen_ist_eine_Lüge',
    default_options: ['cpp_std=c++23']
)

compile_flags = ['-O3', '-Wall', '-Wextra', '-Wno-unused-parameter']
dependencies = [
   dependency('SDL3'),
   dependency('SDL3_ttf')
]

src_dir = 'src/'
src_files = files(
    src_dir + 'input_handler.cpp',
    src_dir + 'decoration.cpp',
    src_dir + 'arg_parse.cpp',
    src_dir + 'cursor.cpp',
    src_dir + 'editor.cpp',
    src_dir + 'utils.cpp',
    src_dir + 'main.cpp',

)

executable('texed', src_files,
    cpp_args: compile_flags,
    dependencies: dependencies
)