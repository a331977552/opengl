#version 330
in vec2 TexCoords;
in vec3	Normal;



out vec4 color;
uniform sampler2D floorTex;
void main(){

	color=texture(floorTex,TexCoords);

}