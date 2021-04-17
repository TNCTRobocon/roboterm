cmake_minimum_required(VERSION 3.0.0)
project(roboterm VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(roboterm main.c)
add_subdirectory(shell)
target_link_libraries(roboterm shell)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_definitions("-Wall -std=gnu99")
