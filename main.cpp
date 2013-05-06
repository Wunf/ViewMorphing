#define _WCHAR_T_DEFINED
#define _STDCALL_SUPPORTED
#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "main.h"

static GLint    ImageWidth;
static GLint    ImageHeight;
static GLint    PixelLength;
static GLubyte* PixelData;
static GLubyte* PixelData2;
static GLubyte* PixelData3;
static GLubyte* PixelData4;
static GLubyte* PixelData5;

const double f = 5.0f;
const double d = 100.0f;
const double pai = 3.1415926;
int lines[10][2][2], lined[10][2][2];


int main(int argc, char * argv[])
{
	ReadFile("1.bmp");
	ChangetoC0();
	ReadFile("2.bmp");
	ChangetoC1();
//	lineinit();
//	Morphing(0.5);
	
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
	glDrawPixels(ImageWidth, ImageHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData2);
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

void ChangetoC0()
{
	double T[4][4];
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			T[i][j] = 0.0f;
		}
	}
	T[0][0] = 1.0f; T[1][1] = 1.0f; T[2][2] = 1.0f; T[3][3] = 1.0f; T[0][3] = d * cos(pai / 4);
	
	double R[4][4];
	R[0][0] = cos(pai / 4); R[0][1] = 0.0f; R[0][2] = -sin(pai / 4); R[0][3] = 0.0f;
	R[1][0] = 0.0f; R[1][1] = 1.0f; R[1][2] = 0.0f; R[1][3] = 0.0f;
	R[2][0] = sin(pai / 4); R[2][1] = 0.0f; R[2][2] = cos(pai / 4); R[2][3] = 0.0f;
	R[3][0] = 0.0f; R[3][1] = 0.0f; R[3][2] = 0.0f; R[3][3] = 1.0f;
	
	double pi[3][4];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			pi[i][j] = 0.0f;
		}
	}
	pi[0][0] = f; pi[1][1] = f; pi[2][2] = 1.0f;
	
	
	double temp[3][4];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			temp[i][j] = 0.0f;
			temp[i][j] += pi[i][0] * R[0][j] + pi[i][1] * R[1][j] + pi[i][2] * R[2][j] + pi[i][3] * R[3][j];
		}
	}
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			pi[i][j] = temp[i][j];
		}
	}
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			temp[i][j] = 0.0f;
			temp[i][j] += pi[i][0] * T[0][j] + pi[i][1] * T[1][j] + pi[i][2] * T[2][j] + pi[i][3] * T[3][j];
		}
	}
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			pi[i][j] = temp[i][j];
		}
	}
	
	double H[3][3];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			H[i][j] = pi[i][j];
		}
	}
	
	if(InverseMatrix(H[0], 3))
		printf("error");
		
	double Hp[3][3];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			Hp[i][j] = 0.0f;
		}
	}
	Hp[0][0] = f; Hp[1][1] = f; Hp[2][2] = 1.0f;
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			temp[i][j] = 0.0f;
			temp[i][j] += Hp[i][0] * H[0][j] + Hp[i][1] * H[1][j] + Hp[i][2] * H[2][j];
		}
	}
	
	PixelData2 = (GLubyte*)malloc(PixelLength);
	for(int i = 0; i < PixelLength; ++i)
		PixelData2[i] = 255;
	int x, y;
	for(int i = 0; i < 10; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			x = int(temp[0][0] * double(lines[i][j][0]) + temp[0][1] * double(lines[i][j][1]) + temp[0][2]);
			y = int(temp[1][0] * double(lines[i][j][0]) + temp[1][1] * double(lines[i][j][1]) + temp[1][2]); 
			lines[i][j][0] = x;
			lines[i][j][1] = y;
		}
	}
	
	for(int i = 0; i < ImageWidth; ++i)
	{
		for(int j = 0; j < ImageHeight; ++j)
		{	
			/*if(i == 2 && j == 164)
				i = 2;*/
			x = int(temp[0][0] * double(i) + temp[0][1] * double(j) + temp[0][2]);
			y = int(temp[1][0] * double(i) + temp[1][1] * double(j) + temp[1][2]);
			int index = ((ImageHeight - y - 1) * ImageWidth * 3) + x * 3;
			int index2 = ((ImageHeight - j - 1) * ImageWidth * 3) + i * 3;
			PixelData2[index] = PixelData[index2];
			PixelData2[index + 1] = PixelData[index2 + 1];
			PixelData2[index + 2] = PixelData[index2 + 2];
		}
	}
}

