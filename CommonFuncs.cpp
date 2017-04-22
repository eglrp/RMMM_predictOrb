/****************************************************************************
Ŀ�ģ�    ��������;��������ͨ�ú�����

��дʱ�䣺2008.11.22
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include "commonfuncs.h"
#include "RTOD_Const.h"
double getModFirst(int length,double x[])
{
	double temp=0.0;
	for(int i=0;i<length;i++)
	{
		temp+=x[i]*x[i];
	}
	return sqrt(temp);
}
void randSed(){
	unsigned int seedVal;
	struct _timeb timeBuf ;
	_ftime (&timeBuf) ;
	seedVal = ( ( ( ( (unsigned int)timeBuf.time & 0xFFFF) +
		(unsigned int)timeBuf.millitm)^
		(unsigned int)timeBuf.millitm) );
	srand ((unsigned int)seedVal) ;
}
double randNum(double limitMin,double limitMax)
{
	double i ,j ,range ;
	range=limitMax-limitMin;    /* r is the range allowed; */
	i  =rand();    /* use the above example in this slot */
	/* Normalize the rand() output (scale to 0 to 1) */
	/* RAND_MAX is defined in stdlib, h */
	j  = ((double)i/(double)RAND_MAX) ;
	/* Scale the output to 1 to 44 */
	i  = (j * (double)range) ;
	i +=limitMin;
	return i;

}
/****************************************************************************
Rotation_x

Ŀ�ģ�  ������X�����ת����

����:
Angle   ��ת��[rad]
Mat     3*3����ת���� 
****************************************************************************/

void Rotation_x( double Angle, double Mat[] )
{
	double C, S;

	C = cos(Angle);
	S = sin(Angle);

	*(Mat+0*3+0) = 1.0; 
	*(Mat+0*3+1) = 0.0;  
	*(Mat+0*3+2) = 0.0;

	*(Mat+1*3+0) = 0.0; 
	*(Mat+1*3+1) =  +C;  
	*(Mat+1*3+2) =  +S;

	*(Mat+2*3+0) = 0.0; 
	*(Mat+2*3+1) =  -S;  
	*(Mat+2*3+2) =  +C;

}

/****************************************************************************
Rotation_y

Ŀ�ģ�  ������X�����ת����

����:
Angle   ��ת��[rad]
Mat     3*3����ת���� 
****************************************************************************/

void Rotation_y( double Angle, double Mat[] )
{
	double C, S;

	C = cos(Angle);
	S = sin(Angle);

	*(Mat+0*3+0) =  +C; 
	*(Mat+0*3+1) = 0.0;  
	*(Mat+0*3+2) =  -S;

	*(Mat+1*3+0) = 0.0; 
	*(Mat+1*3+1) = 1.0; 
	*(Mat+1*3+2) = 0.0;

	*(Mat+2*3+0) =  +S; 
	*(Mat+2*3+1) = 0.0;  
	*(Mat+2*3+2) =  +C;

}

/****************************************************************************
Rotation_z

Ŀ�ģ�  ������X�����ת����

����:
Angle   ��ת��[rad]
Mat     3*3����ת���� 
****************************************************************************/

void Rotation_z( double Angle, double Mat[] )
{
	double C, S;

	C = cos(Angle);
	S = sin(Angle);

	*(Mat+0*3+0) =  +C; 
	*(Mat+0*3+1) =  +S; 
	*(Mat+0*3+2) = 0.0;

	*(Mat+1*3+0) =  -S; 
	*(Mat+1*3+1) =  +C;  
	*(Mat+1*3+2) = 0.0;

	*(Mat+2*3+0) = 0.0;  
	*(Mat+2*3+1) = 0.0; 
	*(Mat+2*3+2) = 1.0;

}

/****************************************************************************
MatrixMultiply

Ŀ�ģ�  ������� M3 = M1*M2

����:
m1      M1������
n1      M1������
m2      M2������
n2      M2������

****************************************************************************/

