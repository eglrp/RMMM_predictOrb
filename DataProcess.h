/****************************************************************************
Ŀ�ģ�    �Թ۲����ݽ��д������ɹ�ͬ�۲�ֵ��̽������
��дʱ�䣺2016.07.27
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#pragma once 
#ifndef _DATAPROCESS_
#define _DATAPROCESS_

#include "RTOD.h"
#include "RTOD_Structs.h"
#include "EKFilter.h"
#include "CommonFuncs.h"
#include "Trace.h"
#include "qualityCheck.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <afx.h>	
#define  Me_L		0.005								//�ز���λ
//#define  Me_P1		0.20
//#define  Me_P2		0.25
#define	 Me_dPC		0.57								/*�޵����α��۲�����*/	
#define  Me_dLC		4.0*Me_L							/*�����޵�����ز��۲�����*/
#define  C_Light	299792458.0							/* Speed of light  [m/s]; IAU 1976  */
#define  SIGMW		10000								/*��ʼ����ģ���ȷ���,1.37��ͨ����ʼ����ʽMW�Ƶ�����*/
#define  SIGL1		10000								/*��ʼL1ģ���ȷ���*/
#define  GF_THRES	0.05								/*LC̽��������ֵ,��λm*/
#define	 LEMTA_NL	0.106953378142147					/*խ�ﲨ������λΪ��*/
#define	 LEMTA_WL	0.861918400322005					/*���ﲨ������λΪ��*/
#define	 RATIO		0.779220779220779					/*ratio=(f2/f1)*/
#define	 RATIO2		0.607185022769438					/*ratio2=(f2/f1)*(f2/f1)*/
#define	 RATIOK		0.124087591240876					/*ratiok=(f1-f2)/(f1+f2)*/
/*�޵������Ͽ���ģ����ǰ�۲�ϵ��Hwl*/
#define  Hwl		0.37748251108920522					/*LEMTA_1*RATIO/(1.0-RATIO2)*/
#define  NC_SIG     0.5									/*�޵����ģ�����������Ϊ0.5m*/
#define  MW_SIG		0.5									/*�����Ϊ0.5��*/
#define	 INTERVAL	600.0								/*���ݿ�ȱʱ�δ���10����������ȫ����ʼ���ز�*/
//����A/B��ͬ�Ĺ۲���������
int GenCommomObs(EpochObsData * Epoch_A,EpochObsData * Epoch_B,Common11Obs * tempComObs);
/*��ʼ��ģ���ȵ�Э���ͨ��Э���������*/
void initialAmbsCov(const double B[48],const double Dl[12],EKFSTATE * EkfState,const double sigma);
void getBpos(double * StateA,double * relState,double * StateB);
void getRelPos(double * StateA,double * StateB,double * relState);
//��������̽�⣬�������������ʶΪ-1�����������ʶΪ0���¹۲�ֵ��ʶΪ1
int CySlipDetection_SD(double * Mat, Common11Obs * preComObs,Common11Obs * curComObs,EKFSTATE * EkfState);
//�ǲ�����̽��
int CySlipDetection_ND(double * Mat, Common11Obs * preComObs,Common11Obs * curComObs,EKFSTATE * EkfState);
//����̽�ⷽ��
/*�����̽��������-����۲�ֵ*/
int GFDetection_SD(OneSat11Obs * PreObs,OneSat11Obs * CurObs, double * dGF);
/*MW̽��������-����۲�ֵ*/
int MWDetection_SD(OneSat11Obs * PreObs,OneSat11Obs * CurObs, double * tempMW);
/*�����̽��������-�ǲ�۲�ֵ*/
int GFDetection_ND(OneSat11Obs * PreObs,OneSat11Obs * CurObs, double * dGF);
/*MW̽��������-�ǲ�۲�ֵ*/
int MWDetection_ND(OneSat11Obs * PreObs,OneSat11Obs * CurObs,double * tempMW);
//ͨ�����������̽����������LLI�����������ж�
int CySlipDetection_LLI(double *Mat,Common11Obs * preComObs,Common11Obs * curComObs,EKFSTATE * EkfState);
int PPRTOD_LC(Common11Obs * CurObs,EKFSTATE * KFState, PPRESULT * Result, double * Mat);	/*��С����̽��������*/
//α�༸�η���Զ���
int PPRTOD_PC(Common11Obs * CurObs,EKFSTATE * KFState, PPRESULT * Result, double * Mat);
void EKFTimeUpdateRel( double OIState[108], EKFSTATE* KFState );
void outRes(int satNumWhole,FILE *fLC,double *OC_LC0,double *OC_LC1,EKFSTATE * KFState);
//��Բ�������
int relEKFilter_PC(double Mat[9],EKFSTATE * KFState);
int relEKFilter_LC(double Mat[9],EKFSTATE * KFState);
//
double getMap(double radEle);									/*�õ�ͶӰ����ֵ*/
/*���α���˲���ǰ��в��ֵ*/
void outResEKFPC(int satNumWhole,double *OC_PC0,double *OC_PC1,EKFSTATE * KFState);
/*����ز��˲���ǰ��в��ֵ*/
void outResEKFLC(int satNumWhole,double *OC_LC0,double *OC_LC1,EKFSTATE * KFState);
/*���LC-PC�Ĳв��ֵ������Ƿ�Ϊ0�����ж�������ģ���ȴ�����ȷ*/
void outResAmbSD(const double sow,const double LC_PC0,const double LC_PC1);
#endif