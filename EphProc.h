/****************************************************************************
Ŀ�ģ�    ��Rinex��ʽ�������ж�ȡGPS��GLONASS��������, ���㵼������
          �Ĺ�����Ӳ�

��дʱ�䣺2008.12.4
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#pragma once 
#ifndef _EPH_READ_PROCESS_H_
#define _EPH_READ_PROCESS_H_

#include <stdio.h>
#include "GPSTime.h"
#include "RTOD_Structs.h"


/***************************************************************************
//
// ReadGPSEph
//
// Ŀ��: ��ȡ�۲�ʱ����ӽ���ĳ��GPS��������.
//
// �������:
//
//  Prn   GPS���Ǻ�
//  Time  �۲�ʱ��
//  IonPara  ������ӳٸ�������
//
//  �������:
//  GPSEphRecord   GPS���������ṹ��, �洢��ӽ�ʱ�̵���������
//
//  ����ֵ:   
// 
     ����ļ���ȡ������, ����0, ���򷵻�1
***************************************************************************/

bool ReadGPSEph( const short Prn, const GPSTIME* Time,
               IONOPARA* IonPara, GPSEPHREC* GPSEphRecord );

/***************************************************************************
//
// CheckGPSEph
//
// Ŀ��: ���������¼�����������Ƿ�Ϊ��ǰ�۲�ʱ�̵�GPS��������, 
         �����������, �����������ļ���ȡ
//
// �������:
//
//  prn   GPS���Ǻ�
//  Time  �۲�ʱ��

  //  �������:

//  IonPara  ������ӳٸ�������
//  GPSEphRecord   GPS��������, �洢��ӽ�ʱ�̵���������
//
//  ����ֵ:   

// ��������ǵ�ǰʱ��, ����true, ���򷵻�false
***************************************************************************/

bool CheckGPSEph( const short prn, const GPSTIME* Time,
               IONOPARA* IonPara, GPSEPHREC* GPSEphRecord );

/***************************************************************************
//
// ComputeGPSOrbitAndClockFullInfo
//
// Ŀ��: ������������, ��������λ��/�ٶ�/�Ӳ�/����ȫ����Ϣ
//
// �������:
//
//  Prn   �������GPS���Ǻ�
//  Time  �źŷ���ʱ�̵�GPSʱ��
//  Eph   GPS��������
//  IonPara  ������ӳٸ�������
//
//  �������:
//  Pos, Vel �źŷ���ʱ�̵�GPS����λ�ú��ٶ�[m, m/s]
//  clkoft, ClkSft  �źŷ���ʱ�̵�GPS�����Ӳ������[s, s/s]
//
����ֵ

  ����ɹ�����true, ���򷵻�false.
***************************************************************************/
bool ComputeGPSOrbitAndClockFullInfo( const int Prn, const GPSTIME* t,
                                     GPSEPHREC* Eph, IONOPARA* IonPara, 
                                     double Pos[3], double Vel[3],
                                     double* ClkOft, double* ClkSft );

/***************************************************************************
//
// ComputeGPSOrbitAndClockPartInfo
//
// Ŀ��: ������������, ��������λ��/�Ӳ���Ϣ
//
// �������:
//
//  Prn   �������GPS���Ǻ�
//  Time  �źŷ���ʱ�̵�GPSʱ��
//  Eph   GPS��������
//  IonPara  ������ӳٸ�������
//
//  �������:
//  Pos     �źŷ���ʱ�̵�GPS����λ�ú��ٶ�[m ]
//  clkoft  �źŷ���ʱ�̵�GPS�����Ӳ������[m ]
//
   ����ֵ

  ����ɹ�����true, ���򷵻�false.
  ***************************************************************************/

bool ComputeGPSOrbitAndClockPartInfo( const int PRN, const GPSTIME* t,
                                     GPSEPHREC* Eph, IONOPARA* IonPara, 
                                     double Pos[3], double* ClkOft );