void MatrixMultiply( int m1, int n1, int m2, int n2, 
	const double M1[], const double M2[], double M3[] )
{    
	int i, j, k;
	double Sum;

	if( n1 != m2 )
	{
		printf("Matrix multiply fail!\n");
		exit(1);
	}

	for ( i=0; i<m1; i++) 
	{
		for ( j=0; j<n2; j++)
		{
			Sum = 0.0;

			for ( k=0; k<n1; k++)   
			{
				Sum += *(M1+i*n1+k) * *(M2+k*n2+j);
			}

			*(M3+i*n2+j) = Sum;
		}
	}
}

/****************************************************************************
MatrixMultiply2
���ܣ�	����ͶԽ���˷�
����:
flag	��ʾ��˵�˳��
flag=0:   M2 = M1*P
flag=1:    M2=P*M1
P       �Խ���,��һά������ʾ[m1]
m1      M1������
n1      M1������
length      M2�����������[length=n1��length=m1]
M1      �������[m1*n1]

������� 

M2    �������[m1*n1]

****************************************************************************/
void MatrixMultiply2( int flag,int m1, int n1, int length, 
	const double M1[], const double P[],double M2[] )
{    
	int i, j;
	double Sum;
	if(0==flag){
		if(n1 != length)
			{
				printf("Matrix multiply fail!\n");
				exit(1);
			}
		for(i=0;i<m1;i++)
			for(j=0;j<n1;j++)
				M2[i*n1+j]=M1[i*n1+j]*P[j];

	}
	if(1==flag){
		if(m1 != length)
			{
				printf("Matrix multiply fail!\n");
				exit(1);
			}
		for(i=0;i<m1;i++)
			for(j=0;j<n1;j++)
				M2[i*n1+j]=M1[i*n1+j]*P[i];
	}
}
/****************************************************************************
MatrixMultiply3

Ŀ�ģ�  ������� M3 = M1*P*M2

����:
m1      M1������
n1      M1������
m2      M2���������[m2=n1]
n2      M2���������
M1      �������[m1*n1]
P       �Խ���,��һά������ʾ[m1]
M2      �������[m2*n2]

������� 

M2    �������[n*n]

****************************************************************************/

void MatrixMultiply3( int m1, int n1, int m2, int n2, 
	const double M1[], const double P[], const double M2[], double M3[] )//û���⣬�ǰ������еľ����������ˣ��м�Ϊ�ԽǾ�����������ʾrj20160721
{    
	int i, j, k;
	double Sum;

	if( n1 != m2 )
	{
		printf("Matrix multiply fail!\n");
		exit(1);
	}

	for ( i=0; i<m1; i++) 
	{
		for ( j=0; j<n2; j++)
		{
			Sum = 0.0;

			for ( k=0; k<n1; k++)   
			{
				Sum += *(M1+i*n1+k) * *(P+k) * *(M2+k*n2+j);
			}

			*(M3+i*n2+j) = Sum;
		}
	}
}
/****************************************************************************
MatrixMultiplyk

Ŀ��: �������Ͼ���ͨ��Ϊ�˵�λȨ����*Ȩ��
����: int m
����: int n
����: double M[]
����: double k
****************************************************************************/
void MatrixMultiplyk(int m,int n,double M[],double k)
{
	int i,j;
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			*(M+i*n+j) = *(M+i*n+j) * k; 
		}
	}
}
/****************************************************************************
ArrayAddition

Ŀ�ģ�   ������� A3 = A1+A2

����:
m      �����ĵ�ά��
A1      
A2
A3

****************************************************************************/
void ArrayAddition(int m,
	const double A1[], const double A2[], double A3[] )
{
	int i;
	for(i=0;i<m;i++)
	{
		A3[i]=A1[i]+A2[i];
	}
}
/****************************************************************************
MatrixAddition

Ŀ�ģ�  ������� M3 = M1+M2

����:
m      M1������
n      M1������

****************************************************************************/

