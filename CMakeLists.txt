cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME ds_sequencer)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set( BGFX_BUILD_EXAMPLES  OFF CACHE INTERNAL "" )

file(GLOB IMGUI_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui.h
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_internal.h
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui/imgui_draw.cpp
)

file(COPY ${IMGUI_SOURCE_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/deps/bgfx.cmake/bgfx/3rdparty/dear-imgui)

file(GLOB IMGUIZMO_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/ImGuizmo/ImSequencer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/ImGuizmo/ImSequencer.cpp
)

file(COPY ${IMGUIZMO_SOURCE_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/deps/bgfx.cmake/bgfx/3rdparty/dear-imgui)

include_directories("deps/bgfx.cmake/bgfx/3rdparty")

add_subdirectory(base)
add_subdirectory(deps/bgfx.cmake)
add_subdirectory(deps/entt)
add_subdirectory(deps/glm)
add_subdirectory(ds_sequencer)
add_subdirectory(renderer)
add_subdirectory(ui)
