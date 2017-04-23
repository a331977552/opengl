#version 330
in vec2 TexCoords;
in vec3	Normal;
in vec3	Position;


out vec4 color;
uniform vec3 camPos;
uniform samplerCube  skybox;
uniform sampler2D cubeTex;
void main(){
	vec3 viewerDir=  normalize(Position-camPos);
	vec3 reflectedDir= reflect(viewerDir,normalize(Normal));
		

	
	  color=texture(skybox,reflectedDir);


}