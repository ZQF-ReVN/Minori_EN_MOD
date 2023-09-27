# Value Define
set(CMAKE_WIN32_EXECUTABLE ON)
set(PROJECT_NAME SimpleLoader)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(RxHook)

# Current Project
project("${PROJECT_NAME}")
add_executable(${PROJECT_NAME} "SimpleLoader.cpp")

