/****************************************************************************
Ŀ�ģ�    ����ʵʱ����, ��϶���ѧģ���˲�ʵʱ����


�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/

#include <math.h>
#include <stdlib.h>
#include "RTOD_Const.h"
#include "CommonFuncs.h"
#include "RefSys.h"
#include "RTOD.h"

ONEPOCHOFSP3     *PRE_EPH;
pcvs_t pcvs;

void initKFState(EKFSTATE  *KFState)
{

	KFState->Step = 120.0;/* ���ֲ��� */
	//����grace-A����
	KFState->Para[0].m_a = 50;//����Ϊ10x10
	KFState->Para[0].n_a = 50;
	KFState->Para[0].GM  = GM_Earth;
	KFState->Para[0].R_ref = R_Earth;

	KFState->CentBias[0] =  0.0;
	KFState->CentBias[1] =  0.0;
	KFState->CentBias[2] = -0.444;

	KFState->Para[0].Mass = 487.2;
	KFState->Para[0].Cd   = 0.5;
	KFState->Para[0].Cr   = 1.1;
	KFState->Para[0].Area_H = 0.955;
	KFState->Para[0].Area_R = 5;

	//����grace-B����
	
	KFState->Para[1].m_a = 50;//����Ϊ10x10
	KFState->Para[1].n_a = 50;
	KFState->Para[1].GM  = GM_Earth;
	KFState->Para[1].R_ref = R_Earth;

	KFState->CentBias[3] =  0.0;
	KFState->CentBias[4] =  0.0;
	KFState->CentBias[5] = -0.444;

	KFState->Para[1].Mass = 487.2;
	KFState->Para[1].Cd   = 0.5;
	KFState->Para[1].Cr   = 1.1;
	KFState->Para[1].Area_H = 0.955;
	KFState->Para[1].Area_R = 5;

	KFState->Tao[0] = 1;KFState->Tao[1] = 1;       /* �������ٶȵ����ʱ��[s]  */
	KFState->Sigma[0] = 5E-6;KFState->Sigma[1] = 5E-6;  /*A�� B���ǲ������ٶȵĹ������ܶ�[m/s^2] */
	//KFState->Sigma[1] = 5E-7;
	//�������EmptyEKFSTATEStruct��EmptyAPRIORBITStruct�г�ʼ��
	KFState->eleMask=0;

	EmptyAPRIORBITStruct( &KFState->AprioriState[0]);
	EmptyAPRIORBITStruct( &KFState->AprioriState[1]);
	EmptyEKFSTATEStruct( KFState );
	EmptyCurComObsStruct(&KFState->CurComObs);
	EmptyddObsStruct(&KFState->ddOBS);

	KFState->wholeComObs=(Common11Obs *)malloc(MAXOBSNUMDAY*sizeof(Common11Obs));
}
void freeKFState(EKFSTATE KFState)
{
	free(KFState.wholeComObs);
}
/***************************************************************************
//
// EmptyAPRIORBITStruct
//
// Ŀ��: ��ʼ��APRIORBIT�ṹ��

�������

ApriOrbit      ����ʼ���ı���

***************************************************************************/

void EmptyAPRIORBITStruct( APRIORBIT* ApriOrbit )
{
	int i;
	ApriOrbit->LeoClk[0] = 0.0;
	ApriOrbit->LeoClk[1] = 0.0;

	ApriOrbit->OrbitAccuracy = 9999.999;
	ApriOrbit->Validate  = 0;

	for( i=0; i<3; i++ )
	{
		ApriOrbit->LeoPos[i] = 0.0;
	}

}

/***************************************************************************
//
// EmptySATLISTStruct
//
// Ŀ��: ��ʼ��SATLIST�ṹ��

�������

SatList      ����ʼ���ı���

***************************************************************************/

void EmptySATLISTStruct( SATLIST* SatList )
{
	int i;

	for( i=0; i<MAXCHANNUM; i++ )
	{
		SatList[i].Prn = 0;
		SatList[i].Status = 0;
		SatList[i].System = UNKS;
	}
}

/***************************************************************************
//
// EmptyPPRESULTStruct
//
// Ŀ��: ��ʼ��PPRESULT�ṹ��

�������

PPResult      ����ʼ���ı���

***************************************************************************/

void EmptyPPRESULTStruct( PPRESULT* PPResult )
{
	int i;

	for( i=0; i<3; i++ )
	{
		PPResult->Position[i] = 0.0;
		PPResult->PosBLH[i]   = 0.0;
		PPResult->Velocity[i] = 0.0;
		PPResult->VelNEU[i]   = 0.0;
		PPResult->MatrixFromBLHtoNEU[3*i+0] = 0.0;
		PPResult->MatrixFromBLHtoNEU[3*i+1] = 0.0;
		PPResult->MatrixFromBLHtoNEU[3*i+2] = 0.0;
	}

	PPResult->RcvClkOft[0] = 0.0;
	PPResult->RcvClkOft[1] = 0.0;
	PPResult->RcvClkSft = 0.0;
	PPResult->PDOP      = 999.0;
	PPResult->GDOP      = 999.0;
	PPResult->HDOP      = 999.0;
	PPResult->VDOP      = 999.0;
	PPResult->TDOP      = 999.0;
	PPResult->SigmaPos  = 999.0;
	PPResult->SigmaVel  = 999.0;
	PPResult->Coverage  = 999.0;
	PPResult->Iterator  = 20;
	PPResult->IsSuccess = false;

	for( i=0; i<MAXCHANNUM; i++ )
	{
		PPResult->Residual[i] = 0.0;

		PPResult->SatList[i].Prn = 0;
		PPResult->SatList[i].Status = 0;
		PPResult->SatList[i].System = UNKS;
	}
}                                  

/***************************************************************************
//
// EmptySATMIDRESULTStruct
//
// Ŀ��: ��ʼ��SATMIDRESULT�ṹ��

�������

Num             SATMIDRESULT�����ά��
SatMidInfo      ����ʼ���ı���

***************************************************************************/

void EmptySATMIDRESULTStruct( int Num, SATMIDRESULT* SatMidInfo )
{
	int i, j;

	for( j=0; j<Num; j++ )
	{
		for( i=0; i<3; i++ )
		{
			SatMidInfo[j].SatPos[i] = 0.0;
			SatMidInfo[j].SatVel[i] = 0.0;
		}

		SatMidInfo[j].Elevation = pi/2.0;
		SatMidInfo[j].IonoCorr  = 0.0;
		SatMidInfo[j].SatClkOft = 0.0;
		SatMidInfo[j].SatClkSft = 0.0;
		SatMidInfo[j].Relativity = 0.0;
		SatMidInfo[j].TropCorr   = 0.0;

	}

}
/*ͨ������������������λ�ú��ٶ�*/
void computeGPSfromSP3(const GPSTIME T_Tm,const int Prn,const int order,
	double * Pos0,double * Vel0,double * ClkOft,double * ClkSft)
{
	int k;
	ONESATEHP OneSatEph;//��������
	//�����ӲΪ���㾫ȷģ������

	//������λ���ĸ���
	gtime_t time;//������λ���ĸ���
	double ep[6];
	double rs[6];
	double dant[3];
	pcv_t *pcvt;
	pcvt=(pcv_t*)malloc(32*sizeof(pcv_t));

	/*��������*/
	Lagrange_SP3(T_Tm,Prn,order,PRE_EPH,&OneSatEph);
	//flag=Lagrange_CLK(T,(*OBS).obs[i].PRN,2,PRECLK,&OneSatClk);
	if (!OneSatEph.ISExist)
	{
		return;//������������ڲ�ʧ�ܣ��򷵻�,���Թ㲥�����������pos��vel���и�ֵ�����ù㲥��������õ��Ľ��
	}
	Pos0[0]  =OneSatEph.satposx;
	Pos0[1]  =OneSatEph.satposy;
	Pos0[2]  =OneSatEph.satposz;
	Vel0[0]	 =OneSatEph.satvelx;
	Vel0[1]  =OneSatEph.satvely;
	Vel0[2]  =OneSatEph.satvelz;
	*ClkOft  =OneSatEph.satclk;
	//���������Ӳ������۸���
	*ClkOft   = *ClkOft - 2.0*VectDot(3,3,Pos0,Vel0)/C_Light/C_Light;
	*ClkSft   = 0.0;//����������ٶԹ㲥�����Ӳ�Ĳ���
	/*����������λ���ĸ���*/

	GPSTimeToep(&T_Tm,ep);
	time=epoch2time(ep);
	setpcv(time, pcvt, &pcvs);
	for(k=0;k<3;k++)
	{
		rs[k] = Pos0[k];
		rs[k+3] = Vel0[k];
	}
	/*rs���ǵع�ϵ�������ٶȺ�����*/
	satantoff(time, rs,&pcvt[Prn-1],dant);
	/*������������λ���ĸ���*/
	for(k=0;k<3;k++)
	{
		Pos0[k] += dant[k];
	}
	free(pcvt);
}
/***************************************************************************
//
// ComputeGPSSatOrbitAtSignalTrans
//
// Ŀ��: �������Ƿ���ʱ�̵����ǹ��,�����Ӳ�,�����ЧӦ,���Ǹ߶Ƚ�
���ǵ�����ת����(�˼����ǽ������).

// �������:
//
Prn             GPS���Ǻ�
Time            �۲�ʱ��
PreLeoOrb[3]    Ԥ�������ؽ��ջ�λ��[m]
PreLeoClk       Ԥ�������ؽ��ջ�GPS�Ӳ�[m]
Height          ���ؽ��ջ��ĸ߳�[m]
GPSEph          ĳ��GPS���ǵ�����
IonPara         ��������

�������

SatMidInfo      ���ǹ��, �ٶȵ��м���������Ϣ

����ֵ

����ɹ�����true, ���򷵻�false.

***************************************************************************/

