#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render_en.h"
#include <vector>
// ��� 

// Default camera values
const float YAW = -90.0f;        // ��ƫ��
const float PITCH = 0.0f;        // ������
const float SPEED = 2.5f;        // �ƶ��ٶ�
const float SENSITIVITY = 0.1f;  // ������
const float ZOOM = 45.0f;        // ���� 

class Camera
{
public:
	glm::vec3 postion_;   // ���λ��
	glm::vec3 front_;     // ����
	glm::vec3 up_;        // ���Ϸ��� 
	glm::vec3 right_;     // ���ҷ���
	glm::vec3 world_up_;  // ?

	// ŷ���� 
	float yaw_;
	float pitch_;

	// ������ƶ� 
		// camera options
	float movement_speed_;
	float mouse_sensitivity_;   // �����ŷ����
	float zoom_;                // ����  �ӽ� fov 

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),float yaw = YAW, float pitch = PITCH);
	Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch);

	// ��ȡ��ͼ����
	glm::mat4 getMatrix();
	// �����¼� 
	void processKeyBoard(camera::Camera_Movement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch = true);
	void processMouseScroll(float yoffset);
	~Camera();
private:
	void updateCameraVectors();
};


#endif // !1
