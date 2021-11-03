#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render_en.h"
#include <vector>
// 相机 

// Default camera values
const float YAW = -90.0f;        // 航偏角
const float PITCH = 0.0f;        // 俯仰角
const float SPEED = 2.5f;        // 移动速度
const float SENSITIVITY = 0.1f;  // 灵敏度
const float ZOOM = 45.0f;        // 缩放 

class Camera
{
public:
	glm::vec3 postion_;   // 相机位置
	glm::vec3 front_;     // 朝向
	glm::vec3 up_;        // 向上方向 
	glm::vec3 right_;     // 向右方向
	glm::vec3 world_up_;  // ?

	// 欧拉角 
	float yaw_;
	float pitch_;

	// 相机的移动 
		// camera options
	float movement_speed_;
	float mouse_sensitivity_;   // 鼠标于欧拉角
	float zoom_;                // 缩放  视角 fov 

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),float yaw = YAW, float pitch = PITCH);
	Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch);

	// 获取视图矩阵
	glm::mat4 getMatrix();
	// 键盘事件 
	void processKeyBoard(camera::Camera_Movement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch = true);
	void processMouseScroll(float yoffset);
	~Camera();
private:
	void updateCameraVectors();
};


#endif // !1