bool ComputeGPSSatOrbitAtSignalTrans( const short Prn, const GPSTIME* Time,
	double PreLeoOrb[3], double PreLeoClk, double &Height,
	GPSEPHREC* GPSEph, IONOPARA* IonPara,
	SATMIDRESULT* SatMidInfo )
{
	int i,k;
	int Iterator;        /* �������ǹ���ĵ�������, С��10�� */
	GPSTIME T_Tm;        /* �źŷ���ʱ�̵�ϵͳʱ�� */
	double  dt0, dt1;          /* �źŴ���ʱ�� [s]  */
	double  RotAng;      /* �źŴ���ʱ���ڵ�����ת�Ƕ�[Rad] */
	double  ClkOft, ClkSft;  /* �����Ӳ������ */
	double  Mat[9], Pos0[3], Pos1[3], Vel0[3];
	double  dPos[3];

	

	dt1 = 0.075;     /*  ��ʼ���Դ���ʱ�� */
	Iterator = 0;
	T_Tm.Week = Time->Week;
	do {
		dt0 = dt1; 
		T_Tm.SecOfWeek = Time->SecOfWeek - dt0 - PreLeoClk/C_Light;

		if( ComputeGPSOrbitAndClockFullInfo( Prn, &T_Tm, GPSEph, IonPara, 
			Pos0, Vel0, &ClkOft, &ClkSft ) == false )
		{
			return false;
		}

		/*Ϊ�˷�ֹ�㲥��������Ӱ�쾫�������������³�ʼ��*/
		/*
		T_Tm.SecOfWeek = Time->SecOfWeek - dt0 - PreLeoClk/C_Light;
		computeGPSfromSP3(T_Tm,Prn,5,Pos0,Vel0,&ClkOft,&ClkSft);
		computeGPSfromSP3(T_Tm,Prn,3,Pos0,Vel0,&ClkOft,&ClkSft);
		computeGPSfromSP3(T_Tm,Prn,4,Pos0,Vel0,&ClkOft,&ClkSft);*/
		/*  ������ת����  */

		RotAng = Omega_WGS * dt0;
		Rotation_z( RotAng, Mat );
		MatrixMultiply( 3, 3, 3, 1, Mat, Pos0, Pos1 );

		for( i=0; i<3; i++ )
		{
			dPos[i] = Pos1[i] - PreLeoOrb[i];
		}

		dt1 = sqrt( VectDot( 3, 3, dPos, dPos ) ) / C_Light;

		Iterator++;  

	} while( (fabs( dt1-dt0 )>1E-8) && (Iterator < 10 ));

	SatMidInfo->SatClkOft = ClkOft;
	SatMidInfo->SatClkSft = ClkSft;

	SatMidInfo->Relativity = 0.0; /* ���������Ӳ��и���, �˴�Ϊ0 */

	/* �����ٶȸ��� */

	MatrixMultiply( 3, 3, 3, 1, Mat, Vel0, SatMidInfo->SatVel );

	CopyArray( 3, SatMidInfo->SatPos, Pos1 );

	/*  ���Ǹ߶ȽǼ��� */

	SatMidInfo->Elevation = SatElev( SatMidInfo->SatPos, PreLeoOrb );

	/*  ��Ƶ���ջ���Tgd����, �����ЧӦ�Ѱ������Ӳ� 

	SatMidInfo->SatClkOft = SatMidInfo->SatClkOft - GPSEph->TGD;  */

	/*   ������ӳټ��� */

	if( IonPara->IsValid )
	{
		SatMidInfo->IonoCorr = KlobucharForLEO( Time, SatMidInfo->SatPos, PreLeoOrb, IonPara );
	}
	else
	{
		SatMidInfo->IonoCorr = 0.0;
	}

	/*    �������ӳٸ���, SPPʹ�ø��Ը���ģ��  */

	if( Height <= 30000.0 )
	{
		SatMidInfo->TropCorr = 2.47/(sin(SatMidInfo->Elevation)+0.0121);
	}
	else
	{
		SatMidInfo->TropCorr = 0.0;
	}

	return true;
}


/***************************************************************************
//
// Klobuchar
//
// Ŀ��: ʹ��Klobucharģ��, ���㵥Ƶ���ջ��ĵ�����ӳٸ�����, �����ڵ������

//
// �������:
//
Time      �۲�ʱ��,��GPSϵͳʱ��
SatPos    ����λ��
RcvPos    ���ջ�λ��
IonoPara  ��������

����ֵ

�ź�·�����������ӳٸ���ֵ.

***************************************************************************/

double Klobuchar( const GPSTIME* Time, double SatPos[3],
	double RcvPos[3], IONOPARA* IonoPara)
{
	int i;
	double correction;
	double RcvBLH[3];       /*  [Rad]  */
	double Mat[9], dPos[3], NEU[3];   /* To NEU matrix */
	double svE, svA;       /* satllite elevation and azimuth [ semi-circle] */
	double phi_u, lambda_u, psi, phi_i, lambda_i, phi_m;
	double iAMP, iPER, t, x, iF, t_iono;

	XYZToBLH( RcvPos, RcvBLH, R_WGS84, F_WGS84 );
	BLHToNEUMatrix( RcvBLH, Mat );

	for( i=0; i<3; i++ )
	{
		dPos[i] = SatPos[i] - RcvPos[i];
	}

	MatrixMultiply( 3, 3, 3, 1, Mat, dPos, NEU );

	svE = atan( NEU[2]/sqrt(NEU[0]*NEU[0]+NEU[1]*NEU[1]) ) / pi;
	svA = atan2( NEU[1], NEU[0] ) / pi;

	phi_u    = RcvBLH[0] / pi;
	lambda_u = RcvBLH[1] / pi;
	psi = 0.0137 / (svE + 0.11) - 0.022;

	phi_i = phi_u + psi * cos(svA*pi);
	if (phi_i > 0.416)
	{
		phi_i = 0.416;
	}
	if (phi_i < -0.416)
	{
		phi_i = -0.416;
	}

	lambda_i = lambda_u + psi * sin(svA*pi) / cos(phi_i*pi);

	phi_m = phi_i + 0.064 * cos((lambda_i - 1.617)*pi);

	iAMP = 0.0;
	iPER = 0.0;
	for (int n = 0; n < 4; n++)
	{
		iAMP += IonoPara->alpha[n] * pow(phi_m, n);
		iPER += IonoPara->beta[n] * pow(phi_m, n);
	}
	if (iAMP < 0.0)
	{
		iAMP = 0.0;
	}
	if (iPER < 72000.0)
	{
		iPER = 72000.0;
	}

	t = 43200.0 * lambda_i + fmod( Time->SecOfWeek, SECPERDAY );
	if (t >= 86400.0)
	{
		t = t - 86400.0;
	}
	if (t < 0)
	{
		t = t + 86400.0;
	}

	x = pi * (t - 50400.0) / iPER; // x is in radians

	/* ͶӰ����  */
	iF = 1.0 + 16.0 * pow(0.53 - svE, 3);    /* �������ʹ�� */

	t_iono = 0.0;
	if (fabs(x) < 1.57)
	{
		t_iono = iF * (5.0e-9 + iAMP * (1 - pow(x, 2)/2 + pow(x, 4)/24));
	}
	else
	{
		t_iono = iF * 5.0e-9;
	}

	correction = t_iono * C_Light;

	return correction;

}