void ChangetoC1()
{
	double T[4][4];
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			T[i][j] = 0.0f;
		}
	}
	T[0][0] = 1.0f; T[1][1] = 1.0f; T[2][2] = 1.0f; T[3][3] = 1.0f; T[0][3] = -d * cos(-pai / 4);
	
	double R[4][4];
	R[0][0] = cos(-pai / 4); R[0][1] = 0.0f; R[0][2] = -sin(-pai / 4); R[0][3] = 0.0f;
	R[1][0] = 0.0f; R[1][1] = 1.0f; R[1][2] = 0.0f; R[1][3] = 0.0f;
	R[2][0] = sin(-pai / 4); R[2][1] = 0.0f; R[2][2] = cos(-pai / 4); R[2][3] = 0.0f;
	R[3][0] = 0.0f; R[3][1] = 0.0f; R[3][2] = 0.0f; R[3][3] = 1.0f;
	
	double pi[3][4];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			pi[i][j] = 0.0f;
		}
	}
	pi[0][0] = f; pi[1][1] = f; pi[2][2] = 1.0f;
	
	
	double temp[3][4];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			temp[i][j] = 0.0f;
			temp[i][j] += pi[i][0] * R[0][j] + pi[i][1] * R[1][j] + pi[i][2] * R[2][j] + pi[i][3] * R[3][j];
		}
	}
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			pi[i][j] = temp[i][j];
		}
	}
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			temp[i][j] = 0.0f;
			temp[i][j] += pi[i][0] * T[0][j] + pi[i][1] * T[1][j] + pi[i][2] * T[2][j] + pi[i][3] * T[3][j];
		}
	}
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			pi[i][j] = temp[i][j];
		}
	}
	
	double H[3][3];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			H[i][j] = pi[i][j];
		}
	}
	
	if(InverseMatrix(H[0], 3))
		printf("error");
		
	double Hp[3][3];
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			Hp[i][j] = 0.0f;
		}
	}
	Hp[0][0] = f; Hp[1][1] = f; Hp[2][2] = 1.0f;
	
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			temp[i][j] = 0.0f;
			temp[i][j] += Hp[i][0] * H[0][j] + Hp[i][1] * H[1][j] + Hp[i][2] * H[2][j];
		}
	}
	
	PixelData3 = (GLubyte*)malloc(PixelLength);
	for(int i = 0; i < PixelLength; ++i)
		PixelData3[i] = 255;
	int x, y;
	for(int i = 0; i < 10; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			x = int(temp[0][0] * double(lined[i][j][0]) + temp[0][1] * double(lined[i][j][1]) + temp[0][2]);
			y = int(temp[1][0] * double(lined[i][j][0]) + temp[1][1] * double(lined[i][j][1]) + temp[1][2]); 
			lined[i][j][0] = x;
			lined[i][j][1] = y;
		}
	}
	
	for(int i = 0; i < ImageWidth; ++i)
	{
		for(int j = 0; j < ImageHeight; ++j)
		{	
			/*if(i == 2 && j == 164)
				i = 2;*/
			x = int(temp[0][0] * double(i) + temp[0][1] * double(j) + temp[0][2]);
			y = int(temp[1][0] * double(i) + temp[1][1] * double(j) + temp[1][2]);
			int index = ((ImageHeight - y - 1) * ImageWidth * 3) + x * 3;
			int index2 = ((ImageHeight - j - 1) * ImageWidth * 3) + i * 3;
			PixelData3[index] = PixelData[index2];
			PixelData3[index + 1] = PixelData[index2 + 1];
			PixelData3[index + 2] = PixelData[index2 + 2];
		}
	}
}

