set(project_name opengl_render_)
# 设置工程名称 
project(${project_name})

# 设置最低的CAMKE版本
cmake_minimum_required(VERSION 3.8)

# 设置C++ 的语言版本s
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

ADD_SUBDIRECTORY(opengl_render_)
ADD_SUBDIRECTORY(deferredshading)
