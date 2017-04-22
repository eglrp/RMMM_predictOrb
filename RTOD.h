/****************************************************************************
Ŀ�ģ�    ����ʵʱ����, ��϶���ѧģ���˲�ʵʱ����

��дʱ�䣺2008.12.5
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#pragma once 
#ifndef _REALTIME_ORBIT_DETERMINATION_
#define _REALTIME_ORBIT_DETERMINATION_

#include "ReadObs.h"
#include "EphProc.h"
#include "EKFilter.h"
#include "ReadPreOrbit.h"
#include "PhaseCent_Cor.h"
extern ONEPOCHOFSP3     *PRE_EPH;
extern pcvs_t pcvs;
/*
  �����źŷ���ʱ�����ǵ�λ�ú��ٶ�, GLONASS����λ��ת��ΪWGS84.
  �����źŷ���ʱ�����ǵ��Ӳ�,

  ���������ЧӦ����
  ���������ӳٸ���

  ���㶨λ�ӳ���
  ��������ӳ���  ���ڳ�ʼ���˲�����

  ���㶨λ�в���
  ���㶨λ����Ԥ����

  �˲������ӳ���


 */

/***************************************************************************
//
// EmptyAPRIORBITStruct
//
// Ŀ��: ��ʼ��APRIORBIT�ṹ��

   �������

    ApriOrbit      ����ʼ���ı���

***************************************************************************/

void EmptyAPRIORBITStruct( APRIORBIT* ApriOrbit );


/***************************************************************************
//
// EmptySATLISTStruct
//
// Ŀ��: ��ʼ��SATLIST�ṹ��

   �������

    SatList      ����ʼ���ı���

***************************************************************************/

void EmptySATLISTStruct( SATLIST* SatList );


/***************************************************************************
//
// EmptyPPRESULTStruct
//
// Ŀ��: ��ʼ��PPRESULT�ṹ��

   �������

    PPResult      ����ʼ���ı���

***************************************************************************/

void EmptyPPRESULTStruct( PPRESULT* PPResult );
                                  

/***************************************************************************
//
// EmptySATMIDRESULTStruct
//
// Ŀ��: ��ʼ��SATMIDRESULT�ṹ��

   �������

    Num             SATMIDRESULT�����ά��
    SatMidInfo      ����ʼ���ı���

***************************************************************************/

void EmptySATMIDRESULTStruct( int Num, SATMIDRESULT* SatMidInfo );


/***************************************************************************
//
// ComputeGPSSatOrbitAtSignalTrans
//
// Ŀ��: �����źŷ���ʱ�̵�GPS���ǹ��,�����Ӳ�,���Ǹ߶Ƚ�
         ���ǵ�����ת����(�˼����ǽ������).

  URAָ��( ���������е�SV accuracy����Ϊ��λ )
  If the value of N is 6 or less, X=pow(2.0, 1+N/2 );
  If the value of N is 6 or more, but less than 15, X=pow(2,N-2);
  N=15 shall indicate the absence of an accuracy prediction and shall advise the
  unauthorized user to use that SV at his own risk.

  SV Healthy (GPS)
  The MSB shall indicate a summary of the health of the NAV data, where
  0 = all NAV data are OK
  1 = some or all NAV data are bad.
  The five LSBs shall indicate the health of the signal components in accordance
  with the codes given as following:
  
  Estimated Group Delay Differential (TGD)
  L1-L2 correction term, for the benifit of "L1 only" or "L2 only" users.
  instruct:  (deltaTsv)L1 = deltaTsv - Tgd
 
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
                                     SATMIDRESULT* SatMidInfo );
                                  
/***************************************************************************
//
// ComputeGLONASSSatOrbitAtSignalTrans
//
// Ŀ��: �������Ƿ���ʱ�̵�GLONASS���ǹ��,�����Ӳ�,�����ЧӦ,���Ǹ߶Ƚ�
         ʱ����GPSʱ��ϵͳΪ�ο�, ���ǹ��ת����WGS84����ϵ.
         ���ǵ�����ת����(�˼����ǽ������).

  // �������:
  //
  // �������:
  //
  Slot            GLONASS���Ǻ�
  Time            �۲�ʱ�̵�GPSʱ��
  PreLeoOrb[3]    Ԥ�������ؽ��ջ�λ��[m]
  PreLeoClk       Ԥ�������ؽ��ջ�GLONASS�Ӳ�[m]
  Height          ���ؽ��ջ��ĸ߳�[m]
  GLOEph          ĳ��GLONASS���ǵ�����
  GloTmCorr       GLONASSʱ��ϵͳ��������
  IonPara         ��������
  
	�������
    
	  SatMidInfo      ���ǹ��, �ٶȵ��м���������Ϣ
	  
		����ֵ
		
		  ����ɹ�����true, ���򷵻�false.
		  
***************************************************************************/

bool ComputeGLONASSSatOrbitAtSignalTrans( const short Slot, const GPSTIME* Time,
                                  double PreLeoOrb[3], double PreLeoClk, double& Height,
                                  GLONASSEPHREC* GLOEph, GLOTIMECORR* GloTmCorr,
                                  IONOPARA* IonPara, SATMIDRESULT* SatMidInfo );
                                  
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
				double Pll[], double Qvv[] );

