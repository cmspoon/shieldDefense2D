# 🛡️ Shield Defense 2D 
    A 2D defense game built with C++, OpenGL, and GLAD.

## Description
You must defend the central power core from enemies approaching from both sides. The power core is protected by a shield, only strong enough to withstand four hits from the enemies. If the enemies break the shield and destroy the power core, the game is **OVER!**

## Controls
| Key | Action |
| :--- | :--- |
| **A / D** | Move the player left and right |
| **SPACE** | Jump |
| **Mouse** | Aim the gun |
| **LMB (Hold)** | Fire bullets toward the cursor's location |
| **+ / -** | Zoom the camera IN / OUT |
| **ESC** | Close the game |

## How to Build

Setting up this project requires a few manual steps because certain files are computer-dependent.

### 1. Prerequisites
Ensure you have a C++ compiler (like MinGW or Clang) and the necessary OpenGL drivers installed on your system.

### 2. Download Dependencies
This project relies on several external libraries. You must download and link these yourself:
* **GLFW**: For window management and input.
* **GLM**: For matrix and vector mathematics.
* **stb_image**: Used for loading textures (included in `support/`).

### 3. Generate GLAD
Because `glad.c` is computer-dependent, it is not included in the repository.
1.  Go to the [GLAD Web Service](https://glad.dav1d.de/).
2.  Select **C/C++** and **OpenGL 3.3** (or higher) **Core** profile.
3.  Click **Generate** and download the resulting `.zip` file.
4.  Place `glad.c` into your project root directory and `glad.h` into your includes.

### 4. Build Instructions
**Using VS Code:**
The repository includes a `.vscode` folder with `tasks.json`. If your include paths are set correctly in `c_cpp_properties.json`, you can simply press `Ctrl+Shift+B` to build.