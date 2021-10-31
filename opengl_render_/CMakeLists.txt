set(project_name opengl_render_)
# 设置工程名称 
project(${project_name})

# 设置最低的CAMKE版本
cmake_minimum_required(VERSION 3.8)

# 设置C++ 的语言版本 
set(CMAKE_CXX_STANDARD 14)

# SET LIB PATH 
SET(LIB_PATH ${CMAKE_SOURCE_DIR}/../3rdpart/x64-windows)


SET(Headers callbackfunction.h renderwidget.h renderbase.h  shader.h vertexarraybufferobject.h bufferobject.h render_en.h openg_texture.h) 

SET(Sources main.cpp callbackfunction.cpp renderwidget.cpp renderbase.cpp  shader.cpp vertexarraybufferobject.cpp bufferobject.cpp openg_texture.cpp
stb_image.cpp)

SET(shader_file shader/triangle.vert shader/triangle.frag)


# 将头文件和源文件 整理到对应的文件夹中 
SOURCE_GROUP("Headers Files" FILES ${Headers})
SOURCE_GROUP("Sources Files" FILES ${Sources})
SOURCE_GROUP("Resource Files" FILES ${shader_file})

add_executable(${project_name} ${Headers} ${Sources} ${shader_file})


# 设置 连接库目录
SET(LINKER_DIRECTORES ${LIB_PATH}/lib)

# 设置连接库的目录 
TARGET_LINK_DIRECTORIES(${project_name} PRIVATE ${LINKER_DIRECTORES})

INCLUDE_DIRECTORIES(${LIB_PATH}/bin)

#设置附加包含目录
TARGET_INCLUDE_DIRECTORIES(
	${project_name}
	PRIVATE
		${LIB_PATH}/include
)


SET(LINKER_LIBRARIES
	debug     glfw3dll     optimized      glfw3dll
	debug     glad         optimized      glad
	opengl32
)

target_link_libraries(${project_name} ${LINKER_LIBRARIES})

# 将当前项目设置为启动项目 
set_property(DIRECTORY ${CMAKE_CURRENT_SOSURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${project_name})