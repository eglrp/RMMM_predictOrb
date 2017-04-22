/****************************************************************************
Ŀ�ģ�    ���Ƕ���ѧ�����㷨, �ö���ѧ�������Ԥ�������Ϊ������, ��α��۲�
          ����, �ƹ㿨�����˲��������ǵ����й������.

��дʱ�䣺2008.12.19
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#pragma once 
#ifndef _EXTEND_KALMAN_FILTER_
#define _EXTEND_KALMAN_FILTER_

#include "RTOD_Structs.h"
#include "DataProcess.h"
#include <stdio.h>
#define	 TAO_VTEC	10
//����α����������б������ӳ���С���˵õ���ֱ������ӳ�
void initialVTEC(const EpochObsData* Epoch,
	const SATMIDRESULT* SatMidInfo,const PPRESULT Result,double * const VTEC,double *const sigma);
// Ŀ��: ��ʼ��AB�����ǿ������˲���״̬
int init2Sat(FILE * Fobs_A,OBSTYPELIST * ObsTypeList_A,EpochObsData * Epoch_A,GPSEPHREC* GPSEph_A,SATMIDRESULT * SatMidInfo_A,
	FILE * Fobs_B,OBSTYPELIST * ObsTypeList_B,EpochObsData * Epoch_B,GPSEPHREC* GPSEph_B,SATMIDRESULT * SatMidInfo_B,
	double OiState[108],EKFSTATE * EkfState);

/****************************************************************************
initASat

Ŀ��: ��ʼ���ο���A��״̬
����: FILE * Fobs_A
����: OBSTYPELIST * ObsTypeList_A
����: EpochObsData * Epoch_A
����: GPSEPHREC * GPSEph_A
����: SATMIDRESULT * SatMidInfo_A
����: double OiState[54]
����: EKFSTATE * EkfState
****************************************************************************/
int initASat(FILE * Fobs_A,OBSTYPELIST * ObsTypeList_A,EpochObsData * Epoch_A,GPSEPHREC* GPSEph_A,SATMIDRESULT * SatMidInfo_A,
	double OiState[54],EKFSTATE * EkfState);

/****************************************************************************
initRel

Ŀ��: ��ʼ�����״̬
����: FILE * Fobs_B
����: OBSTYPELIST * ObsTypeList_B
����: EpochObsData * Epoch_B
����: GPSEPHREC * GPSEph_B
����: SATMIDRESULT * SatMidInfo_B
����: double OiState[54]
����: EKFSTATE * EkfState
****************************************************************************/
int initRel(FILE * Fobs_A, OBSTYPELIST * ObsTypeList_A, EpochObsData * Epoch_A, GPSEPHREC* GPSEph_A, SATMIDRESULT * SatMidInfo_A, FILE * Fobs_B,OBSTYPELIST * ObsTypeList_B,EpochObsData * Epoch_B,GPSEPHREC* GPSEph_B,SATMIDRESULT * SatMidInfo_B, double OiState[54],EKFSTATE * EkfState);
// Ŀ��: ��׼�۲�ֵ�뿨�����˲���ʱ�� //���ȷ��һ����0 30s 60s����������Ҫ�ú���
void alignObsEKF(int graceType,FILE * Fobs_A,OBSTYPELIST * ObsTypeList_A,EpochObsData * Epoch_A,GPSEPHREC* GPSEph_A,SATMIDRESULT * SatMidInfo_A,
	FILE * Fobs_B,OBSTYPELIST * ObsTypeList_B,EpochObsData * Epoch_B,GPSEPHREC* GPSEph_B,SATMIDRESULT * SatMidInfo_B,
	EKFSTATE * EkfState);

// Ŀ��: ��׼�����ǹ۲�ֵ��ʱ�� 
void alignEpoch( FILE * Fobs_A,OBSTYPELIST * ObsTypeList_A,EpochObsData * Epoch_A,FILE * Fobs_B,OBSTYPELIST * ObsTypeList_B,EpochObsData * Epoch_B);

//��ʱ���жϵĸ���
int intUpdate(FILE * Fout_A,EpochObsData * Epoch_A,
	FILE * Fout_B,
	double   OiState[108],EKFSTATE * EkfState);

// Ŀ��: ���EKFSTATE�ṹ��
void EmptyEKFSTATEStruct( EKFSTATE* EkfState );
//��ʼ��˫��						
void EmptyddObsStruct(DDPSEUDOOBS *ddObs);
//��ʼ����ͬ����
void EmptyCurComObsStruct(Common11Obs* ComObs);
//��ʼ��֮ǰ�Ĺ�ͬ���ǽṹ��
void EmptyComObssStruct(Common11Obs * ComObss);
//��ʼ�����Ź�ͬ���ǽṹ��
void EmptyOneSat11Obs(OneSat11Obs * oneSat11Obs);

