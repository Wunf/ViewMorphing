#include <math.h>
#include <iostream>

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

int main(int argc, char * argv[])
{
	double Pm[4][2], Pc[4][2], P[16][2];
	Pm[0][0] = 0.0f;Pm[0][1] = 0.0f;
	Pm[1][0] = 3.0f;Pm[1][1] = 0.0f;
	Pm[2][0] = 0.0f;Pm[2][1] = 3.0f;
	Pm[3][0] = 3.0f;Pm[3][1] = 3.0f;
	Pc[0][0] = 0.0f;Pc[0][1] = 0.0f;
	Pc[1][0] = 3.0f;Pc[1][1] = 0.0f;
	Pc[2][0] = 0.0f;Pc[2][1] = 3.0f;
	Pc[3][0] = 3.0f;Pc[3][1] = 3.0f;
	P[0][0] = 0.0f;P[0][1] = 0.0f;
	P[1][0] = 1.0f;P[1][1] = 0.0f;
	P[2][0] = 2.0f;P[2][1] = 0.0f;
	P[3][0] = 3.0f;P[3][1] = 0.0f;
	P[4][0] = 0.0f;P[4][1] = 1.0f;
	P[5][0] = 1.0f;P[5][1] = 1.0f;
	P[6][0] = 2.0f;P[6][1] = 1.0f;
	P[7][0] = 3.0f;P[7][1] = 1.0f;
	P[8][0] = 0.0f;P[8][1] = 2.0f;
	P[9][0] = 1.0f;P[9][1] = 2.0f;
	P[10][0] = 2.0f;P[10][1] = 2.0f;
	P[11][0] = 3.0f;P[11][1] = 2.0f;
	P[12][0] = 0.0f;P[12][1] = 3.0f;
	P[13][0] = 1.0f;P[13][1] = 3.0f;
	P[14][0] = 2.0f;P[14][1] = 3.0f;
	P[15][0] = 3.0f;P[15][1] = 3.0f;
	double H[4][4];
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			H[i][j] = exp(- sqrt((Pm[i][0] - Pc[j][0]) * (Pm[i][0] - Pc[j][0]) + (Pm[i][1] - Pc[j][1]) * (Pm[i][1] - Pc[j][1])) / 64);
			//if(i == j) H[i][j] += 0.01;
		}
	}
	
	if(InverseMatrix(H[0], 4))
		std::cout<<"error";
		
	double w[4][2];
	for(int i = 0; i < 4; i++)
	{
		w[i][0] = 0.0f;
		w[i][1] = 0.0f;
		for(int j = 0; j < 4; j++)
		{
			w[i][0] += H[i][j] * Pc[j][0];
			w[i][1] += H[i][j] * Pc[j][1];
		}
	}
	
	for(int i = 0; i < 16; ++i)
	{
		double x = 0.0f, y = 0.0f, z = 0.0f, x0 = P[i][0], y0 = P[i][1];
		for(int j = 0; j < 4; j++)
		{
			double temp = exp(- sqrt((x0 - Pc[j][0]) * (x0 - Pc[j][0]) + (y0 - Pc[j][1]) * (y0 - Pc[j][1])) / 64);
			x += w[j][0] * temp;
			y += w[j][1] * temp;
			z += w[j][2] * temp;
		}
		P[i][0] = x;
		P[i][1] = y;
	}
	
	for(int i = 0; i < 16; ++i)
	{
		std::cout << P[i][0] << " " << P[i][1] << std::endl;
	}
	return 0;
	
}