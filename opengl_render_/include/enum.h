#ifndef RENDER_EN_H_
#define RENDER_EN_H_

/*
 * 渲染使用的宏的相关的定义
 * 
 */

namespace oglRender
{
    namespace objectBuffer
    {
        enum class object_type
        {
            vertex_buffer,
            index_buffer
        };

        enum class object_access
        {
            read_only,
            write_only,
            read_write
        };

        enum class object_access_flags
        {
            read,
            write,
            invalidate
        };

    };
    namespace texture
    {
        enum class wrap
        {
            s,
            t,
            r
        };

        enum class fileter
        {
            min,
            mag
        };
    }
    namespace camera
    {
        enum class camera_movement
        {
            forward,
            backward,
            left,
            right
        };
    };
    namespace renderWidget
    {
        enum class gl_version_major
        {
            major_3 = 3,
            major_4 = 4,
        };
        enum class gl_version_minor
        {
            minor_0 = 0,
            minor_1 = 1,
            minor_2 = 2,
            minor_3 = 3,
            minor_4 = 4,
            minor_5 = 5,
            minor_6 = 6,
            minor_7 = 7,
            minor_8 = 8,
            minor_9 = 9
        };
    }
    namespace shader
    {
        enum class shader_type
        {
            vertex_shader,
            fragment_shader,
            geometry_shader,
            program

        };
    }

}

#endif 