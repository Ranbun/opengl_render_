#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	:front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(SPEED), mouse_sensitivity_(SENSITIVITY), zoom_(ZOOM)
{
	postion_ = position;
	world_up_ = up;
	yaw_ = yaw;
	pitch_ = pitch;
	updateCameraVectors();
}

Camera::Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch)
	: front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(SPEED), mouse_sensitivity_(SENSITIVITY), zoom_(ZOOM)
{
	postion_ = glm::vec3(posx, posy, posz);
	world_up_ = glm::vec3(upx, upy, upz);

	yaw_ = yaw;
	pitch_ = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getMatrix()
{
	return glm::lookAt(postion_,postion_ + front_,up_);
}

void Camera::processKeyBoard(camera::Camera_Movement direction, float deltaTime)
{
	float velocity = movement_speed_ * deltaTime;
	if (direction == camera::Camera_Movement::FORWARD)
		postion_ += front_ * velocity;
	if (direction == camera::Camera_Movement::BACKWARD)
		postion_ -= front_ * velocity;
	if (direction == camera::Camera_Movement::LEFT)
		postion_ -= right_ * velocity;
	if (direction == camera::Camera_Movement::RIGHT)
		postion_ += right_ * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
	xoffset *= mouse_sensitivity_;
	yoffset *= mouse_sensitivity_;

	yaw_ += xoffset;
	pitch_ += yoffset;

	// 限制 俯仰角的范围 
	if (constrain_pitch)
	{
		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		if (pitch_ < -89.0f)
			pitch_ = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	zoom_ -= (float)yoffset;
	if (zoom_ < 1.0f)
		zoom_ = 1.0f;
	if (zoom_ > 45.0f)
		zoom_ = 45.0f;
}

Camera::~Camera()
{
}

// 重新计算朝向 
void Camera::updateCameraVectors()
{
	// 重新计算 相机的朝向 
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front);

	right_ = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up_ = glm::normalize(glm::cross(right_, front_));
}
