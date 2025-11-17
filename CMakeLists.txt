cmake_minimum_required(VERSION 3.16)
project(ded-zemax-abracat)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIbrARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(sdl EXCLUDE_FROM_ALL)

# add_executable(hello WIN32 main.cpp sdl-adapter.cpp plane.cpp texture.cpp event.cpp widget.cpp wcontainer.cpp
#     myvector.cpp reactor.cpp desktop.cpp button.cpp scroll.cpp optical-scene.cpp optical-object.cpp
#     optical-control.cpp
    
#     window.cpp)

# add_executable(hello WIN32 src/main.cpp src/sdl-adapter.cpp src/texture.cpp src/event.cpp src/widget.cpp src/wcontainer.cpp
#     src/myvector.cpp src/desktop.cpp src/optical-scene.cpp src/optical-object.cpp
    
#     src/window.cpp src/my-dr4-plugin.cpp)

# add_executable(hello WIN32 src/main.cpp src/sdl-adapter.cpp src/texture.cpp src/myvector.cpp
    
#     src/window.cpp src/my-dr4-plugin.cpp)

# target_link_libraries(hello PRIVATE SDL3::SDL3)
# target_include_directories(hello PRIVATE ./include/ ./src)
# target_compile_options(hello PRIVATE -Wno-narrowing)

add_library(plugin SHARED src/sdl-adapter.cpp src/texture.cpp src/myvector.cpp
    src/window.cpp src/my-dr4-plugin.cpp)
target_link_libraries(plugin PUBLIC SDL3::SDL3)
target_include_directories(plugin PUBLIC ./include/ ./src)
target_compile_options(plugin PUBLIC -Wno-narrowing)

add_executable(main src/main.cpp src/widget.cpp src/optical-object.cpp src/optical-scene.cpp)
target_include_directories(main PRIVATE ./include/)
target_link_libraries(main PRIVATE plugin)
