#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <gl/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "MathLibrary.h"
GLuint loadBMP_custom(const char * imagepath)
{

        printf("Reading image %s\n", imagepath);

        // Data read from the header of the BMP file
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;
        unsigned int width, height;
        // Actual RGB data
        unsigned char * data;

        // Open the file
        FILE * file = fopen(imagepath,"rb");
        if (!file)                                                          {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); return 0;}

        // Read the header, i.e. the 54 first bytes

        // If less than 54 byes are read, problem
        if ( fread(header, 1, 54, file)!=54 ){ 
                printf("Not a correct BMP file\n");
                return 0;
        }
        // A BMP files always begins with "BM"
        if ( header[0]!='B' || header[1]!='M' ){
                printf("Not a correct BMP file\n");
                return 0;
        }
        // Make sure this is a 24bpp file
        if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
        if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

        // Read the information about the image
        dataPos    = *(int*)&(header[0x0A]);
        imageSize  = *(int*)&(header[0x22]);
        width      = *(int*)&(header[0x12]);
        height     = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
        if (dataPos==0)      dataPos=54; // The BMP header is done that way

        // Create a buffer
        data = new unsigned char [imageSize];

        // Read the actual data from the file into the buffer
        fread(data,1,imageSize,file);

        // Everything is in memory now, the file wan be closed
        fclose (file);

        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // OpenGL has now copied the data. Free our own version
        delete [] data;

        // Poor filtering, or ...
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

        // ... nice trilinear filtering.
		glBindTexture(GL_TEXTURE_2D, 0);
        // Return the ID of the texture we just created
        return textureID;
}
void UniformMatrix(char *name, GLuint &program,mat4 Matrix)
{
	int Loc=glGetUniformLocation(program,name);
	
	glUniformMatrix4fv(Loc,1,GL_TRUE,Matrix.M);
};
bool loadOBJ(const char *path, std::vector<vec3> & out_vertices, std::vector<vec2> & out_uvs,std::vector<vec3> & out_normals){
        printf("Loading OBJ file %s...\n", path);

        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<vec3> temp_vertices; 
        std::vector<vec2> temp_uvs;
        std::vector<vec3> temp_normals;
        FILE * file = fopen(path, "r");
        if( file == NULL ){
                printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
                return false;
        }
        while( 1 ){
                char lineHeader[100];
                // read the first word of the line
                int res = fscanf(file, "%s", lineHeader);
                if (res == EOF)
                        break; // EOF = End Of File. Quit the loop.

                // else : parse lineHeader
                
                if ( strcmp( lineHeader, "v" ) == 0 ){
                        vec3 vertex;
                        fscanf(file, "%f %f %f\n", &vertex.X, &vertex.Y, &vertex.Z );
                        temp_vertices.push_back(vertex);
                }else if ( strcmp( lineHeader, "vt" ) == 0 ){
                        vec2 uv;
                        fscanf(file, "%f %f\n", &uv.X, &uv.Y );
                        //uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
                        temp_uvs.push_back(uv);
                }else if ( strcmp( lineHeader, "vn" ) == 0 ){
                        vec3 normal;
                        fscanf(file, "%f %f %f\n", &normal.X, &normal.Y, &normal.Z );
                        temp_normals.push_back(normal);
                }else if ( strcmp( lineHeader, "f" ) == 0 ){
                        std::string vertex1, vertex2, vertex3;
                        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                        if (matches != 9){
                                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                                return false;
                        }
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        uvIndices    .push_back(uvIndex[0]);
                        uvIndices    .push_back(uvIndex[1]);
                        uvIndices    .push_back(uvIndex[2]);
                        normalIndices.push_back(normalIndex[0]);
                        normalIndices.push_back(normalIndex[1]);
                        normalIndices.push_back(normalIndex[2]);
                }else{
                        // Probably a comment, eat up the rest of the line
                        char stupidBuffer[100];
                        fgets(stupidBuffer, 100, file);
                }

        }

        // For each vertex of each triangle
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){

                // Get the indices of its attributes
                unsigned int vertexIndex = vertexIndices[i];
                unsigned int uvIndex = uvIndices[i];
                unsigned int normalIndex = normalIndices[i];
                
                // Get the attributes thanks to the index
                vec3 vertex = temp_vertices[ vertexIndex-1 ];
                vec2 uv = temp_uvs[ uvIndex-1 ];
                vec3 normal = temp_normals[ normalIndex-1 ];
                
                // Put the attributes in buffers
                out_vertices.push_back(vertex);
                out_uvs     .push_back(uv);
                out_normals .push_back(normal);
        
        }

        return true;
}