#version 330
 uniform sampler2D Sampler;
 uniform int Coord;
 
 in vec3 pos;
 in vec3 norm;
 
 out vec4 FragColor;
void main()
{
	FragColor=vec4(normalize(norm),1);
}