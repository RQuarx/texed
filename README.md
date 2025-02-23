# texed
A simple editor made with C++ and SDL3

# How to build

If you haven't, install clang to your system

```bash
$ git clone https://github.com/RQuarx/texed/
$ cd texed
$ CXX=clang++ meson setup target
$ meson compile -C target
$ ./target/texed # run the program
```
