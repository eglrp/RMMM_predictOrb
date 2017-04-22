/****************************************************************************
Ŀ�ģ�    ����ʱ��ṹ�弰���໥ת������,
�ռ�ֱ������ʹ�������໥ת��
��дʱ�䣺2008.11.22
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#include <math.h>
#include "GPSTime.h"
#include "RTOD_Const.h"
#include "CommonFuncs.h"
//2000��1��1��12�㣨J2000������������Ϊ2451545.0
//sec�Ǵ�J2000��ʼ�������
double SecTimeToJD(double sec)
{
	double jd=0.0,frac;
	frac = fmod(sec,1.0);
	int isec;
	if(frac < 0.0) 
		frac+=1.0;
	isec = sec - frac + 0.5;
	jd = isec/86400.0 + 2451545.0;
	return jd;
}
//�����յ���������
double JDToMJD(double jd)
{
	return jd - 2400000.5;
}
//sec��ͨ��ʱ
void SecTimeToCT(double sec,COMMONTIME *CT)
{
	MJDTIME MJDT;
	double jd=SecTimeToJD(sec);
	double MJD=JDToMJD(jd);
	MJDT.FracDay=fmod(MJD,1.0);
	MJDT.Days=MJD-MJDT.FracDay;
	MJDTimeToCommonTime(&MJDT,CT);
}
/****************************************************************************
CommonTimeToMJDTime
Ŀ�ģ���ͨ��ʱת��Ϊ��������

������
CT   ͨ��ʱ
MJDT ��������
****************************************************************************/

void CommonTimeToMJDTime( const COMMONTIME* CT, MJDTIME* MJDT)
{
	int y, m, temp;    /* ��ʱ����    */

	y = CT->Year;

	if( CT->Year < 100 )     /* ʹ����λ��Ƿ� */
	{
		if( CT->Year < 80 )   /* ֻ֧��1980~2079  */
		{
			y = y + 2000;  
		}
		else   
		{
			y = y + 1900;
		}
	}

	if( CT->Month <= 2 )   
	{
		y = y - 1;
		m = CT->Month + 12;
	}
	else
	{
		m = CT->Month;
	}

	temp = ( int )( 365.25 * y );
	temp += ( int )( 30.6001 * ( m+1 ) );
	temp += CT->Day;
	temp += -679019;

	MJDT->Days = temp;

	MJDT->FracDay = CT->Hour + CT->Minute / 60.0 + CT->Second / SECPERHOUR;
	MJDT->FracDay = MJDT->FracDay / 24.0;

}

/****************************************************************************
MJDTimeToCommonTime
Ŀ�ģ�����������ת��Ϊͨ��ʱ

������
MJDT ��������
CT   ͨ��ʱ
****************************************************************************/

void MJDTimeToCommonTime( const MJDTIME* MJDT, COMMONTIME* CT )
{
	int a,b,c,d,e;

	a = ( int )( MJDT->Days + MJDT->FracDay + 2400000.5 + 0.5 );
	b = a + 1537;
	c = ( int )( ( b-122.1)/365.25 );
	d = ( int )( 365.25 * c );
	e = ( int )( ( b - d ) / 30.6001 );

	CT->Day = b - d - ( int )( 30.6001 * e );
	CT->Month = e - 1 - 12 * ( int )( e / 14 );
	CT->Year = c - 4715 - ( int )(( 7 + CT->Month ) / 10 );

	CT->Hour = ( int )( MJDT->FracDay * 24 );
	CT->Minute = ( int )( ( MJDT->FracDay * 24 - CT->Hour ) * 60 );
	CT->Second = ( ( MJDT->FracDay * 24 - CT->Hour ) * 60 - CT->Minute ) * 60;
}

/****************************************************************************
GPSTimeToMJDTime
Ŀ�ģ���GPSʱ��ת��Ϊ��������

������
GT   GPSʱ��
MJDT ��������
****************************************************************************/

