# XenonVk

**Under development...**

## Description

A simple 3D Vulkan game engine for my personal study and interests.

This project was initially created by following the tutorial
of [**Little Vulkan Engine**](https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P) 
for learning Vulkan and game engine structure.
More features will be added in the future.

------

## Installation

### Prerequisites & Dependencies

- CMake (>= 3.20)
- C++ compiler: ```g++/clang++/msvc/AppleClang/etc.```
- Vulkan SDK: https://vulkan.lunarg.com/sdk/home

### Install Vulkan(Linux)

This project has set up [GitHub Actions](.github/workflows/cmake-build-multi.yml),
follow the instruction to install the dependencies and build.

```bash    
sudo apt update
sudo apt install -y libvulkan-dev vulkan-tools glslang-tools
```

For development in Debug
mode, [Vulkan SDK](https://vulkan.lunarg.com/doc/view/latest/linux/getting_started_ubuntu.html#vulkan-sdk-layers) is
needed to enable `validation_layer`.
You can install it using following commands.

```bash
wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo tee /etc/apt/trusted.gpg.d/lunarg.asc
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list http://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list
sudo apt update
sudo apt install vulkan-sdk
```

### Install Vulkan(Windows & MacOS)

For Windows or MacOS, you can use [Vulkan SDK](https://vulkan.lunarg.com/) installer.


------

## Build & Run

You can use `bash build.sh` to build the project or `bash run.sh` to build and run the project. (Not tested on MacOS)

Or you can use **Clion IDE** or **VS Code** to build and run the project.(Tested)


-----

## External Libraries

The following libraries are used and pre-installed in this project [External](Engine/External) folder.

| File              | Version | Project Link                                   |
|-------------------|---------|------------------------------------------------|
| stb_image.h       | v2.29   | https://github.com/nothings/stb                |
| tiny_obj_loader.h | v2.0.0  | https://github.com/tinyobjloader/tinyobjloader |

_The following dependencies will be installed **automatically** by CMake._

| Library | Project Link                  |
|---------|-------------------------------|
| GLFW    | https://www.glfw.org/         |
| GLM     | https://github.com/g-truc/glm |

------

## TODO

- [ ] Physics Engine (Working on)
- [X] ECS: Entity Component System
- [X] Refactor#2: naming, structure
- [ ] Rendering: Texture
- [ ] Rendering: Better Lighting System including Shadow Mapping, Reflection, etc.
- [ ] GUI: ImGUI
- [ ] Asset Importer: USD, FBX, etc.
- [ ] Refactor#3: cleanup, structure

------

## Reference & Resources

- Little Vulkan Engine: https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P
- ECS: https://austinmorlan.com/posts/entity_component_system/
- Game Loop: https://gameprogrammingpatterns.com/game-loop.html
- Asset Models from https://free3d.com/3d-models/obj?page=7
