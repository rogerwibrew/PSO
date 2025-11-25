cmake_minimum_required(VERSION 3.14)
project(PSO VERSION 1.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for language servers (clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include FetchContent module for downloading dependencies
include(FetchContent)

# Download and configure GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make GoogleTest available
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create source and include directories if they don't exist
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/source)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)

# Collect source files (excluding main.cpp for library)
file(GLOB_RECURSE SOURCES "source/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Main executable
add_executable(pso ${SOURCES} source/main.cpp)

# Test executable
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
if(TEST_SOURCES)
    # Tests use library sources but not main.cpp (GoogleTest provides its own main)
    add_executable(pso_tests ${TEST_SOURCES} ${SOURCES})
    target_link_libraries(pso_tests GTest::gtest_main)

    # Discover tests
    include(GoogleTest)
    gtest_discover_tests(pso_tests)
endif()
