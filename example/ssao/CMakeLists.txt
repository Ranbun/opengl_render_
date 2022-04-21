set(project_name ssao)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
# 设置工程名称
project(${project_name})

#三方库的根目录
SET(3RD_LIB_PATH ${CMAKE_SOURCE_DIR}/../3rdpart/x64-windows)

# 添加源文件
SET(Headers 
	ssao.h
) 
SET(Sources 
	ssao.cpp
	main.cpp 

)
SET(shader_file
	#bin/shader/load_model/model_load.vert
	#bin/shader/load_model/model_load.frag
)

# 将头文件和源文件 整理到对应的文件夹中 
SOURCE_GROUP("Headers Files" FILES ${Headers})
SOURCE_GROUP("Sources Files" FILES ${Sources})
SOURCE_GROUP("Resource Files" FILES ${shader_file})

# 设置链接flags
set(CMAKE_EXE_LINKER_FLAGS  /NODEFAULTLIB:"MSVCRT.lib")

# 屏蔽掉额外的控制台 
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# 生成可执行文件
add_executable(${project_name} ${Headers} ${Sources}) # ${shader_file})

# 设置 连接库目录
SET(LINKER_DIRECTORES ${3RD_LIB_PATH}/lib)
# 新增链接库目录
list(APPEND LINKER_DIRECTORES "${CMAKE_SOURCE_DIR}/opengl_render_/lib")
# 设置连接库目录到工程
TARGET_LINK_DIRECTORIES(${project_name} PRIVATE ${LINKER_DIRECTORES})

#设置包含目录
TARGET_INCLUDE_DIRECTORIES(
	${project_name}
	PRIVATE
		${3RD_LIB_PATH}/include
		${CMAKE_SOURCE_DIR}/opengl_render_/include
)

SET(LINKER_LIBRARIES
	#debug     glfw3dll          optimized      glfw3dll
	#debug     glad              optimized      glad
	#debug     assimp-vc142-mt   optimized      assimp-vc142-mt
	#opengl32
	render_lib
)

# 链接外部静态库
target_link_libraries(${project_name} ${LINKER_LIBRARIES} )

# 将当前项目设置为启动项目 
# set_property(DIRECTORY ${CMAKE_CURRENT_SOSURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${project_name})