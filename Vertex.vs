#version 330
layout(location = 0)in vec3 position;
layout(location = 1)in vec3 Normal;
uniform mat4 PVM;
out vec3 norm;
out vec3 pos;
void main()
{
   gl_Position=PVM*vec4(position,1.0);
   pos=position;
   norm=Normal;
}