#include "Main.h"
 

Game  * Main::game= new Game(WIDTH,HEIGHT);
void Main::initEnv() {
	glfwInit();
	window=glfwCreateWindow(600, 400, "beanbean", NULL, NULL);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (!window) {
		std::cout << "fail to make window" << std::endl;
		return;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	glewInit();
	glGetError();
	game->init();
	
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode , int action, int mods) {
	
		if (key <= 1024 && key>0)
		{
			if (action == GLFW_PRESS) {				
				game->keys[key] = true; 
				if (game->keys[GLFW_KEY_ESCAPE]) {
					//glfwSetWindowShouldClose(window,GLFW_)
					exit(0);
				}
			}else if(action ==GLFW_RELEASE){
				game->keys[key] = false;
			}
		}
	});
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2, 0.2, 0.4, 0.5);

}




void Main::start() {
	
	while (!glfwWindowShouldClose(window))
	{
		float currentTime= glfwGetTime();
		float  deltTime = currentTime-lastTime;
		lastTime = currentTime;
		game->processInput(deltTime);
		game->update(deltTime);
		glClear(GL_COLOR_BUFFER_BIT);
		game->render();	
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	ResourceManager::getInstance().clear();
	glfwTerminate();	
}

Main::Main()
{

}


Main::~Main()
{
}
