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
	layout(location = 2) in vec2 TexCoord;

	out vec3 Normal;
	out vec2 texCoord;
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
		texCoord = TexCoord;
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
		in vec2 texCoord;
	
		struct Light {
			vec3 position;
			vec3 diffuse;
			vec3 specular;
			vec3 ambient;
		};
		struct Material {
			//vec3 diffuse;
			//vec3 ambient;
		
			//vec3 specular;
			float shininess;
		};

uniform sampler2D  texDiffuse;
uniform sampler2D  texSpecular;
//观察者位置
uniform vec3 viewerPos;
uniform Material material;
uniform Light light;
void main() {
	//ambient  就是物体自身带的,或者说周围的颜色,在没有灯光照射的情况下, 所以用   result= ambient强度*灯光颜色, 
	//比如  0.1f *(1.0f,1.0f,1.0f) = (0.1f,0.1f,0.1f) 可以得出当前环境颜色,可以看出,ambient因子小的时候,周围光比较暗
	//然后   ambient=result * 物体颜色;  
	//比如   (0.1f,0.1f,0.1f) * (1.0f,0.5f,0.0f)=  (0.1f,0.1f,0.0f);可以看出物体的颜色会被影响. 
	//结果,如果物体的当前颜色如果比当前周围光的颜色强,那么会被消弱, 如果没有, 还会继续被消弱.
	//所以就是整体被消弱.


	vec3 ambient=vec3(texture(texDiffuse,texCoord)) *light.ambient ;
	
	//diffuse

	//获取当前灯的位置(lampPos),和物体位置也就是fragPos 相减得出 然后 normalize 得出 灯的方向                                                                                                                                                                                                                                                                                                                                                                       
	vec3 lampDirection = normalize(light.position - FragPos);
	//防止当前向量不是单位向量和计算失误. 
	 vec3 norm = normalize(Normal);
	//通过公式可得,  dot(x,y) =||x||*||y||*cos0; 由于x 和y都是单位向量,则 可以直接得出cos0;
	 float cosA= dot(lampDirection, norm);
	 //由于cosA有可能是负数,负数意味着与法线形成的夹角大于90°,光照是达不到的,所以会变成0,公式:
	  float cosB=  max(cosA, 0.0f);
	 vec3 diffuse =	vec3(texture(texDiffuse,texCoord)) *light.diffuse *cosB ;
	  
	 vec3 viewerDir=normalize(viewerPos - FragPos);
	  //获取反射光,利用系统函数 reflect(args1,args2)  
	  //args1必须是灯光方向而不是灯的方向,所以是灯的反方向.args2是当前面的法向量
	  //通过该函数可以得到反射光的方向. 已经被normalize过
	  vec3 reflectedDir= reflect(-lampDirection, norm);
	  //算出观察者和反射光的夹角值,
	  float  cosA1= dot(viewerDir, reflectedDir);
	  //如同上面,不可以为负数, 则
	  float cosB1 = max(cosA1, 0.0f);
	        cosB1 = pow(cosB1, material.shininess);
	  //最后计算 cosB1的 shininess(高光系数) 次方;得出:
	
	  //乘以镜面反射强度,和灯光颜色
	  vec3 specular =   vec3(texture(texSpecular,texCoord)) *light.specular *cosB1  ;
	  vec3 result = vec3(ambient+diffuse+specular);
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
Texture	*container2;
Texture *container;
Texture *awesomeface;
Texture * container2_specular;
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

	GLuint lightPosition= glGetUniformLocation(objectShader->program,  "light.position");
	GLuint lightdiffuse= glGetUniformLocation(objectShader->program, "light.diffuse");
	GLuint lightambient= glGetUniformLocation(objectShader->program, "light.ambient");
	GLuint lightspecular= glGetUniformLocation(objectShader->program, "light.specular");
	
	GLuint materialshininess= glGetUniformLocation(objectShader->program,  "material.shininess");

	GLuint materialspecular= glGetUniformLocation(objectShader->program,"material.specular");
	
	glUniform3f(lightPosition, ligthPostion.x, ligthPostion.y, ligthPostion.z);
	glUniform3f(lightdiffuse, 0.5f, 0.5f, 0.5f);
	glUniform3f(lightambient,  0.2f, 0.2f, 0.2f);
	glUniform3f(lightspecular,  1.0f, 1.0f, 1.0f);

	glUniform1f(materialshininess, 32.f);
	glUniform3f(materialspecular, 0.50196078f, 0.50196078f, 0.50196078f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, container2->texId);
	glUniform1i(glGetUniformLocation(objectShader->program, "texDiffuse"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, container2_specular->texId);
	glUniform1i(glGetUniformLocation(objectShader->program, "texSpecular"), 1);
	
	glUniformMatrix4fv(objModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(objViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(objProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
					
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
	container2 = new Texture("container2.png");
	container2_specular = new Texture("container2_specular.png");


	camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
	objectShader->generateProgram(verSource, fragSource);
	lampShader->generateProgram(verSource, lightfragSource);
    GLfloat vertices[] = {
     // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6* sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
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

