cmake_minimum_required(VERSION 3.14)

project(arcademia_template)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET TRUE CACHE BOOL "" FORCE) 
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
if(EMSCRIPTEN)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
        GIT_SHALLOW 1
    )
    FetchContent_Populate(raylib)

    set(RAYLIB_WEB_LIB "${CMAKE_SOURCE_DIR}/web/runtimes/wasm/libraylib.web.a")
    if(NOT EXISTS "${RAYLIB_WEB_LIB}")
        message(FATAL_ERROR "Prebuilt web raylib not found at ${RAYLIB_WEB_LIB}")
    endif()

    add_library(raylib STATIC IMPORTED GLOBAL)
    set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${RAYLIB_WEB_LIB}"
    )
    target_include_directories(raylib INTERFACE ${raylib_SOURCE_DIR}/src)
else()
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
endif()

# imgui
find_package(imgui QUIET)
if(NOT imgui_FOUND) # <--- FIXED: Case-sensitivity
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.92.7
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(imgui)
endif()

# raylib - imgui
find_package(rlimgui QUIET)
if(NOT rlimgui_FOUND) # <--- FIXED: Case-sensitivity
    FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui
        GIT_TAG Raylib_6_0
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(rlimgui)
endif()

# ---------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------

# Build imgui
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# Link rlimgui
add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(rlimgui PUBLIC raylib imgui)

# Source Files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib rlimgui) 

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
    $<$<BOOL:${EMSCRIPTEN}>:PLATFORM_WEB>
)

# ---------------------------------------------------------------------------
# Emscripten / Web build
# ---------------------------------------------------------------------------
if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html" OUTPUT_NAME "index")

    target_link_options(${PROJECT_NAME} PRIVATE
        "-sUSE_GLFW=3"
        "-sASYNCIFY"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sGROWABLE_ARRAYBUFFERS=0"
        "-sFORCE_FILESYSTEM=1"
        "--preload-file=${CMAKE_SOURCE_DIR}/resources@resources"
        "--shell-file=${CMAKE_SOURCE_DIR}/web/shell.html"
    )
endif()

# ---------------------------------------------------------------------------
# Resources (native builds only -- web packages resources via --preload-file)
# ---------------------------------------------------------------------------
if(NOT EMSCRIPTEN)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        COMMENT "Copying resources to build directory"
    )
endif()

# ---------------------------------------------------------------------------
# Run Command
# ---------------------------------------------------------------------------
if(NOT EMSCRIPTEN)
    add_custom_target(run
        COMMAND $<TARGET_FILE:${PROJECT_NAME}>
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# ---------------------------------------------------------------------------
# clangd support
# ---------------------------------------------------------------------------
if(NOT EMSCRIPTEN)
    set(CLANGD_FLAGS
        "-I${CMAKE_SOURCE_DIR}/include"
        "-I${raylib_SOURCE_DIR}/src"
        "-I${imgui_SOURCE_DIR}"
        "-I${rlimgui_SOURCE_DIR}"
        "-std=c++20"
    )
    list(JOIN CLANGD_FLAGS "\n" CLANGD_FLAGS_STR)
    file(GENERATE
        OUTPUT "${CMAKE_SOURCE_DIR}/compile_flags.txt"
        CONTENT "${CLANGD_FLAGS_STR}\n"
    )
endif()