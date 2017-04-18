// Learning2.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#ifndef DLSL

#define  DLSL(version,source)"#version " #version " core\n"  #source
#endif // !DLSL
//
const char * verSource = DLSL(330,
							  //����λ��.
	layout(location = 0) in vec3 position;
	//ÿ����ķ�����.
	layout(location = 1) in vec3 normal;
	out vec3 Normal;
	//��ǰ�������������
	out vec3 FragPos;
	//�������Ϊ�仯,λ��,����,��ת
	uniform mat4 model;
	//�����
	uniform mat4 view;
	//ͶӰ��ʽ
	uniform mat4 projection;	
	void main() {
			//ͨ��ת��(����Ϊɶ��֪��)���Ի�ȡ����ʵʱת�������Ժ�ķ�����
		Normal = mat3(transpose(inverse(model))) * normal;  
		//
		gl_Position =  projection*view*model*vec4( position,1.0f);
		// (��ת ,λ��,�����Ժ��) λ�� *��ʼλ�� �õ���ǰ��������λ��.Ȼ��ͨ��out ���ݸ�fragment
		FragPos = vec3(model*vec4(position, 1.0f));
	}


);
//������ɫ,��Ҫ����
const char * fragSource = DLSL(330,
		//ÿ����,��ķ�����
		in vec3 Normal;
		//�����λ��
		in vec3 FragPos;
		//����������ɫ
		out vec4 color;
	
//�Ƶ�λ��
uniform vec3 lampPos;
//������ɫ
uniform vec3 objectColor;
//�Ƶ���ɫ,
uniform vec3 lightColor;
//�۲���λ��
uniform vec3 viewerPos;

void main() {
	//ambient  ���������������,����˵��Χ����ɫ,��û�еƹ�����������, ������   result= ambientǿ��*�ƹ���ɫ, 
	//����  0.1f *(1.0f,1.0f,1.0f) = (0.1f,0.1f,0.1f) ���Եó���ǰ������ɫ,���Կ���,ambient����С��ʱ��,��Χ��Ƚϰ�
	//Ȼ��   ambient=result * ������ɫ;  
	//����   (0.1f,0.1f,0.1f) * (1.0f,0.5f,0.0f)=  (0.1f,0.1f,0.0f);���Կ����������ɫ�ᱻӰ��. 
	//���,�������ĵ�ǰ��ɫ����ȵ�ǰ��Χ�����ɫǿ,��ô�ᱻ����, ���û��, �������������.
	//���Ծ������屻����.

	float ambientStrength = 0.2f;
	vec3 ambient= ambientStrength*lightColor;
	
	//diffuse

	//��ȡ��ǰ�Ƶ�λ��(lampPos),������λ��Ҳ����fragPos ����ó� Ȼ�� normalize �ó� �Ƶķ���                                                                                                                                                                                                                                                                                                                                                                       
	vec3 lampDirection = normalize(lampPos - FragPos);
	//��ֹ��ǰ�������ǵ�λ�����ͼ���ʧ��. 
	 vec3 norm = normalize(Normal);
	//ͨ����ʽ�ɵ�,  dot(x,y) =||x||*||y||*cos0; ����x ��y���ǵ�λ����,�� ����ֱ�ӵó�cos0;
	 float cosA= dot(lampDirection, norm);
	 //����cosA�п����Ǹ���,������ζ���뷨���γɵļнǴ���90��,�����Ǵﲻ����,���Ի���0,��ʽ:
	 float cosB=  max(cosA, 0.0f);
	 //������90�ȷ�Χ��,�Ƕ�B Խ��,cosBԽС,��֮Խ��,����Ϲ���������Ĺ���. 
	 //����.��ͬһ��ƽ����,��ƹ���ĵ�,�ƹ�ͷ��ߵĽǶ�С,��ǿ�ȴ�,��֮,Զ����ǿ��С.
	 //���ó���ֵ���� ������ɫ.���Եó���ɫֵ.
	  vec3 diffuse= cosB*lightColor;

	  //diffuse �� ambient �������ÿ�������ɫ.Ҳ����ÿ����� ��Χ����ɫ�� diffuse��ÿ����Ĺ���ɫ,�ó����
	  //�ڷ����γɼнǴ���90��������,�޷����䵽,����cosB��0;�� diffuseҲ��Ϊ0; �����Ϊʲô����滹��ambient����ɫ
	
	  //specular
	 
	  //��ȡ�۲���,Ҳ����������ķ���,Ҳ���Ǵ�����ÿһ���㵽������Ǹ���ķ���
	  vec3 viewerDir=normalize(viewerPos - FragPos);
	  //��ȡ�����,����ϵͳ���� reflect(args1,args2)  
	  //args1�����ǵƹⷽ������ǵƵķ���,�����ǵƵķ�����.args2�ǵ�ǰ��ķ�����
	  //ͨ���ú������Եõ������ķ���. �Ѿ���normalize��
	  vec3 reflectedDir= reflect(-lampDirection, norm);
	  //����۲��ߺͷ����ļн�ֵ,
	  float  cosA1= dot(viewerDir, reflectedDir);
	  //��ͬ����,������Ϊ����, ��
	  float cosB1= max(cosA1, 0.0f);
	  //������ cosB1�� shininess(�߹�ϵ��) �η�;�ó�:
	  float shininess = 32.f;
	  //���Ծ��淴��ǿ��,�͵ƹ���ɫ
	  float specularStrength = 0.5f;
	  vec3 specular =  pow(cosB1, shininess)  * specularStrength *   lightColor;
	   vec3 result = vec3(ambient+diffuse+specular)*objectColor;
	  color = vec4(result, 1.0f);
}

);

//�ƹ���ɫ, ��ɫ.
const char * lightfragSource = DLSL(330,

									out vec3 color;
void main() {
	color = vec3(1.0f);
}

);


