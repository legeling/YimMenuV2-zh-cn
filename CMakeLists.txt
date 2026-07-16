cmake_minimum_required(VERSION 3.20.x)

include(cmake/cross-compile.cmake)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" YIMMENU_VERSION LIMIT_COUNT 1)
string(STRIP "${YIMMENU_VERSION}" YIMMENU_VERSION)
if(NOT YIMMENU_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
    message(FATAL_ERROR "VERSION must contain a semantic version such as 1.1.0")
endif()

project(YimMenuV2 VERSION "${YIMMENU_VERSION}" DESCRIPTION "A new base using new C++ features optimised for speed and ease of use")

set(YIMMENU_BUILD_TAG "v${PROJECT_VERSION}-zh-cn.dev" CACHE STRING "Full Chinese release tag embedded in the build")
set(YIMMENU_BUILD_TAG_PREFIX "v${PROJECT_VERSION}-zh-cn.")
string(FIND "${YIMMENU_BUILD_TAG}" "${YIMMENU_BUILD_TAG_PREFIX}" YIMMENU_BUILD_TAG_PREFIX_POSITION)
if(NOT YIMMENU_BUILD_TAG_PREFIX_POSITION EQUAL 0)
    message(FATAL_ERROR "YIMMENU_BUILD_TAG must match VERSION, for example v${PROJECT_VERSION}-zh-cn.1")
endif()

string(LENGTH "${YIMMENU_BUILD_TAG_PREFIX}" YIMMENU_BUILD_TAG_PREFIX_LENGTH)
string(SUBSTRING "${YIMMENU_BUILD_TAG}" ${YIMMENU_BUILD_TAG_PREFIX_LENGTH} -1 YIMMENU_BUILD_REVISION)
if(NOT YIMMENU_BUILD_REVISION MATCHES "^(dev|[1-9][0-9]*)$")
    message(FATAL_ERROR "YIMMENU_BUILD_TAG revision must be a positive integer or the local .dev suffix")
endif()

configure_file(cmake/Version.hpp.in generated/Version.hpp @ONLY)

# libs
include(cmake/async-logger.cmake)
include(cmake/imgui.cmake)
include(cmake/json.cmake)
include(cmake/minhook.cmake)
include(cmake/luajit.cmake)

# source
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE SRC_FILES
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.cpp"    
)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_precompile_headers(${PROJECT_NAME} PUBLIC "${SRC_DIR}/common.hpp")

add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    IMGUI_DEFINE_MATH_OPERATORS
)

message(STATUS "Setting up include directories.")
target_include_directories(${PROJECT_NAME} PRIVATE
    "${SRC_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
    "${imgui_SOURCE_DIR}"
    "${minhook_SOURCE_DIR}/include"
    "${minhook_SOURCE_DIR}/src/hde"
    "${luajit_SOURCE_DIR}/src"
    "${luajit_BINARY_DIR}/src"
)

message(STATUS "Setting up linked libraries")
target_link_libraries(${PROJECT_NAME} PRIVATE
    # Project dependencies
    AsyncLogger
    imgui
    minhook
    nlohmann_json::nlohmann_json
    dbghelp

    # ImGui dependencies
    $<IF:$<BOOL:${CROSSCOMPILE}>,d3dcompiler_47,d3dcompiler>
    dwmapi
    gdi32
    libluajit
)

target_compile_options(${PROJECT_NAME} PRIVATE
    # Do not warn about non-reproducible code
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-date-time>
    $<$<COMPILE_LANG_AND_ID:CXX,Clang>:-Wno-date-time>
)
