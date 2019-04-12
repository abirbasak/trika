cmake_minimum_required(VERSION 3.5)
project(trika VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)

add_library(trika ${HEADERS} ${SOURCES})

target_include_directories(trika PUBLIC include )
target_compile_definitions(trika PUBLIC )
target_compile_features(trika PUBLIC cxx_std_17)

group_sources(include)
group_sources(source)
