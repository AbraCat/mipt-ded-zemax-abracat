cmake_minimum_required(VERSION 3.16)
project(ded-zemax-abracat)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIbrARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(sdl EXCLUDE_FROM_ALL)
add_subdirectory(sdl-ttf EXCLUDE_FROM_ALL)

add_library(dr4 SHARED src/sdl-adapter.cpp src/texture.cpp src/myvector.cpp src/my-utils.cpp
    src/window.cpp src/my-dr4-plugin.cpp)
target_link_libraries(dr4 PUBLIC SDL3::SDL3 SDL3_ttf::SDL3_ttf)
target_include_directories(dr4 PUBLIC ./include/ ./h)

add_library(pp SHARED src/my-pp-plugin.cpp src/my-utils.cpp)
target_include_directories(pp PUBLIC ./include/ ./h)

add_executable(main source/hui/widget.cpp source/hui/event.cpp source/hui/container.cpp source/hui/ui.cpp source/cum/manager.cpp
    src/main.cpp src/myvector.cpp src/my-utils.cpp
    src/wcontainer.cpp src/desktop.cpp src/button.cpp src/optical-object.cpp src/optical-scene.cpp src/optical-control.cpp
    src/mycanvas.cpp src/tool-widget.cpp src/draggable.cpp src/scroll.cpp
    )
target_include_directories(main PRIVATE ./include/ ./h)
target_compile_options(main PRIVATE -Wno-narrowing)
