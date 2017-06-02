#include "Texture2D.h"

	
	void Texture2D::generate(const  GLchar *const str) {
		this->width = width;
		this->height = height;
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_T);
		unsigned char *data=SOIL_load_image(str, &width, &height, &channels, SOIL_LOAD_AUTO);
		if (!data)
		{
			std::cout << "fail to load image:  "<< str << "\n-- --------------------------- --" << std::endl;
			return;
		}
		GLuint format = GL_RGBA;
		if (channels == 3) {
			format = GL_RGB;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(data);

	}
	void Texture2D::bind()const{
	
		glBindTexture(GL_TEXTURE_2D, ID);
		
	}

	void Texture2D::generateEmpty(GLuint width, GLuint height)
	{
		this->width = width;
		this->height = height;
		this->channels = 3;
		
		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	Texture2D::Texture2D() :width(0), height(0), wrap_S(GL_REPEAT), wrap_T(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR) {

	}
	Texture2D::~Texture2D(){

	}
