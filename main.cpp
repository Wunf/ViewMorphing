#define _WCHAR_T_DEFINED
#define _STDCALL_SUPPORTED
#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include "main.h"

using namespace std;

static GLint    ImageWidth;
static GLint    ImageHeight;
static GLint    PixelLength;
static GLubyte* PixelData;
static GLubyte* PixelData2;
static GLubyte* PixelData3;
static GLubyte* PixelData4;

const double f = 5.0f;
const double d = 1000.0f;
const double pai = 3.1415926f;
const int pn = 8;
const int ln = 10;
int points[pn][2], pointd[pn][2], pointm[pn][2], pointm1[pn][2], pointm2[pn][2];
int line[ln][2];
double t;


int main(int argc, char * argv[])
{
	lineinit();
	ReadFile("1.bmp");
	ReprojectionI0();
	ReadFile("2.bmp");
	ReprojectionI1();
	t = 0.0f;
	Morphing(t);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(ImageWidth * 3, ImageHeight );
	glutInitWindowPosition(50, 70);
	glutCreateWindow("View Morphing");

	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	
	free(PixelData);
	return 0;
}

void ReadFile(const char* filename)
{
	FILE* pFile = fopen(filename, "rb");
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
}

void MyDisplay()
{
	glDrawPixels(ImageWidth * 3, ImageHeight , GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData4);
	glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{

	switch(key) 
	{
	case 27:
		exit(0);
		break;
	case 'n':
		t += 0.1f;
		free(PixelData4);
		Morphing(t);
		glutPostRedisplay();
		break;
	}
}

void ReprojectionI0()
{
	double R[3][3];
	R[0][0] = cos(-pai / 4);     R[0][1] = 0.0f;     R[0][2] = -sin(-pai / 4) * f; 
	R[1][0] = 0.0f;              R[1][1] = 1.0f;     R[1][2] = 0.0f; 
	R[2][0] = sin(-pai / 4) / f; R[2][1] = 0.0f;     R[2][2] = cos(-pai / 4);
	
	PixelData2 = (GLubyte*)malloc(PixelLength * 3);
	for(int i = 0; i < PixelLength * 3; ++i)
		PixelData2[i] = 255;
	double x, y, x1, y1;
	
	for(int i = 0; i < ImageWidth; ++i)
	{
		for(int j = 0; j < ImageHeight ; ++j)
		{	
			x = (double(i) - double(ImageWidth) / 2) / 100.0f;
			y = (double(ImageHeight) / 2 - double(j)) / 100.0f;	
			if(x <= 0)
			{
				x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (1 / sin(pai / 4));
				y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (1 / sin(pai / 4));
			}
			else
			{
				x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (double(x) * 0.39f + 1 / sin(pai / 4));
				y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (double(x) * 0.39f + 1 / sin(pai / 4));
			}
			int n = ImageHeight / 2 - int(y1 * 100.0f + 0.5f);
			int m = int(x1 * 100.0f + 0.5f) + ImageWidth / 2;
			int index = ((ImageHeight - j - 1) * ImageWidth * 3) + i * 3;
			int index2 = ((ImageHeight - n - 1) * ImageWidth * 3 * 3) + m * 3;
			if(index2 >= 0 && index2 < PixelLength * 3)
			{
				PixelData2[index2] = PixelData[index];
				PixelData2[index2 + 1] = PixelData[index + 1];
				PixelData2[index2 + 2] = PixelData[index+ 2];
			}
		}
	}
	
	for(int i = 0; i < pn; ++i)
	{
		x = (double(points[i][0]) - double(ImageWidth) / 2) / 100.0f;
		y = (double(ImageHeight) / 2 - double(points[i][1])) / 100.0f;	
		if(x <= 0)
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (1 / sin(pai / 4));
		}
		else
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (double(x) * 0.39f + 1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (double(x) * 0.39f + 1 / sin(pai / 4));
		}
		int n = ImageHeight / 2 - int(y1 * 100.0f + 0.5f);
		int m = int(x1 * 100.0f + 0.5f) + ImageWidth / 2;
		points[i][0] = m;
		points[i][1] = n;
	}
	
	for(int i = 0; i < pn; ++i)
	{
		x = (double(pointm1[i][0]) - double(ImageWidth) / 2) / 100.0f;
		y = (double(ImageHeight) / 2 - double(pointm1[i][1])) / 100.0f;	
		if(x <= 0)
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (1 / sin(pai / 4));
		}
		else
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (double(x) * 0.39f + 1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (double(x) * 0.39f + 1 / sin(pai / 4));
		}
		int n = ImageHeight / 2 - int(y1 * 100.0f + 0.5f);
		int m = int(x1 * 100.0f + 0.5f) + ImageWidth / 2;
		pointm1[i][0] = m;
		pointm1[i][1] = n;
	}
}

