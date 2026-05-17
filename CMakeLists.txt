cmake_minimum_required(VERSION 3.14)

project(arcademia_template)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET FALSE CACHE BOOL "" FORCE)
include(FetchContent)

# ---------------------------------------------------------------------------
# Custom build configurations
# ---------------------------------------------------------------------------
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES
        Debug
        Release
        RelWithDebInfo
        MinSizeRel
        Arcademia
        Arcademia_Debug
        CACHE STRING "" FORCE
    )
endif()

set(CMAKE_CXX_FLAGS_ARCADEMIA "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_ARCADEMIA_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------

# raylib
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if(NOT raylib_cpp_FOUND)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v6.0.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# ---------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
    CMAKE_C_STANDARD 11
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Arcademia>:ARCADEMIA>
    $<$<CONFIG:Arcademia_Debug>:ARCADEMIA DEBUG>
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

# ---------------------------------------------------------------------------
# Resources
# ---------------------------------------------------------------------------

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMENT "Copying resources to build directory"
)

# ---------------------------------------------------------------------------
# Run Command
# ---------------------------------------------------------------------------

add_custom_target(run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# ---------------------------------------------------------------------------
# clangd support
# ---------------------------------------------------------------------------
set(CLANGD_FLAGS
    "-I${CMAKE_SOURCE_DIR}/include"
    "-I${CMAKE_BINARY_DIR}/_deps/raylib-src/src"
    "-I${CMAKE_BINARY_DIR}/_deps/raylib_cpp-src/include"
    "-std=c++20"
)
list(JOIN CLANGD_FLAGS "\n" CLANGD_FLAGS_STR)
file(GENERATE
    OUTPUT "${CMAKE_SOURCE_DIR}/compile_flags.txt"
    CONTENT "${CLANGD_FLAGS_STR}\n"
)