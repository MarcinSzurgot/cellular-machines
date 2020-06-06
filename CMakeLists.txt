cmake_minimum_required(VERSION 3.12)
project(cellular-machines)

include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY  https://github.com/sfml/sfml
    GIT_TAG         2.5.1
)

FetchContent_Populate(sfml)

add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
)