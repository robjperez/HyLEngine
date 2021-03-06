cmake_minimum_required(VERSION 3.0)
project(imgui)

add_library(imgui
	imconfig.h
	imgui.cpp
	imgui.h
	imgui_impl_glfw.h
	imgui_impl_glfw.cpp
	imgui_impl_opengl3.cpp
	imgui_impl_opengl3.h
  imstb_textedit.h
  imgui_tables.cpp
  imgui_widgets.cpp
  imgui_draw.cpp
  imgui_internal.h)

include_directories(
  ../glfw-3.3.2/include
  ../glad/include
)

target_compile_definitions(imgui PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(imgui PUBLIC
	imconfig.h
	imgui.h
	imgui_impl_glfw.h
	imgui_impl_opengl3.h
  imgui_internal.h)
