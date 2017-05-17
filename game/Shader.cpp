#include "Shader.h"

	GLuint ID;
	Shader::Shader() {

	}
	Shader& Shader::use() {
		glUseProgram(ID);
		return *this;
	}
	void Shader::compile(const GLchar *  vertexSource, const GLchar * fragmeSource, const GLchar * geometrySource ){
		GLuint vertexShader, fragmentShader, geomotryShader;
		
		vertexShader =glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "vertex");


		fragmentShader =glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmeSource, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(vertexShader, "fragment");


		if (geometrySource != nullptr) {

			geomotryShader =glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geomotryShader, 1, &geometrySource, NULL);
			glCompileShader(geomotryShader);
		}
		this->ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		
		if (geometrySource != nullptr) {
			glAttachShader(ID, geomotryShader);
		}
		glLinkProgram(ID);
		checkCompileErrors(ID, "program");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometrySource != nullptr) {
			glDeleteShader(geomotryShader);
		}

			
	}
	void Shader::setFloat(const GLchar * name,GLfloat value,GLboolean useShader){
		if (useShader) {
			use();
		}
		glUniform1f(glGetUniformLocation(ID, name), value);

	}
	void Shader::setInt(const GLchar * name,GLint value,GLboolean useShader){
		if (useShader) {
			use();
		}
		glUniform1i(glGetUniformLocation(ID, name), value);
	}
	void Shader::setVector2f(const GLchar * name,const GLfloat x,const GLfloat y,GLboolean useShader){
		if (useShader) {
			use();
		}
		glUniform2f(glGetUniformLocation(ID, name),x,y);
	
	}
	void Shader::setVector2f(const GLchar * name,const  glm::vec2 &value,GLboolean useShader){
	
		if (useShader) {
			use();
		}
		glUniform2fv(glGetUniformLocation(ID, name),1,glm::value_ptr(value));

	}
	void Shader::setVector3f(const GLchar * name,const GLfloat x,const GLfloat y,const GLfloat z,GLboolean useShader){
		if (useShader) {
			use();
		}
		glUniform3f(glGetUniformLocation(ID, name),x,y,z);

	
	}
	void Shader::setVector3f(const GLchar * name,const  glm::vec3 &value,GLboolean useShader){
	
		if (useShader) {
			use();
		}
		glUniform3fv(glGetUniformLocation(ID, name),1,glm::value_ptr(value));

	}
		
	void Shader::setVector4f(const GLchar * name,const GLfloat x,const GLfloat y,const GLfloat z,const GLfloat w,GLboolean useShader){
	
		if (useShader) {
			use();
		}
		glUniform4f(glGetUniformLocation(ID, name),x,y,z,w);

		
	
	}
	void Shader::setVector4f(const GLchar * name,const  glm::vec4 &value,GLboolean useShader){
	
		if (useShader) {
			use();
		}
		glUniform4fv(glGetUniformLocation(ID, name),1,glm::value_ptr(value));

	}
	void Shader::setMatrix4(const GLchar * name,const  glm::mat4 &value,GLboolean useShader){
	
		if (useShader) {
			use();
		}
		glUniformMatrix4fv(glGetUniformLocation(ID, name),1,GL_FALSE,glm::value_ptr(value));
	}

	 Shader::~Shader(){
	}

	void  Shader::checkCompileErrors(GLuint object, std::string type){
		GLint success;
		GLchar info[1024];
		if (type == "program") {
			glGetProgramiv(object, GL_LINK_STATUS , &success);

			if (!success) {
				glGetProgramInfoLog(object, 1024, 0, info);
				std::cout <<"fail to link  program "<< info << "\n-- ---------------------------------------- --" << std::endl;
			}
		}
		else if (type == "vertex") {
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(object, 1024, 0, info);
				std::cout <<"fail to compile vertexShader "<< info << "\n-- ---------------------------------------- --" << std::endl;
			}
		}
		else if (type == "fragment") {
				glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(object, 1024, 0, info);
				std::cout <<"fail to compile fragmentShader "<< info << "\n-- ---------------------------------------- --" << std::endl;
			}
		}else {		
				std::cout << "wrong type of shader!!" <<__FILE__ <<"   \n"<< __func__ << std::endl;
			}
			


	}

