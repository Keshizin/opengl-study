cmake_minimum_required(VERSION 3.18)
project(opengl-study)
set(APP_EXE appgl)

message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

include_directories("${PROJECT_SOURCE_DIR}/inc")
link_directories("${PROJECT_SOURCE_DIR}/lib")
add_executable(${APP_EXE} WIN32 main.cpp)
target_link_libraries(${APP_EXE} kengine opengl32 glu32)