void ReprojectionI1()
{
	double R[3][3];
	R[0][0] = cos(pai / 4);     R[0][1] = 0.0f;     R[0][2] = -sin(pai / 4) * f; 
	R[1][0] = 0.0f;              R[1][1] = 1.0f;     R[1][2] = 0.0f; 
	R[2][0] = sin(pai / 4) / f; R[2][1] = 0.0f;     R[2][2] = cos(pai / 4);
	
	PixelData3 = (GLubyte*)malloc(PixelLength * 3);
	for(int i = 0; i < PixelLength * 3; ++i)
		PixelData3[i] = 255;
	double x, y, x1, y1;
	
	for(int i = 0; i < ImageWidth; ++i)
	{
		for(int j = 0; j < ImageHeight ; ++j)
		{	
			x = (double(i) - double(ImageWidth) / 2) / 100.0f;
			y = (double(ImageHeight) / 2 - double(j)) / 100.0f;	
			if(x >= 0)
			{
				x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (1 / sin(pai / 4));
				y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (1 / sin(pai / 4));
			}
			else
			{
				x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (double(-x) * 0.39f + 1 / sin(pai / 4));
				y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (double(-x) * 0.39f + 1 / sin(pai / 4));
			}
			int n = ImageHeight / 2 - int(y1 * 100.0f + 0.5f);
			int m = int(x1 * 100.0f + 0.5f) + ImageWidth * 5 / 2;
			int index = ((ImageHeight - j - 1) * ImageWidth * 3) + i * 3;
			int index2 = ((ImageHeight - n - 1) * ImageWidth * 3 * 3) + m * 3;
			if(index2 >= 0 && index2 < PixelLength * 3)
			{
				PixelData3[index2] = PixelData[index];
				PixelData3[index2 + 1] = PixelData[index + 1];
				PixelData3[index2 + 2] = PixelData[index+ 2];
			}
		}
	}
	
	for(int i = 0; i < pn; ++i)
	{
		x = (double(pointd[i][0]) - double(ImageWidth) / 2) / 100.0f;
		y = (double(ImageHeight) / 2 - double(pointd[i][1])) / 100.0f;	
		if(x >= 0)
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (1 / sin(pai / 4));
		}
		else
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (double(-x) * 0.39f + 1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (double(-x) * 0.39f + 1 / sin(pai / 4));
		}
		int n = ImageHeight / 2 - int(y1 * 100.0f + 0.5f);
		int m = int(x1 * 100.0f + 0.5f) + ImageWidth * 5 / 2;
		pointd[i][0] = m;
		pointd[i][1] = n;
	}
	
	for(int i = 0; i < pn; ++i)
	{
		x = (double(pointm2[i][0]) - double(ImageWidth) / 2) / 100.0f;
		y = (double(ImageHeight) / 2 - double(pointm2[i][1])) / 100.0f;	
		if(x >= 0)
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (1 / sin(pai / 4));
		}
		else
		{
			x1 = (R[0][0] * x + R[0][1] * y + R[0][2]) * (double(-x) * 0.39f + 1 / sin(pai / 4));
			y1 = (R[1][0] * x + R[1][1] * y + R[1][2]) * (double(-x) * 0.39f + 1 / sin(pai / 4));
		}
		int n = ImageHeight / 2 - int(y1 * 100.0f + 0.5f);
		int m = int(x1 * 100.0f + 0.5f) + ImageWidth * 5 / 2;
		pointm2[i][0] = m;
		pointm2[i][1] = n;
	}
}

