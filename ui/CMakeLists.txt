project(ui)

add_library(${PROJECT_NAME} STATIC "")

target_sources(${PROJECT_NAME}
    PRIVATE
    # INCLUDES
    include/ui/ui.h
    # SOURCES
    src/ui.cpp
    )

target_link_libraries(${PROJECT_NAME}
                      PUBLIC
                      base
                      bgfx
                      EnTT
                      example-common
                      )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
