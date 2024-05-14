# Little Game Engine(Temp, WIP)

**Under development...**

## Description

A Vulkan game engine for personal study and interests.

This project was initially created by following the tutorial
of [Little Vulkan Engine](https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P) for learning Vulkan and Game Engine.
More features will be added soon.

## Installation

### Prerequisites

- CMake
- C++ compiler: g++ or clang++ or msvc
- Vulkan SDK: https://vulkan.lunarg.com/sdk/home
- GLFW3
- GLM

### Install Dependencies(Linux)

```bash    
sudo apt update
sudo apt install -y libvulkan-dev vulkan-tools glslang-tools libglfw3-dev libeigen3-dev libglm-dev
```

### Install Dependencies(Windows)

You can use [vcpkg](https://vcpkg.io/en/) or [MSYS2](https://www.msys2.org/) to install the dependencies.

If you use vcpkg on Clion, follow the [official guide](https://www.jetbrains.com/help/clion/package-management.html) to integrate vcpkg with Clion.


## External Libraries

The following libraries are used and pre-installed in this project.

| File              | Version | Project Link                                   |
|-------------------|---------|------------------------------------------------|
| stb_image.h       | v2.29   | https://github.com/nothings/stb                |
| tiny_obj_loader.h | v2.0.0  | https://github.com/tinyobjloader/tinyobjloader |

## TODO

- [ ] Rename the project
- [ ] Feature: Texture
- [ ] Feature: Shadow
- [ ] Feature: PBR
- [ ] GUI: ImGUI
- [ ] Mouse Input
- [ ] Physics Engine
- [ ] Audio Engine(optional)

## Current Problems

1. [Performance] When VSync off, the load of GPU is too high, especially on high resolution. There are whistle noise from the GPU. However, when VSync on, there is noticeable input and display lag.
2. [VLayer]`validation layer: Validation Performance Warning: [ UNASSIGNED-CoreValidation-Shader-OutputNotConsumed ]`
3. [Object Model] On large models, some faces are missing. 

## Reference

- https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P
- https://austinmorlan.com/posts/entity_component_system/