/***************************************************************************
//
// KlobucharForLEO
//
// Ŀ��: ʹ��Klobucharģ��, ���㵥Ƶ���ջ��ĵ�����ӳٸ�����. ����LEO��GPS
���ǵ�λ��, �����������. ��ģ�������ڵ͹����ǵĵ�Ƶ��������

ģ�Ͳο�����:
THE JOURNAL OF NAVIGATION (2002), 55, 293��304. 
The Royal Institute of Navigation 
DOI: 10.1017}S0373463302001789 Printed in the United Kingdom
Ionospheric Correction for GPS Tracking of LEO Satellites
Oliver Montenbruck and Eberhard Gill (German Aerospace Center DLR)

//
// �������:
//
Time      �۲�ʱ��,��GPSϵͳʱ��
SatPos    ����λ��
RcvPos    ���ջ�λ��
IonoPara  ��������

����ֵ

�ź�·�����������ӳٸ���ֵ.

***************************************************************************/

double KlobucharForLEO( const GPSTIME* Time, double SatPos[3],
	double RcvPos[3], IONOPARA* IonoPara)
{
	int i;
	double correction, alpha;
	double dPos[3], HIP, NormPos, Tmp; 
	double Mat[9], NEU[3], RcvBLH[3];      /* To NEU matrix */
	double phi_u, lambda_u, psi, phi_i, lambda_i, phi_m;
	double H0, dH, svE, svA;    /* satllite elevation and azimuth [ semi-circle] */
	double iAMP, iPER, t, x, iF, t_iono;

	H0 = 420000.0;                  /* ���������ܶ����ĸ߶� */
	dH = 100000.0;

	NormPos = sqrt( VectDot( 3, 3, RcvPos, RcvPos ) );
	if( NormPos < 5000.0 || NormPos > ( R_WGS84 + 1E6 ) )     /* ���ջ�λ�ô��� */
	{
		return 0.0;
	}

	XYZToBLH( RcvPos, RcvBLH, R_WGS84, F_WGS84 );
	BLHToNEUMatrix( RcvBLH, Mat );

	for( i=0; i<3; i++ )
	{
		dPos[i] = SatPos[i] - RcvPos[i];
	}

	MatrixMultiply( 3, 3, 3, 1, Mat, dPos, NEU );

	svE = atan( NEU[2]/sqrt(NEU[0]*NEU[0]+NEU[1]*NEU[1]) )/pi;
	svA = atan2( NEU[1], NEU[0] )/pi;

	phi_u    = RcvBLH[0] / pi;
	lambda_u = RcvBLH[1] / pi;

	Tmp = exp( 1.0 - exp( ( -RcvBLH[2]+H0 )/dH ) );
	HIP = H0 - dH * log( 1.0 - log( ( 2.71828183 + Tmp )/2.0 ) ); /* e=2.71828183 */
	alpha = ( 1.359140915 - Tmp/2.0 ) / ( 2.71828183 - exp( 1.0 - exp( H0/dH ) ) );

	/* ������ļн� zdh06411@163.com, caohaisheng@yahoo.com.cn */

	Tmp = asin( NormPos * cos(svE*pi) / ( R_WGS84 + HIP ) );
	psi = 0.5 - Tmp/pi - svE;
	iF = 1.0 / cos( Tmp );

	phi_i = phi_u + psi * cos(svA*pi);

	if (phi_i > 0.416)
	{
		phi_i = 0.416;
	}
	if (phi_i < -0.416)
	{
		phi_i = -0.416;
	}

	lambda_i = lambda_u + psi * sin(svA*pi) / cos(phi_i*pi);

	phi_m = phi_i + 0.064 * cos((lambda_i - 1.617)*pi);

	iAMP = 0.0;
	iPER = 0.0;
	for (int n = 0; n < 4; n++)
	{
		iAMP += IonoPara->alpha[n] * pow(phi_m, n);
		iPER += IonoPara->beta[n]  * pow(phi_m, n);
	}
	if (iAMP < 0.0)
	{
		iAMP = 0.0;
	}
	if (iPER < 72000.0)
	{
		iPER = 72000.0;
	}

	t = 43200.0 * lambda_i + fmod( Time->SecOfWeek, SECPERDAY );
	if (t >= 86400.0)
	{
		t = t - 86400.0;
	}
	if (t < 0)
	{
		t = t + 86400.0;
	}

	x = pi2 * (t - 50400.0) / iPER; // x is in radians

	t_iono = 0.0;
	if (fabs(x) < 1.57)
	{
		t_iono = iF * (5.0e-9 + iAMP * (1 - pow(x, 2)/2 + pow(x, 4)/24) );
	}
	else
	{
		t_iono = iF * 5.0e-9;
	}

	correction = t_iono * C_Light;

	return correction;

}

