#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace glm;
class Camera
{
	Camera(vec3 position = vec3(0.0f, 0.0f, 3.0f),
		   vec3 worldUp = vec3(0.0f, 1.0f, 0.0f),
		   float yaw = -90.f,
		   float zoom = 45.f,
		   float pitch = 0.0f);
public:
	static Camera& getInstance();
	bool keys[1024] = { 0 };
	float yaw;
	float pitch;
	float zoom;
	vec3 position;
	vec3 up;
	vec3 front;
	vec3 worldUp;
	vec3 right;
	const float mouseSensitivity = 0.05f;
	const float scrollSensitivity = 0.1f;
	const float moveSensitivity = 7.f;
	mat4 getMatrix();
	~Camera();
	void  onMovement();
	void processMouseCallBack(double xpos,double ypos);
	void processKeyCallBack(int key,int action);
	void processScrollCallBack(double yoffset);


private :

	void	setupCameraVector();
};