/***************************************************************************
//
// Klobuchar
//
// Ŀ��: ʹ��Klobucharģ��, ���㵥Ƶ���ջ��ĵ�����ӳٸ�����

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
                                     double RcvPos[3], IONOPARA* IonoPara);

/***************************************************************************
//
// KlobucharWithScale
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
                                     double RcvPos[3], IONOPARA* IonoPara);


/***************************************************************************
//
// PointPositionRTOD
//
// Ŀ��: ��������������һ����Ԫ�Ĺ۲�����C1, ���е��㶨λ(������GPS, 
         ��GLONASS, GPS/GLONASS��϶�λ), Ϊ�˲��ṩ��ʼ������Ϣ.

//
// �������:
//
    GPSEph          GPS��������[32]
    IonPara         ��������
    GLOEph          GLONASS��������[32]
    GloTmCorr       GLONASSʱ����UTC(SU)ϵͳ��ĸ���ֵ
    EpochObs        ��Ԫ�Ĺ۲�����, �Դ�˳��������ǹ��
    PreOrb          Ԥ�������ؽ��ջ�λ��[m],Ԥ�������ؽ��ջ��Ӳ�[m](GPS, GLONASS)

  �������
  
    SatMidInfo      ���ǹ��, �ٶȵ��м���������Ϣ, �����˲�����
    Result          ���㶨λ���
  
  ����ֵ
    
    ���붨λ��������

***************************************************************************/
int PointPositionRTOD( int graceType, GPSEPHREC* GPSEph, IONOPARA* IonPara,
                      EpochObsData* Epoch, APRIORBIT* PreOrb, 
                      SATMIDRESULT* SatMidInfo, PPRESULT* Result );

/***************************************************************************
//
// PointPositionRTODInECI
//
// Ŀ��: ��������������һ����Ԫ�Ĺ۲�����C1, ���е��㶨λ(������GPS, 
         ��GLONASS, GPS/GLONASS��϶�λ), Ϊ�˲��ṩ��ʼ������Ϣ.

//
// �������:
//
    GPSEph          GPS��������[32]
    IonPara         ��������
    GLOEph          GLONASS��������[32]
    GloTmCorr       GLONASSʱ����UTC(SU)ϵͳ��ĸ���ֵ
    EpochObs        ��Ԫ�Ĺ۲�����, �Դ�˳��������ǹ��
    PreOrb          Ԥ�������ؽ��ջ�λ��[m],Ԥ�������ؽ��ջ��Ӳ�[m](GPS, GLONASS)

  �������
  
    SatMidInfo      ���ǹ��, �ٶȵ��м���������Ϣ, �����˲�����
    Result          ���㶨λ���
  
  ����ֵ
    
    ���붨λ��������

**************************************************************************
int PointPositionRTODInECI( GPSEPHREC* GPSEph, IONOPARA* IonPara,
                      GLONASSEPHREC* GLOEph, GLOTIMECORR* GloTmCorr,
                      EpochObsData* Epoch, APRIORBIT* PreOrb, 
                      SATMIDRESULT* SatMidInfo, PPRESULT* Result );
*/

/***************************************************************************
//
// PointPositionVelocityDetermination
//
// Ŀ��: ��������������һ����Ԫ�Ĺ۲����ݵ�D1, ���е������(������GPS, 
         ��GLONASS, GPS/GLONASS��϶�λ), Ϊ�˲��ṩ��ʼ�ٶ���Ϣ. ���������,
         CHAMP���ǵ�����û�ж����չ۲�����, �����ø��ӳ���.

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
                                       SATMIDRESULT* SatMidInfo, PPRESULT* Result );


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
                              EpochObsData* Epoch, SATLIST* SatList );
//LC̽��ֲ�
bool DetectOutliers_rel( double Accuracy, double O_C[], Common11Obs * CurComObs, SATLIST* SatList, double sigma );
void DeleteOutlierFrom_rel( int* GPSSatNum, double Resid[], double A[], double P[], SATLIST* SatList );
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
                                double A[], double P[], SATLIST* SatList );



/***************************************************************************
//
// CheckOutPPPostResidual
//
// Ŀ��: �ڵ��㶨λ֮��, �����в���м������. ���������ȷ�Ժ�ÿ�����ǵĲв�
         �Ƿ����. ����вȷ��Ϊ�ֲ�, ���������˲�����.
         
//
// �������:
//
    n                ��Ԫ��������
	Residual         ���㶨λ�۲�ֵ�в�
	Qvv              �۲�ֵ�в������
    Result           ���㶨λ�Ľ��, ̽�⵽�ֲ�����, ����Result��SatList����
***************************************************************************/
void CheckPostResidual_W( int n, double sigma0, double Residual[], double Qvv[], PPRESULT* Result );


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
	PPResult        ���㶨λ���

  �������
  
    KFState         �˲�������
  
  ����ֵ
    
    ���붨λ��������

***************************************************************************/
int EKFilterRTODA( double Mat[9], SATMIDRESULT* SatMidInfo_A, EpochObsData* Epoch_A,
				 PPRESULT* PPResult, EKFSTATE* KFState );

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
void EKFConvergencyCheck( int graceType, EKFSTATE* KFState );
/*����ƽ���۲�ֵЭ������*/
void ComputeQLL( int m, int n, const double A[],const double InvN[],const double AT[], const double Pll[], double QLL[] );
void initKFState(EKFSTATE *KFState);
void freeKFState(EKFSTATE KFState);
/*ͨ������������������λ�ú��ٶ�*/
void computeGPSfromSP3(const GPSTIME T_Tm,const int Prn,const int order, double * Pos0,double * Vel0,double * ClkOft,double * ClkSft);
#endif