#define _WCHAR_T_DEFINED
#define _STDCALL_SUPPORTED
#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

static GLint    ImageWidth;
static GLint    ImageHeight;
static GLint    PixelLength;
static GLubyte* PixelData;

int main(int argc, char * argv[])
{
	FILE* pFile = fopen("1.bmp", "rb");
    if( pFile == 0 )
        exit(0);
	
	fseek(pFile, 0x0012, SEEK_SET);
    fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
    fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);
	
	PixelLength = ImageWidth * 3;
    while( PixelLength % 4 != 0 )
        ++PixelLength;
    PixelLength *= ImageHeight;
	
	PixelData = (GLubyte*)malloc(PixelLength);
    if( PixelData == 0 )
        exit(0);
    
    fseek(pFile, 54, SEEK_SET);
    fread(PixelData, PixelLength, 1, pFile);
	
	fclose(pFile);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(ImageWidth, ImageHeight);
	glutInitWindowPosition(50, 70);
	glutCreateWindow("View Morphing");

	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	
	free(PixelData);
	return 0;
}

void MyDisplay()
{

	glDrawPixels(ImageWidth, ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);
	glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{

	switch(key) 
	{
	case 27:
		exit(0);
		break;
	}
}