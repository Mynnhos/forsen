cmake_minimum_required(VERSION 3.0.0)
project(forsen VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 23)


include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)

add_executable(forsen ${SOURCES})
set_target_properties(forsen PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin/")

target_link_libraries( forsen ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_BUILD_TYPE Release)
include(CPack)
