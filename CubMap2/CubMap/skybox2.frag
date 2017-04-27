#version 330
in vec3 texCoords;
out vec4 color ;
uniform samplerCube sky;
void main(){
	color =texture(sky,texCoords);
}