void lineinit()
{
	//source
	ifstream fin("pointsmapping.txt");
	
	for(int i = 0; i < pn; ++i)
	{
		fin >> points[i][0] >> points[i][1] >> pointd[i][0] >> pointd[i][1] >> pointm1[i][0] >> pointm1[i][1] >> pointm2[i][0] >> pointm2[i][1];
	}
	
	for(int i = 0; i < ln; ++i)
	{
		fin >> line[i][0] >> line[i][1];
	}
	
	fin.close();
}

void Morphing(double t)
{
	int (*pp1)[2], (*pp2)[2];
	GLubyte * pImage;
	int point[pn][2];
	
	for(int i = 0; i < pn; ++i)
	{
		pointm[i][0] = (pointm1[i][0] + pointm2[i][0]) / 2;
		pointm[i][1] = (pointm1[i][1] + pointm2[i][1]) / 2;
	}
	
	if(t <= 1.0f)
	{
		pp1 = points;
		pp2 = pointm;
		pImage = PixelData2;
	}
	else
	{
		pp1 = pointd;
		pp2 = pointm;
		t = 2.0f - t;
		pImage = PixelData3;
	}
	
	for(int i = 0; i < pn; ++i)
	{
		point[i][0] = int(t * double(pp2[i][0]) + (1 - t) * double(pp1[i][0]) + 0.5);
		point[i][1] = int(t * double(pp2[i][1]) + (1 - t) * double(pp1[i][1]) + 0.5);
	}
	
	const double a = 0.1f, b = 1.5f, p = 0.5f; 
	PixelData4 = (GLubyte*)malloc(PixelLength * 3);
	for(int x = 0; x < ImageWidth * 3; ++x)
	{
		for(int y = 0; y < ImageHeight; ++y)
		{
			if(x == 414 && y == 38)
				x = 414;
			double Dsumx = 0.0f;
			double Dsumy = 0.0f;
			double weightsum = 0.0f;
			for(int k = 0; k < ln; ++k)
			{
				int x1 = point[line[k][0]][0], y1 = point[line[k][0]][1], x2 = point[line[k][1]][0], y2 = point[line[k][1]][1];
				int x1p = pp1[line[k][0]][0], y1p = pp1[line[k][0]][1], x2p = pp1[line[k][1]][0], y2p = pp1[line[k][1]][1];
				double length = sqrt(double((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
				double lengthp = sqrt(double((x1p - x2p) * (x1p - x2p) + (y1p - y2p) * (y1p - y2p)));
				if(length == 0.0f || lengthp == 0.0f)
					continue;
				double u = double((x1 - x) * (x1 - x2) + (y1 - y) * (y1 - y2)) / (length * length);
				double v = double((x1 - x) * (y1 - y2) + (y1 - y) * (x2 - x1)) / length;
				double dx = double(x1p) + u * double(x2p - x1p) + v * double(y2p - y1p) / lengthp - double(x);
				double dy = double(y1p) + u * double(y2p - y1p) + v * double(x1p - x2p) / lengthp - double(y);
				
				double dist;
				if(u >= 0 && u <= 1)
					dist = abs(v);
				else if(u < 0)
					dist = sqrt(double((x - x1) * (x - x1) + (y - y1) * (y - y1)));
				else
					dist = sqrt(double((x - x2) * (x - x2) + (y - y2) * (y - y2)));
					
				double weight = pow(pow(length, p) / (a + dist), b);
				Dsumx += dx * weight;
				Dsumy += dy * weight;
				weightsum += weight;
			}
			int xp = x + int(Dsumx / weightsum);
			int yp = y + int(Dsumy / weightsum);
			int index = ((ImageHeight - y - 1) * ImageWidth * 3 * 3) + x * 3;
			int index2 = ((ImageHeight - yp - 1) * ImageWidth * 3 * 3) + xp * 3;
			if(index2 < 0 || index2 >= PixelLength * 3)
			{
				PixelData4[index] = 255;
				PixelData4[index + 1] = 255;
				PixelData4[index + 2] = 255;
			}
			else
			{
				PixelData4[index] = pImage[index2];
				PixelData4[index + 1] = pImage[index2 + 1];
				PixelData4[index + 2] = pImage[index2 + 2];
			}
		}
	}
} 