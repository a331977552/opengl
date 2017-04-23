#include "stdafx.h"
#include "Texture.h"

Texture::Texture(vector<string> paths) {

	generateTextureCubeMap(paths);

}
Texture::Texture(string path) {
	
	if (path == "") {
		std::cout << "Texture path error" << std::endl;
		return;
	}
	int width, height, channel;
	unsigned char * data=SOIL_load_image(path.c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);
	if (!data) {
		REMINDER("fail to load  image:",path.c_str());
		return;
	}
	
	generateTexture2D(data, width, height, channel);

	


}


Texture::~Texture() {

	}

void Texture::generateTextureCubeMap(vector<string> paths) {
	
	GLuint type = GL_RGBA;
	

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	int width, height, channel;
	
	for (int i = 0; i < paths.size();i++) {
		string  path = paths[i];
	
		unsigned char * data=SOIL_load_image(path.c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);
		if (!data) {
			REMINDER("fail to load  image:",path.c_str());
			return;
		}
	
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);
}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


	}
void Texture::generateTexture2D(unsigned char  *data, int width, int height, int channel)
{GLuint type = GL_RGBA;
	if (channel == 3) {
		type = GL_RGB;
	}

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);
	glBindTexture(GL_TEXTURE_2D,0);
	


}
