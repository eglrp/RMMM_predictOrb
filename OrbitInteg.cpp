/****************************************************************************
Ŀ�ģ�    ����ѧ�������,ʹ��RK4, RKF4,�Լ������ֵ�Ⱥ���

��дʱ�䣺2008.11.26
�汾:     V1.1
��Ȩ��    �人��ѧ

2009.3.11  �޸���Hermite5�����е�һ������
****************************************************************************/

#include <math.h>
#include "RTOD_Const.h"
#include "CommonFuncs.h"
#include "RefSys.h"
#include "OrbitInteg.h"


/***************************************************************************
//
// RK4Step
//
// Purpose:
//
//   ����-����4�׵�������������
//
// Input/Output:
//
//   Mjd_GPS    ��ʼʱ��MJDʱ��(GPS Time),���ֺ�Ϊmjd_gps+step
//   Step       ����ʱ����[s]
//   Y0         ���ֳ�ֵ,Mjd_GPSʱ�̵�����λ�ú��ٶ�[m, m/s],
//                 ���ֺ�Ϊ��һʱ�̵�����λ�ú��ٶ�
//   Para       ����ѧģ�Ͳ���
//
***************************************************************************/
/*
void RK4Step( MJDTIME* Mjd_GPS, double step, double Y0[6], DYNMODELPARA* Para )
{
	int i;
	double Pos[3];
	double Vel[4][3], Acc[4][3];
	double Prec[9], Nut[9], GH[9], Pole[9], Mat[9], E[9], T[9];
	MJDTIME Mjd_TT, Mjd_UTC, Mjd_UT1;
	EOPPARA CurrEop;
	double h = step / SECPERDAY;

	Mjd_TT.Days = Mjd_GPS->Days;
	Mjd_TT.FracDay = Mjd_GPS->FracDay + (TT_TAI-GPST_TAI)/SECPERDAY;
	Mjd_UTC.Days = Mjd_TT.Days;
	Mjd_UTC.FracDay = Mjd_TT.FracDay - TT_UTC() / SECPERDAY;
	InterposeEOP( &Mjd_UTC, &CurrEop );

	Mjd_UT1.Days = Mjd_UTC.Days;
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	PrecMatrix( MJD_J2000, &Mjd_TT, Prec );
	NutMatrixSimple( &Mjd_TT, Nut );
	MatrixMultiply( 3, 3, 3, 3, Nut, Prec, T );

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );

	for( i=0; i<3; i++ )
	{
		Pos[i] = Y0[i];
		Vel[0][i] = Y0[3+i];
	}

	AccelMain(TODO, &Mjd_TT, Pos, Vel[0], E, T, Para , Acc[0] );

	//   compute right function at the second time 

	Mjd_TT.FracDay += h/2.0;  
	Mjd_UTC.FracDay += h/2.0;
	InterposeEOP( &Mjd_UTC, &CurrEop );
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );      //��ʱ����û�п��������¶��仯

	for( i=0; i<3; i++ )
	{
		Pos[i]    = Y0[i]   + Vel[0][i] * step / 2.0;
		Vel[1][i] = Y0[3+i] + Acc[0][i] * step / 2.0;
	}
	AccelMain(TODO, &Mjd_TT, Pos, Vel[1], E, T, Para , Acc[1] );

	//   compute right function at the third time 

	for( i=0; i<3; i++ )
	{
		Pos[i]    = Y0[i]   + Vel[1][i] * step / 2.0;
		Vel[2][i] = Y0[3+i] + Acc[1][i] * step / 2.0;
	}
	AccelMain(TODO, &Mjd_TT, Pos, Vel[2], E, T, Para , Acc[2] );

	//   compute right function at the fourth time 

	Mjd_TT.FracDay  += h/2.0;  
	Mjd_UTC.FracDay += h/2.0;
	InterposeEOP( &Mjd_UTC, &CurrEop );
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );      //��ʱ����û�п��������¶��仯

	for( i=0; i<3; i++ )
	{
		Pos[i]    = Y0[i]   + Vel[2][i] * step;
		Vel[3][i] = Y0[3+i] + Acc[2][i] * step;
	}
	AccelMain(TODO, &Mjd_TT, Pos, Vel[3], E, T, Para , Acc[3] ); 

	Mjd_GPS->FracDay += h;
	if( Mjd_GPS->FracDay >= 1.0 )    // ������ 
	{
		Mjd_GPS->FracDay -= 1.0;
		Mjd_GPS->Days    += 1;
	}

	for( i=0; i<3; i++ )
	{
		Y0[i]   += ( Vel[0][i] + 2.0*Vel[1][i] + 2.0*Vel[2][i] + Vel[3][i] ) * step / 6.0;
		Y0[i+3] += ( Acc[0][i] + 2.0*Acc[1][i] + 2.0*Acc[2][i] + Acc[3][i] ) * step / 6.0;
	}

}
*/

