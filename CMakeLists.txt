cmake_minimum_required(VERSION 3.10)
project(riptide-game LANGUAGES CXX)

find_package(SFML 3 COMPONENTS Graphics)

file(GLOB SRC_FILES src/*.hpp src/*.cpp)
add_executable(riptide-game main.cpp ${SRC_FILES})
target_compile_features(riptide-game PRIVATE cxx_std_17)
target_link_libraries(riptide-game PRIVATE SFML::Graphics)
