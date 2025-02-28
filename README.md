# texed
A simple **tex**t **ed**itor made with C++ and SDL3

## REMINDER!
This program is only designed to work on Linux systems, and I have no plans to make this work on any other platforms.

## Installation Instructions

To build this program, ensure that you have the following packages installed on your system:

- **clang**
- **git**
- **SDL3**
- **SDL3_ttf**: Only install if your SDL3 package doesnt include SDL3_ttf
- **fmt**
- **meson**

## How to build

```bash
$ git clone https://github.com/RQuarx/texed/
$ cd texed
$ CXX=clang++ meson setup target
$ meson compile -C target
$ ./target/texed -h # Printing the help message
```

## Configurations

All available configuration options are on `./config.hpp`,
and you can customise the program further by changing the source code

## Links

- **SDL**: https://www.libsdl.org/
- **fmt**: https://github.com/fmtlib/fmt

## License

This program is licensed under the GNU General Public License V3
