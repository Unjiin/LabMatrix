cmake_minimum_required(VERSION 3.20)
project(LabMatrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(LabMatrix STATIC
        src/labmatrix.cpp
)

target_include_directories(LabMatrix PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(MatrixApp
        app/main.cpp
)
target_link_libraries(MatrixApp PRIVATE LabMatrix)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(LabMatrixTests
        tests/test_tvector.cpp
)
target_link_libraries(LabMatrixTests PRIVATE LabMatrix GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(LabMatrixTests)

