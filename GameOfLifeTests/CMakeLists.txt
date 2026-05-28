cmake_minimum_required(VERSION 3.16)

project(GameOfLifeTests)

enable_testing()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_test(NAME GameOfLifeTests COMMAND GameOfLifeTests)

qt_add_executable(GameOfLifeTests
    tst_gameoflifetests.cpp
)

target_link_libraries(GameOfLifeTests PRIVATE 
	Qt6::Test
	GameOfLifeLib
)

target_include_directories(GameOfLifeTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../GameOfLife
)
