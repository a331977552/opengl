#version 330
in vec3  Normal;
in vec2  TexCoords;
in vec3 fragPos;
out vec4 color ;
uniform sampler2D tex;
uniform vec3 viewPos;
uniform vec3 lightPos;

uniform vec3 ambient;
uniform vec3 specular;
uniform vec3 diffuse;
uniform float shininess;




void main(){
	 vec3 norm=normalize(Normal);
	
	
	 vec3 diffuse = max(dot(normalize( lightPos-fragPos),norm),0) *diffuse;

	  vec3 reflectedDir=   reflect (-normalize( lightPos-fragPos) , norm) ;

	 
	 vec3 viewerDir= normalize (viewPos-fragPos);
	 

	 vec3 specular  = pow(  max(dot ( reflectedDir,viewerDir),0),shininess )  *specular;


	color =  vec4( (ambient+ diffuse + specular) ,1.0f ) * texture(tex,TexCoords);

}
