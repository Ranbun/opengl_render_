# OpenGL Render

- 为了学习`OpenGL `搭建的框架,使用`cmake`编写的方便移植的工程

#### 1、使用的库：

- `glfw3` -- 窗口
- `glad`  -- 获取函数接口
- `stb` -- 图片加载库
- 。。。。。

#### 2、如何使用

- 进入`opengl_render_`项目的根目录下，执行一下命令：

  - ```powershell
    cd opengl_render_/build
    cmake ..
    ```

  - ![](https://raw.githubusercontent.com/CuntBoy/images/main/github_lib/opengl_render/cmake_opengl_render.png)

  

#### 3、自己编写场景渲染

1、继承`RenderBase`类，然后重写`render`函数，在`render`函数中编写自己要显示的场景。

![](https://raw.githubusercontent.com/CuntBoy/images/main/github_lib/opengl_render/override_render_.png)

2、实例化第一步的对象，调用`RenderWidget`对象的`setRenderObect(RenderBase * object)`函数

![](https://raw.githubusercontent.com/CuntBoy/images/main/github_lib/opengl_render/setRenderObject.png)

3、修改`cmake`文件，编译，运行

#### 4、小案例

![](https://raw.githubusercontent.com/CuntBoy/images/main/github_lib/opengl_render/show_render_result.png)
