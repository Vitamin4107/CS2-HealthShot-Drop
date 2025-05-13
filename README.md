## System Requirements
- **Operating System / ОС:** Windows (64-bit; tested only on Windows 10/11)
  
- **Tools:**  
   Microsoft Visual Studio 2022 (with cl.exe, rc.exe, link.exe), required Windows-based build tools.
  
- **Dependencies**  
  - [MetaMod:Source](https://github.com/alliedmodders/metamod-source) (zLib/libpng license)
  - [Xbyak](https://github.com/herumi/xbyak/tree/master?tab=readme-ov-file) (BSD-3-Clause License)
  - [CMake](https://github.com/Kitware/CMake?tab=readme-ov-file) (BSD-3-Clause License)


## How to Compile the Plugin
### Repository Setup

1. **Cloning the Repository:**
  ```bash
  git clone https://github.com/YourUsername/CS2-HealthShot-Drop.git
  ```
2. **(Optional) Updating Submodules:**
  If you will want use submodules:
  ```bash
  git submodule update --init --recursive
  ```

This project has been fully configured to use CMake as its build system. All build settings, including dependency paths and compiler/linker options, are defined in the CMakeLists.txt file.

3. **Build**

Open a Command Prompt: For best results, use the Developer Command Prompt for Visual Studio.

Configure the Project:

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```
Build the Project:

```bash
cmake --build build --config Release
```
This command builds the project in the Release configuration. The output DLL will be located inside the generated build output directory (for example, in build/bin).


## License
Please refer to the included LICENSE files for:
  - The main project license (BSD)
  - Third-party licenses for MetaMod:Source (zLib/libpng) and Xbyak (BSD-3-Clause).