void MatrixAddition( int m, int n,
	const double M1[], const double M2[], double M3[] )
{
	int i, j;

	for( i=0; i<m; i++ )
	{
		for( j=0; j<n; j++ )
		{
			*(M3+i*n+j) = *(M1+i*n+j) + *(M2+i*n+j);
		}
	}
}

/****************************************************************************
MatrixAddition2

Ŀ�ģ�  ������� M2 = M1+M2

����:
m      M1������
n      M1������

****************************************************************************/

void MatrixAddition2( int m, int n,
	const double M1[], double M2[] )
{
	int i, j;

	for( i=0; i<m; i++ )
	{
		for( j=0; j<n; j++ )
		{
			*(M2+i*n+j) = *(M1+i*n+j) + *(M2+i*n+j);
		}
	}
}

/****************************************************************************
MatrixAddition3

Ŀ��: �ԽǾ���+����
����: int n ����������
����: const double M1[] �ԽǾ���
����: double M2[] ����
****************************************************************************/
void MatrixAddition3(int n,
	const double M1[], double M2[])
{
	int i;
	for (i=0;i<n;i++)
	{
		*(M2+i*n+i) = *(M1+i) + *(M2+i*n+i);
	}
}
/****************************************************************************
MatrixSubstract

Ŀ�ģ�  ������� M3 = M1-M2

����:
m      M1������
n      M1������

****************************************************************************/

void MatrixSubstract( int m, int n, 
	const double M1[], const double M2[], double M3[] )
{
	int i, j;

	for( i=0; i<m; i++ )
	{
		for( j=0; j<n; j++ )
		{
			*(M3+i*n+j) = *(M1+i*n+j) - *(M2+i*n+j);
		}
	}

}

/****************************************************************************
MatrixTranspose

Ŀ�ģ�  ����ת�� 

����:
m      M1������
n      M1������
M1     �������
MT     �������  MT = = M1(T)

����֤, 2008.12.8
****************************************************************************/

void MatrixTranspose( int m, int n, const double M1[], double MT[] )
{
	int i, j;
	for( i=0; i<m; i++ )
	{
		for( j=0; j<n; j++ )
		{
			*(MT+j*m+i) = *(M1+i*n+j);
		}
	}
}

/****************************************************************************
VectDot    a = A . B

Ŀ�ģ�  �������������ĵ��

����:
m      A������Ԫ�ظ���
n      B������Ԫ�ظ���, Ҫ��m=n

����ֵ  ���
****************************************************************************/

double VectDot( int m, int n, const double A[], const double B[] )
{
	int i;
	double Sum;

	if ( m != n )
	{
		printf("Vector dot fail!\n");
		exit(1);
	}

	Sum = 0.0;

	for( i=0; i<m; i++ )
	{
		Sum = Sum + *(A+i) * *(B+i);
	}

	return ( Sum );
}

/****************************************************************************
CrossDot    C = A X B

Ŀ�ģ�  �������������Ĳ����ֻ֧��3�������3����

����:
m      A������Ԫ�ظ���
n      B������Ԫ�ظ���, Ҫ��m=n


****************************************************************************/

void CrossDot( int m, int n, const double A[], const double B[], double C[] )
{ 
	if ( (n!=3) || (m!=3) )
	{
		printf("Cross dot fail!\n");
		exit(1);
	}

	C[0] = A[1]*B[2] - A[2]*B[1];
	C[1] = A[2]*B[0] - A[0]*B[2];
	C[2] = A[0]*B[1] - A[1]*B[0];

} 

/****************************************************************************
Dyadic    a = A . B

Ŀ�ģ�  ��������������Dyadic��

����:
m      A������Ԫ�ظ���
n      B������Ԫ�ظ���

�������:
Mat    Dyadic������[m*n]

����ֵ  ���
****************************************************************************/

void Dyadic( int m, int n, const double A[], const double B[], double Mat[] )
{
	int i, j;

	for( i=0; i<m; i++ )
	{
		for( j=0; j<n; j++ )
		{
			*(Mat+i*n+j) = *(A+i) * *(B+j);
		}
	}
}