void InitAStateAndCov(const GPSTIME* Time, double Cd, double Cr, double Tau, 
                        PPRESULT* Result, EKFSTATE* EkfState );



void InitRelStateAndCov(const GPSTIME* Time, PPRESULT Result, EKFSTATE* EkfState );
/***************************************************************************
//
// FormStateTransMatrixFromDyn
//
// Ŀ��: ���ݶ���ѧ�������ɵ�״̬ת�ƾ���(6*6), ������ջ��Ӳ�/����ѧ��������, 
         �γ����յ��ƹ㿨�����˲���״̬ת�ƾ���(EKFDIMENSION*EKFDIMENSION].
//
// �������:

    Tao      �������ٶȵ�һ�׸�˹����ɷ�ģ�͵����ʱ��
    Step     �ƹ㿨�����˲���ʱ����¼��[s]
    w[3]     �������ٶ�[3ά]  m/s^2
    STMDyn   ����ѧ�������ɵ�״̬ת�ƾ���, ֻ����λ�ú��ٶ�[6*8]
    
   �������
   
    STM     �ƹ㿨������״̬ת�ƾ���[EKFDIMENSION*EKFDIMENSION]

***************************************************************************/

void FormStateTransMatrixFromDynRTN4A( const double Tao, const double Step, double State[ADIMENSION], double STMDyn[48], double STM[] );

void FormStateTransMatrixFromDynRTN4rel( const double Tao, const double Step, double State[RELDIMENSION], double STMDyn[48], double STM[] );
void DMCrtn4A( const double Tao, const double Step, double State[]);
//�������ٶ�����rtn������������xyz����
void UpdateDeterministicComponentFromDMCrtn( const double Tao, const double Step,
	double State[]);

/***************************************************************************
//
// UpdateGNSSReceiverClockOffset
//
// Ŀ��: ����һ�׸�˹����ɷ�ģ��, ʱ�����GNSS���ջ���GPS�Ӳ��GLONASS�Ӳ�.
         �������ĵ�, ��ϸ�Ż�.

// �������:

    Tao      �������ٶȵ�һ�׸�˹����ɷ�ģ�͵����ʱ��
    Step     �ƹ㿨�����˲���ʱ����¼��[s]
    w[3]     �������ٶ�[3ά]  m/s^2
    State    ����ѧ����Ԥ�������ǹ��, ֻ����λ�ú��ٶ�[6]
    

***************************************************************************/

void UpdateGNSSReceiverClockOffset( double Tao, double Step,
                                 double w[3], double State[6] );
void DMCrtn4Rel( const double Tao, const double Step, double State[]);
void UdProcessNoiseCovRTN4A(double Tao, double Step, double Sigma,
	double State[6], double Qcov[] );
                   
void UdProcessNoiseCovRTN4rel( double Tao, double Step, double Sigma, double State[6], double Qcov[] );
int intUpdate(FILE * Fout_A,EpochObsData * Epoch_A,
	FILE * Fout_B,
	double   OiState[108],EKFSTATE * EkfState);
void EKFTimeUpdateA(double OIState[54], EKFSTATE * KFState );
bool EKFMeasureUpdateA( double O_C, double R, double H[], EKFSTATE* KFState );
/***************************************************************************
//
// EKFMeasureUpdate
//
// Ŀ��: ʹ��UD�˲���ԭ��, ��ÿ����Ԫ�Ķ�����ǵĹ۲�ֵ�������, ���ζԽ��ջ���
         ״̬���в�������.

// �������:

    O_C      �۲�ֵ������ֵ[m]
    R        �۲�ֵ�ľ���ָ��[m]
    H        �۲�ֵ���Ի���ϵ������
    
    �������
        
    KFState  ����Ϊ����ʱ����º�Ŀ������˲�״̬��Э���������Ϣ, ����һ��GNSS
             ���ǵĹ۲����ݸ��º�, ������º��״̬��Э���������Ϣ

***************************************************************************/
bool EKFMeasureUpdateRel( double O_C, double R, double H[], EKFSTATE* KFState );
//�������
int WholeMeasureUpdate(FILE * Fout_A,EpochObsData * Epoch_A,GPSEPHREC* GPSEph_A,SATMIDRESULT * SatMidInfo_A,
	FILE * Fout_B,EpochObsData * Epoch_B,GPSEPHREC* GPSEph_B,SATMIDRESULT * SatMidInfo_B,
	double  OiState[108],EKFSTATE* KFState);
void ABTimeUpdate(double OiState[108],EKFSTATE* KFState);
#endif




















