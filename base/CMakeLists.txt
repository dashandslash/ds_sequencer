project(base)

add_library(${PROJECT_NAME} STATIC "")

target_sources(${PROJECT_NAME}
    PRIVATE
    # INCLUDES
    include/base/utils.h
    include/base/components/basic.h
    # SOURCES
    # src/utils.cpp
    src/components/basic.cpp
    )

target_link_libraries(${PROJECT_NAME}
                        PUBLIC
                        bgfx
                        bx
                        glm
                        # example-common
                    )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
