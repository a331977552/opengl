#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

class LightTest
{
	public  :
	GLuint VBO, VAO, EBO, UBO,cubeVao;
	Shader * cubeShader,*skyShader,*lightShader;
	Texture *cubeTex,*skyTex,*floorTex;
	void onDraw() {
	  
	   glm::mat4 projection = 	glm::perspective(Camera::getInstance().zoom, 1.5f, 0.1f, 100.f);
	   glm::mat4 view = Camera::getInstance().getMatrix();
	   glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	   glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), glm::value_ptr(projection));
	   glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), glm::value_ptr(view));
	   glBindBuffer(GL_UNIFORM_BUFFER, 0);
	    		 glm::mat4 model;
	     /*light*/
	   model = glm::mat4();
	   model = glm::scale(model, vec3(0.5f,0.5f, 0.5f));
	   model = glm::translate(model, vec3(3.f,8.f,3.f));
	
	   lightShader->useProgram();
	   glUniformMatrix4fv(glGetUniformLocation(lightShader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	   glBindVertexArray(VAO);
	   glDrawArrays(GL_TRIANGLES, 0, 36);
	   glBindVertexArray(0);
	   //cube 
	   model = glm::mat4();
	   cubeShader->useProgram();
	   glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	   glUniform3fv(glGetUniformLocation(cubeShader->program, "viewPos"), 1,glm::value_ptr(Camera::getInstance().position));
	   glUniform3f(glGetUniformLocation(cubeShader->program, "lightPos"),3.f,8.f,3.f);
	      glUniform3f(glGetUniformLocation(cubeShader->program, "ambient"),0.2f,0.2f,0.2f);
	   glUniform3f(glGetUniformLocation(cubeShader->program, "specular"),0.8,0.5,0.3f);
	   glUniform3f(glGetUniformLocation(cubeShader->program, "diffuse"),0.8f,0.3,0.1f);
	   glUniform1f(glGetUniformLocation(cubeShader->program, "shininess"),32.f);





	   glUniform1i(glGetUniformLocation(cubeShader->program, "tex"),0);
	   glActiveTexture(GL_TEXTURE0);	 
	   glBindTexture(GL_TEXTURE_2D, cubeTex->texID);
	   glBindVertexArray(VAO);
	   glDrawArrays(GL_TRIANGLES, 0, 36);
	   glBindVertexArray(0);
	   
	   
	
	   /*floor*/
	   model = glm::translate(model, vec3(0.f, -0.56f, 0.f));
	   model = glm::scale(model, vec3(100.f, 0.1f, 100.f));
	   glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	   glUniform1i(glGetUniformLocation(cubeShader->program, "tex"),0);
	   glActiveTexture(GL_TEXTURE0);	 
	   glBindTexture(GL_TEXTURE_2D, floorTex->texID);
	   glBindVertexArray(VAO);
	   glDrawArrays(GL_TRIANGLES, 0, 36);
	   glBindVertexArray(0);

	  
	   
	   


	   //sky box 
	   glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	   view = glm::mat4(glm::mat3(view));
	   glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), glm::value_ptr(view));
	   glBindBuffer(GL_UNIFORM_BUFFER, 0);
	   glBindTexture(GL_TEXTURE_2D,0);

	   skyShader->useProgram();
	   glActiveTexture(GL_TEXTURE0);
	   glBindTexture(GL_TEXTURE_CUBE_MAP, skyTex->texID);
	   glUniform1i(glGetUniformLocation(skyShader->program, "sky"),0);
	   glBindVertexArray(cubeVao);
	   glDrawArrays(GL_TRIANGLES, 0, 36);
	   glBindVertexArray(0);


	

	 


	
	}
		void initData() {
			  GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  	 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	 0.0f, 1.0f
};
			
			  	  GLfloat skyVertices[] = {
    -1.f, -1.0f, -1.f, 
     1.f, -1.0f, -1.f, 
     1.f,  1.0f, -1.f, 
     1.f,  1.0f, -1.f, 
    -1.f,  1.0f, -1.f, 
    -1.f, -1.0f, -1.f, 

    -1.f, -1.0f,  1.f, 
     1.f, -1.0f,  1.f, 
     1.f,  1.0f,  1.f, 
     1.f,  1.0f,  1.f, 
    -1.f,  1.0f,  1.f, 
    -1.f, -1.0f,  1.f, 

    -1.f,  1.0f,  1.f, 
    -1.f,  1.0f, -1.f, 
    -1.f, -1.0f, -1.f, 
    -1.f, -1.0f, -1.f, 
    -1.f, -1.0f,  1.f, 
    -1.f,  1.0f,  1.f, 

     1.f,  1.0f,  1.f, 
     1.f,  1.0f, -1.f, 
     1.f, -1.0f, -1.f, 
     1.f, -1.0f, -1.f, 
     1.f, -1.0f,  1.f, 
     1.f,  1.0f,  1.f, 
	
    -1.f, -1.0f, -1.f, 
     1.f, -1.0f, -1.f, 
     1.f, -1.0f,  1.f, 
     1.f, -1.0f,  1.f, 
    -1.f, -1.0f,  1.f, 
    -1.f, -1.0f, -1.f, 

    -1.f,  1.0f, -1.f, 
     1.f,  1.0f, -1.f, 
     1.f,  1.0f,  1.f, 
     1.f,  1.0f,  1.f, 
    -1.f,  1.0f,  1.f, 
    -1.f,  1.0f, -1.f, 
};

			  cubeShader = new Shader("cube.ver","cube.frag");
			 lightShader = new Shader("Light.ver", "Light.frag");
			  skyShader = new Shader("skybox2.ver","skybox2.frag");
			  cubeTex = new Texture("container.jpg");
			  floorTex = new Texture("floor.png");

			  vector<string> vector;
			  vector.push_back("sky/vr_rt.tga");
			  vector.push_back("sky/vr_lf.tga");
			  vector.push_back("sky/vr_up.tga");
			  vector.push_back("sky/vr_dn.tga");
			  vector.push_back("sky/vr_bk.tga");
			  vector.push_back("sky/vr_ft.tga");
				
			  skyTex = new Texture(vector);

			  GLuint skyVBO;
			  glGenBuffers(1, &VBO);
			  glGenBuffers(1, &skyVBO);

			  glGenBuffers(1, &UBO);
			  glGenVertexArrays(1, &VAO);
			  glBindVertexArray(VAO);
			  glBindBuffer(GL_ARRAY_BUFFER, VBO);
			  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
			  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)   (3* sizeof(GLfloat)));
			  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6* sizeof(GLfloat)));
			  glEnableVertexAttribArray(0);
			  glEnableVertexAttribArray(1);
			  glEnableVertexAttribArray(2);
			  glBindVertexArray(0);


			  glGenVertexArrays(1, &cubeVao);
			  glBindVertexArray(cubeVao);
			  glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
			  glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
			  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);		
			  glEnableVertexAttribArray(0);
			  glBindVertexArray(0);





			  glBindBuffer(GL_UNIFORM_BUFFER,UBO);
			  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_DYNAMIC_DRAW);
			  GLuint index= glGetUniformBlockIndex(cubeShader->program, "ViewBlock");
			  glUniformBlockBinding(cubeShader->program, index, 0);
			  glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO,0, sizeof(glm::mat4) * 2);
			  glBindBuffer(GL_UNIFORM_BUFFER, 0);




			 







		}
public:
		
	LightTest() {

		}
	~LightTest() {

		}
};