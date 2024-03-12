# Little Game Engine

**Under development...**

## Description

A Vulkan game engine for personal study and interests.

This project is first created by following the tutorial
of [Little Vulkan Engine](https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P).

## Installation

### Prerequisites

- CMake
- vcpkg(Recommend) or MinGW(MSYS2)
- gcc or clang or msvc
- Vulkan SDK : https://vulkan.lunarg.com/sdk/home
- GLFW3
- GLM

### Clion (for both Windows && Linux)

1. Open the project with Clion.
2. Setup vcpkg:
   https://www.jetbrains.com/help/clion/package-management.html
3. Install the dependencies by vcpkg.
4. Build and Run the project.

### Visual Studio Code

1. Install the dependencies by vcpkg.
2. Open the project with VSCode.
3. Install the CMake Tools, C/C++ extensions.
4. Build and Run the project.

### Vcpkg Install Packages

If not installed yet, install vcpkg by following the instructions here:

https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd

Then install the dependencies using
`vcpkg install [PACKAGE_NAME]`

### MinGW(MSYS2) Install Packages

Work in progress...

## TODO

- [ ] This name is too common. Change it to something unique.
- [ ] Separate 2D into a new project that only for 2D. This one is for 3D.
- [ ] Refactor to make it more modular.
- [ ] Feature: Texture
- [ ] Feature: Shadow
- [ ] Feature: PBR
- [ ] GUI: ImGUI
- [ ] Mouse Input
- [ ] Physics Engine
- [ ] Audio Engine(optional)

## Current Problems

1. When VSync off, the load of GPU is too high, especially on high resolution. I can hear whistle noise from the GPU.
2. However, when VSync on, there is noticeable input and display lag.
3. `validation layer: Validation Performance Warning: [ UNASSIGNED-CoreValidation-Shader-OutputNotConsumed ]` from
   Vulkan SDK.

## Reference

- https://vcpkg.io/en/
- https://youtu.be/Y9U9IE0gVHA?si=42keJCaEPE-R697P
- https://austinmorlan.com/posts/entity_component_system/
