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
const double f = 3.0f;
const double d = 50.0f;

int main(int argc, char * argv[])
{
	ReadFile();
	ChangetoC0();
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

void ReadFile()
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
	T[0][0] = 1.0f; T[1][1] = 1.0f; T[2][2] = 1.0f; T[3][3] = 1.0f; T[0][3] = d * cos(45);
	
	double R[4][4];
	R[0][0] = cos(45); R[0][1] = 0.0f; R[0][2] = -cos(45); R[0][3] = 0.0f;
	R[1][0] = 0.0f; R[1][1] = 1.0f; R[1][2] = 0.0f; R[1][3] = 0.0f;
	R[2][0] = cos(45); R[2][1] = 0.0f; R[2][2] = cos(45); R[2][3] = 0.0f;
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
	for(int i = 0; i < ImageWidth; ++i)
	{
		for(int j = 0; j < ImageHeight; ++j)
		{	
			x = int(temp[0][0] * double(i) + temp[0][1] * double(j) + temp[0][2]);
			y = int(temp[1][0] * double(i) + temp[1][1] * double(j) + temp[1][2]);
			int index = ((ImageHeight - y - 1) * ImageWidth * 3) + x * 3;
			int index2 = ((ImageHeight - j - 1) * ImageWidth * 3) + i * 3;
			PixelData2[index] = PixelData[index];
			PixelData2[index + 1] = PixelData[index + 1];
			PixelData2[index + 2] = PixelData[index + 2];
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