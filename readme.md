# XenonVk

**Under development...**

## Description

A 3D Vulkan game engine for personal study and interests.

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

- [ ] Refactor the project: naming, structure
- [ ] Feature: Texture
- [ ] Feature: Shadow
- [ ] Feature: PBR
- [ ] GUI: ImGUI
- [ ] Physics Engine (Working on)
- [ ] Audio Engine(optional)

## Current Problems

1. [Object Model] On large models, some faces are missing. 
2. `vkCreateShaderModule():  SPIR-V Capability FragmentBarycentricKHR was declared, but one of the following requirements is required (VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV::fragmentShaderBarycentric).`
3. GitHub CI is not working properly

## Reference

- https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P
- https://austinmorlan.com/posts/entity_component_system/
