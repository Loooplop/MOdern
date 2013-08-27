#pragma once
#include "MathLibrary.h"
class Pipeline
{
public:
	Pipeline(void);
	~Pipeline(void);
	mat4 GetPVMatrix();
	mat4 GetModelMatrix();
	mat4 GetViewMatrix();
	mat4 GetPerspectiveMatrix();
	void Translate(float x,float y,float z);
	void Scale(float x,float y,float z);
	void Rotate(float phi,float theta,float psi);
	void SetPerspectiveMatrix(float fov,float aspect,float near,float far);
	void SetCamera(vec3 Origin,vec3 Target);
	void SetCamera(float Origin_x,float Origin_y,float Origin_z,float Target_x,float Target_y,float Target_z);

		 vec3 Translation;
		 vec3 Rotation;
		 vec3 ScaleV;
		 vec3 Camera_Origin,Camera_Target;
		 vec4 Perspective;
		 TransMatrix Handle;
};

