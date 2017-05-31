#version 330
in vec2 texCoords;
uniform sampler2D tex;
uniform vec4 particleColor;
out vec4 color;
void main(){
	color = texture(tex,texCoords)*particleColor;

}

