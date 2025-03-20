cmake_minimum_required(VERSION 3.30)

project(HelloLights LANGUAGES CXX C)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(DEP_DIR ${CMAKE_SOURCE_DIR}/vendor)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/model.cpp
    ${SRC_DIR}/utility.cpp
)

set(GLAD_SRC ${DEP_DIR}/glad/src/glad.c)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC ${DEP_DIR}/glad/include)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE glad)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SRC_DIR}
    ${DEP_DIR}/glad/include
)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMENT "Copying assets directory..."
)

add_dependencies(${PROJECT_NAME} copy_assets)