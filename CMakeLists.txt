# Main CMakeLists
cmake_minimum_required(VERSION 3.12)

# Project Name
project(RiaLibrary)

# MSVC Setting
if(MSVC)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    add_definitions(-DUNICODE -D_UNICODE)
endif()

# Value Define
set(LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")
set(THIRD_DIR "${CMAKE_CURRENT_LIST_DIR}/third")

# Out Path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/.out/exe)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/.out/lib)

# Library
    add_subdirectory("${LIB_DIR}/RxHook")
    add_subdirectory("${LIB_DIR}/MinoriFileHook")

# Project
    add_subdirectory("src/Eden_EN2CN")
    add_subdirectory("src/Eden_EN2JP")
    add_subdirectory("src/EdenPM_EN2CN")
    add_subdirectory("src/EdenPM_EN2JP")
    add_subdirectory("src/EF_First_AA_EN2JP")
    add_subdirectory("src/EF_Latter_AA_EN2JP")
    add_subdirectory("src/SimpleLoader")