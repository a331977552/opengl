// Learning2.cpp : 定义控制台应用程序的入口点。
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
							  //物体位置.
	layout(location = 0) in vec3 position;
	//每个点的法向量.
	layout(location = 1) in vec3 normal;
	out vec3 Normal;
	//当前物体的世界坐标
	out vec3 FragPos;
	//物体的行为变化,位移,缩放,旋转
	uniform mat4 model;
	//照相机
	uniform mat4 view;
	//投影方式
	uniform mat4 projection;	
	void main() {
			//通过转换(具体为啥不知道)可以获取物体实时转动缩放以后的法向量
		Normal = mat3(transpose(inverse(model))) * normal;  
		//
		gl_Position =  projection*view*model*vec4( position,1.0f);
		// (旋转 ,位移,缩放以后的) 位置 *初始位置 得到当前世界坐标位置.然后通过out 传递给fragment
		FragPos = vec3(model*vec4(position, 1.0f));
	}


);
//物体颜色,需要计算
const char * fragSource = DLSL(330,
		//每个点,面的法向量
		in vec3 Normal;
		//物体的位置
		in vec3 FragPos;
		//最后输出的颜色
		out vec4 color;
	
//灯的位置
uniform vec3 lampPos;
//物体颜色
uniform vec3 objectColor;
//灯的颜色,
uniform vec3 lightColor;
//观察者位置
uniform vec3 viewerPos;

void main() {
	//ambient  就是物体自身带的,或者说周围的颜色,在没有灯光照射的情况下, 所以用   result= ambient强度*灯光颜色, 
	//比如  0.1f *(1.0f,1.0f,1.0f) = (0.1f,0.1f,0.1f) 可以得出当前环境颜色,可以看出,ambient因子小的时候,周围光比较暗
	//然后   ambient=result * 物体颜色;  
	//比如   (0.1f,0.1f,0.1f) * (1.0f,0.5f,0.0f)=  (0.1f,0.1f,0.0f);可以看出物体的颜色会被影响. 
	//结果,如果物体的当前颜色如果比当前周围光的颜色强,那么会被消弱, 如果没有, 还会继续被消弱.
	//所以就是整体被消弱.

	float ambientStrength = 0.2f;
	vec3 ambient= ambientStrength*lightColor;
	
	//diffuse

	//获取当前灯的位置(lampPos),和物体位置也就是fragPos 相减得出 然后 normalize 得出 灯的方向                                                                                                                                                                                                                                                                                                                                                                       
	vec3 lampDirection = normalize(lampPos - FragPos);
	//防止当前向量不是单位向量和计算失误. 
	 vec3 norm = normalize(Normal);
	//通过公式可得,  dot(x,y) =||x||*||y||*cos0; 由于x 和y都是单位向量,则 可以直接得出cos0;
	 float cosA= dot(lampDirection, norm);
	 //由于cosA有可能是负数,负数意味着与法线形成的夹角大于90°,光照是达不到的,所以会变成0,公式:
	 float cosB=  max(cosA, 0.0f);
	 //由于在90度范围内,角度B 越大,cosB越小,反之越大,则符合光照漫反射的规则. 
	 //比如.在同一个平面上,离灯光近的点,灯光和法线的角度小,则强度大,反之,远处则强度小.
	 //最后得出的值乘以 光线颜色.可以得出颜色值.
	  vec3 diffuse= cosB*lightColor;

	  //diffuse 加 ambient 可以算出每个点的颜色.也就是每个点的 周围光颜色加 diffuse的每个点的光颜色,得出结果
	  //于法线形成夹角大于90°的情况下,无法照射到,所以cosB是0;则 diffuse也就为0; 这就是为什么别的面还是ambient的颜色
	
	  //specular
	 
	  //获取观察者,也就是照相机的方向,也就是从物体每一个点到照相机那个点的方向
	  vec3 viewerDir=normalize(viewerPos - FragPos);
	  //获取反射光,利用系统函数 reflect(args1,args2)  
	  //args1必须是灯光方向而不是灯的方向,所以是灯的反方向.args2是当前面的法向量
	  //通过该函数可以得到反射光的方向. 已经被normalize过
	  vec3 reflectedDir= reflect(-lampDirection, norm);
	  //算出观察者和反射光的夹角值,
	  float  cosA1= dot(viewerDir, reflectedDir);
	  //如同上面,不可以为负数, 则
	  float cosB1= max(cosA1, 0.0f);
	  //最后计算 cosB1的 shininess(高光系数) 次方;得出:
	  float shininess = 32.f;
	  //乘以镜面反射强度,和灯光颜色
	  float specularStrength = 0.5f;
	  vec3 specular =  pow(cosB1, shininess)  * specularStrength *   lightColor;
	   vec3 result = vec3(ambient+diffuse+specular)*objectColor;
	  color = vec4(result, 1.0f);
}

);

//灯光颜色, 白色.
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
	//画白炽灯
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
					//橙色物品
	glUniform3f(objColorLoc,1.0f, 0.5f, 0.31f);
					//灯光发白光
	glUniform3f(ligthInObjLoc,1.0f, 1.0f, 1.0f);
					//灯在世界坐标的位置
	glUniform3f(lampPosObjLoc,ligthPostion.x,ligthPostion.y,ligthPostion.z);
					//观察者,也就是照相机,在世界中的位置
	glUniform3f(viewerPosLoc,camera->Position.x,camera->Position.y,camera->Position.z);
	//画物体块
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
	  //隐藏鼠标,并且捕捉他.
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

