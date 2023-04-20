cmake_minimum_required(VERSION 3.19)
project(SDLTest)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED COMPONENTS SDL2)

add_executable(${PROJECT_NAME} src/main.c src/game.c)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
