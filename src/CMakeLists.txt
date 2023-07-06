set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# GLFW
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" off)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" off)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" off)
option(GLFW_INSTALL "Generate installation target" off)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" off)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)
# Use vulkan headers from glfw
include_directories(${GLFW_DIR}/deps)

# Dear ImGui
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(LIBRARIES "glfw;Vulkan::Vulkan")

add_executable(bootstrap bootstrap.cpp run.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Staff/Staff.cpp)
target_link_libraries(bootstrap PRIVATE lib PRIVATE ${LIBRARIES})
target_include_directories(bootstrap PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Staff)
target_compile_definitions(bootstrap PUBLIC -DImTextureID=ImU64)