/***************************************************************************
//
// RKF4Step
//
// Purpose:
//
//   ����-����-Felberg4�׵�������������, ֻ��������λ�ú��ٶ�
//
// Input/Output:
//
//   Mjd_GPS    ��ʼʱ��MJDʱ��(GPS Time),���ֺ�Ϊmjd_gps+step
//   Step       ����ʱ����[s]
//   Y0         ���ֳ�ֵ,Mjd_GPSʱ�̵�����λ�ú��ٶ�[m, m/s],
//                 ���ֺ�Ϊ��һʱ�̵�����λ�ú��ٶ�
//   Para       ����ѧģ�Ͳ���
//
***************************************************************************/
/*
void RKF4Step( MJDTIME* Mjd_GPS, double step, double Y0[6], DYNMODELPARA* Para )
{
	int i;
	double Pos[3];
	double Vel[5][3], Acc[5][3];
	double Prec[9], Nut[9], GH[9], Pole[9], Mat[9], E[9], T[9];
	MJDTIME Mjd_TT, Mjd_UTC, Mjd_UT1;
	EOPPARA CurrEop;
	double h = step / SECPERDAY;

	Mjd_TT.Days = Mjd_GPS->Days;
	Mjd_TT.FracDay = Mjd_GPS->FracDay + (TT_TAI-GPST_TAI)/SECPERDAY;
	Mjd_UTC.Days = Mjd_TT.Days;
	Mjd_UTC.FracDay = Mjd_TT.FracDay - TT_UTC() / SECPERDAY;
	InterposeEOP( &Mjd_UTC, &CurrEop );

	Mjd_UT1.Days = Mjd_UTC.Days;
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	PrecMatrix( MJD_J2000, &Mjd_TT, Prec );
	NutMatrixSimple( &Mjd_TT, Nut );
	MatrixMultiply( 3, 3, 3, 3, Nut, Prec, T );

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );

	for( i=0; i<3; i++ )
	{
		Pos[i] = Y0[i];
		Vel[0][i] = Y0[3+i];
	}

	AccelMain(TODO, &Mjd_TT, Pos, Vel[0], E, T, Para , Acc[0] );

	//   compute right function at the second time 

	Mjd_TT.FracDay += h/4.0;  
	Mjd_UTC.FracDay += h/4.0;
	InterposeEOP( &Mjd_UTC, &CurrEop );
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );      //��ʱ����û�п��������¶��仯

	for( i=0; i<3; i++ )
	{
		Pos[i]    = Y0[i]   + Vel[0][i] * step / 4.0;
		Vel[1][i] = Y0[3+i] + Acc[0][i] * step / 4.0;
	}
	AccelMain(TODO, &Mjd_TT, Pos, Vel[1], E, T, Para , Acc[1] );

	//  compute right function at the third time 

	Mjd_TT.FracDay += h/8.0;  
	Mjd_UTC.FracDay += h/8.0;
	InterposeEOP( &Mjd_UTC, &CurrEop );
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );      // ��ʱ����û�п��������¶��仯

	for( i=0; i<3; i++ )
	{
		Pos[i]    = Y0[i]   + (Vel[0][i]*3/32 + Vel[1][i]*9/32.0) * step;
		Vel[2][i] = Y0[3+i] + (Acc[0][i]*3/32 + Acc[1][i]*9/32.0) * step;
	}
	AccelMain(TODO, &Mjd_TT, Pos, Vel[2], E, T, Para , Acc[2] );

	//   compute right function at the fourth time 

	Mjd_TT.FracDay  += h*57/104.0;  
	Mjd_UTC.FracDay += h*57/104.0;
	InterposeEOP( &Mjd_UTC, &CurrEop );
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );      // ��ʱ����û�п��������¶��仯

	for( i=0; i<3; i++ )
	{
		Pos[i]    = Y0[i]   + (Vel[0][i]*1932/2197.0 - Vel[1][i]*7200/2197.0
			+ Vel[2][i]*7296/2197.0 )* step;
		Vel[3][i] = Y0[3+i] + (Acc[0][i]*1932/2197.0 - Acc[1][i]*7200/2197.0
			+ Acc[2][i]*7296/2197.0 )* step;
	}
	AccelMain(TODO, &Mjd_TT, Pos, Vel[3], E, T, Para , Acc[3] ); 

	//   compute right function at the fourth time 

	Mjd_TT.FracDay  += h/13.0;  
	Mjd_UTC.FracDay += h/13.0;
	InterposeEOP( &Mjd_UTC, &CurrEop );
	Mjd_UT1.FracDay = Mjd_UTC.FracDay + CurrEop.dUT1 / SECPERDAY;

	GHAMatrix( &Mjd_UT1, GH );
	PoleMatrix( CurrEop.x, CurrEop.y, Pole );
	MatrixMultiply( 3, 3, 3, 3, Pole, GH, Mat );
	MatrixMultiply( 3, 3, 3, 3, Mat, T, E );      // ��ʱ����û�п��������¶��仯

	for( i=0; i<3; i++ )
	{
		Pos[i]    = Y0[i]   + (Vel[0][i]*439/216.0 - Vel[1][i]*8.0
			+ Vel[2][i]*3680/513.0 - Vel[3][i]*845/4104.0 )* step;
		Vel[4][i] = Y0[3+i] + (Acc[0][i]*439/216.0 - Acc[1][i]*8.0
			+ Acc[2][i]*3680/513.0 - Acc[3][i]*845/4104.0 )* step;
	}
	AccelMain(TODO, &Mjd_TT, Pos, Vel[4], E, T, Para , Acc[4] ); 

	//  ���ֽ��  

	Mjd_GPS->FracDay += h;
	if( Mjd_GPS->FracDay >= 1.0 )    // ������ 
	{
		Mjd_GPS->FracDay -= 1.0;
		Mjd_GPS->Days    += 1;
	}

	for( i=0; i<3; i++ )
	{
		Y0[i]   += ( Vel[0][i]*25/216 + Vel[2][i]*1408/2565
			+ Vel[3][i]*2197/4104 - Vel[4][i]/5 ) * step;
		Y0[i+3] += ( Acc[0][i]*25/216 + Acc[2][i]*1408/2565
			+ Acc[3][i]*2197/4104 - Acc[4][i]/5 ) * step;
	}
}

*/
/***************************************************************************
//
// OrbitIntegToGivenTime
//
// Purpose:
//
//   ʹ��RKF4�������������������й�����֣�Ԥ������ʱ�����ǵĹ��
//
// Input/Output:
//
//   Mjd_GPS             ��ʼʱ������MJDʱ���ʾ��GPSʱ
//   Mjd_GivenTime       ��ҪԤ������Ĺ۲�ʱ�̣���Mjd_GPS��������ͬ
//   Y0                  ���ֳ�ֵ,Mjd_GPSʱ�̵�����λ�ú��ٶ�[m, m/s],
//                              ���ֺ�Ϊ��һʱ�̵�����λ�ú��ٶ�
//   Para                ����ѧģ�Ͳ���
//
***************************************************************************/
/*
void OrbitIntegToGivenTime( MJDTIME* Mjd_GPS, MJDTIME* Mjd_GivenTime,
	double Y0[6], DYNMODELPARA* Para )
{
	short   sign;
	double  Step;              // ���ֲ���[s] 
	double  dT;                //��һʱ���뵱ǰʱ���ʱ������s�� 

	dT = ( Mjd_GivenTime->Days - Mjd_GPS->Days 
		+ Mjd_GivenTime->FracDay - Mjd_GPS->FracDay ) * SECPERDAY;

	if( dT >= 0.0 )
	{
		sign = 1;
	}
	else
	{
		sign = -1;
	}

	do{
		if( fabs(dT) > 30.0 )  //���ֲ���Ϊ30s 
		{
			Step = 30.0 * sign;    //  ���û��ֲ��� 
		}
		else
		{
			Step = dT;
		}

		RKF4Step( Mjd_GPS, Step, Y0, Para );

		dT = dT - Step;

	}while( fabs(dT) > 1E-8 );

}
*/
/***************************************************************************
//
// RKF4OrbitSTM
//
// Purpose:
//
//   ����-����-Felberg4�׵�������������, ���й����״̬ת�ƾ������
//
// Input/Output:
//
//   Mjd_GPS    ��ʼʱ��MJDʱ��(GPS Time),���ֺ�Ϊmjd_gps+step
//   Step       ����ʱ����[s]
//   Y0         �����ת�ƾ����ֵ,Mjd_GPSʱ�̵�����λ�ú��ٶ�[m, m/s],
//                 ���ֺ�Ϊ��һʱ�̵�����λ�ú��ٶ�
//   Y0         ת�ƾ���һ��Ϊ�Խǵ�λ�󣬵���ǰ�滹��6����Ϊ0�Ĺ��Ʋ�����
//   IntState   ��Hermit5�����ڲ�����Ҫ��״̬�������ڴ˴���ʼ��[λ��/�ٶ�/���ٶ�]
//   Para       ����ѧģ�Ͳ���
//
***************************************************************************/
void RKF4OrbitSTM( int graceType, MJDTIME* Mjd_GPS, double step, double Y0[54], 
	SCState Stat[2], DYNMODELPARA* Para )
{
	int i;
	double Y[54], dY[5][54];
	double h = step / SECPERDAY;

	for( i=0; i<54; i++ )
	{
		Y[i] = Y0[i];
	}

	VarEquation(graceType, Mjd_GPS, Y, dY[0] , Para );     /* first time */

	/* ��ʼ�������ڲ�״̬���� Stat[0] */

	for( i=0; i<3; i++ )
	{
		Stat[0].Pos[i]   = Y0[i];
		Stat[0].Pos[i+3] = *(Y0+3+i);
		Stat[0].Acc[i]   = *(dY[0]+3+i);
	}
	/**********************************/

	Mjd_GPS->FracDay += h/4.0;
	for( i=0; i<54; i++ )
	{
		Y[i] = Y0[i] + dY[0][i] * step / 4.0;
	}
	VarEquation(graceType, Mjd_GPS, Y, dY[1] , Para );     /* 2nd time */


	Mjd_GPS->FracDay += h/8.0;
	for( i=0; i<54; i++ )
	{
		Y[i] = Y0[i] + (dY[0][i]*3/32 + dY[1][i]*9/32.0) * step;
	}
	VarEquation(graceType, Mjd_GPS, Y, dY[2] , Para );     /* 3rd time */


	Mjd_GPS->FracDay += h*57.0/104.0;
	for( i=0; i<54; i++ )
	{
		Y[i] = Y0[i] + (dY[0][i]*1932/2197.0 - dY[1][i]*7200/2197.0
			+dY[2][i]*7296/2197.0) * step;
	}
	VarEquation(graceType, Mjd_GPS, Y, dY[3] , Para );     /* 4th time */


	Mjd_GPS->FracDay += h/13.0;
	for( i=0; i<54; i++ )
	{
		Y[i] = Y0[i] + (dY[0][i]*439/216.0 - dY[1][i]*8.0
			+ dY[2][i]*3680/513.0 - dY[3][i]*845/4104.0) * step;
	}
	VarEquation(graceType, Mjd_GPS, Y, dY[4] , Para );     /* 5th time */
	


	/*  ���ֽ��  */

	if( Mjd_GPS->FracDay >= 1.0 )    /*  ������ */
	{
		Mjd_GPS->FracDay -= 1.0;
		Mjd_GPS->Days    += 1;
	}

	for( i=0; i<54; i++ )
	{
		Y0[i]   += ( dY[0][i]*25/216 + dY[2][i]*1408/2565
			+ dY[3][i]*2197/4104 - dY[4][i]/5 ) * step;//
	}

	/* ��ʼ���յ���ڲ�״̬���� Stat[1] */

	for( i=0; i<3; i++ )
	{
		Stat[1].Pos[i]   = Y0[i];
		Stat[1].Pos[i+3] = *(Y0+3+i);
		Stat[1].Acc[i]   = dY[0][i+3]*25/216 + dY[2][i+3]*1408/2565
			+ dY[3][i+3]*2197/4104 - dY[4][i+3]/5 ;//���ﻹδ�����ڲ壬ֻ�Ǽ����ڲ�t2ʱ�̵�״̬������û��
	}
	/**********************************/

}


