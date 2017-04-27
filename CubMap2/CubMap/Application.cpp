
#include "Application.h"
#include "LightTest.h"

void Application::initGL() {
	glfwInit();
	window=glfwCreateWindow(WIDTH, HEIGHT, "test go", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, [](GLFWwindow *window,int key,int scanncode,int action,int modes) {
		Camera::getInstance().processKeyCallBack(key, action);

	});
	glfwSetCursorPosCallback(window, [](GLFWwindow *window,double xpos,double ypos) {
		Camera::getInstance().processMouseCallBack(xpos, ypos);

	});
	glfwSetScrollCallback(window, [](GLFWwindow *window,double xoffset,double yoffset) {
					Camera::getInstance().processScrollCallBack(yoffset);
	});
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glfwSetWindowPos(window, 400, 200);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();
	}
	void Application::initData() {
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
		  GLfloat skyboxVertices[] = {
    // Positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,


    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
		  vector<string> face;
		face.push_back("i:/testpicture/right.jpg");
		face.push_back("i:/testpicture/left.jpg");
		face.push_back("i:/testpicture/top.jpg");
		face.push_back("i:/testpicture/bottom.jpg");
		face.push_back("i:/testpicture/back.jpg");
		face.push_back("i:/testpicture/front.jpg");
		modelCharacter = new Model("H:/model/planet.obj");
		skyBoxtex = new Texture(face);
		cubetex = new Texture("i:/container.jpg");
		floorTex = new Texture("i:/testpicture/floor.png");
		
		cubeShader = new Shader("cubeVer.ver", "cubeVer.frag");

		skyboxShader = new Shader("skybox.ver", "skybox.frag");
		floorShader = new Shader("floor.ver", "floor.frag");


		GLuint cubeVBO;
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1,&cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3* sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6* sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);
	
		glBindVertexArray(0);

		



		GLuint cubeVBO2;
		glGenVertexArrays(1, &skyVAO);
		glGenBuffers(1,&cubeVBO2);
		glBindVertexArray(skyVAO);
		glBindBuffer(GL_ARRAY_BUFFER,cubeVBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(GL_FLOAT), (void *)(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);



	}
	


	void Application::start() {
		initGL();
		//initData();
		LightTest test;
		test.initData();
		while (!glfwWindowShouldClose(window))
		{
			Camera::getInstance().onMovement();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();
			//onDraw();
			test.onDraw();
			glfwSwapBuffers(window);
		}
	}
	void Application::onDraw() {
		/*
		glm::mat4 projection = glm::perspective(Camera::getInstance().zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = Camera::getInstance().getMatrix();  //glm::lookAt(vec3(3.0f,3.0f,3.0f),vec3(0.0f,0.0f,.0f),vec3(0.0f,1.0f,0.0f));
		glm::mat4 model;

			    // Draw skybox first
       // glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader->useProgram();
		view = glm::mat4(glm::mat3(view));	// Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader->program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
      

		// skybox cube
        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(skyboxShader->program, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxtex->texID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
       // glDepthMask(GL_TRUE);

		view = Camera::getInstance().getMatrix(); 
		model = glm::translate(model, vec3(0.0f,0.0f,0.0f));
		cubeShader->useProgram();
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "projection"),1,GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "view"),1,GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->program, "model"),1,GL_FALSE, glm::value_ptr(model));
		
		glUniform1i(glGetUniformLocation(cubeShader->program, "cubeTex"), 0);
		//glBindTexture(GL_TEXTURE_2D, cubetex->texID);
	
	
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		modelCharacter->Draw(*cubeShader);

		floorShader->useProgram();
		model = glm::mat4();
		
		model =  glm::scale(model,vec3(100.f, 0.2f, 100.f));
		model = glm::translate(model,vec3(0.f, -2.5f, 0.f));
		glUniform1i(glGetUniformLocation(floorShader->program, "floorTex"),0);
		
		glBindTexture(GL_TEXTURE_2D, floorTex->texID);
		glUniformMatrix4fv(glGetUniformLocation(floorShader->program, "projection"),1,GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(floorShader->program, "view"),1,GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(floorShader->program, "model"),1,GL_FALSE, glm::value_ptr(model));


		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);

		
	*/

	

	}


	Application::Application() {
	}
	Application::~Application() {

	}

