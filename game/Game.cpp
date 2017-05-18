#include "Game.h"

Game::Game(GLuint width, GLuint height):state(GameState::ACTIVE),width(width),height(height),keys() {

	}
void Game::init() {
	Shader  hero=ResourceManager::getInstance().loadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr,"hero");
	Texture2D  tex=ResourceManager::getInstance().loadTexture2D("resources/hero.png","hero");
	ResourceManager::getInstance().loadTexture2D("resources/brick.png", "brick");
	ResourceManager::getInstance().loadTexture2D("resources/brick_soild.png", "brick_solid");
	ResourceManager::getInstance().loadTexture2D("resources/background.jpg", "background");
	ResourceManager::getInstance().loadTexture2D("resources/paddle.png", "paddle");
	
    glm::vec2 paddlePos=glm::vec2(width / 2 - playerSize.x / 2, height - playerSize.y);
	ball = new BallObject(paddlePos+glm::vec2(playerSize.x / 2-ballSize.x/2,-ballSize.y),ballSize,tex);
	ball->velocity = ball_velocity;

	GameLevel level1("resources/level1.cfg", (float)width, height*0.5);
	GameLevel level2("resources/level2.cfg", (float)width, height*0.5);
	GameLevel level3("resources/level3.cfg", (float)width, height*0.5);
	GameLevel level4("resources/level4.cfg", (float)width, height*0.5);
	GameLevel level5("resources/level5.cfg", (float)width, height*0.5);
	gameLevels.push_back(level1);
	gameLevels.push_back(level2);
	gameLevels.push_back(level3);
	gameLevels.push_back(level4);
	gameLevels.push_back(level5);
	currentLevel = 0;




	glm::mat4 projection = glm::ortho(0.f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.f, -1.0f, 1.0f);
	paddle = new GameObject(paddlePos, playerSize, ResourceManager::getInstance().getTexture2D("paddle"));

	hero.use().setMatrix4("projection", projection);
	hero.setInt("tex", 0);

	sprite = new SpriteRenderer(hero);

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
void Game::update(GLfloat dt) {
		ball->move(dt, width);
		gameLevels[currentLevel].doCircleCollision(*ball);
}
void Game::render() {
		sprite->drawSprite(ResourceManager::getInstance().getTexture2D("background"), glm::vec2(0, 0), glm::vec2(width, height));
	if (state == GameState::ACTIVE) {
		gameLevels[currentLevel].draw(*sprite);
		paddle->draw(*sprite);
		if (ball->isStuck) {
		ball->position = paddle->position + glm::vec2(playerSize.x / 2 - ballSize.x / 2, -ballSize.y), ballSize;
		}
		ball->draw(*sprite);
	}


}



Game::~Game() {
	
}