/***************************************************************************
//
// Hermite5
//
// Purpose:
//
//   Hermite 5�׶���ʽ�ڲ����ǵĹ��
//
// Input/Output:
//
//   S0         ��ʼʱ�����ǵ�״̬����[λ��/�ٶ�/���ٶ�]
//   S1         ��ֹʱ�����ǵ�״̬����
//   CurrState  �ڲ�ʱ�̵�����״̬
****************************************************************************/

void Hermite5( const SCState* S0, const SCState* S1, SCState* CurrState )
{
	int i;
	double delta, step;   /* delta is coefficent, step is time space */
	double dr[6], dv[6];  /* dr dv is coefficent for pos and vel interpolation */

	step = (S1->Mjd_GPS.Week-S0->Mjd_GPS.Week)*SECPERWEEK +
		S1->Mjd_GPS.SecOfWeek - S0->Mjd_GPS.SecOfWeek;

	/*      0<delta<1   */
	delta = ((CurrState->Mjd_GPS.Week-S0->Mjd_GPS.Week)*SECPERWEEK +
		CurrState->Mjd_GPS.SecOfWeek-S0->Mjd_GPS.SecOfWeek) / step;

	dr[0] = 1.0 + ( -10 + 15*delta - 6*delta*delta ) * pow(delta, 3.0 );
	dr[1] = delta + ( -6 + 8*delta - 3*delta*delta ) * pow(delta, 3.0 );
	dr[2] = (1 - 3*delta + 3*delta*delta - pow(delta,3.0)) * pow(delta, 2.0)/2.0;
	dr[3] = 1.0 - dr[0];
	dr[4] = ( -4.0 + 7*delta -3*delta*delta ) * pow(delta, 3.0);
	dr[5] = ( 1.0 -2*delta + delta*delta ) *pow(delta,3.0)/2.0;

	dv[0] = 30.0 * ( -1 + 2*delta - delta*delta ) * pow(delta, 2.0 );
	dv[1] = 1.0 + ( -18 + 32*delta - 15*delta*delta ) * pow(delta, 2.0 );
	dv[2] = delta + (-9 + 12*delta -5*delta*delta ) * pow(delta, 2.0) / 2.0;
	dv[3] = - dv[0];
	dv[4] = ( -12.0 + 28*delta -15*delta*delta ) * pow(delta, 2.0);
	dv[5] = ( 3.0 -8*delta + 5*delta*delta ) *pow(delta, 2.0) / 2.0;

	for( i=0; i<3; i++ )
	{
		CurrState->Pos[i] = dr[0]*S0->Pos[i] + dr[1]*step*S0->Pos[i+3] + dr[2]*step*step*S0->Acc[i]
		+ dr[3]*S1->Pos[i] + dr[4]*step*S1->Pos[i+3] + dr[5]*step*step*S1->Acc[i];
		CurrState->Pos[i+3] = dv[0]*S0->Pos[i]/step + dv[1]*S0->Pos[i+3] + dv[2]*step*S0->Acc[i]
		+ dv[3]*S1->Pos[i]/step + dv[4]*S1->Pos[i+3] + dv[5]*step*S1->Acc[i];
	}

}


/***************************************************************************
//
// InitStateTranMatrix
//
// Purpose:
//
//   ��ʼ��������������е�״̬ת�ƾ���, ��ֵΪ��λ��I
//
// Input/Output:
//
//   row    ״̬ת�ƾ��������
col    ״̬ת�ƾ��������
STM    ״̬ת�ƾ�������

****************************************************************************/

void InitStateTranMatrix( int row, int col, double STM[] )
{
	int j, k;

	for( j=0; j<row; j++ )
	{
		for( k=0; k<col; k++ )
		{
			if(j==k)  
			{
				*(STM+j*8+k) = 1.0;
			}
			else
			{
				*(STM+j*8+k) = 0.0;
			}
		}
	}

}

