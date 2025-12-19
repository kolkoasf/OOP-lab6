cmake_minimum_required(VERSION 3.20)
project(lab6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(GTest QUIET)

if(NOT GTest_FOUND)
  message(STATUS "GTest not found. Fetching GTest...")
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    QUIET
  )
  FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

add_executable(lab6_main src/main.cpp
  src/bear.cpp src/werewolf.cpp src/rogue.cpp
  src/npc.cpp src/fight_visitor.cpp src/observers.cpp src/game_manager.cpp src/npc_factory.cpp)

add_executable(lab6_tests tests/tests.cpp
  src/bear.cpp src/werewolf.cpp src/rogue.cpp
  src/npc.cpp src/fight_visitor.cpp src/observers.cpp src/game_manager.cpp src/npc_factory.cpp)

target_link_libraries(lab6_tests PRIVATE GTest::gtest_main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(lab6_tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