/***************************************************************************
//
// PointPositionRTOD
//
// Ŀ��: ��GPS��GLONASS���ǵ�α��, ���е��㶨λ, �����˫Ƶ����, �������������ӳ�.
����ǵ�Ƶ����,�øĽ���Klobucharģ��.         

//
// �������:
//
GPSEph          GPS��������[32]
IonPara         ��������
GLOEph          GLONASS��������[32]
GloTmCorr       GLONASSʱ����UTC(SU)ϵͳ��ĸ���ֵ
EpochObs        ��Ԫ�Ĺ۲�����, �Դ�˳��������ǹ��
AprioriPos      ���ջ�����������, �������Ԥ�����
AprioriClk      ���ջ��������Ӳ�[GPS,GLONASS]

�������

SatMidInfo      ���ǹ��, �ٶȵ��м���������Ϣ, �����˲�����
Result          ���㶨λ���(�ع�ϵ)

����ֵ

���붨λ��������

�㷨����:

����֯�۲�����, ������������Ϣ( ������״ζ�λ�����ú�Ķ�λ, ������ϢΪ0.0),
���㵼�����ǹ�����Ӳ���Ϣ, Ȼ��Թ۲����ݽ��дֲ�̽��, ����Ǵֲ�����, �ù۲�ֵ
����Ϊ������.


***************************************************************************/
int PointPositionRTOD( int graceType, GPSEPHREC* GPSEph, IONOPARA* IonPara,
	EpochObsData* Epoch, APRIORBIT* PreOrb, 
	SATMIDRESULT* SatMidInfo, PPRESULT* Result )
{
	int i, j, k, Iterator;                     /*  IteratorΪ���㶨λ��������  */
	int SatNumUsed;      /*  ���㶨λʹ�õ�������, GPS������, GLONASS������ */
	short Prn;
	short PosKind;            /* GPS:0, GLO:1, GPS&GLO:2  */

	int    PRValid;                            /* �۲�ֵ��α������Ժ�α��۲�ֵ */
	double PRange;
	double AprioriPos[3], AprioriClk[2], BLH[3];
	double Range, Ion, Height;                                 /*  ���ջ��뵼������֮��ľ���  */
	double BPos[MAXCHANNUM];                      /* α�������յĹ۲�ֵ-����ֵ */
	double MeasA[MAXCHANNUM*4], MeasAT[MAXCHANNUM*4];          /* �۲���� [MAXCHANNUM][4] */
	double Weight[MAXCHANNUM];                               /* Ȩ����, ��λ��ֻȡ�Խ���Ԫ�� */
	double Qvv[MAXCHANNUM];                               /* �۲�ֵ��������б������Խ���Ԫ�� */
	double ATA[16], InvATA[16];
	double ATB[4];
	double dPos[4];               /* ��λ���  */
	double Residual[MAXCHANNUM] = {0.0};  /* ��λ�в�  */



	if( PreOrb->Validate )
	{
		CopyArray( 3, AprioriPos, PreOrb->LeoPos );
		CopyArray( 2, AprioriClk, PreOrb->LeoClk );
	}
	else
	{
		AprioriPos[0] = 0.0;
		AprioriPos[1] = 0.0;
		AprioriPos[2] = 0.0;
		AprioriClk[0] = 0.0;
		AprioriClk[1] = 0.0;
	}

	Iterator = 0;

	do{
		SatNumUsed = 0;
		XYZToBLH( AprioriPos, BLH, R_WGS84, F_WGS84 );
		Height = BLH[2];

		for( i=0; i<Epoch->SatNum; i++ )
		{
			Result->SatList[i].Prn = Epoch->SatObs[i].Prn;
			if(Epoch->SatObs[i].System!=GPS)
				continue;
			Result->SatList[i].System = Epoch->SatObs[i].System;

			PRValid = GetOneSatPseudoRange( Epoch->SatObs[i].System,
				&Epoch->SatObs[i], &PRange, &Ion );

			if(PRValid > 0 )
			{
				Prn = Epoch->SatObs[i].Prn;

				if( ComputeGPSSatOrbitAtSignalTrans( Prn, &Epoch->Time, AprioriPos, AprioriClk[0], 
					Height, &GPSEph[Prn-1], IonPara, &SatMidInfo[i])
					&& ( SatMidInfo[i].Elevation >= 10.0*Rad ))   /*  �߶ȽǴ���10�� */
				{
					for(k=0;k<3;k++)
					{
						Epoch->SatObs[i].satPos[k] = SatMidInfo[i].SatPos[k];
					}

					Range = 0.0;
					for( k=0; k<3; k++ )
					{
						Range = Range + pow( Epoch->SatObs[i].satPos[k]-AprioriPos[k], 2.0 );
					}

					Range = sqrt( Range ); 
					MeasA[SatNumUsed*4+0] = ( AprioriPos[0] - Epoch->SatObs[i].satPos[0] )/Range;
					MeasA[SatNumUsed*4+1] = ( AprioriPos[1] - Epoch->SatObs[i].satPos[1] )/Range;
					MeasA[SatNumUsed*4+2] = ( AprioriPos[2] - Epoch->SatObs[i].satPos[2] )/Range;
					MeasA[SatNumUsed*4+3] = 1.0;             /* ���ջ�GPSϵͳ�Ӳ�ϵ�� */
					Weight[SatNumUsed]    = 1.0;  // * pow( sin(SatMidInfo[i].Elevation), 2.0 );

					BPos[SatNumUsed] = PRange - Range - AprioriClk[0] 
					+ SatMidInfo[i].SatClkOft * C_Light - SatMidInfo[i].TropCorr; 

					if( PRValid == 1 )
					{		
						BPos[SatNumUsed] = BPos[SatNumUsed] - SatMidInfo[i].IonoCorr;//����ǵ�Ƶ
					}

					Result->SatList[i].Status = 1;        /* ���뵥�㶨λ���� */
					
					SatNumUsed++;
				}
				else    
				{
					Epoch->SatObs[i].Used = 0;     /* ��������߶Ƚǵ� */                      

				}
				
			}
			Epoch->SatObs[i].Ion=Ion;			  /*������˫Ƶ�õ��ĵ�����ӳ٣��������˫Ƶ����Ϊ0*/
		}

		/*  ��ɲв����ݼ���, ���дֲ���,   */
		//����Ҫ������λ�ú;��Ȳ��ܽ��м��飬ͨ���۲�ֵ�в���н��ջ��Ӳ���鷽��

		if( PreOrb->Validate )
		{
			if( DetectPseudoRangeOutliers(graceType, PreOrb->OrbitAccuracy, BPos, Epoch , Result->SatList ) )
			{
				DeleteOutlierFromObsList( &SatNumUsed, BPos, MeasA, Weight, Result->SatList);
			}

			PreOrb->Validate = 0;   /*  ÿ����Ԫֻ̽��һ��  */
		}

		/*  �������  */

		for( k=SatNumUsed; k<MAXCHANNUM; k++)  /* ���������� */
		{
			BPos[k] = 0.0;
			Weight[k] = 0.0;
			for( j=0; j<4; j++ )
			{
				MeasA[k*4+j] = 0.0;
			}
		}

		if( SatNumUsed>=4 )  /* ������GLONASS���� */
		{

			MatrixTranspose( SatNumUsed, 4, MeasA, MeasAT );
			MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 4, MeasAT, Weight, MeasA, ATA );
			MatrixInv( 4, ATA, InvATA );

			MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 1, MeasAT, Weight, BPos, ATB );
			MatrixMultiply( 4, 4, 4, 1, InvATA, ATB, dPos );

			AprioriClk[0] = AprioriClk[0] + dPos[3];

		}
		else
		{
			//            printf("\nCannot position because of only %2d satellites at %10.3f epoch.\n",
			//                SatNumUsed, Epoch->Time.SecOfWeek );

			break;
		}

		for( k=0; k<3; k++ )
		{
			AprioriPos[k] = AprioriPos[k] + dPos[k];
		}

		Iterator++;
		Result->Iterator = Iterator;
		Result->Coverage = VectDot( 3, 3, dPos, dPos );

	}while ( (Result->Coverage>1E-6) && (Iterator < 10) );
	//ѭ���������߶Ƚ�
	for(i=0;i<Epoch->SatNum;i++)
		Epoch->SatObs[i].elevation=SatMidInfo[i].Elevation;
	Result->SatNumUsed = SatNumUsed;

	CopyArray( 3, Result->Position, AprioriPos );
	CopyArray( 2, Result->RcvClkOft, AprioriClk );

	XYZToBLH( Result->Position, Result->PosBLH, R_WGS84, F_WGS84 );
	BLHToNEUMatrix( Result->PosBLH, Result->MatrixFromBLHtoNEU );

	/* ����۲�ֵ�в� */

	MatrixMultiply( SatNumUsed, 4, 4, 1, MeasA, dPos, Residual );//
	for( i=0; i<SatNumUsed; i++ )
	{
		Residual[i] = Residual[i] - BPos[i];
	}

	/* ����۲�ֵ��������Э������ */

	ComputeQvv( SatNumUsed, 4, MeasA, InvATA, MeasAT, Weight, Qvv );

	Result->PDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] );
	Result->GDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] + InvATA[15] );
	Result->HDOP = sqrt( InvATA[0] + InvATA[5] );
	Result->VDOP = sqrt( InvATA[10] );
	Result->TDOP = sqrt( InvATA[15] );

	if( SatNumUsed > 5 ) /* ����5�����ǲż��㶨λ�����, ��ǿ�ɿ��� */
	{
		Result->SigmaPos = 0.0;

		for( j=0, i=0; i<Epoch->SatNum; i++ )
		{
			if( Result->SatList[i].Status == 1 )
			{
				Result->Residual[i] = Residual[j];
				Result->SigmaPos = Result->SigmaPos + Weight[i]*pow( Residual[i], 2.0 );
				j++;
			}
		}
		Result->SigmaPos = sqrt( Result->SigmaPos / (SatNumUsed-4) );
	}
	else
	{
		Result->SigmaPos = 999.99;
	}

	/* ���ֲ�̽�� */

	if( SatNumUsed > 5 )
	{
		CheckPostResidual_W( Epoch->SatNum, 3.0, Residual, Qvv , Result );
		//������ʱ�����дֲ���飬�Ƶ�����
	}
	/*RJ-2016-9-20
	**��ģ����Ϊ�˰ѵ��㶨λ������ֲ��������Ϊ������
	*/
	for(i=0;i<Epoch->SatNum;i++)
	{
		if(Result->SatList[i].Status!=1&&(Epoch->SatObs[i].Used==1))
			Epoch->SatObs[i].Used = 0;
	}
	if( Result->Coverage < 1E-6 && Result->Iterator < 10 &&
		Result->PDOP < 8.0 && Result->SigmaPos < 5.0 && SatNumUsed > 5 )
	{
		Result->IsSuccess = true;
	}
	else
	{
		Result->IsSuccess = false;
	}

	return SatNumUsed;
}

