# Define the project
project('texed', 'cpp', version: '0.1.0', default_options: ['cpp_std=c++17'])

# Compiler options
compile_flags = ['-O3', '-Wall', '-Wextra', '-Wno-unused-parameter']
dependencies = [
  dependency('SDL3'),
  dependency('SDL3_ttf')
]

# Source directory
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

# Create the executable
executable('texed', src_files,
  cpp_args: compile_flags,
  dependencies: dependencies
)

# Optionally, you can define a custom target for cleaning
# However, Meson handles cleaning automatically, so this is not necessary.