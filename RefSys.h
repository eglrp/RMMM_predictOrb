/****************************************************************************
Ŀ�ģ�    ���嶨��ϵͳ��Ҫ��ʱ��ϵͳ������ϵͳ�����໥ת������,
          ��������ʱ/����ʱ/GPSʱ/����ѧʱ��ʱ��ϵͳ;J2000����ϵ
          /���ĵع�ϵ������ϵͳ��ת��

��дʱ�䣺2008.11.22
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#pragma once 
#ifndef _REFSYS_H_
#define _REFSYS_H_

#include "RTOD_Structs.h"


double TT_UTC( void );
void   SetGPST_UTC( double val );
double GetGPST_UTC();
int    InitEOPPara( const MJDTIME* CurrTime );
void   InterposeEOP(const MJDTIME* time, EOPPARA* CurrEop);

/****************************************************************************
CheckLeapSecInEOP

  Ŀ�ģ����EOP[3]�е�LeapSec�Ƿ���������������������е�����
  
****************************************************************************/

void CheckLeapSecInEOP();


/***************************************************************************
//
// MeanObliquity
//
// Purpose:
//
//   Computes the mean obliquity of the ecliptic
//
// Input/Output:
//
//   Mjd_TT    Modified Julian Date (Terrestrial Time)
//   <return>  Mean obliquity of the ecliptic,[Rad]
//
***************************************************************************/
double MeanObliquity ( const MJDTIME* Mjd_TT); 


/***************************************************************************
//
// EclMatrix
//
// Purpose:
//
//   Transformation of to ecliptical coordinates
// equatorial
// Input/Output:
//
//   Mjd_TT    Modified Julian Date (Terrestrial Time)
//   <return>  Transformation matrix
//
***************************************************************************/

void EclMatrix ( const MJDTIME* Mjd_TT, double Mat[]);  


/***************************************************************************
//
// PrecMatrix
//
// Purpose:
//
//   Precession transformation of equatorial coordinates
//
// Input/Output:
//
//   Mjd_1     Epoch given (Modified Julian Date TT)
//   MjD_2     Epoch to precess to (Modified Julian Date TT)
//   <return>  Precession transformation matrix
//
***************************************************************************/

void PrecMatrix (double Mjd_1, const MJDTIME* Mjd_2, double Mat[]);

//------------------------------------------------------------------------------
//
// NutAngles 
//
// Purpose:
//
//   Nutation in longitude and obliquity
//
// Input/Output:
//
//   Mjd_TT    Modified Julian Date (Terrestrial Time)
//   <return>  Nutation matrix
//
//------------------------------------------------------------------------------

void NutAngles (const MJDTIME* Mjd_TT, double* dpsi, double* deps);

/***************************************************************************
//
// NutMatrix 
//
// Purpose:
//
//   Transformation from mean to true equator and equinox
//
// Input/Output:
//
//   Mjd_TT    Modified Julian Date (Terrestrial Time)
//   <return>  Nutation matrix
//
***************************************************************************/

void NutMatrix ( const MJDTIME* Mjd_TT, double Mat[]);


/***************************************************************************
//
// NutMatrixSimple 
//
// Purpose:
//
//   Transformation from mean to true equator and equinox (low precision)
//
// Input/Output:
//
//   Mjd_TT    Modified Julian Date (Terrestrial Time)
//   <return>  Nutation matrix
//
***************************************************************************/
void NutMatrixSimple ( const MJDTIME* Mjd_TT, double Mat[]);

/***************************************************************************
//
// EqnEquinox 
//
// Purpose:
//
//   Computation of the equation of the equinoxes
//
// Input/Output:
//
//   Mjd_TT    Modified Julian Date (Terrestrial Time)
//   <return>  Equation of the equinoxes
//
// Notes:
//
//   The equation of the equinoxes dpsi*cos(eps) is the right ascension of the 
//   mean equinox referred to the true equator and equinox and is equal to the 
//   difference between apparent and mean sidereal time.
//
***************************************************************************/

