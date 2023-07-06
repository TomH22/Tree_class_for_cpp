# Tree class for C++
**This repository contains a tree class for C++ with iterator and an example.**
## Tree class ##
- can be found at [libs/Tree.h](/libs/Tree.h)

The file should be a header-only library. The goal is to use this file in other projects.
## Sample implementation for creating tree data ##
```C++
Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);// highest rank
Staff quaas(L"Quaas", Staff::RANK::SERGANT);
Staff cox(L"Cox", Staff::RANK::SERGANT);

TreeModel<Staff> treeModel(loos);// Create tree model. Give the constructor also root element.
treeModel.Append(quaas, loos);// 1. sergant of staff sergant
treeModel.Append(cox, loos);// 2. sergant of staff sergant
treeModel.Append(Staff(L"Epheser", Staff::RANK::CORPORAL), cox);
```
## This sample application ##
The sample application was created using the graphical user interface [Dear ImGui](https://github.com/ocornut/imgui). I have chosen military staff for illustration. The main purpose of this application is to output a list and also filter elements in this one.
[![Video](https://img.youtube.com/vi/whSA_vjmbig/0.jpg)](https://www.youtube.com/watch?v=whSA_vjmbig)

docu in progress ... (TODO)
## Usage
### Linux
Just run test with command line.
1. download repository
```
git clone https://github.com/TomH22/Tree_class_for_cpp.git
```
2. build with cmake
```
cmake -S . -B ../../tree_build
```
3. 
### Windows
Build all.
1. clone repository
```
git clone https://github.com/TomH22/Tree_class_for_cpp.git
```
2. clone glfw repository
```
git clone --recursively https://github.com/glfw/glfw.git
```
3. clone imgui repository
```
git clone --recursively https://github.com/ocornut/imgui.git
```
4. install Vulkan
5. change Tree_class_for_cpp/CMakeLists.txt rows
```
    # Set this to point to an up-to-date GLFW repo.
    set(GLFW_DIR "${PROJECT_SOURCE_DIR}/../glfw")

    # Set this to point to an up-to-date imgui repo.
    set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/../imgui")
```
