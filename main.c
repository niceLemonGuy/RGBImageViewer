/*
 * main.c
 *
 *  Created on: Sep 21, 2020
 *      Author: roman
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "esUtil/esUtil.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Attribute locations
   GLint  positionLoc;
   GLint  texCoordLoc;

   // Sampler location
   GLint samplerLoc;

   // Texture handle
   GLuint textureId;

} UserData;

///
// Create texture image
//
GLuint knCreateTextureFromBuf(const char *filename, GLint width, GLint height)
{
	FILE *file;
	GLuint textureId;
	const int SIZE = width * height * 3;
	uint8_t buf[SIZE];

	if((file = fopen(filename, "r")) == NULL) {
		printf("Cannot open file.");
	}
	fread(buf, 1, SIZE, file);
	fclose(file);

	// Use tightly packed data
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	// Generate a texture object
	glGenTextures ( 1, &textureId );

	// Bind the texture object
	glBindTexture ( GL_TEXTURE_2D, textureId );

	// Load the texture
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 640, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, buf );

	// Set the filtering mode
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	return textureId;
}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext, const char *filename, GLint w, GLint h )
{
   esContext->userData = malloc(sizeof(UserData));
   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =
      "attribute vec4 a_position;   \n"
      "attribute vec2 a_texCoord;   \n"
      "varying vec2 v_texCoord;     \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = a_position; \n"
      "   v_texCoord = a_texCoord;  \n"
      "}                            \n";

   GLbyte fShaderStr[] =
      "precision mediump float;                            \n"
      "varying vec2 v_texCoord;                            \n"
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the attribute locations
   userData->positionLoc = glGetAttribLocation ( userData->programObject, "a_position" );
   userData->texCoordLoc = glGetAttribLocation ( userData->programObject, "a_texCoord" );

   // Get the sampler location
   userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );

   // Load the texture
   userData->textureId = knCreateTextureFromBuf(filename, w, h);

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   return GL_TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLfloat vVertices[] = { -1.0f, -1.0f, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0
                           -1.0f,  1.0f, 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
							1.0f,  1.0f, 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
							1.0f, -1.0f, 0.0f,  // Position 3
                            1.0f,  0.0f         // TexCoord 3
                         };
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( userData->positionLoc, 3, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), vVertices );
   // Load the texture coordinate
   glVertexAttribPointer ( userData->texCoordLoc, 2, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );

   glEnableVertexAttribArray ( userData->positionLoc );
   glEnableVertexAttribArray ( userData->texCoordLoc );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->textureId );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->samplerLoc, 0 );

   glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->textureId );

   // Delete program object
   glDeleteProgram ( userData->programObject );

   free(esContext->userData);
}

int main ( int argc, char **argv )
{
   ESContext esContext;
   UserData  userData;

	if (argc != 4) {
		puts("Invalid number of arguments.");
		return 1;
	}

   esInitContext ( &esContext );
   esContext.userData = &userData;

//   printf("Enter filename: ");
//   char filename[200];
//   scanf("%s", &filename);
//   printf("Enter width: ");
//   int width;
//   scanf("%d", &width);
//   printf("Enter height: ");
//   int height;
//   scanf("%d", &height);
//   printf("\n");

//   int i;
//
//	printf("%d\n", argc);
//
//	for (i=0; i < argc; i++)
//		puts(argv[i]);

   int w = 0, h = 0;
   w = strtol(argv[2], NULL, 10);
   h = strtol(argv[3], NULL, 10);

   esCreateWindow ( &esContext, "Simple Texture 2D", w, h, ES_WINDOW_RGB );

   if ( !Init ( &esContext, argv[1], w, h ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );

   esMainLoop ( &esContext );

   ShutDown ( &esContext );
}


