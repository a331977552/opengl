#version 330
layout(location  =0) in vec4 position;

uniform mat4 projection;

uniform vec2 offset;

out vec2 texCoords;
void main(){
 texCoords=position.zw;

 float scale =10.f;
 gl_Position = projection  * vec4(position.xy *scale  +offset,0,1);


}