/***************************************************************************
//
// ComputeQvv
//
// Ŀ��: ���㶨λ��, ����۲�ֵ��������Э��������, ����̽�����۲�ֵ�ֲ�����.

//
// �������:
//
m               �۲�ֵ����, �����ʹ�õ�������
n               ���ƵĲ�������, ���ڵ�ϵͳ��λΪ4, ˫ϵͳ��϶�λΪ5
A               �۲�ϵ������ [m*n]
InvN            Inv(ATPA)
AT              A�����ת��
Pll             �۲�ֵ��Ȩ��,Ϊ�Խ���Ԫ��

�������

Qvv             ��������Э��������, Ϊ�Խ���Ԫ��

***************************************************************************/
void ComputeQvv( int m, int n, double A[], double InvN[], double AT[], 
	double Pll[], double Qvv[] )
{
	int i;
	double AN[MAXCHANNUM*5], ANAT[MAXCHANNUM*MAXCHANNUM];

	MatrixMultiply( m, n, n, n, A, InvN, AN );
	MatrixMultiply( m, n, n, m, AN, AT, ANAT );

	for( i=0; i<m; i++ )
	{
		Qvv[i] = 1.0/Pll[i] - *(ANAT+i*m+i);

	}
}
/****************************************************************************
ComputeQLL

Ŀ��: ����ƽ���۲�ֵЭ������
����: int m
����: int n
����: const double A[]
����: const double InvN[]
����: const double AT[]
����: const double Pll[]
����: double QLL[]
****************************************************************************/
void ComputeQLL( int m, int n, const double A[],const double InvN[],const double AT[], 
	const double Pll[], double QLL[] )
{
	int i;
	double Q[MAXCHANNUM];
	for (i=0;i<m;i++)
	{
		Q[i]=1.0/Pll[i];
	}
	double AN[MAXCHANNUM*5], ANAT[MAXCHANNUM*MAXCHANNUM];
	double QANAT[MAXCHANNUM*MAXCHANNUM],QANATQ[MAXCHANNUM*MAXCHANNUM];
	MatrixMultiply( m, n, n, n, A, InvN, AN );
	MatrixMultiply( m, n, n, m, AN, AT, ANAT );
	MatrixMultiply2(1,m,m,m,ANAT,Q,QANAT);
	MatrixMultiply2(1,m,m,m,QANAT,Q,QANATQ);

	for( i=0; i<m; i++ )
	{
		QLL[i] = Q[i] - *(QANATQ+i*m+i);

	}
}
/***************************************************************************
//
// PointPositionVelocityDetermination
//
// Ŀ��: ��������������һ����Ԫ�Ĺ۲����ݵ�D1, ���е������(������GPS, 
��GLONASS, GPS/GLONASS��϶�λ), Ϊ�˲��ṩ��ʼ�ٶ���Ϣ. ���������,
CHAMP���ǵ�����û�ж����չ۲�����, ���ȵ���CreateDopplerObs����, ����
�����չ۲�����, �������ø��ӳ���.

GPS��GLONASSʱ��ϵͳ֮����Ӳ�仯���ڴ˱���Ϊ����ͬ��,
�����ڲ�������ʱֻ����һ��Ƶ�����, ���ƵĲ�����4ά��.

ʹ�÷���: ���ӳ���ֻ����PointPositionRTOD��������, ��Ϊ���õ�SatMidInfo
�͵��㶨λ��λ����Ϣ.

//
// �������:
//
EpochObs        ��Ԫ�Ĺ۲�����, 
GLOEph          GLONASS��������, ���л�ȡÿ�����ǵ�Ƶ������
SatMidInfo      ���ǹ��, �ٶȵ��м���������Ϣ
Result          ���㶨λ���, ���а������㶨λ��������Ϣ

����ֵ

������ٵ�������

***************************************************************************/
int PointPositionVelocityDetermination( EpochObsData* Epoch,
	SATMIDRESULT* SatMidInfo, PPRESULT* Result )
{
	int i, j, k;    
	int Prn;
	int SatNumUsed;                               /*  ���㶨λʹ�õ������� */

	double Range;                                 /*  ���ջ��뵼������֮��ľ���  */
	double BVel[MAXCHANNUM];                /* �����յĹ۲�ֵ-����ֵ */
	double MeasA[MAXCHANNUM*4], MeasAT[MAXCHANNUM*4];          /* �۲���� [MAXCHANNUM][5] */
	double Weight[MAXCHANNUM];                               /* Ȩ����, ��λ��ֻȡ�Խ���Ԫ�� */
	double ATA[16], InvATA[16];
	double ATB[4];
	double dPos[4];                                /* ��λ���  */
	double Residual[MAXCHANNUM];                   /* ��λ�в�  */
	double GLOL1WaveLen;                           /* ÿ��GLONASS���ǵ�L1���� */

	SatNumUsed = 0;

	/* ׼���۲�����  */

	for( i=0; i<Epoch->SatNum; i++ )
	{
		if( Epoch->SatObs[i].Used != 1 )
		{
			continue;
		}

		if( Epoch->SatObs[i].System == GPS )   
		{
			for( j=0; j<MAXOBSTYPENUM; j++ )
			{
				if( (Epoch->SatObs[i].Data[j].Availability == true) 
					&& ( Epoch->SatObs[i].Data[j].Type == D1 ) )
				{
					for( k=0; k<3; k++ )
					{
						dPos[k] = SatMidInfo[i].SatPos[k] - Result->Position[k];
					}

					Range = sqrt( VectDot( 3, 3, dPos, dPos ) );

					MeasA[SatNumUsed*4+0] = -dPos[0] / Range;
					MeasA[SatNumUsed*4+1] = -dPos[1] / Range;
					MeasA[SatNumUsed*4+2] = -dPos[2] / Range;
					MeasA[SatNumUsed*4+3] = 1.0;             /* ���ջ�GPSϵͳƵ��ϵ�� */

					Weight[SatNumUsed]    = 2 * pow( sin(SatMidInfo[i].Elevation), 2.0 );

					BVel[SatNumUsed] = -Epoch->SatObs[i].Data[j].Obs*C_Light/FG1_Freq  /* ע������չ۲�ֵ�ķ��� */
						- VectDot( 3, 3, dPos, SatMidInfo[i].SatVel )/Range
						+ SatMidInfo[i].SatClkSft*C_Light;

					SatNumUsed++;
				}
			}
		}

		if( Epoch->SatObs[i].System == GLONASS )  
			continue;
	}

	/*  �������  */

	for( k=SatNumUsed; k<MAXCHANNUM; k++)
	{
		BVel[k] = 0.0;
		for( j=0; j<4; j++ )
		{
			MeasA[k*4+j] = 0.0;
		}
	}

	for( k=0; k<4; k++ )
	{
		dPos[k] = 0.0;
	}

	if( SatNumUsed >= 4 )
	{
		MatrixTranspose( SatNumUsed, 4, MeasA, MeasAT );
		MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 4, MeasAT, Weight, MeasA, ATA );
		MatrixInv( 4, ATA, InvATA );

		MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 1, MeasAT, Weight, BVel, ATB );
		MatrixMultiply( 4, 4, 4, 1, InvATA, ATB, dPos );  /* dPos�����ٶ���Ϣ��Ƶ����Ϣ */

		/* ����۲�ֵ�в�;������� */

		MatrixMultiply( SatNumUsed, 4, 4, 1, MeasA, dPos, Residual );

		Result->SigmaVel = 0.0;
		for( i=0; i<SatNumUsed; i++ )
		{
			Residual[i] = Residual[i] - BVel[i];
			Result->SigmaVel = Result->SigmaVel + Weight[i]*pow( Residual[i], 2.0 );
		}

	}

	/* ����۲�в��׼�� */

	if( SatNumUsed > 4 )
	{
		Result->SigmaVel = sqrt( Result->SigmaVel / (SatNumUsed-4) );
	}       
	else
	{
		Result->SigmaVel = 999.99;
	}

	/*  ������ٽ�� */

	CopyArray( 3, Result->Velocity, dPos );
	Result->RcvClkSft = dPos[3];//������ջ�����

	MatrixMultiply( 3, 3, 3, 1, Result->MatrixFromBLHtoNEU, Result->Velocity, Result->VelNEU );

	return SatNumUsed;

}

