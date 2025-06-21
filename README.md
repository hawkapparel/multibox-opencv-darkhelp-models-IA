# wxWidgets CMake Clang Template
---

Download, build and statically link wxWidgets as the GUI library for your C++ project!
Solve the problem of missing wxWidgets headers and libraries in your C++ project.

### Works in VSCode and Windsurf!

### System Requirements

To get started with this project, ensure you have the following tools and libraries:

- **CMake (Version 3.14 or later):** Essential for building and managing the project files. (4.0.2 Tested)
- **clangd C++ Compiler:** https://clangd.llvm.org/installation (20.1.7 Tested)
- **wxWidgets** https://wxwidgets.org/downloads/  (Stable 3.2.8 Tested)

Setup video reference: https://www.youtube.com/watch?v=3NsbeBDtMCc
References: https://github.com/lszl84/wx_cmake_fetchcontent_template

### Building the Project

#### Debug

Follow these steps to build the project:

1. **Create a build directory & configure the build:**
   ```bash
   cmake -S. -Bbuild
   ```

2. **Build the project:**
   ```bash
   cmake --build build -j
   ```

This will create a `build` directory and compile all necessary artifacts there. The main executable will be located in `build/`.

#### Release

For release build use `--config Release` on Windows:

```bash
cmake -S. -Bbuild
cmake --build build -j --config Release
```

Artifacts for both configurations will be generated in the `build` directory.

On Mac or Linux you'll need to maintain two build trees:

```bash
cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
cmake -S. -Bbuild-rel -DCMAKE_BUILD_TYPE=Release
cmake --build build-rel -j
```

#### Ready configuration for VSCode and Windsurf (Run and Debug)
in .vscode/launch.json have ready configurations for Debug and Release.
For Debug: (Debug) Launch
For Release: (msvc Release) Launch

### License

MIT License. Can be used in closed-source commercial products.