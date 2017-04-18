#ifndef __TEXTURE_H__
#define  __TEXTURE_H__
#include <SOIL\SOIL.h>
#include <GLFW\glfw3.h>
#include <GL\glew.h>
#include <sstream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
using std::string;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
class Texture {

public:
	GLuint texId;
	Texture(string path) {
		if (path == "") {
			cout << "texture path is NULL" << endl;
			return;
		}
		int width, height, channels,type=GL_RGBA;
		unsigned char *  data=SOIL_load_image(path.c_str(), &width, &height,&channels ,SOIL_LOAD_AUTO);
		if (!data) {
			cout << "fail to load  image, please check the path" << endl;
			return;
		}
		if (channels == 3) {
			type = GL_RGB;
		}
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}



};



#endif // !__TEXTURE_H__