/***************************************************************************
//
// DetectPseudoRangeOutliers
//
// Ŀ��: ���������������С���˹��ƵĽ�������, ������������ǵ�O-C
(�۲�ֵ��ȥ����ֵ), �Ƚ�ÿ�����Ǽ�������Ľ��ջ��Ӳ��Ƿ�һ��.
�������Ԫ����GPS��GLONASS����, ��ֿ�̽��, ��Ϊ���߼���Ľ��ջ��Ӳһ��.

//
// �������:
//
Accuracy        ���������ݵľ���ָ��, ����ȷ���ֲ��޳�����ֵ.
O_C             α��۲�ֵ��ȥ�۲�ֵ�ļ���ֵ, ��ÿ�����ǵĽ��ջ��Ӳ�.
Epoch           ��Ԫ�۲�����, ̽�⵽�ֲ�󽫸���������Ϊ�ֲ�۲�ֵ,������
SatList         ��O_C���Ӧ, ��ȷ֪�ֲ�۲�ֵ�������ǿ�����

���ز���

��̽�⵽�ֲ�ʱ, ����1, ���򷵻�0.

***************************************************************************/
bool DetectPseudoRangeOutliers( int graceType, double Accuracy, double O_C[], 
	EpochObsData* Epoch, SATLIST* SatList )
{
	int  i, j;
	int  GPSSatNum, GLOSatNum;

	double GVal[MAXCHANNUM]={0.0};  /*  ��ʱ�洢GPS, GLONASS��o_c */
	double GMean ;                   /* ��ֵ */
	double GStd,  GLim;        /* ��׼��, �޲� */
	bool   HasOutlier = false;
	FILE   *ocFile;
	char   graceName[20]="";
	char   outName[30]="";
	//graceType for the out file name of o-c
	if(graceType==0)
		sprintf(graceName,"GRACE-A");
	else
		sprintf(graceName,"GRACE-B");
	
	GPSSatNum = 0;

	GLim   = Accuracy;

	/* ��GPS��GLONASS���ǵ�O_C���ݷֿ� */

	for( j=0,i=0; i<MAXCHANNUM; i++ )
	{
		if( (SatList[i].Status==1) && (SatList[i].System==GPS) )
		{
			char cPrn[3];
			sprintf(outName,"OutFile\\OC\\%s_%d.txt",graceName,SatList[i].Prn);
			ocFile = fopen(outName,"a+");
			fprintf(ocFile,"%10d%14.3f%14.3f\n",Epoch->Time.Week,Epoch->Time.SecOfWeek,O_C[j]);
			fclose(ocFile);
			GVal[GPSSatNum] = O_C[j];
			GPSSatNum++;
			j++;
		}
	}

	mbbub( GPSSatNum, GVal );     /* ����, �޳����˵�����׼�� */
	GMean = GVal[GPSSatNum/2];    /* ȡ�������м�����Ϊƽ��ֵ */

	GStd = 0.0;
	for( j=0, i=1; i<GPSSatNum-1; i++ )   /* ȥ���˵�����׼�� */
	{
		if( fabs(GVal[i]-GMean) < 20*GLim )
		{
			GStd = GStd + (GVal[i]-GMean) * (GVal[i]-GMean);
			j++;
		}
	}
	if( j>0 )  GLim = sqrt( GStd/j );
	
	GLim = __max( GLim, Accuracy );

	if( GLim < 3.0 )    /* ��ֹ��׼����㲻׼ȷ,�����ɾ�� */
	{
		GLim = 3.0;
	}

	for( j=0,i=0; i<MAXCHANNUM; i++ )
	{
		if( (SatList[i].Status==1) && (SatList[i].System==GPS) )
		{
			if( fabs(O_C[j] - GMean ) >= 3*GLim)
			{
				// 				printf( "Outlier: %10.2f G%02d %12.3f %12.3f %12.3f\n", 
				// 					Epoch->Time.SecOfWeek, SatList[i].Prn, O_C[j], GMean, O_C[j]-GMean );
				Epoch->SatObs[i].Used = -1;
				SatList[i].Status = -1;
				HasOutlier = true;
			}
			j++;
		}
	}
	return HasOutlier;
}
/***************************************************************************
//
// DetectPseudoRangeOutliers
//
// Ŀ��: ���������������С���˹��ƵĽ�������, ������������ǵ�O-C
(�۲�ֵ��ȥ����ֵ), �Ƚ�ÿ��GPS���Ǽ�������Ľ��ջ�����Ӳ��Ƿ�һ��.

//
// �������:
//
Accuracy        ���������ݵľ���ָ��, ����ȷ���ֲ��޳�����ֵ.
O_C             LC�۲�ֵ��ȥ�۲�ֵ�ļ���ֵ, ��ÿ�����ǵĽ��ջ��Ӳ�.
Epoch           ��Ԫ�۲�����, ̽�⵽�ֲ�󽫸���������Ϊ�ֲ�۲�ֵ,������
SatList         ��O_C���Ӧ, ��ȷ֪�ֲ�۲�ֵ�������ǿ�����

���ز���

��̽�⵽�ֲ�ʱ, ����1, ���򷵻�0.

***************************************************************************/
bool DetectOutliers_rel( double Accuracy, double O_C[], 
	Common11Obs * CurComObs, SATLIST* SatList, double sigma )
{
	int  i, j;
	int  GPSSatNum;

	double GVal[MAXCHANNUM]={0.0};						/*  ��ʱ�洢GPSo_c */
	double GMean;								/* ��ֵ */
	double GStd, GLim;						/* ��׼��, �޲� */
	bool   HasOutlier = false;

	GPSSatNum = 0;

	GLim = Accuracy;

	/* ��GPS��GLONASS���ǵ�O_C���ݷֿ� */

	for( j=0,i=0; i<MAXCHANNUM; i++ )
	{
		if( SatList[i].Status==1 )
		{
			GVal[GPSSatNum] = O_C[j];
			GPSSatNum++;
			j++;
		}
	}

	mbbub( GPSSatNum, GVal );     /* ����, �޳����˵�����׼�� */

	GMean = GVal[GPSSatNum/2];    /* ȡ�������м�����Ϊƽ��ֵ */

	GStd = 0.0;
	for( j=0, i=1; i<GPSSatNum-1; i++ )   /* ȥ���˵�����׼�� */
	{
		if( fabs(GVal[i]-GMean) < 20*GLim )
		{
			GStd = GStd + (GVal[i]-GMean) * (GVal[i]-GMean);
			j++;
		}
	}
	if( j>0 )  GLim = sqrt( GStd/j );
	GLim = __max( GLim, Accuracy );
	
	if( GLim < sigma )    /* ��ֹ��׼����㲻׼ȷ,�����ɾ�� */
	{
		GLim = sigma;
	}

	for( j=0,i=0; i<MAXCHANNUM; i++ )
	{
		if( SatList[i].Status==1 )
		{
			if( fabs(O_C[j] - GMean ) >= 3*GLim)
			{
				// 				printf( "Outlier: %10.2f G%02d %12.3f %12.3f %12.3f\n", 
				// 					Epoch->Time.SecOfWeek, SatList[i].Prn, O_C[j], GMean, O_C[j]-GMean );
				SatList[i].Status = -1;
				if(sigma>1)
					CurComObs->comobs[SatList[i].index].used= -1;
				else
					CurComObs->comobs[SatList[i].index].flag= -1;
				HasOutlier = true;
			}
			j++;
		}
	}
	return HasOutlier;
}
/***************************************************************************
//
// DeleteOutlierFromLC
//
// Ŀ��: ��̽����дֲ�۲�ֵ��, �˺�����ɾ���ֲ����ǵĹ۲����/��Ȩϵ��/�в����ݵ�,
����ֲ����ݲ�����С���˹���

//
// �������:
//
GPSSatNum        GPS������.
Resid            �۲�ֵ-����ֵ
A                �۲�ϵ������
P                �۲�ֵȨ����

***************************************************************************/
void DeleteOutlierFrom_rel( int* GPSSatNum, double Resid[],
	double A[], double P[], SATLIST* SatList )
{
	int i, j, k;
	int satNum=*GPSSatNum;
	for( j=0,i=0; i<satNum; i++ )
	{
		if( SatList[i].Status == 1 )  /* ���� */
		{
			j++;
		}
		else                /* �дֲ� */
		{
			*GPSSatNum = *GPSSatNum - 1;
			satNum=satNum-1;
			for( k=j; k<satNum-1; k++ )
			{
				Resid[k] = Resid[k+1];
				P[k]     = P[k+1];
				A[k*5+0] = A[(k+1)*5+0];
				A[k*5+1] = A[(k+1)*5+1];
				A[k*5+2] = A[(k+1)*5+2];
				A[k*5+3] = A[(k+1)*5+3];
				A[k*5+4] = A[(k+1)*5+4];
			}

		}
	}

}
/***************************************************************************
//
// DeleteOutlierFromObsList
//
// Ŀ��: ��̽����дֲ�۲�ֵ��, �˺�����ɾ���ֲ����ǵĹ۲����/��Ȩϵ��/�в����ݵ�,
����ֲ����ݲ�����С���˹���

//
// �������:
//
GPSSatNum        GPS������.
GLOSatNum        GLONASS������ 
Resid            �۲�ֵ-����ֵ
A                �۲�ϵ������
P                �۲�ֵȨ����
SatList          ��O_C���Ӧ, ��ȷ֪�ֲ�۲�ֵ�������ǿ�����

***************************************************************************/
void DeleteOutlierFromObsList( int* GPSSatNum, double Resid[],
	double A[], double P[], SATLIST* SatList )
{
	int i, j, k;

	for( j=0,i=0; i<MAXCHANNUM; i++ )
	{
		if( SatList[i].Status == 0 ) /* û������, ���� */
		{
			continue;            
		}
		else if( SatList[i].Status == 1 )  /* ���� */
		{
			j++;
		}
		else                /* �дֲ� */
		{
			if( SatList[i].System == GPS )
			{
				*GPSSatNum = *GPSSatNum - 1;
			}
			for( k=j; k<MAXCHANNUM-1; k++ )
			{
				Resid[k] = Resid[k+1];
				P[k]     = P[k+1];
				A[k*4+0] = A[(k+1)*4+0];
				A[k*4+1] = A[(k+1)*4+1];
				A[k*4+2] = A[(k+1)*4+2];
				A[k*4+3] = A[(k+1)*4+3];
			}

		}
	}

}