bool keys[512];

Camera *camera;
float rotateDegreeX = 0;
float rotateDegreeY = 0;
float translateX = 0;
float translateY = 0;

Shader  *objectShader;
Shader *	lampShader;
GLuint VAO,VAO2;
Texture *container;
Texture *awesomeface;
void  draw() {

	lampShader->useProgram();
	glm::mat4 model,view,projection;
	GLuint lampModelLoc= glGetUniformLocation(lampShader->program, "model");
	GLuint lampViewLoc= glGetUniformLocation(lampShader->program, "view");
	GLuint lampProjectionLoc= glGetUniformLocation(lampShader->program, "projection");
	projection =glm::perspective(45.f,1.f,0.1f,100.f);
	view = camera->GetViewMatrix();
	glm::vec3 ligthPostion(1.0f+translateX, 1.0f+translateY, 2.0f);
	model =glm::translate(model, ligthPostion);
	model = glm::scale(model, glm::vec3(0.2));

	glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	//���׳��
    glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	


	



	objectShader->useProgram();
	model = glm::mat4();
	model = glm::rotate(model, rotateDegreeX, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotateDegreeY, glm::vec3(1, 0, 0));

	GLuint objModelLoc= glGetUniformLocation(objectShader->program, "model");
	GLuint objViewLoc= glGetUniformLocation(objectShader->program, "view");
	GLuint objProjectionLoc= glGetUniformLocation(objectShader->program, "projection");
	GLuint viewerPosLoc= glGetUniformLocation(objectShader->program, "viewerPos");

	GLuint objColorLoc= glGetUniformLocation(objectShader->program, "objectColor");
	GLuint ligthInObjLoc= glGetUniformLocation(objectShader->program, "lightColor");
	GLuint lampPosObjLoc= glGetUniformLocation(objectShader->program, "lampPos");

	
	glUniformMatrix4fv(objModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(objViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(objProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
					//��ɫ��Ʒ
	glUniform3f(objColorLoc,1.0f, 0.5f, 0.31f);
					//�ƹⷢ�׹�
	glUniform3f(ligthInObjLoc,1.0f, 1.0f, 1.0f);
					//�������������λ��
	glUniform3f(lampPosObjLoc,ligthPostion.x,ligthPostion.y,ligthPostion.z);
					//�۲���,Ҳ���������,�������е�λ��
	glUniform3f(viewerPosLoc,camera->Position.x,camera->Position.y,camera->Position.z);
	//�������
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);


}
void   initGL() {
	
	objectShader = new Shader();
	lampShader = new Shader();
	container  = new Texture("container.jpg");
	awesomeface = new Texture("awesomeface.png");
	camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
	objectShader->generateProgram(verSource, fragSource);
	lampShader->generateProgram(verSource, lightfragSource);
    GLfloat vertices[] = {
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
void movement() {
  static GLfloat 	rotateSpeed=0.5f;
    static GLfloat 	translateSpeed=5.f;

	static GLfloat lastFrame = 0;
	GLfloat currentTime= glfwGetTime();
    GLfloat deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
    float cameraSpeed=1.0f*deltaTime;
	
	if( keys[GLFW_KEY_W]) {
		camera->ProcessKeyboard(Camera_Movement::FORWARD,deltaTime);
	}if (keys[GLFW_KEY_S]) {
		camera->ProcessKeyboard(Camera_Movement::BACKWARD,deltaTime);
	}if (keys [GLFW_KEY_A]) {
		camera->ProcessKeyboard(Camera_Movement::LEFT,deltaTime);
	}if (keys[GLFW_KEY_D]) {
	   	camera->ProcessKeyboard(Camera_Movement::RIGHT,deltaTime);
	}if (keys[GLFW_KEY_LEFT]) {
		

		rotateDegreeX -= deltaTime*rotateSpeed;
	}if (keys[GLFW_KEY_RIGHT]) {
		rotateDegreeX += deltaTime*rotateSpeed;
	}
	if (keys[GLFW_KEY_UP]) {
		
		rotateDegreeY -= deltaTime*rotateSpeed;
	}if (keys[GLFW_KEY_DOWN]) {
		rotateDegreeY += deltaTime*rotateSpeed;
	}
	if (keys[GLFW_KEY_KP_4]) {
		
		translateX-=deltaTime*translateSpeed;
		
	}if (keys[GLFW_KEY_KP_6]) {
		translateX += deltaTime*translateSpeed;
	}
	if (keys[GLFW_KEY_KP_8]) {
		
		translateY -= deltaTime*translateSpeed;
	}if (keys[GLFW_KEY_KP_2]) {
		translateY += deltaTime*translateSpeed;
	}


	
}
void  keyFunc(GLFWwindow  *window, int key, int scancode, int action, int mode) {
	
		if (action == GLFW_PRESS) {
			keys[key]=true;
		}
		else if (action == GLFW_RELEASE) {
				keys[key]=false;
		}

		if (key == GLFW_KEY_ESCAPE && action ==GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static  bool firstMouse = true;
	static float lastX = 400,lastY=300;
	if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left 
    lastX = xpos;
    lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);

}
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	camera->ProcessMouseScroll(yoffset);
}
int main(){

	glfwInit();
	GLFWwindow	*window=glfwCreateWindow(400, 400, "test", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, keyFunc);
	  //�������,���Ҳ�׽��.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scrollCallBack);
	glewExperimental = GL_TRUE;
	glewInit();
	
	glClearColor(0.2f, 0.2f, 0.3f, 0.3f);
	initGL();
	int width, height;
	glfwSetWindowPos(window,400, 300);
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		 glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		movement();
		draw();
		glfwSwapBuffers(window);
	}






	return 0;
}