double EqnEquinox (const MJDTIME* Mjd_TT);


/***************************************************************************
//
// GMST
//
// Purpose:
//
//   Greenwich Mean Sidereal Time
//
// Input/Output:
//
//   Mjd_UT1   Modified Julian Date UT1
//   <return>  GMST in [rad]
//
***************************************************************************/

double GMST ( const MJDTIME* Mjd_UT1 );


/***************************************************************************
//
// GAST
//
// Purpose:
//
//   Greenwich Apparent Sidereal Time
//
// Input/Output:
//
//   Mjd_UT1   Modified Julian Date UT1
//   <return>  GMST in [rad]
//
***************************************************************************/

double GAST ( const MJDTIME* Mjd_UT1);


/***************************************************************************
//
// GHAMatrix
//
// Purpose:
//
//   Transformation from true equator and equinox to Earth equator and 
//   Greenwich meridian system 
//
// Input/Output:
//
//   Mjd_UT1   Modified Julian Date UT1
//   <return>  Greenwich Hour Angle matrix
//
***************************************************************************/

void GHAMatrix ( const MJDTIME* Mjd_UT1, double Mat[] );


/***************************************************************************
//
// PoleMatrix
//
// Purpose:
//
//   Transformation from pseudo Earth-fixed to Earth-fixed coordinates
//   for a given date
//
// Input/Output:
//
//   x, y      Pole Motion para[Rad]
//   <return>  Pole matrix
//
***************************************************************************/

void PoleMatrix ( double x, double y, double Mat[]);

/***************************************************************************
//
// ICRF_ITRF_MJD
//
// Ŀ��: ����ϵ��ع�ϵ�������໥ת��,���а���λ�ú��ٶȷ���, ����ʱ��
         ΪMJD��ʾ����TTʱ.
//
// ����:
//
//   Mjd1     Epoch given (Modified Julian Date TT)
//   Mjd2     Epoch to precess to (Modified Julian Date TT)
//   flag     1: ������ϵ���ع�ϵ 
              0: �ӵع�ϵ������ϵ
//   ICRF     ����ϵ���꣬���а���λ�ú��ٶ�
//   ITRF     �ع�ϵ����
//
***************************************************************************/
void ICRF_ITRF_MJD( const double Mjd1, const MJDTIME* Mjd2,
               int flag, double ICRF[6], double ITRF[6] );
			   
/***************************************************************************
//
// ICRF_ITRF_GPST
//
// Ŀ��: ����ϵ��ع�ϵ�������໥ת��,���а���λ�ú��ٶȷ���, ����ʱ��
         ΪMJD��ʾ����TTʱ.
//
// ����:
//
//   Mjd1     Epoch given (Modified Julian Date TT)
//   CT       GPSʱ��
//   flag     1: ������ϵ���ع�ϵ 
              0: �ӵع�ϵ������ϵ
//   ICRF     ����ϵ���꣬���а���λ�ú��ٶ�
//   ITRF     �ع�ϵ����
//
***************************************************************************/
void ICRF_ITRF_GPST( const double Mjd1, const GPSTIME* GT,
               int flag, double ICRF[6], double ITRF[6] );


/***************************************************************************
//
// ICRF_ITRF_Matrix
//
// Ŀ��: ����ϵ��ع�ϵ�������໥ת������,���а���λ�ú��ٶȷ���.
         ʱ����GPSʱ.
//
// ����:
//
//   Mjd1     Epoch given (Modified Julian Date TT, MJD_J2000)
//   CT       GPSʱ��
//   flag     1: ������ϵ���ع�ϵ 
              0: �ӵع�ϵ������ϵ
//   Mat      ת������
//
***************************************************************************/
void ICRF_ITRF_Matrix( const double Mjd1, const GPSTIME* CT,
               int flag, double Mat[9] );



#endif