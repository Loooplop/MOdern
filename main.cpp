//------------------------------------------------------
//Includes
#include "MathLibrary.h"
#include "Shaders.h"
#include "Main.h"
#include "ImageLoader.h"
#include "Pipeline.h"
using namespace std;
//-------------------------------------------------------
//Variables
TransMatrix Handler; //Handler object that codes uses for Creating the Model, View, and Projection matrices
GLuint Vert,Frag,ProgId;//Variables that hold handles to Vertex Shader, Fragment Shader, and Program
GLuint vboID,Tex,Coordinate;//Vertex Buffer Object Handle
Pipeline Renderer;
float X=0;
float Y=0;
float Z=5;
double time=0; //DEBUG-Used for visual testing of Rotation Model Matrix
vec3Vector Normals,Position;
vec2Vector UV;
GLfloat vertices[] = {0,0,0,
	                  0,1,0,
					  1,1,0,
                      
					  0,0,0,
                      1,0,0,
                      1,1,0 };// Vetex Data Sent to Video Device through Vertex Buffer Object
GLfloat TEx[]={0,0,
	           0,1,
			   1,1,
               
			   0,0,
               1,0,
               1,1};
GLfloat Coord[]={-5,0,0,
	              5,0,0,

				  0,-5,0,
				  0,5,0,

				  0,0,5,
				  0,0,-5,

                  0,0,5,
                  0,2,5,

				  0,2,5,
				  0,2,0};
GLuint Texture_0;
//--------------------------------------------------------
//Function Header-ResizeFunction.
//INPUTS Integer Width = Width of Current Window
//       Integer Height = Height of Current Window
// OUTPUT NONE-/Void Function
void ResizeFunction(int Width, int Height)
{
	glViewport(0, 0, Width,Height);// Reset ViewPort
	Renderer.SetPerspectiveMatrix(45,(float)Width/(float)Height,1,1000);
};
float v;
float FPS_Variable=0;
void CalculateFrameRate()
    {

        static float framesPerSecond    = 0.0f;       // This will store our fps
        static float lastTime   = 0.0f;       // This will hold the time from the last frame
        float currentTime = GetTickCount() * 0.001f;    
        ++framesPerSecond;
        if( currentTime - lastTime > 1.0f )
        {
            lastTime = currentTime;
            FPS_Variable=framesPerSecond;
            char title[80];
            sprintf(title, "VBO Example - %.2f", FPS_Variable);
            glutSetWindowTitle(title);
            framesPerSecond = 0;
        }
    }
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	v += 0.1f;
	Renderer.Rotate(0,v,0);
	Renderer.Translate(0,0,0);

	UniformMatrix("PVM",ProgId,Renderer.GetPVMatrix());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, Coordinate);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER,Tex);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, Position.size());
    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	CalculateFrameRate();
    glutSwapBuffers();
};
void SetupVAO()
{
	
	glGenBuffers(1, &vboID);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glGenBuffers(1, &Tex);
    
    glBindBuffer(GL_ARRAY_BUFFER, Tex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &Coordinate);
    
    glBindBuffer(GL_ARRAY_BUFFER, Coordinate);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*Position.size(), &Position[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void KeyBoard(unsigned char Key,int x,int y)
{
	switch(Key)
	{
	case 'a':
		X -=1;
		Renderer.SetCamera(X,Y,Z,0,0,0);
		break;
	case 'd':
		X +=1;
		Renderer.SetCamera(X,Y,Z,0,0,0);
		break;
	case 'w':
		Y +=1;
		Renderer.SetCamera(X,Y,Z,0,0,0);
		break;
	case 's':
		Y -=1;
		Renderer.SetCamera(X,Y,Z,0,0,0);
		break;
	case 'q':
		Z -=1;
		Renderer.SetCamera(X,Y,Z,0,0,0);
		break;
	case 'z':
		Z +=1;
		Renderer.SetCamera(X,Y,Z,0,0,0);
		break;
	case 'i':
		cout<<Renderer.Camera_Origin<<endl;
		break;
	};
}
int main(int argc, char* argv[])
{
	loadOBJ("suzanne.obj",Position,UV,Normals);
	Renderer.SetPerspectiveMatrix(45,1024/768,1,1000);
	Renderer.SetCamera(0,0,5,0,0,0);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(640,480);
    glutCreateWindow("Hi");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glewExperimental=TRUE;
    glewInit();
	setShaders("Vertex.vs","Fragment.fs",Vert,Frag,ProgId);
	Texture_0 = loadTexture("fnejr.bmp");
	SetupVAO();
	glUniform1i(glGetUniformLocation(ProgId,"Sampler"),0);
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(KeyBoard);
	glutIdleFunc(RenderFunction);
	glutMainLoop();
	exit(0);
};