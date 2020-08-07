cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME ds_sequencer)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set( BGFX_BUILD_EXAMPLES  OFF CACHE INTERNAL "" )

set(CMAKE_TARGET_DEFINITIONS_CXX
  "ENTRY_CONFIG_IMPLEMENT_MAIN=1"
  )

# include_directories("deps/imgui/")
include_directories("deps/bgfx.cmake/bgfx/3rdparty")

set(SOURCES
    ds_sequencer/src/main.cpp
    )

set(HEADERS 
    )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_dependencies(ds_sequencer example-common)

target_link_libraries(${PROJECT_NAME}
                      PUBLIC
                      bgfx
                      example-common
                      )

# message("include dir ${CMAKE_PREFIX_PATH}/include" )
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_PREFIX_PATH}/include")

add_subdirectory(deps/bgfx.cmake)

# include(deps/bgfx.cmake/cmake/util/ConfigureDebugging.cmake)
# include(deps/bgfx.cmake/cmake/examples.cmake)
# include(deps/bgfx.cmake/cmake/util/ConfigureDebugging.cmake)
# include(deps/bgfx.cmake/cmake/3rdparty/meshoptimizer.cmake)
# include_directories(deps/bgfx.cmake/cmake/3rdparty/dear-imgui)

target_compile_definitions(ds_sequencer PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1" )

set_target_properties(ds_sequencer PROPERTIES LINK_FLAGS "/ENTRY:\"mainCRTStartup\"" )