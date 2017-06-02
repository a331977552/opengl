#include "PostProcessor.h"


PostProcessor::PostProcessor(Shader processShader, GLuint width, GLuint height)
:processShader(processShader),width(width),height(height),confuse(false),chaos(false){

	initRender();
}

void PostProcessor::beginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

}

void PostProcessor::endRender()
{

	glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void PostProcessor::render(GLfloat dt)
{
	processShader.use();
	processShader.setFloat("time",dt);
	this->processShader.setInt("confuse", confuse);
	this->processShader.setInt("chaos", chaos);
	this->processShader.setInt("shake", shake);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex.ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);


}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::initRender()
{
	glGenFramebuffers(1, &FBO);
	glGenFramebuffers(1, &MSFBO);
	glGenRenderbuffers(1, &RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
	}

	tex.generateEmpty(width,height);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	GLuint VBO;
	GLfloat vertices[] = {
		// Pos        // Tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	processShader.setInt("tex", 0,true);
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};
	

	processShader.setVector2fv("offsets",9,(GLfloat *)offsets);
	GLint edge_kernel[9] = {
	  -1, -1, -1,
	  -1,  8, -1,
	  -1, -1, -1
	};
	

	glUniform1iv(glGetUniformLocation(this->processShader.ID, "edge_kernel"), 9, edge_kernel);
	GLfloat blur_kernel[9] = {
	  1.0 / 16, 2.0 / 16, 1.0 / 16,
	  2.0 / 16, 4.0 / 16, 2.0 / 16,
	  1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(processShader.ID, "blur_kernel"), 9, blur_kernel);

}