void swap(double &a,double &b)
{
	double temp=a;
	a=b;
	b=temp;
}
/**********************************************
*函数名：InverseMatrix       
*函数介绍：求逆矩阵（高斯—约当法） 
*输入参数：矩阵首地址（二维数组）matrix，阶数row
*输出参数：matrix原矩阵的逆矩阵
*返回值：成功，0；失败，1
*调用函数：swap(double &a,double &b)
*作者：vcrs
*完成时间：2009-10-04
**********************************************/
int InverseMatrix(double *matrix,const int &row)
{
	double *m=new double[row*row];
	double *ptemp,*pt=m;
	int i,j;
	ptemp=matrix;
	for (i=0;i<row;i++)
	{
		for (j=0;j<row;j++)
		{
			*pt=*ptemp;
			ptemp++;
			pt++;
		}
	}
	int k;

	int *is=new int[row],*js=new int[row];
	for (k=0;k<row;k++)
	{
		double max=0;
		//全选主元
		//寻找最大元素
		for (i=k;i<row;i++)
		{
			for (j=k;j<row;j++)
			{
				if (fabs(*(m+i*row+j))>max)
				{
					max=*(m+i*row+j);
					is[k]=i;
					js[k]=j;
				}
			}
		}
		if (0 == max)
		{
			return 1;
		}
		//行交换
		if (is[k]!=k)
		{
			for (i=0;i<row;i++)
			{
				swap(*(m+k*row+i),*(m+is[k]*row+i));
			}
		}
		//列交换
		if (js[k]!=k)
		{
			for (i=0;i<row;i++)
			{
				swap(*(m+i*row+k),*(m+i*row+js[k]));
			}
		}
		*(m+k*row+k)=1/(*(m+k*row+k));

		for (j=0;j<row;j++)
		{
			if (j!=k)
			{
				*(m+k*row+j)*=*((m+k*row+k));
			}
		}
		for (i=0;i<row;i++)
		{
			if (i!=k)
			{
				for (j=0;j<row;j++)
				{
					if(j!=k)
					{
						*(m+i*row+j)-=*(m+i*row+k)**(m+k*row+j);
					}
				}
			}
		}
		for (i=0;i<row;i++)
		{
			if(i!=k)
			{
				*(m+i*row+k)*=-(*(m+k*row+k));
			}
		}
	}
	int r;
	//恢复
	for (r=row-1;r>=0;r--)
	{
		if (js[r]!=r)
		{
			for (j=0;j<row;j++)
			{
				swap(*(m+r*row+j),*(m+js[r]*row+j));
			}
		}
		if (is[r]!=r)
		{
			for (i=0;i<row;i++)
			{
				swap(*(m+i*row+r),*(m+i*row+is[r]));
			}
		}
	}
	ptemp=matrix;
	pt=m;
	for (i=0;i<row;i++)
	{
		for (j=0;j<row;j++)
		{
			*ptemp=*pt;
			ptemp++;
			pt++;
		}
	}
	delete []is;
	delete []js;
	delete []m;
	return 0;
}

void lineinit()
{
	//source
	lines[0][0][0] = 50;  lines[0][0][1] = 100;
	lines[0][1][0] = 50;  lines[0][1][1] = 300;
	
	lines[1][0][0] = 50;  lines[1][0][1] = 100;
	lines[1][1][0] = 200;  lines[1][1][1] = 50;
	
	lines[2][0][0] = 50;  lines[2][0][1] = 300;
	lines[2][1][0] = 200;  lines[2][1][1] = 350;
	
	lines[3][0][0] = 200;  lines[3][0][1] = 50;
	lines[3][1][0] = 200;  lines[3][1][1] = 350;
	
	lines[4][0][0] = 200;  lines[4][0][1] = 50;
	lines[4][1][0] = 350;  lines[4][1][1] = 100;
	
	lines[5][0][0] = 200;  lines[5][0][1] = 350;
	lines[5][1][0] = 350;  lines[5][1][1] = 300;
	
	lines[6][0][0] = 350;  lines[6][0][1] = 100;
	lines[6][1][0] = 350;  lines[6][1][1] = 300;
	
	lines[7][0][0] = 350;  lines[7][0][1] = 100;
	lines[7][1][0] = 350;  lines[7][1][1] = 100;
	
	lines[8][0][0] = 350;  lines[8][0][1] = 300;
	lines[8][1][0] = 350;  lines[8][1][1] = 300;
	
	lines[9][0][0] = 350;  lines[9][0][1] = 100;
	lines[9][1][0] = 350;  lines[9][1][1] = 300;
	
	//destiny
	lined[0][0][0] = 50;  lined[0][0][1] = 100;
	lined[0][1][0] = 50;  lined[0][1][1] = 300;
	
	lined[1][0][0] = 50;  lined[1][0][1] = 100;
	lined[1][1][0] = 50;  lined[1][1][1] = 100;
	
	lined[2][0][0] = 50;  lined[2][0][1] = 300;
	lined[2][1][0] = 50;  lined[2][1][1] = 300;
	
	lined[3][0][0] = 50;  lined[3][0][1] = 100;
	lined[3][1][0] = 50;  lined[3][1][1] = 300;
	
	lined[4][0][0] = 50;  lined[4][0][1] = 100;
	lined[4][1][0] = 200;  lined[4][1][1] = 50;
	
	lined[5][0][0] = 50;  lined[5][0][1] = 300;
	lined[5][1][0] = 200;  lined[5][1][1] = 350;
	
	lined[6][0][0] = 200;  lined[6][0][1] = 50;
	lined[6][1][0] = 200;  lined[6][1][1] = 350;
	
	lined[7][0][0] = 200;  lined[7][0][1] = 50;
	lined[7][1][0] = 350;  lined[7][1][1] = 100;
	
	lined[8][0][0] = 200;  lined[8][0][1] = 350;
	lined[8][1][0] = 350;  lined[8][1][1] = 100;

	lined[9][0][0] = 350;  lined[9][0][1] = 100;
	lined[9][1][0] = 350;  lined[9][1][1] = 300;
}

