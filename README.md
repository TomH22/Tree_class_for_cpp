# Tree class for C++
**This repository provides a tree class implementation for C++, including iterators, tests, and an example.**

The header only file can be found here: [lib/Tree.h](/lib/Tree.h)

## Sample code for creating tree data ##
```C++
#include "Tree.h"
using namespace henn;
...

Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);// highest rank
Staff quaas(L"Quaas", Staff::RANK::SERGANT);
Staff cox(L"Cox", Staff::RANK::SERGANT);

TreeModel<Staff> treeModel(loos);// Create tree model. Give the constructor also root element.
treeModel.Append(quaas, loos);// 1. sergant of staff sergant
treeModel.Append(cox, loos);// 2. sergant of staff sergant
treeModel.Append(Staff(L"Epheser", Staff::RANK::CORPORAL), cox);
```
## Sample application ##
The sample application demonstrates the usage of the tree class with a graphical user interface implemented using [Dear ImGui](https://github.com/ocornut/imgui). The example uses military staff data for illustration purposes. The main functionality of the application is to display and filter elements in the tree.

Watch the video demonstration: 

[![Video](https://img.youtube.com/vi/whSA_vjmbig/0.jpg)](https://www.youtube.com/watch?v=whSA_vjmbig)


## Usage
**Building the project**

If you just want to build the tests,  you can skip the steps 1, 2, 3, 4 and 5.

>This was tested on Windows 10 (07/2023).

1. clone this repository
```
git clone https://github.com/TomH22/Tree_class_for_cpp.git
```
2. clone the GLFW repository
```
git clone --recursive https://github.com/glfw/glfw.git
```
3. clone the Dear ImGui repository
```
git clone --recursive https://github.com/ocornut/imgui.git
```
4. install Vulkan SDK (minimal installation is sufficient)

   https://vulkan.lunarg.com/sdk/home#windows

5. modify 2 lines in the [CMakeLists.txt](CMakeLists.txt) file to point to the locations of the "glfw" and "imgui" repositories. For example: 
```
    # Set this to point to an up-to-date GLFW repo.
    set(GLFW_DIR "C:/Users/Tom/Documents/git/glfw")

    # Set this to point to an up-to-date imgui repo.
    set(IMGUI_DIR "C:/Users/Tom/Documents/git/imgui")
```
6. build the project using CMake
```
cmake -S . -B ../../tree_build
```
7. open the project, for example, with Visual Studio