/***************************************************************************
//
// ReadGLONASSEph
//
// Ŀ��: ��ȡ�۲�ʱ����ӽ���GLONASS��������, ���浽������¼����.
//
// �������:
//
//  slot  GLONASS���Ǻ�
//  Time  �۲�ʱ��
//  GloTmCorr GLONASSʱ����UTC(SU)ϵͳ��ĸ���ֵ
//  
//  �������:
//  GLOEphRecord   GLONASS��������, �洢��ӽ�ʱ�̵���������
//
//  ����ֵ:   
// ����ļ���ȡ������, ����0, ���򷵻�1
***************************************************************************/

bool ReadGLONASSEph( const short Slot, const GPSTIME* Time,
                    GLOTIMECORR* GloTmCorr, GLONASSEPHREC* GLOEphRecord );

/***************************************************************************
//
// CheckGLOEph
//
// Ŀ��: ���������¼�����Ƿ�Ϊ��ǰ�۲�ʱ�̵�GLONASS��������, �����������,
         �����������ļ���ȡ.
//
// �������:
//
//  slot  GLONASS���Ǻ�
//  Time  �۲�ʱ��(UTC)
//
//  �������:

  //  GloTmCorr      GLONASSʱ����UTC(SU)ϵͳ��ĸ���ֵ
  //  GLOEphRecord   GLONASS��������, �洢��ӽ�ʱ�̵���������
  //
//  ����ֵ:   
// ����ļ���ȡ������, ����0, ���򷵻�1
***************************************************************************/

bool CheckGLOEph( int Slot, const GPSTIME* Time, GLOTIMECORR* GloTmCorr, 
                 GLONASSEPHREC* GLOEphRecord );

/***************************************************************************
//
// ComputeRightFunc
//
// Ŀ��: GLONASS���ǹ���Ļ����Һ���, ���ڼ������Ǽ��ٶȺ��ٶ�
//
// �������:
//
//  Pos   ��ǰʱ�̵�����λ��
//  Vel   ��ǰʱ�̵������ٶ�
//  Acc   ��ǰʱ�̵����Ǽ��ٶ�

   �������:
   dY[6]  ��һʱ�̵������ٶȺͼ��ٶ�

***************************************************************************/
void ComputeRightFunc( double Pos[3], double Vel[3], double Acc[3],
                      double dY[6] );


/***************************************************************************
//
// ComputeGLONASSSOrbit
//
// Ŀ��: ������������, ��������λ��/�ٶ�
//
// �������:
//
//  Slot  �������GLONASS���Ǻ�
//  Time  �źŷ���ʱ�̵�GLONASSϵͳʱ��
//  Eph   Slot���ǵ�GLONASS��������
//  GloTmCorr GLONASSʱ����UTC(SU)ϵͳ��ĸ���ֵ
//  �������:
//  Pos, Vel �źŷ���ʱ�̵�GPS����λ�ú��ٶ�[m, m/s]
//
   ����ֵ

  ����ɹ�����true, ���򷵻�false.
***************************************************************************/
bool ComputeGLONASSSOrbit( const int Slot, const GPSTIME* t,
                             GLONASSEPHREC* Eph, GLOTIMECORR* GloTmCorr,
                             double Pos[3], double Vel[3] );


/***************************************************************************
//
// ComputeGLONASSSClockInfo
//
// Ŀ��: ������������, �����Ӳ�/������Ϣ
//
// �������:
//
//  Slot  �������GLONASS���Ǻ�
//  Time  �źŷ���ʱ�̵�GLONASSϵͳʱ��
//  Eph   Slot���ǵ�GLONASS��������
//  GloTmCorr GLONASSʱ����UTC(SU)ϵͳ��ĸ���ֵ

//  �������:
//  clkoft, ClkSft  �źŷ���ʱ�̵�GPS�����Ӳ������[m, m/s]

  ����ֵ

  ����ɹ�����true, ���򷵻�false.
***************************************************************************/
bool ComputeGLONASSSClockInfo( const int Prn, const GPSTIME* t,
                             GLONASSEPHREC* Eph, GLOTIMECORR* GloTmCorr,
                             double* ClkOft, double* ClkSft );

#endif