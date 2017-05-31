#include "ParticleGenerator.h"




void ParticleGenerator::update(GLfloat dt, GameObject *obj, GLuint newParticles, glm::vec2 offset /*= glm::vec2(0.0f, 0.0f)*/)
{
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], *obj, offset);
	}
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.life -= dt;
		if (p.life > 0.0f) {
			p.color.a -= dt*2.5f;
			p.position -= p.velocity*dt;
		}

	}

}



void ParticleGenerator::init()
{
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	for (int i = 0; i < amount; i++) {
		particles.push_back(Particle());
	}
}




GLuint ParticleGenerator::firstUnusedParticle()
{
	for (int i = firstUnusedPartice; i < particles.size(); i++)
	{
		if (particles[i].life <= 0.0f) {
			firstUnusedPartice = i;
			return i;
		}
	}

	for (int i = 0; i < firstUnusedPartice; i++)
	{
		if (particles[i].life <= 0.0f) {
			firstUnusedPartice = i;
			return i;
		}
	}
	firstUnusedPartice = 0;
	return 0;	
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset /*= glm::vec2(0, 0)*/)
{
	
	GLfloat randow = (rand() % 100 -50) / 10.f;
	particle.color = glm::vec4(1.0f);
	particle.life = 1.f;
	particle.position = obj.position + offset + glm::vec2(randow);
	particle.velocity = obj.velocity*0.1f;

}

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D tex, GLuint amount)
:shader(shader),tex(tex),amount(amount),firstUnusedPartice(0){
	init();
}

void ParticleGenerator::draw()
{

	  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader.use();
	shader.setInt("tex", 0);
	tex.bind();
	for (int i = 0; i < amount; i++) {
		
		Particle & particle=particles[i];
		if (particle.life > 0.0f) {
			shader.setVector2f("offset", particle.position);
			shader.setVector4f("particleColor", particle.color);		
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

ParticleGenerator::~ParticleGenerator()
{
}
