## System Requirements
- **Operating System / ОС:** Windows (64-bit; tested only on Windows 10/11)
  
- **Tools:**  
   Microsoft Visual Studio 2022 (with cl.exe, rc.exe, link.exe), required Windows-based build tools.
  
- **Dependencies**  
  - [MetaMod:Source](https://github.com/alliedmodders/metamod-source) (zLib/libpng license)
  - [Xbyak](https://github.com/herumi/xbyak/tree/master?tab=readme-ov-file) (BSD-3-Clause License)

---

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
### Starting the Build

Before starting the build, please review the `build_config.ini` file. This file contains essential settings for the build process. Make sure to update any paths or settings as needed for your environment.

## Configuration in `build_config.ini`

- **[Paths]**
  - `BASE_PATH`: The root directory of the project.
  - `VSDEV_PATH`: The path to `vsdevcmd.bat` used for setting up the Visual Studio environment (used only if compile not via developer console)
  
- **[Tools]**
  - `GIT_CMD`: The command for Git (e.g., `git.exe`).
  - `CL_CMD`: The C/C++ compiler (e.g., `cl.exe`).
  - `RC_CMD`: The resource compiler (e.g., `rc.exe`).
  - `LINK_CMD`: The linker (e.g., `link.exe`).

- **[Project]**
  - `PLUGIN_NAME`: The name of the plugin source file.
  - `OUTPUT_NAME`: The output DLL name.
  - `LINKER_PARAM`: Parameters for the linker.

- **[Xbyak]**
  - `XBYAK_REPO`: The repository URL for the xbyak submodule.
  - `XBYAK_BRANCH`: The branch name for the xbyak submodule.

Please update any incorrect paths or settings to match your system configuration.

## Build Options

There are two ways to perform the build:

## Build from a Regular Command Prompt
If you run the build from a standard Command Prompt, the script will use the `VSDEV_PATH` from `build_config.ini` to call `vsdevcmd.bat`

## Running the Build
After verifying and updating `build_config.ini` as necessary, start the build by running:
```bash
build.bat
```
## Verifying the Build
After a successful build, a file named according to the OUTPUT_NAME parameter will appear in the project directory.

### License
Please refer to the included LICENSE files for:
  - The main project license (BSD)
  - Third-party licenses for MetaMod:Source (zLib/libpng) and Xbyak (BSD-3-Clause).
