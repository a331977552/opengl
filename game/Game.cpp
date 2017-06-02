#include "Game.h"

Game::Game(GLuint width, GLuint height):state(GameState::ACTIVE),width(width),height(height),keys() {

	}
void Game::init() {
	Shader  hero = ResourceManager::getInstance().loadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "hero");
	Shader  particle = ResourceManager::getInstance().loadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
	Shader  postprocessShader = ResourceManager::getInstance().loadShader("shaders/postprocessShader.vs", "shaders/postprocessShader.fs", nullptr, "particle");

	Texture2D  tex = ResourceManager::getInstance().loadTexture2D("resources/hero.png", "hero");
	ResourceManager::getInstance().loadTexture2D("resources/particle.png", "particle");

	ResourceManager::getInstance().loadTexture2D("resources/brick.png", "brick");
	ResourceManager::getInstance().loadTexture2D("resources/brick_soild.png", "brick_solid");
	ResourceManager::getInstance().loadTexture2D("resources/background.jpg", "background");
	ResourceManager::getInstance().loadTexture2D("resources/paddle.png", "paddle");


	processor = new PostProcessor(postprocessShader, width, height);
	GameLevel *level1 = new GameLevel("resources/level1.cfg", (float)width, (float)(height*0.5), *processor);
	GameLevel *level2 = new GameLevel("resources/level2.cfg", (float)width, (float)(height*0.5), *processor);
	GameLevel *level3 = new GameLevel("resources/level3.cfg", (float)width, (float)(height*0.5), *processor);
	GameLevel *level4 = new GameLevel("resources/level4.cfg", (float)width, (float)(height*0.5), *processor);
	GameLevel *level5 = new GameLevel("resources/level5.cfg", (float)width, (float)(height*0.5), *processor);
	gameLevels.push_back(level1);
	gameLevels.push_back(level2);
	gameLevels.push_back(level3);
	gameLevels.push_back(level4);
	gameLevels.push_back(level5);
	currentLevel = 0;


	glm::mat4 projection = glm::ortho(0.f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.f, -1.0f, 1.0f);

	texRender = new TextRender(width, height);

	glm::vec2 paddlePos = glm::vec2(width / 2 - playerSize.x / 2, height - playerSize.y);
	ball = new BallObject(paddlePos + glm::vec2(playerSize.x / 2 - ballSize.x / 2, -ballSize.y), ballSize, tex);
	ball->velocity = ball_velocity;
	paddle = new GameObject(paddlePos, playerSize, ResourceManager::getInstance().getTexture2D("paddle"));

	hero.use().setMatrix4("projection", projection);
	hero.setInt("tex", 0);

	sprite = new SpriteRenderer(hero);
	particle.setMatrix4("projection", projection, true);
	particleGenerator = new ParticleGenerator(particle, ResourceManager::getInstance().getTexture2D("particle"), 500);
	irrklang::ISoundEngine *soundEngine = SimpleAudioManager::getInstance().getSoundEngine();

	soundEngine->play2D("resources/breakout.mp3", true);
	soundEngine->addSoundSourceFromFile("resources/solid.wav", irrklang::E_STREAM_MODE::ESM_AUTO_DETECT, true);
	soundEngine->addSoundSourceFromFile("resources/bleep.mp3", irrklang::E_STREAM_MODE::ESM_AUTO_DETECT, true);
	soundEngine->addSoundSourceFromFile("resources/powerup.wav", irrklang::E_STREAM_MODE::ESM_AUTO_DETECT, true);

	gameLevels[currentLevel]->setOnBrickDestoriedListener([](void)	{
			
		
				
	}
		);
}
void Game::processInput(GLfloat dt) {
	if (state == GameState::ACTIVE) {
		 float velocity=VELOCITY*dt;
		if (keys[GLFW_KEY_A]) {
			if (paddle->position.x >= 0) {
				paddle->position.x -= velocity;
				if (paddle->position.x < 0)
					paddle->position.x = 0;
			}					
		}
		if (keys[GLFW_KEY_D]) {
			if (paddle->position.x <= (width - playerSize.x)) {
				paddle->position.x += velocity;
				if (paddle->position.x >  (width - playerSize.x))
					paddle->position.x =  (width - playerSize.x);
			}
			
		}
		if (keys[GLFW_KEY_SPACE]) {
			ball->isStuck = false;
		}
	
	}



	}
void Game::resetGame() {
	ball->isStuck = true;
	ball->velocity = ball_velocity;
	glm::vec2 paddlePos(width / 2 - playerSize.x / 2, height - playerSize.y);
	paddle->position = paddlePos;
	ball->position = paddlePos + glm::vec2(playerSize.x / 2 - ballSize.x / 2, -ballSize.y);
	const char * c=( "resources/level"+std::to_string(currentLevel+1)+".cfg").c_str();
	string  str = ("resources/level" + std::to_string(currentLevel+1) + ".cfg");

	gameLevels[currentLevel]->reload(str.c_str(), (float)width, height*0.5);


}
void Game::update(GLfloat dt) {
	if (ball->position.y >= height) {
		resetGame();
	}
		
		particleGenerator->update(dt, ball, 2, glm::vec2(ball->radius.x / 2));
		ball->move(dt, width);
		gameLevels[currentLevel]->doPaddCollision(*ball,*paddle);
		gameLevels[currentLevel]->doCircleCollision(*ball);		
		if (processor->shake) {
			processor->shakeTime -= dt;
			if (processor->shakeTime <= 0.0f) {
				processor->shake = false;
			}
		}


}
void Game::render() {
	
	processor->beginRender();
	sprite->drawSprite(ResourceManager::getInstance().getTexture2D("background"), glm::vec2(0, 0), glm::vec2(width, height));
	if (state == GameState::ACTIVE) {
		gameLevels[currentLevel]->draw(*sprite);
		paddle->draw(*sprite);
		if (ball->isStuck) {
		ball->position = paddle->position + glm::vec2(playerSize.x / 2 - ballSize.x / 2, -ballSize.y), ballSize;
		}
		particleGenerator->draw();
		ball->draw(*sprite);
	}
		processor->endRender();
		processor->render(glfwGetTime());
		
		texRender->renderText("current score:"+std::to_string(gameLevels[currentLevel]->score), 5,5, 1.0f, glm::vec3(1.0f, 1.0f, 0));
}



Game::~Game() {
	
}