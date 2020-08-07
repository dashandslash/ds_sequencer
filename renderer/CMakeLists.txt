project(renderer)

add_library(${PROJECT_NAME} STATIC "")

target_sources(${PROJECT_NAME}
    PRIVATE
    # INCLUDES
    include/renderer/renderer.h
    # SOURCES
    src/renderer.cpp
    )

target_link_libraries(${PROJECT_NAME}
                      PUBLIC
                      bgfx
                      example-common
                      )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