void GPSTimeToMJDTime( const GPSTIME* GT, MJDTIME* MJDT )
{
	int day;

	day = ( int )( GT->SecOfWeek / SECPERDAY );
	MJDT->FracDay = GT->SecOfWeek / SECPERDAY - day;

	MJDT->Days = JAN61980 + GT->Week * 7 + day;

}

/****************************************************************************
MJDTimeToGPSTime
Ŀ�ģ�����������ת��ΪGPSʱ��

������
GT   GPSʱ��
MJDT ��������
****************************************************************************/
void MJDTimeToGPSTime ( const MJDTIME* MJDT, GPSTIME* GT )
{
	int RemainDay;

	GT->Week = (int)(( MJDT->Days - JAN61980) / 7 ); 

	RemainDay = MJDT->Days - GT->Week * 7 - JAN61980;

	GT->SecOfWeek = ( RemainDay + MJDT->FracDay ) * SECPERDAY;

}

/****************************************************************************
CommonTimeToGPSTime
Ŀ�ģ���ͨ��ʱת��ΪGPSʱ��

������
CT   ͨ��ʱ
GT   GPSʱ��
****************************************************************************/
void CommonTimeToGPSTime ( const COMMONTIME* CT, GPSTIME* GT )
{
	MJDTIME mjd;

	CommonTimeToMJDTime( CT, &mjd );
	MJDTimeToGPSTime( &mjd, GT );

}

/****************************************************************************
GPSTimeToCommonTime
Ŀ�ģ���GPSʱ��ת��Ϊͨ��ʱ

������
GT   GPSʱ��
CT   ͨ��ʱ
****************************************************************************/
void GPSTimeToCommonTime ( const GPSTIME* GT, COMMONTIME* CT )
{
	MJDTIME mjd;

	GPSTimeToMJDTime( GT, &mjd );
	MJDTimeToCommonTime( &mjd, CT );

}
/****************************************************************************
GPSTimeToep
Ŀ�ģ���GPSʱ��ת��Ϊͨ��ʱep

������
GT   GPSʱ��
ep   rtklib�µ�ͨ��ʱ
****************************************************************************/
void GPSTimeToep ( const GPSTIME* GT, double ep[6])
{
	COMMONTIME CT;
	MJDTIME mjd;

	GPSTimeToMJDTime( GT, &mjd );
	MJDTimeToCommonTime( &mjd, &CT );
	ep[0] = (double)CT.Year;
	ep[1] = (double)CT.Month;
	ep[2] = (double)CT.Day;
	ep[3] = (double)CT.Hour;
	ep[4] = (double)CT.Minute;
	ep[5] = (double)CT.Second;

}

/****************************************************************************
CheckGPSTime
Ŀ�ģ����GPSʱ���ʽ�Ƿ���ȷ, �������Ƿ����604800��С��0 

������
GT   GPSʱ��
****************************************************************************/
void CheckGPSTime( GPSTIME* GT )
{
	if( GT->SecOfWeek<0.0 )
	{
		GT->Week = GT->Week - 1;
		GT->SecOfWeek = GT->SecOfWeek + SECPERWEEK;
	}
	else if( GT->SecOfWeek>SECPERWEEK )
	{
		GT->Week = GT->Week + 1;
		GT->SecOfWeek = GT->SecOfWeek - SECPERWEEK;
	}

}

/****************************************************************************
XYZToBLH
Ŀ�ģ����ռ�ֱ������ת��Ϊ�������

������
XYZ   �ռ�ֱ������[m]
BLH   �������[Rad, m]
R     �ο�����ĳ��뾶[m]
F     ����(1/f)
****************************************************************************/
void XYZToBLH( const double XYZ[], double BLH[], double R, double F )
{
	short   Iterator;
	double  e2, dZ, rho2, dZ_new, SinPhi;
	double  ZdZ, Nh, N;

	e2      = F * ( 2.0 - F );
	rho2 = XYZ[0] * XYZ[0] + XYZ[1] * XYZ[1]; 
	dZ = e2 * XYZ[2];

	Iterator = 0;
	for(;;)
	{
		ZdZ    =  XYZ[2] + dZ;
		Nh     =  sqrt ( rho2 + ZdZ*ZdZ ); 

		if( Nh < 1.0 )  /* ���XYZ����Ϊ0.0����� */
		{
			BLH[0] = 0.0;
			BLH[1] = 0.0;
			BLH[2] = 0.0;
			return;
		}

		SinPhi =  ZdZ / Nh;                    
		N      =  R / sqrt(1.0 - e2*SinPhi*SinPhi); 
		dZ_new =  N * e2 * SinPhi;

		Iterator = Iterator + 1;
		if( (fabs(dZ-dZ_new) < 1E-10) || (Iterator >=10) )
		{
			break;
		}

		dZ = dZ_new;
	}

	BLH[1] = atan2 ( XYZ[1], XYZ[0] );
	BLH[0] = atan2 ( ZdZ, sqrt(rho2) );
	BLH[2] = Nh - N;
}

