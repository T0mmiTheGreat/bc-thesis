# bc-thesis

This project is my bachelor thesis.

I am not allowed to collaborate on this project with anyone alse, so I'm keeping this repo read-only.

Why "bc"? It's bc I'm dumb and used the Czech abbr for bachelor (BA)...

## Supported platforms

You should be able to build and run this project on Windows, most Linux distributions and probably even macOS. I managed to build the program on Windows (using the MSYS2 subsystem) and Ubuntu 22.04. Compilation on other platforms shall be possible, God willing.

## Installation

You need these tools:

- Git, GCC, etc.,
- CMake,
- Libraries: [SDL2](https://www.libsdl.org/) (+ satellite libraries: SDL2_ttf, SDL2_image), [CGAL](https://www.cgal.org/), [yaml-cpp](https://github.com/jbeder/yaml-cpp)

1.  Clone this repository using `git clone --recurse-submodules <repo-url>`. If you forgot to use the `--recurse-submodules` flag, you can still run `git submodule init` and then `git submodule update` (or combine these commands by running `git submodule update --init`) to get the submodules. You need them because this project uses the [libSDL2pp](https://github.com/libSDL2pp/libSDL2pp) and [ImGui](https://github.com/ocornut/imgui) libraries as a Git submodules.

2.  `cd` to the "src/" directory and run `cmake .`. It should build the application.

3.  ???

4.  Profit.
