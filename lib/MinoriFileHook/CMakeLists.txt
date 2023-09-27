# Library Name
set(PROJECT_NAME MinoriFileHook)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(RxHook)

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MinoriFileHook.cpp" 
)

# Set Current Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
