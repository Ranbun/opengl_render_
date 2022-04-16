#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "enum.h"

// Default camera values
constexpr float YAW = -90.0f; // ��ƫ��
constexpr float PITCH = 0.0f; // ������
constexpr float SPEED = 2.5f; // �ƶ��ٶ�
constexpr float SENSITIVITY = 0.1f; // ������
constexpr float ZOOM = 45.0f; // ���� 

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

    // �����¼� 
    void processKeyBoard(camera_movement direction, float delta_time);
    void processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
    void processMouseScroll(float y_offset);

    // �@ȡ��ǰ���ӽ�
    float fov() const;
    glm::vec3 position() const;
    // ��ȡ��ͼ����
    glm::mat4 getViewMatrix() const;

    ~Camera() = default;

private:
    void updateCameraVectors();

protected:
    glm::vec3 m_position{}; // ���λ��
    glm::vec3 m_front{}; // ����
    glm::vec3 m_up{}; // ���Ϸ��� 
    glm::vec3 m_right{}; // ���ҷ���
    glm::vec3 m_worldUp{}; // �����������S���Ϸ��� 

    // ŷ���� 
    float m_yaw{};
    float m_pitch{};

    // ������ƶ� 
    // camera options
    float m_movementSpeed{};
    float m_mouseSensitivity{}; // �����ŷ����
    float m_zoom{}; // ����  �ӽ� fov 
};


#endif // !1
