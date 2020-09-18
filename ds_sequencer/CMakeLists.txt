project(app)

include_directories("include")

set(SOURCES
    src/main.cpp
    )

set(HEADERS 
    )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_dependencies(app example-common ui)

target_link_libraries(${PROJECT_NAME}
                      PUBLIC
                      base
                      bgfx
                      EnTT
                      example-common
                      glm
                      renderer
                      ui
                      )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)


target_compile_definitions(app PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1" )

if(WIN32)
    set_target_properties(app PROPERTIES LINK_FLAGS "/ENTRY:\"mainCRTStartup\"" )
endif()
