# Library Name
set(PROJECT_NAME RxHook)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
include_directories("${THIRD_DIR}/detours/include/")
link_libraries("${THIRD_DIR}/detours/lib.X86/detours.lib")

# Set Head File Path
set(HDR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/RxHook.h")

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxHook/API.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxHook/Mem.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/RxHook/Write.cpp" 
)

# Set Current Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
