#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "enum.h"

// Default camera values
constexpr float YAW = -90.0f; // 航偏角
constexpr float PITCH = 0.0f; // 俯仰角
constexpr float SPEED = 2.5f; // 移动速度
constexpr float SENSITIVITY = 0.1f; // 灵敏度
constexpr float ZOOM = 45.0f; // 缩放 

using namespace oglRender::camera;  // NOLINT(clang-diagnostic-header-hygiene)

class Camera
{
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
        , glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    explicit Camera(float pos_x, float pos_y, float pos_z, float upx, float upy, float upz, float yaw, float pitch);

    Camera(const Camera& another) = delete;
    Camera& operator=(const Camera& another) = delete;

    Camera(const Camera&& another) = delete;
    Camera& operator=(const Camera&& another) = delete;

    // 键盘事件 
    void processKeyBoard(camera_movement direction, float delta_time);
    void processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
    void processMouseScroll(float y_offset);

    // 獲取当前的视角
    float fov() const;
    glm::vec3 position() const;
    // 获取视图矩阵
    glm::mat4 getViewMatrix() const;

    ~Camera() = default;

private:
    void updateCameraVectors();

protected:
    glm::vec3 m_position{}; // 相机位置
    glm::vec3 m_front{}; // 朝向
    glm::vec3 m_up{}; // 向上方向 
    glm::vec3 m_right{}; // 向右方向
    glm::vec3 m_worldUp{}; // 在世界坐標軸的上方向 

    // 欧拉角 
    float m_yaw{};
    float m_pitch{};

    // 相机的移动 
    // camera options
    float m_movementSpeed{};
    float m_mouseSensitivity{}; // 鼠标于欧拉角
    float m_zoom{}; // 缩放  视角 fov 
};


#endif // !1
