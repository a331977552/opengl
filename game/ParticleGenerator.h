#pragma once
#include "ResourceManager.h"
#include <vector>
#include "GameObject.h"
using std::vector;
class ParticleGenerator
{
	
public:
	struct  Particle{
		float life;
		glm::vec2 velocity,position;
		glm::vec4 color;
		Particle() :life(0), velocity(0), position(0), color(1.0f) {

		}

	};

	void update(GLfloat dt, GameObject *obj, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	
	ParticleGenerator(Shader shader,Texture2D tex,GLuint amount);
	void draw();

private :
	vector<Particle> particles;
	void init();
	Texture2D tex;
	GLuint amount,VAO;
	Shader shader;
	GLuint firstUnusedParticle();
	void respawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset = glm::vec2(0, 0));
	GLuint firstUnusedPartice;
public :

	~ParticleGenerator();
};

