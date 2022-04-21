set(static_lib_name render_lib)
# 设置工程名称 
#project(${project_name})

# 设置最低的CAMKE版本
#cmake_minimum_required(VERSION 3.8)

# 设置C++ 的语言版本s
#set(CMAKE_CXX_STANDARD 14)

# set target output
# SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/opengl_render_/lib)

# SET LIB PATH 
SET(3RD_LIB_PATH ${CMAKE_SOURCE_DIR}/../3rdpart/x64-windows)

SET(Headers 
	include/renderwidget.h 
	include/shader.h 
	include/vertexarraybufferobject.h 
	include/bufferobject.h 
	include/enum.h
	include/texture.h 
	include/camera.h
	include/model.h
	include/mesh.h
	include/widget_attribute.h
	include/ofapp.h
	include/framebufferobject.h
	include/renderbufferobject.h
	# include/deferredshading.h
) 


SET(Sources 
	# main.cpp 
	renderwidget.cpp 
	shader.cpp 
	vertexarraybufferobject.cpp 
	bufferobject.cpp 
	texture.cpp
	stb_image.cpp 
	camera.cpp
	model.cpp
	mesh.cpp
	widget_attribute.cpp
	# deferredshading.cpp 
)

SET(shader_file
		bin/shader/load_model/model_load.vert
		bin/shader/load_model/model_load.frag
)

# 将头文件和源文件 整理到对应的文件夹中 
SOURCE_GROUP("Headers Files" FILES ${Headers})
SOURCE_GROUP("Sources Files" FILES ${Sources})
SOURCE_GROUP("Resource Files" FILES ${shader_file})

# 设置链接flags
# set(CMAKE_EXE_LINKER_FLAGS  /NODEFAULTLIB:"MSVCRT.lib")
# 屏蔽掉额外的控制台 
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
# add_executable(${project_name} ${Headers} ${Sources} ${shader_file})
add_library(${static_lib_name} STATIC ${Headers} ${Sources})

# 设置 连接库目录
SET(LINKER_DIRECTORES ${3RD_LIB_PATH}/lib)

# 设置连接库的目录 
TARGET_LINK_DIRECTORIES(${static_lib_name} PRIVATE ${LINKER_DIRECTORES})

INCLUDE_DIRECTORIES(${3RD_LIB_PATH}/bin)

#设置附加包含目录
TARGET_INCLUDE_DIRECTORIES(
	${static_lib_name}
	PRIVATE
		${3RD_LIB_PATH}/include
		./include
)

SET(LINKER_LIBRARIES
	debug     glfw3dll             optimized      glfw3dll
	debug     glad                 optimized      glad
	debug     assimp-vc142-mt      optimized      assimp-vc142-mt
	opengl32
)

target_link_libraries(${static_lib_name} ${LINKER_LIBRARIES} Qt5::Widgets)

# 将当前项目设置为启动项目 
# set_property(DIRECTORY ${CMAKE_CURRENT_SOSURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${static_lib_name})