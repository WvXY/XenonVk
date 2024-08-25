# XenonVk

**Under development...**

## Description

A 3D Vulkan game engine for my personal study and interests.

This project was initially created by following the tutorial
of [Little Vulkan Engine](https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P) for learning Vulkan and Game Engine.
More features will be added soon.

## Installation

### Prerequisites

- CMake
- C++ compiler: g++ or clang++ or msvc
- Vulkan SDK: https://vulkan.lunarg.com/sdk/home
- GLFW3
- GLM or Eigen3

### Install Dependencies(Linux)

This project has set up [GitHub Actions](.github/workflows/cmake-single-platform.yml), 
follow the instruction to install the dependencies and build.

```bash    
sudo apt update
sudo apt install -y libvulkan-dev vulkan-tools glslang-tools libglfw3-dev libeigen3-dev libglm-dev
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

### Install Dependencies(Windows)

You can use [vcpkg](https://vcpkg.io/en/) or [MSYS2](https://www.msys2.org/) to install the dependencies.

If you use vcpkg on Clion, follow the [official guide](https://www.jetbrains.com/help/clion/package-management.html) to
integrate vcpkg with Clion.

## Build & Run

You can use `bash build.sh` to build the project or `bash run.sh` to build and run the project.

## External Libraries

The following libraries are used and pre-installed in this project.

| File              | Version | Project Link                                   |
|-------------------|---------|------------------------------------------------|
| stb_image.h       | v2.29   | https://github.com/nothings/stb                |
| tiny_obj_loader.h | v2.0.0  | https://github.com/tinyobjloader/tinyobjloader |

## TODO
- [ ] Physics Engine (Working on)
- [ ] ECS: Entity Component System
- [ ] Refactor the project: naming, structure
- [ ] Feature: Texture
- [ ] Feature: Shadow
- [ ] Feature: PBR
- [ ] GUI: ImGUI
- [ ] Audio Engine(optional)

## Current Problems

1. GitHub CI is not working properly: enabled extensions in shaders
2. Some function calls are not in the proper place. 
3. Refactor 2D renderer and separate some functions.

## Reference

- Little Vulkan Engine: https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P
- ECS: https://austinmorlan.com/posts/entity_component_system/
- Game Loop: https://gameprogrammingpatterns.com/game-loop.html