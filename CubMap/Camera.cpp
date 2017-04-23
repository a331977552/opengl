#include "stdafx.h"
#include "Camera.h"


Camera::Camera(vec3 position ,
			   vec3 worldUp,
			   float yaw,
			   float zoom,float pitch) {
	this->position = position;
	this->worldUp = worldUp;
	this->yaw = yaw;
	this->zoom = zoom;
	this->pitch = pitch;
	setupCameraVector();

}

	 Camera& Camera::getInstance() {
	    static 	 Camera * camera = new Camera();
		return *camera;
	 }

	
	
	 Camera::~Camera() {

	}
	void  Camera::onMovement() {
	    static float lastTime=	glfwGetTime();
		float currentTime=glfwGetTime();
		float deltaTime=currentTime - lastTime;
		
	
		lastTime = currentTime;
		if (keys[GLFW_KEY_W]) {
			position +=  (front*deltaTime*moveSensitivity);
		}
		if (keys[GLFW_KEY_S]) {
				position -=  (front*deltaTime*moveSensitivity);
		}
		if (keys[GLFW_KEY_A]) {
				position -=  (right*deltaTime*moveSensitivity);
		}
		if (keys[GLFW_KEY_D]) {
				position +=  (right*deltaTime*moveSensitivity);
		}

	}
	void Camera::processMouseCallBack(double xpos,double ypos) {
		static bool isFirst = true;
		static float lastX,lastY;
		float xoffset, yoffset;
		if (isFirst) {
			lastX = xpos;
			lastY = ypos;
			isFirst = false;
		}
		xoffset = (xpos - lastX)*mouseSensitivity;
		yoffset = (lastY - ypos)*mouseSensitivity;
		lastX = xpos;
		lastY = ypos;
		

		this->yaw += xoffset;
		this->pitch += yoffset;
		if (pitch < -89.f) {
			pitch = -89.f;
		}
		if (pitch > 89.f) {
			pitch = 89.f;
		}
		setupCameraVector();
	}
	mat4 Camera::getMatrix() {
		return glm::lookAt(position, position + front, up);
	}
	void Camera::processKeyCallBack(int key,int action) {
		std::cout << "key" << key<<std::endl;
		
		if (key == GLFW_KEY_ESCAPE) {
			exit(0);
		}
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
	void Camera::processScrollCallBack(double yoffset) {
		yoffset *= scrollSensitivity;
	
		if (zoom >= 1.0f && zoom <= 45.f) {
				zoom -= yoffset;
		}
		if (zoom < 1.0f) {
			zoom = 1.0f;
		}if (zoom > 45.f) {
			zoom = 45.f;
		}

		std::cout << "key" << zoom<<std::endl;

	}


	void	Camera::setupCameraVector() {
			  // Calculate the new Front vector
        glm::vec3 mfront;
        mfront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        mfront.y = sin(glm::radians(this->pitch));
        mfront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(mfront);
        // Also re-calculate the Right and Up vector
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->up    = glm::normalize(glm::cross(this->right, this->front));


	}