/***************************************************************************
//
// CheckPostResidual_W
//
// Ŀ��: �ڵ��㶨λ֮��, �����в���м������_W����. ���������ȷ�Ժ�ÿ�����ǵĲв�
�Ƿ����. ����вȷ��Ϊ�ֲ�, ���ʶ.

//
// �������:
//
n                ��Ԫ��������
Residual         ���㶨λ�۲�ֵ�в�
Qvv              �۲�ֵ�в������
Result           ���㶨λ�Ľ��, ̽�⵽�ֲ�����, ����Result��SatList����
***************************************************************************/
void CheckPostResidual_W( int n, double sigma0, double Residual[], double Qvv[], PPRESULT* Result )
{
	int i, j;
	double w;                    /* w�����ͳ���� */
	//double sigma0 = 3.0;         /* �۲�ֵ��ͳ�ƾ��� */

	for( j=0, i=0; i<n; i++ )
	{
		if( Result->SatList[i].Status == 1 )
		{
			w = Residual[j] / sigma0 / sqrt( Qvv[j] );

			if( fabs(w) > 5.0 )
			{
				Result->SatList[i].Status = -2;  /* �ù۲�ֵ���˲��н���ʹ�� */

				printf( "W-Test %5.2f Failed.\n", w );
			}

			j++;

		}
	}

}

/****************************************************************************
CheckPostResidual_t

Ŀ��: t�ֲ�����-��w����ͬ�������ڶ����۲�ֵ
����: int n
����: double sigma0
����: double Residual[]
����: double Qvv[]
����: PPRESULT * Result
****************************************************************************/
void CheckPostResidual_t( int n, double postSigma, double Residual[], double QLL[], PPRESULT* Result )
{
	int i, j;
	double t;                    /* t�����ͳ���� */

	for( j=0, i=0; i<n; i++ )
	{
		if( Result->SatList[i].Status == 1 )
		{
			t = Residual[j] / postSigma / sqrt( QLL[j] );

			if( fabs(t) > 7.0 )					 /*�����ɶȴ���2ʱ��7��֤alpha/2<0.01��5��֤alpha/2<0.02*/
			{
				Result->SatList[i].Status = -2;  /* �ù۲�ֵ���˲��н���ʹ�� */

				printf( "t-Test %5.2f Failed.\n", t );
			}

			j++;

		}
	}

}

