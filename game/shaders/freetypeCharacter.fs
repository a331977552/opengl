#version 330

uniform sampler2D tex;
uniform vec3 texColor;
in vec2 texCoords;
out vec4 color;
void main(){
	
	vec4 co =   vec4(1.0f,1.0f,1.0f,texture(tex,texCoords).r);
	color =co*vec4( texColor,1.0f);
}