/****************************************************************************
BLHToXYZ
Ŀ�ģ����������ת��Ϊ�ռ�ֱ������

������
BLH   �������[Rad, m]
XYZ   �ռ�ֱ������[m]
R     �ο�����ĳ��뾶[m]
F     ����(1/f)
****************************************************************************/
void BLHToXYZ( const double BLH[], double XYZ[], double R, double F )
{
	double  e2, CosLat, SinLat;
	double  N;

	e2      = F * ( 2.0 - F );
	CosLat = cos( BLH[0] );        
	SinLat = sin( BLH[0] );

	N = R_Earth / sqrt(1.0-e2*SinLat*SinLat);

	XYZ[0] =  (         N+BLH[2])*CosLat*cos(BLH[1]);
	XYZ[1] =  (         N+BLH[2])*CosLat*sin(BLH[1]);
	XYZ[2] =  ((1.0-e2)*N+BLH[2])*SinLat;
}

/****************************************************************************
CoorTranFromPZ90ToWGS84

Ŀ�ģ�PZ90 ��WGS84����ת��
������������Գ¿���<GPS��GLONASS��λ�ɹ�������ת��>
���ͨ��,2002�������

X(WGS84) = X(PZ90) + dX0 + R*X(PZ90)
|  D   -R3    R2 |
R = |  R3   D    -R1 |
| -R2   R1    D  |
dX0 = [ 7cm, 0, -77cm ]
D = -3E-9,  R1 = -19 mas, R2 = -4mas,  R3 = 353mas

MCCת������:
dX0 = [-0.47m, -0.51m, -1.56m]
D = 22E-9,   R3=1.728E-6, R2=-0.017E-6, R1 = -0.076E-6

MIT�ṩ��ת������:
dX0 = [0.0m, 2.5m, 0.0m]
D = 0,   R3=1.9E-6, R2=0.0, R1 = 0.0

2007.9.20��, ת������Ϊ
dX0 = [-0.36m, 0.08m, 0.180m]


������
BLH   �������[Rad, m]
XYZ   �ռ�ֱ������[m]
R     �ο�����ĳ��뾶[m]
F     ����(1/f)
****************************************************************************/
/*  */

void CoorTranFromPZ90ToWGS84( double PZ90[3], double WGS84[3] )
{
	/*  double X[3], dX0[3] = { 0.07, 0.0, -0.77 };
	double R[9] = { 1.0-3E-9, -353/Arcs/1000.0, -4/Arcs/1000.0,
	353/Arcs/1000.0,  1.0-3E-9, 19/Arcs/1000.0,
	4/Arcs/1000.0,  -19/Arcs/1000.0, 1.0-3E-9 };
	*/ 
	/*    double X[3], dX0[3] = { -0.47, -0.51, -1.560 };
	double R[9] = { 1.0-3E-9,    -1.728E-6, -0.017E-6,
	1.728E-6,  1.0-3E-9,        0.076E-6,
	0.017E-6,  -0.076E-6,    1.0-3E-9 };
	*/   
	double X[3], dX0[3] = { 0.0, 2.5, 0.0 };
	double R[9] = { 1.0, -1.9E-6, 0.0,
		1.9E-6,  1.0,    0.0,
		0.0,  0.0,    1.0 };

	MatrixMultiply( 3, 3, 3, 1, R, PZ90, X );

	MatrixAddition( 3, 1, X, dX0, WGS84 );
}


