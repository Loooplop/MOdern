#include "Pipeline.h"


Pipeline::Pipeline(void)
{
	vec3 Reset(0,0,0);
	Translation=Reset;
	Rotation=Reset;
	ScaleV=vec3(1,1,1);
	Camera_Origin=Reset;
	Camera_Target=Reset;
    Perspective=vec4(0,0,0,0);
}


Pipeline::~Pipeline(void)
{

}
mat4 Pipeline::GetPVMatrix()
{
	mat4 Model = Handle.TranslationMxv(Translation)*Handle.RotateXYZMxf(Rotation.X,Rotation.Y,Rotation.Z)*Handle.ScaleMxv(ScaleV);
	mat4 View = Handle.ViewMx(Camera_Origin,Camera_Target,vec3(0,1,0));
	mat4 Pers = Handle.PerspectiveMx(Perspective.X,Perspective.Y,Perspective.Z,Perspective.W);
	return Pers*View*Model;
};
mat4 Pipeline::GetModelMatrix()
{
	return Handle.TranslationMxv(Translation)*Handle.RotateXYZMxf(Rotation.X,Rotation.Y,Rotation.Z)*Handle.ScaleMxv(ScaleV);
}
mat4 Pipeline::GetViewMatrix()
{
	return Handle.ViewMx(Camera_Origin,Camera_Target,vec3(0,1,0));
};
mat4 Pipeline::GetPerspectiveMatrix()
{
  return Handle.PerspectiveMx(Perspective.X,Perspective.Y,Perspective.Z,Perspective.W);
};
void Pipeline::Translate(float x,float y,float z)
{
	Translation = vec3(x,y,z);
};
void Pipeline::Rotate(float phi,float theta,float psi)
{
	Rotation=vec3(phi,theta,psi);
};
void Pipeline::Scale(float x,float y,float z)
{
    ScaleV=vec3(x,y,z);		
};
void Pipeline::SetPerspectiveMatrix(float fov,float aspect,float near,float far)
{
	Perspective=vec4(fov,aspect,near,far);
};
void Pipeline::SetCamera(vec3 Origin,vec3 Target)
{
  Camera_Origin=Origin;
  Camera_Target=Target;
};
void Pipeline::SetCamera(float Origin_x,float Origin_y,float Origin_z,float Target_x,float Target_y,float Target_z)
{
	Camera_Origin=vec3(Origin_x,Origin_y,Origin_z);
  Camera_Target=vec3(Target_x,Target_y,Target_z);
};