/***************************************************************************
//
// EKFilterRTOD
//
// Ŀ��: ��ʵʱ���㶨λ�Ļ����ϣ����ƹ㿨�����˲���ÿ�ŵ������ǵĹ۲����ݽ���
�������¡�

�˲��������̣�

��ʼ������λ�ú��ٶȣ�ת�������Ĺ���ϵ

����ѧ�������Ԥ��

���ܹ���ڲ�

�˲���ʱ�����

���㶨�죬�ع�ϵ�����ϵ��ת������
�˲��������£�ֱ���ڹ���ϵ�и��£�

//
// �������:
//

Mat[9]          �ع�ϵת��������ϵ��ת������
SatMidInfo      ���ǹ��, �ٶȵ��м���������Ϣ, �����˲�����
EpochObs        ��Ԫ�Ĺ۲�����, �Դ�˳��������ǹ��
PPResult        ���㶨λ���(�ع�ϵ)

�������

KFState         �˲�������

����ֵ

���붨λ��������

***************************************************************************/
int EKFilterRTODA( double Mat[9], SATMIDRESULT* SatMidInfo_A, EpochObsData* Epoch_A,
	PPRESULT* PPResult, EKFSTATE* KFState )
{
	int i, k;
	int SatUsedNum[2];

	int  PRValid;
	double PRange, Ion,Phase;

	double Range;         /* ���ջ��뵼������֮��ľ������ֵ */
	double dPos[3];       /* ���ջ������뵼������λ��֮�� */

	double ApriState[ADIMENSION], AECF[3], ARange; /*��ǰ״̬����������״̬������Ϣʹ��*/
	double H[ADIMENSION];  /* �۲ⷽ�����Ի�ϵ������ */
	double O_C;              /* �۲�ֵ��ȥ����ֵ       */

	double O_C0, O_C1;  /* �˲�����ǰ/��Ĺ۲�ֵ-����ֵ */

	PRValid = 0;
	SatUsedNum[0] = 0;
	SatUsedNum[1] = 0;
	if( PPResult[0].IsSuccess == true )
	{
		KFState->StateA[6] = PPResult[0].RcvClkOft[0];
	}
	if( PPResult[0].SatNumUsed < 4 )  /* С��4�����ǲ����� */
	{
		return 0 ;
	}

	CopyArray( ADIMENSION, ApriState, KFState->StateA );

	//A�ǵ����������
	for( i=0; i<Epoch_A->SatNum; i++ )
	{
		for( k=0; k<ADIMENSION; k++ )
		{
			H[k] = 0.0;
		}

		if( PPResult[0].SatList[i].Status == 1 )
		{
			MatrixMultiply( 3, 3, 3, 1, Mat, &KFState->StateA[0], &KFState->StateInECEFA[0] );

			PRValid = GetOneSatPseudoRange( Epoch_A->SatObs[i].System, 
				&Epoch_A->SatObs[i], &PRange, &Ion );
			Range = 0.0;
			for( k=0; k<3; k++ )
			{
				dPos[k] = KFState->StateInECEFA[k] - SatMidInfo_A[i].SatPos[k];
				Range = Range + dPos[k]*dPos[k];
			}
			Range = sqrt( Range );

			H[0] = ( dPos[0]*Mat[0] + dPos[1]*Mat[3] + dPos[2]*Mat[6] )/Range;//ǰ����ջ�λ���Ѿ�ת���ع�ϵ������Ϊʲô��Ҫ����ת������
			H[1] = ( dPos[0]*Mat[1] + dPos[1]*Mat[4] + dPos[2]*Mat[7] )/Range;//�����ǰѵع�ϵת�ع���ϵ����Ϊ�˲�״̬���ڹ���ϵ��
			H[2] = ( dPos[0]*Mat[2] + dPos[1]*Mat[5] + dPos[2]*Mat[8] )/Range;//������Ϣ�Ǳ������ڵع�ϵ�͹���ϵ��ֵ����

			if(PPResult[0].SatList[i].System == GPS )
			{
				H[6] = 1.0;
				O_C = PRange - Range - KFState->StateA[6] + SatMidInfo_A[i].SatClkOft*C_Light;

				if( PRValid == 1 )
				{
					O_C = O_C - SatMidInfo_A[i].IonoCorr;   // 
				}
			}

			//A�Ǿ��Բ�������Ȩ
			if( EKFMeasureUpdateA(O_C, 5, H, KFState ) )//����5�ǹ۲�����R��Ϊɶ��Ϊ5
			{
				SatUsedNum[0]++;				
			}
			else
			{
				printf( "A�� Kalman Filter Failed:%10.3f Prn%3d %10.3f\n", 
					Epoch_A->Time.SecOfWeek, Epoch_A->SatObs[i].Prn, O_C );
			}

		}
	}
	
	KFState->SatNumUsed[0] = SatUsedNum[0];
	KFState->ApriSigma[0] = 0.0;
	KFState->PostSigma[0] = 0.0;

	MatrixMultiply( 3, 3, 3, 1, Mat, &ApriState[0], AECF );   /*��ǰ״̬���� */
	MatrixMultiply( 3, 3, 3, 1, Mat, &KFState->StateA[0], KFState->StateInECEFA );

	for( i=0; i<Epoch_A->SatNum; i++ )
	{
		if( PPResult[0].SatList[i].Status != 1 )
		{
			continue;
		}

		PRValid = GetOneSatPseudoRange( Epoch_A->SatObs[i].System, 
			&Epoch_A->SatObs[i], &PRange, &Ion );

		Range = 0.0;
		ARange = 0.0;
		for( k=0; k<3; k++ )
		{	
			ARange = ARange + pow( AECF[k]-SatMidInfo_A[i].SatPos[k], 2.0 );
			Range  = Range  + pow( KFState->StateInECEFA[k]-SatMidInfo_A[i].SatPos[k], 2.0 );
		}
		Range = sqrt( Range );
		ARange = sqrt( ARange );

		if ( Epoch_A->SatObs[i].System == GPS )
		{
			O_C0  = PRange - ARange - KFState->StateA[6]   /*��ǰ�в�,ʹ������Ӳ� */
			+ SatMidInfo_A[i].SatClkOft*C_Light + SatMidInfo_A[i].Relativity;
			O_C1 = PRange - Range - KFState->StateA[6] /* ���в� */
			+ SatMidInfo_A[i].SatClkOft*C_Light + SatMidInfo_A[i].Relativity;

			if( PRValid == 1 )
			{
				O_C0  = O_C0  - SatMidInfo_A[i].IonoCorr; 
				O_C1  = O_C1  - SatMidInfo_A[i].IonoCorr; 
			}
		}

		KFState->ApriSigma[0] = KFState->ApriSigma[0] + pow( O_C0, 2.0);
		KFState->PostSigma[0] = KFState->PostSigma[0] + pow( O_C1, 2.0 );

		/* ʱ�� Prn SPP�в� Ԥ���в� ���º�в� ˫Ƶ�������� ��Ƶ�������� ���Ǹ߶Ƚ� */

		if( PPResult[0].SatList[i].Status == 1 )
		{
			/*fprintf( FRESIDUAL, " %10.1f %3d %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %2d \n", 
				Epoch->Time.SecOfWeek, Epoch->SatObs[i].Prn, 
				PPResult->Residual[i], O_C0, O_C1, Ion,
				SatMidInfo[i].IonoCorr, SatMidInfo[i].Elevation*Deg,
				PPResult->PDOP,PPResult->SigmaPos, PPResult->SatList[i].Status );*/
		}
	}

	if( KFState->SatNumUsed[0] > 0 )
	{
		KFState->ApriSigma[0] = sqrt( KFState->ApriSigma[0] / SatUsedNum[0] );
		KFState->PostSigma[0] = sqrt( KFState->PostSigma[0] / SatUsedNum[0] );
	}
	else
	{
		KFState->ApriSigma[0] = 999.0;
		KFState->PostSigma[0] = 999.0;
	}

	// 	fprintf( FRESIDUAL, " %10.1f %8.3f %8.3f %8.3f %2d %2d %2d, 111111 \n", 
	// 				Epoch->Time.SecOfWeek, PPResult->SigmaPos, KFState->ApriSigma,
	// 				KFState->PostSigma, Epoch->SatNum, PPResult->GPSSatNum+PPResult->GLOSatNum,
	// 				KFState->SatNum );


	EKFConvergencyCheck(0,KFState);

	//B�ǵ����������

	return SatUsedNum[0];//A��B�ǹ�ͬ������
}
/***************************************************************************
//
// EKFConvergencyCheck
//
// Ŀ��: ��鿨�����˲��Ƿ�ɢ,���������˲��Ƿ�����.

����: 

1. �����˲�����ǰ�Ĳв�ƽ�������˲����º�Ĳв�ƽ���͵������Ƿ�����������;
2. ���㶨λ�Ĺ��(�ڽϺõ�״̬��)���˲����µĹ���Ƚ�, �������Ƿ�һ��.

���µ�������, ����ǰ��sigma, ���º��sigma

//
// �������:
//

PPResult        ���㶨λ���
KFState         �˲����ƽ��

�������

����KFState�е�KFConvergency����.

***************************************************************************/
void EKFConvergencyCheck(int graceType, EKFSTATE* KFState )
{
	double limit;//�жϷ�ɢsigma��ֵ
	double Cov;
	if(graceType>=1){//��Բ�������
		graceType=1;
		limit=10.0;
		Cov=KFState->CovRel[0]+KFState->CovRel[ADIMENSION+1]+KFState->CovRel[2*RELDIMENSION+2];
	}
	else
	{//A�ǲ�������
		limit=20.0;
		Cov=KFState->CovA[0]+KFState->CovA[ADIMENSION+1]+KFState->CovA[2*RELDIMENSION+2];
		
	}
	switch( KFState->KFConvergency[graceType] )
	{
	case 0:
		if( sqrt(Cov)< 5.0 )  /* ��ʾ��������λ����� */
		{
			KFState->KFConvergency[graceType] = 1;
		}
		break;

	case 1:
		if( KFState->SatNumUsed[graceType] > 0 )  /* �����˲������Ժ�, �в���Ȼ�ܴ� */
		{
			if( KFState->PostSigma[graceType] > limit )
			{
				KFState->KFConvergency[graceType] = 2;
				KFState->IsInitial[graceType] = 0;      /* ��Ҫ���³�ʼ�� */
			}

		}
		break;

	default:
		break;
	}

	if( KFState->KFConvergency[graceType] == 2 )
	{
		printf( "Epoch %10.1f Grace%d Kalman filtering diverged!.\n", KFState->Time.SecOfWeek,graceType+1 );
	}
}
