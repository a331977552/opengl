#include "TextRender.h"



void TextRender::update(float dt)
{

}

void TextRender::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{

	
	textRenderShader.use();
	textRenderShader.setVector3f("texColor", color);
	textRenderShader.setInt("tex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	
	std::string::iterator  it= text.begin();
	for (it; it != text.end();it++)
	{
		 
		char  c=*it;
		Character   ch= characters[c];
		GLfloat xpos = x + ch.bearing.x*scale;
		GLfloat ypos = y + ( characters['H'].bearing.y-ch.bearing.y)*scale;
		GLfloat w = ch.size.x*scale;
		GLfloat h = ch.size.y*scale;


		GLfloat  vertices[6][4]{
			  { xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindTexture(GL_TEXTURE_2D, ch.texId);
		glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) *scale;
		
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

TextRender::TextRender(GLuint width,GLuint height)
{
	textRenderShader = ResourceManager::getInstance().loadShader("shaders/freetypeCharacter.vs", "shaders/freetypeCharacter.fs", nullptr, "freetypeCharacter");

	textRenderShader.setMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	textRenderShader.setInt("tex", 0);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft, "resources/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, 20);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


TextRender::~TextRender()
{
}
