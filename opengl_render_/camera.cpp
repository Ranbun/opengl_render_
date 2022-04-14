#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position)
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_worldUp(up)
    , m_yaw(yaw), m_pitch(pitch)
    , m_movementSpeed(SPEED)
    , m_mouseSensitivity(SENSITIVITY)
    , m_zoom(ZOOM)
{
    updateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float upx, float upy, float upz, float yaw, float pitch)
    : m_position(glm::vec3(pos_x, pos_y, pos_z))
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_worldUp(glm::vec3(upx, upy, upz))
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_movementSpeed(SPEED)
    , m_mouseSensitivity(SENSITIVITY)
    , m_zoom(ZOOM)
{
    updateCameraVectors();
}

Camera::Camera(const Camera& another)
{
        
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyBoard(camera::Camera_Movement direction, float delta_time)
{
    const float velocity = m_movementSpeed * delta_time;
    if (direction == camera::Camera_Movement::FORWARD)
        m_position += m_front * velocity;
    if (direction == camera::Camera_Movement::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == camera::Camera_Movement::LEFT)
        m_position -= m_right * velocity;
    if (direction == camera::Camera_Movement::RIGHT)
        m_position += m_right * velocity;
}

void Camera::processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch)
{
    x_offset *= m_mouseSensitivity;
    y_offset *= m_mouseSensitivity;

    m_yaw += x_offset;
    m_pitch += y_offset;

    // ���� �����ǵķ�Χ 
    if (constrain_pitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float y_offset)
{
    m_zoom -= (float)y_offset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

float Camera::fov() const
{
    return m_zoom;
}

// ���¼��㳯�� 
void Camera::updateCameraVectors()
{
    // ���¼��� ����ĳ��� 
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