/****************************************************************************
VeloTranFromPZ90ToWGS84

Ŀ�ģ�PZ90 ��WGS84�ٶ�ת��
������������Գ¿���<GPS��GLONASS��λ�ɹ�������ת��>
���ͨ��,2002�������

X(WGS84) = X(PZ90) + dX0 + R*X(PZ90)
|  D   -R3    R2 |
R = |  R3   D    -R1 |
| -R2   R1    D  |
dX0 = [ 7cm, 0, -77cm ]
D = -3E-9,  R1 = -19 mas, R2 = -4mas,  R3 = 353mas

MCCת������:
dX0 = [-0.47m, -0.51m, -1.56m]
D = 22E-9,   R3=1.728E-6, R2=-0.017E-6, R1 = -0.076E-6

MIT�ṩ��ת������:
dX0 = [0.0m, 2.5m, 0.0m]
D = 0,   R3=1.9E-6, R2=0.0, R1 = 0.0


������
BLH   �������[Rad, m]
XYZ   �ռ�ֱ������[m]
R     �ο�����ĳ��뾶[m]
F     ����(1/f)
****************************************************************************/
/*  */

void VeloTranFromPZ90ToWGS84( double PZ90[3], double WGS84[3] )
{
	/*   double R[9] = { 1.0-3E-9,    -1.728E-6, -0.017E-6,
	1.728E-6,  1.0-3E-9,     0.076E-6,
	0.017E-6,  -0.076E-6,    1.0-3E-9 };
	double R[9] = { 1.0-3E-9, -353/Arcs/1000.0, -4/Arcs/1000.0,
	353/Arcs/1000.0,  1.0-3E-9, 19/Arcs/1000.0,
	4/Arcs/1000.0,  -19/Arcs/1000.0, 1.0-3E-9 };
	*/ 
	double R[9] = { 1.0, -1.9E-6, 0.0,
		1.9E-6,  1.0,    0.0,
		0.0,  0.0,    1.0 };

	MatrixMultiply( 3, 3, 3, 1, R, PZ90, WGS84 );

}


/***************************************************************************
//
// BLHToNEUMatrix
//
// Ŀ��: �����վ���NEU��ת�������

//
// �������:
//
BLH      �����վ������[Rad, m]

�������

H        ת����NEU��վ��ƽ�������ת����

***************************************************************************/

void BLHToNEUMatrix( double BLH[3], double H[9])
{
	double sinB, cosB;
	double sinL, cosL;

	sinB = sin( BLH[0] );
	cosB = cos( BLH[0] );
	sinL = sin( BLH[1] );
	cosL = cos( BLH[1] );

	H[0]= -sinB * cosL;
	H[1]= -sinB * sinL;
	H[2]=  cosB;

	H[3]= -sinL;
	H[4]=  cosL;
	H[5]=  0.0;

	H[6]=  cosB * cosL;
	H[7]=  cosB * sinL;
	H[8]=  sinB;
}


/***************************************************************************
//
// PhaseCentToMassCent
//
// Ŀ��: ��GNSS���ջ�������λ��������ת������������

˵��: �ǹ�ϵ����(��CHAMP����Ϊ��)

ԭ��: ���ǵ���������
X��:  ָ�����Ƿ��з���
Z��:  ָ�������������
Y��:  ��Z��X�����������ϵ

//
// �������:
//
Flag      1Ϊ��λ����ת������������, 0Ϊ��������ת����λ����
Bias      �ǹ�ϵ�е�GNSS���ջ�������λ�������������ĵ�ƫ�����[m]

�������

State[6]  ����Ϊ���ؽ��ջ���λ�ú��ٶ�, ���Ϊ�����������λ��

***************************************************************************/