/****************************************************************************
SatElev

Ŀ�ģ�  �������Ǹ߶Ƚ�, ������ջ�λ��û�г�ʼ��, ����Pai/2.0

����:

SatPos[3]  ����λ��[m]
RCVPos[3]  ���ջ�λ��[m]

����ֵ

���Ǹ߶Ƚ�[Rad]

****************************************************************************/

double SatElev( const double SatPos[], const double RCVPos[] )
{
	int    i;
	double Elev;
	double RcvR, SatRcvR;
	double dPos[3];       /* ����λ������ջ���λ�ò�ֵ */


	for( i=0; i<3; i++ )
	{
		dPos[i] = SatPos[i] - RCVPos[i];
	}

	RcvR    = sqrt( VectDot(3, 3, RCVPos, RCVPos ) );
	SatRcvR = sqrt( VectDot(3, 3, dPos,   dPos ) );

	if( fabs( RcvR * SatRcvR ) <= 1.0 ) /* ��������λ�û���ջ�λ���Ƿ�Ϊ0 */
	{
		Elev = pi / 2.0;
	}
	else
	{
		Elev = VectDot( 3, 3, RCVPos, dPos ) / ( RcvR * SatRcvR );

		Elev = pi/2.0 - acos( Elev );
	}

	return Elev;


}

/****************************************************************************
MatrixInv

Ŀ�ģ�  ��������  

����:
n      M1������������
a      �������
b      �������   b=inv(a)

****************************************************************************/

void MatrixInv(int n,double *a,double *b)
{
	double max0;
	int i,j,k,m;
	int r,s,ib[30],jb[30],ex;

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			b[i*n+j]=a[i*n+j];
		}
	}
	for(i=0;i<n;i++)
	{
		*(ib+i)=-1;
		*(jb+i)=-1;
	}
	for(k=0;k<n;k++)
	{
		max0=0.0;
		for(i=0;i<n;i++)
		{
			for(m=0;m<n;m++)
			{
				if (i==*(jb+m))
					goto loop1;
			}
			for(j=0;j<n;j++)
			{
				for (m=0;m<n;m++)
				{
					if(j==*(ib+m))   goto loop2;
				}
				if(max0<fabs(*(b+n*i+j)))
				{
					max0=fabs(*(b+n*i+j));
					r=i; s=j;
				}
loop2: ;
			}
loop1: ;
		}
		*(ib+r)=s; *(jb+s)=r;
		for(i=0;i<n;i++)
		{
			if(i!=r)
			{
				for(j=0;j<n;j++)
				{
					if(j!=s)
						*(b+n*i+j)=*(b+n*i+j)-*(b+i*n+s)*(*(b+n*r+j))/(*(b+n*r+s));
				}
				*(b+n*i+s)=*(b+i*n+s)/(*(b+n*r+s));
			}
		}
		for(j=0;j<n;j++)
		{
			if(j!=s)  *(b+n*r+j)=-(*(b+n*r+j))/(*(b+n*r+s));
		}
		*(b+n*r+s)=1/(*(b+n*r+s));
	}
	for(j=0;j<n-1;j++)
	{
		if(*(ib+j)!=j)
		{
			for (k=j+1;k<n;k++)
			{
				if(*(ib+k)==j)
				{
					for(i=0;i<n;i++)
					{
						max0=*(b+n*k+i);
						*(b+n*k+i)=*(b+n*j+i);
						*(b+n*j+i)=max0;
					}
					ex=*(ib+j);
					*(ib+j)=*(ib+k);
					*(ib+k)=ex;
				}
			}
		}
	}
	for(j=0;j<n-1;j++)
	{
		if(*(jb+j)!=j)
		{
			for(k=j+1;k<n;k++)
			{
				if(*(jb+k)==j)
				{
					for(i=0;i<n;i++)
					{
						max0=*(b+n*i+k);
						*(b+n*i+k)=*(b+n*i+j);
						*(b+n*i+j)=max0;
					}
					ex=*(jb+j);
					*(jb+j)=*(jb+k);
					*(jb+k)=ex;
				}
			}
		}
	}
}

