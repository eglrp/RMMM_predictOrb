/****************************************************************************
Ŀ�ģ�    ����ʱ��ṹ�弰���໥ת������,
          �ռ�ֱ������ʹ�������໥ת��
��дʱ�䣺2008.11.22
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#pragma once 
#ifndef _GPS_TIME_H_
#define _GPS_TIME_H_

#include "RTOD_Structs.h"

/* ͨ��ʱ,GPSʱ�ͼ�������֮����໥ת������*/

void CommonTimeToMJDTime( const COMMONTIME* CT, MJDTIME* MJDT);
void MJDTimeToCommonTime( const MJDTIME* MJDT, COMMONTIME* CT );
void GPSTimeToMJDTime( const GPSTIME* GT, MJDTIME* MJDT );
void MJDTimeToGPSTime ( const MJDTIME* MJDT, GPSTIME* GT );
void CommonTimeToGPSTime ( const COMMONTIME* CT, GPSTIME* GT );
void GPSTimeToCommonTime ( const GPSTIME* GT, COMMONTIME* CT );

/* ���GPSʱ���ʽ�Ƿ���ȷ, �������Ƿ����604800��С��0  */
void CheckGPSTime( GPSTIME* GT );

/* �ռ�ֱ������,���������໥ת������ */

void XYZToBLH( const double xyz[], double blh[], double R, double F );
void BLHToXYZ( const double BLH[], double XYZ[], double R, double F );

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
    
    H        ��ת����

***************************************************************************/

void BLHToNEUMatrix( double BLH[3], double H[9]);


/* PZ90 ��WGS84����ת�� */

void CoorTranFromPZ90ToWGS84( double PZ90[3], double WGS84[3] );
void VeloTranFromPZ90ToWGS84( double PZ90[3], double WGS84[3] );

/***************************************************************************
//
// PhaseCentToMassCent
//
// Ŀ��: ��GNSS���ջ�������λ��������ת������������

//
// �������:
//
   Flag      1Ϊ��λ����ת������������, 0Ϊ��������ת����λ����
   Bias      �ǹ�ϵ�е�GNSS���ջ�������λ�������������ĵ�ƫ�����[m]

   �������
    
   State[6]  ����Ϊ���ؽ��ջ���λ�ú��ٶ�, ���Ϊ�����������λ��

***************************************************************************/

void PhaseCentToMassCent( bool Flag, const double Bias[3], double State[6] );


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
   dXYZ      �ռ�ֱ������ϵ�µ�ָ�����
   dRTN      RTN����ϵ�µ��������

***************************************************************************/

void XYZToRTN( double State[6], double dXYZ[3], double dRTN[3] );
//���ɾ���
void MatrixXYZ2RTN(double State[6],double transMat[9],int flag);
/*��satods�ṹ��GPSTIMEת��rtklibͨ��ʱep,Ϊ������rtklib��ľ���������λ���ĸ���ģ��*/
void GPSTimeToep ( const GPSTIME* GT, double ep[6]);
void SecTimeToCT(double sec,COMMONTIME * CT);
#endif