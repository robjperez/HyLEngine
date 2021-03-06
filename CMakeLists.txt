cmake_minimum_required(VERSION 3.0)

project(HyLEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
  main.cc
  BOX.h)
set(SHADERS shaders/vertex.vert shaders/fragment.frag)
set(TARGET HyLEngine)

add_executable(${TARGET} ${SOURCES} ${SHADERS})
add_subdirectory(glad/)
add_subdirectory(glfw-3.3.2/)
add_subdirectory(imgui/)

target_link_libraries(
  ${TARGET}
  glfw
  glad
  imgui
  )

