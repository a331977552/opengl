#version 330

layout(location  =0) in vec4 position;



uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;



out vec2 texCoords;
void main(){

	
	
	gl_Position =    vec4(position.xy,0,1);
	if(confuse){
		texCoords=vec2(1-position.z,1-position.w);
	}else if(chaos){
		float strength=0.03f;
		texCoords=vec2(position.z+  sin(time)* strength ,position.w+  cos(time)* strength );
	}else{
		texCoords=position.zw;
	}

	if(shake){
		float strength=0.01f;
		gl_Position.x+= cos(time*10)*0.01f;
		gl_Position.y+= cos(time*15)*0.01f;	
	}

}