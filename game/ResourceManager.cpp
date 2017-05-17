#include "ResourceManager.h"


 Shader ResourceManager::loadShader(const char * vShaderFile, const char * fShaderFile, const char * gShaderFile,std::string name){
	 return  shaders[name]=loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);

	
 }
	 Texture2D ResourceManager::loadTexture2D(const char *path,std::string name, bool alpha){
		return  texture2Ds[name] = loadTexture2DFromFile (path);
	}
	 Texture2D ResourceManager::getTexture2D(std::string name){

		 return  texture2Ds[name];
	}
	 Shader ResourceManager::getShader(std::string name){
		  return  shaders[name];
	}
	 void ResourceManager::clear(){

		 for (std::map<std::string, Shader>::iterator it = shaders.begin(); it != shaders.end(); it++) {
			    std::pair<std::string,Shader>  pair=(*it);
				glDeleteProgram(pair.second.ID);
		}		 
		 for (std::map<std::string, Texture2D>::iterator it = texture2Ds.begin(); it != texture2Ds.end(); it++) {
			    std::pair<std::string,Texture2D>  pair=(*it);
				glDeleteTextures(1,&pair.second.ID);
		}
		 shaders.clear();
		 texture2Ds.clear();


	}

	  ResourceManager& ResourceManager::getInstance() {
		 static ResourceManager *manger = new ResourceManager();
		 return *manger;
	 }


	

	Shader ResourceManager::loadShaderFromFile(const char * vShaderFile, const char * fShaderFile, const char * gShaderFile){
		
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		
		try {
			
			std::ifstream vf(vShaderFile);
			std::ifstream ff(fShaderFile);
			stringstream stream1,stream2;
			stream1 << vf.rdbuf();
			stream2 << ff.rdbuf();
			vertexCode = stream1.str();
			fragmentCode = stream2.str();
		
			if (gShaderFile)
			{				
				std::ifstream gf(gShaderFile);
				stringstream stream3;
				stream3 << gf.rdbuf();	
				geometryCode = stream3.str();
			}

		}
		catch (std::exception e) {
				 std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;

		}
	const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();

	Shader shader;
	shader.compile(vShaderCode, fShaderCode, geometryCode==""?nullptr: geometryCode.c_str());
	return shader;
	}
	Texture2D ResourceManager::loadTexture2DFromFile(const char *path){
		Texture2D tex;
		tex.generate(path);
		return tex;
	}

	ResourceManager::ResourceManager() {

	}