void Morphing(double t)
{
	int linem[10][2][2];
	for(int i = 0; i < 10; ++i)
	{
		linem[i][0][0] = int(t * double(lines[i][0][0]) + (1 - t) * double(lined[i][0][0]) + 0.5);
		linem[i][0][1] = int(t * double(lines[i][0][1]) + (1 - t) * double(lined[i][0][1]) + 0.5);
		linem[i][1][0] = int(t * double(lines[i][1][0]) + (1 - t) * double(lined[i][1][0]) + 0.5);
		linem[i][1][1] = int(t * double(lines[i][1][1]) + (1 - t) * double(lined[i][1][1]) + 0.5);
	}
	
	const double a = 0.1f, b = 1.5f, p = 0.5f; 
	PixelData4 = (GLubyte*)malloc(PixelLength);
	PixelData5 = (GLubyte*)malloc(PixelLength);
	for(int x = 0; x < ImageWidth; ++x)
	{
		for(int y = 0; y < ImageHeight; ++y)
		{
			double Dsumx = 0.0f;
			double Dsumy = 0.0f;
			double weightsum = 0.0f;
			for(int k = 0; k < 10; ++k)
			{
				int x1 = linem[k][0][0], y1 = linem[k][0][1], x2 = linem[k][1][0], y2 = linem[k][1][1];
				int x1p = lines[k][0][0], y1p = lines[k][0][1], x2p = lines[k][1][0], y2p = lines[k][1][1];
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
			int index = ((ImageHeight - y - 1) * ImageWidth * 3) + x * 3;
			int index2 = ((ImageHeight - yp - 1) * ImageWidth * 3) + xp * 3;
			if(index2 < 0 || index2 >= PixelLength)
			{
				PixelData4[index] = 255;
				PixelData4[index + 1] = 255;
				PixelData4[index + 2] = 255;
			}
			else
			{
				PixelData4[index] = PixelData2[index2];
				PixelData4[index + 1] = PixelData2[index2 + 1];
				PixelData4[index + 2] = PixelData2[index2 + 2];
			}
		}
	}
	for(int x = 0; x < ImageWidth; ++x)
	{
		for(int y = 0; y < ImageHeight; ++y)
		{
			double Dsumx = 0.0f;
			double Dsumy = 0.0f;
			double weightsum = 0.0f;
			for(int k = 0; k < 10; ++k)
			{
				int x1 = linem[k][0][0], y1 = linem[k][0][1], x2 = linem[k][1][0], y2 = linem[k][1][1];
				int x1p = lined[k][0][0], y1p = lined[k][0][1], x2p = lined[k][1][0], y2p = lined[k][1][1];
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
			int index = ((ImageHeight - y - 1) * ImageWidth * 3) + x * 3;
			int index2 = ((ImageHeight - yp - 1) * ImageWidth * 3) + xp * 3;
			if(index2 < 0 || index2 >= PixelLength)
			{
				PixelData5[index] = 255;
				PixelData5[index + 1] = 255;
				PixelData5[index + 2] = 255;
			}
			else
			{
				PixelData5[index] = PixelData3[index2];
				PixelData5[index + 1] = PixelData3[index2 + 1];
				PixelData5[index + 2] = PixelData3[index2 + 2];
			}
		}
	}
}