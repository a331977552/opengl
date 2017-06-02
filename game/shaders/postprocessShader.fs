#version 330
in vec2 texCoords;

out vec4 color;
uniform sampler2D tex;
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;


void main(){

	color = texture(tex,texCoords);
	vec3[9] sample;
	if(chaos ||shake){
		for(int i=0;i<9;i++){
			sample[i]=vec3(texture(tex,texCoords.st+offsets[i]));
		}
	}
	if(chaos){
		   for(int i = 0; i < 9; i++)
            color += vec4(sample[i] * edge_kernel[i], 0.0f);
			color.a = 1.0f;
	}



}

