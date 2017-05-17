#version 330
layout (location = 0) in vec4 position;
out vec2 texCoords;

uniform mat4 projection;
uniform mat4 model;


void main(){
	
	texCoords= position.zw;
	gl_Position = projection*model * vec4(position.xy,0.0,1.0);

}