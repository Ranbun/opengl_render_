## OpenGL render

- 简单的关于OpenGL的封装，可以更方便的渲染一些场景

#### 1、三方库

- `glm`
- `stb_image`
- `assimp`
- `glad`
- `glfw`

- ......

- 下载好上述的三方库之后(建议使用`vcpkg`)

- 修改`opengl_render_`目录下的`Cmake`文件的参数

  ![](https://raw.githubusercontent.com/CuntBoy/images/main/github/others_lib_dir.png)

  修改为你的三方库的位置

#### 2、编译程序

- 进入`opengl_render_`目录

  ```bash
  cd opengl_render_
  ```

- 使用外部编译，创建`build`文件夹,并进入

  ```bash
  mkdir build
  ```

- 使用`Cmake`

  ```bash
  cmake ..
  ```

  