/****************************************************************************
mbbub

Ŀ�ģ�  ʵ��ð������  

����:
n      ���������еĳ���
p      ʵ������

****************************************************************************/
void mbbub( int n, double p[] )
{
	int m, k, i, j;
	double d;
	k=0;
	m=n-1;

	while (k<m)
	{
		j=m-1;
		m=0;
		for(i=k; i<=j; i++ )
		{
			if(p[i]>p[i+1])
			{
				d=p[i];
				p[i]=p[i+1];
				p[i+1]=d;
				m=i;
			}
		}

		j=k+1;
		k=0;
		for(i=m;i>=j;i--)
		{
			if(p[i-1]>p[i])
			{
				d=p[i];
				p[i]=p[i-1];
				p[i-1]=d;
				k=i;
			}
		}
	}  
}

/****************************************************************************
MatrixPrintf

Ŀ�ģ�  ��������ʾ����Ļ��  

����:
m, n   ���������������
Mat    ����ʾ�ľ���

****************************************************************************/
void MatrixPrintf( int m, int n, double Mat[] )
{
	int i, j;

	for( i=0; i<m; i++ )
	{
		for( j=0; j<n; j++ )
		{
			printf( "  %lf ", *(Mat+i*n+j) );
		}
		printf("\n");
	}
}

/****************************************************************************
MatrixFprintf

Ŀ�ģ�  ��������ʾ���ļ���  

����:
m, n   ���������������
Mat    ����ʾ�ľ���

****************************************************************************/
void MatrixFprintf( int m, int n, FILE* Fout, double Mat[] )
{
	int i, j;

	for( i=0; i<m; i++ )
	{
		for( j=0; j<n; j++ )
		{
			fprintf( Fout, "  %lf ", *(Mat+i*n+j) );
		}
		fprintf( Fout, "\n" );
	}
}

/****************************************************************************
CopyArray

Ŀ�ģ�  ��һ�����鿽������һ��������( ��ά�������һά��ʾ )  

����:
n      ����������Ԫ�ظ���
Dist   Ŀ������
Sour   Դ����

****************************************************************************/
void CopyArray( int n, double Dist[], const double Sour[] )
{
	int i;

	for( i=0; i<n; i++ )
	{
		Dist[i] = Sour[i];
	}
}
/****************************************************************************
CopyMatrix

Ŀ��: ��Sour����۳��Ӿ����Ƶ�Dist������
����: int mLengh1   Dist��������
����: int nLengh1   Dist��������
����: int mstart1   Dist�Ӿ�����ʼ����
����: int nstart1   Dist�Ӿ�����ʼ����
����: double Dist[] Ŀ�ľ���
����: int mLengh2   Sour��������
����: int nLengh2   Sour��������
����: int mstart2   Sour�Ӿ�����ʼ����
����: int nstart2   Sour�Ӿ�����ʼ����
����: const double Sour[]
****************************************************************************/
void CopyMatrix(int mLengh1,int nLengh1,double Dist[],
	int mLengh2,int nLengh2,int mstart2,int nstart2,const double Sour[])
{
	int i,j,mstart,nstart;
	mstart=mstart2-1;
	nstart=nstart2-1;
	if (mstart2+mLengh1>mLengh2 || nstart2+nLengh1>nLengh2)
	{
		return;
	}
	for(i=0;i<mLengh1;i++)
	{
		for (j=0;j<nLengh1;j++)
		{
			Dist[i*nLengh1+j]=Sour[(mstart2+i)*nLengh2+nstart2+j];
		}
	}
}
void ArraySub(int m, const double A1[], const double A2[], double A3[])
{
	int i;
	for (i=0;i<m;i++)
	{
		A3[i]=A1[i]-A2[i];
	}
}
