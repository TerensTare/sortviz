
# SortViz - A sorting algorithm visualizer written in C++20

This repo contains a program that demonstrates how common sorting algorithms work step by step.


## Building and running

To build and run the repo, you need a C++ compiler that supports C++20, [cmake](https://cmake.org/) (>= 3.25) and [vcpkg](https://github.com/microsoft/vcpkg) to manage the dependencies. On Linux [ninja](https://ninja-build.org/) is needed to configure the project with CMake. Please refer to the corresponding documentation on how to obtain these dependencies.


### 1. Installing the dependencies

The project uses SDL2 for rendering the data being sorted and ImGui to provide a simple control panel within the application. To install these packages, run the following command:

```sh
# Replace {arch} and {os} with what you want to target.
# Run `vcpkg help triplets` for a list of available options.
# Remember the arch for later steps.
# Also run this from a folder outside of the project, as vcpkg won't allow you
# to run the command from there.
vcpkg install sdl2 imgui imgui[sdl2-binding] imgui[sdl2-renderer-binding] --triplet={arch}-{os}
```

### 2. Configuring with CMake

CMake needs to configure the project first before building, which can be done with the following command:

```sh
# Presets are of format {gen}-{arch}, where {gen} and {arch} are as following:
# {gen} - vs2019, vs2022 and ninja
# {arch} - x86, arm64, x64
# eg. to use Visual Studio 2022 to build in x64 mode, run `cmake --preset vs2022-x64`
# Also, for macOS there is the preset xcode (so command would be `cmake --preset xcode`).
# If you are using an IDE/editor, it should automatically list the compatible presets for you.
# Remember to match the arch with the one you installed the vcpkg packages for.
cmake --preset {my-preset}
```

### 3. Building the project

To build the project, run the following command:

```sh
# See `CMakePresets.json` for a list of available presets.
# If you are using an IDE/editor, it should automatically list the compatible ones.
# Remember to match the arch with the one you installed vcpkg packages for.
cmake --build . --preset {my-preset}
```

The project should then be built and you should see where the executable file is located.


## License

The code is licensed under the [MIT license](./LICENSE).

## Contributing

Please open an issue if you notice a bug or have an idea for the project. Thank you!