void PhaseCentToMassCent( bool Flag, const double Bias[3], double State[6] )
{
	int    i;
	double ModR;             /* ���ǵĵ���ʸ�� */
	double ModVel;           /* �����ٶȵ�ģ   */
	double X[3] = { 0.0 }, Y[3] = { 0.0 }, Z[3] = { 0.0 };

	ModR   = sqrt( VectDot( 3, 3, State, State ) );
	ModVel = sqrt( VectDot( 3, 3, &State[3], &State[3] ) );

	if( (ModR <= (R_WGS84-1000.))
		&& (ModVel < 1000.0) )    /* ����λ�ò���ȷ, ������ */
	{
		return; 
	}

	for( i=0; i<3; i++ )
	{
		Z[i] = State[i] / ( -1.0*ModR );
		X[i] = State[i+3] / ModVel;
	}

	CrossDot( 3, 3, Z, X, Y );

	if( Flag )   /* ��λ���ĵ��������� */
	{
		for( i=0; i<3; i++ )
		{
			State[i] = State[i] - ( X[i]*Bias[0] + Y[i]*Bias[1] + Z[i]*Bias[2] );
		}
	}
	else
	{
		for( i=0; i<3; i++ )
		{
			State[i] = State[i] + ( X[i]*Bias[0] + Y[i]*Bias[1] + Z[i]*Bias[2] );
		}

	}
}

/***************************************************************************
//
// XYZToRTN
//
// Ŀ��: ���ռ�ֱ������ϵ�µ��������ת��ΪRTN�������ϵ�ķ���

˵��: RTNָ���Ǿ�������ͷ���

//
// �������:
//
State     ���ǵ��˶�״̬������λ�ú��ٶȣ����ڶ���RTN����ϵ
dXYZ      �ռ�ֱ������ϵ�µ��������
dRTN      RTN����ϵ�µ��������

***************************************************************************/

void XYZToRTN( double State[6], double dXYZ[3], double dRTN[3] )   
{
	int i;
	double X[3],Y[3],Z[3], dis;
	for(i=0;i<3;i++)
		dRTN[i]=0.0;
	dis = sqrt( VectDot( 3, 3, State, State ) );
	X[0] = State[0]/dis;
	X[1] = State[1]/dis;
	X[2] = State[2]/dis;

	CrossDot( 3, 3, State, &State[3], Z );

	dis = sqrt( VectDot( 3, 3, Z, Z ) );
	Z[0] = Z[0]/dis;
	Z[1] = Z[1]/dis;
	Z[2] = Z[2]/dis;

	CrossDot( 3, 3, Z, X, Y );

	for( i=0; i<3; i++ )
	{
		dRTN[i] = X[i]*dXYZ[0]+Y[i]*dXYZ[1]+Z[i]*dXYZ[2];//ԭ��������
		/*
		dRTN[0] += X[i]*dXYZ[i];//rj20160721
		dRTN[1] += Y[i]*dXYZ[i];
		dRTN[2] += Z[i]*dXYZ[i];
		*/
	}


}
//����xyzתrtn��ת������rj20160721
//flat=1  XYZtoRTN
//flat=0  RTNtoXYZ
void MatrixXYZ2RTN(double State[6],double transMat[9],int flag)
{
	int i;
	double X[3],Y[3],Z[3], dis;

	dis = sqrt( VectDot( 3, 3, State, State ) );
	X[0] = State[0]/dis;
	X[1] = State[1]/dis;
	X[2] = State[2]/dis;

	CrossDot( 3, 3, State, &State[3], Z );

	dis = sqrt( VectDot( 3, 3, Z, Z ) );
	Z[0] = Z[0]/dis;
	Z[1] = Z[1]/dis;
	Z[2] = Z[2]/dis;
	CrossDot( 3, 3, Z, X, Y );
	if(flag==1)//1��XYZ2RTN
		for( i=0; i<3; i++ )
		{
			transMat[i]=X[i];
			transMat[i+3]=Y[i];
			transMat[i+6]=Z[i];
		}
	else//0��RTN2XYZ
		for( i=0; i<3; i++ )
		{
			transMat[i*3]=X[i];
			transMat[i*3+1]=Y[i];
			transMat[i*3+2]=Z[i];
		}
}