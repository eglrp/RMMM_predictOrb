#pragma once
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "stdlib.h"
#include <cstring>
#include "gpstime.h"
#include "RTOD_Structs.h"

struct ONESATEHP //һ����Ԫһ�����ǵľ������������Ǻš�λ�á��ٶȡ��Ӳ 
{
	int PRN;     //���ǵ�PRN��

	bool   ISExist; //�жϸ����ǵľ��������Ƿ����

	double  satposx;            //����λ��-x
	double  satposy;            //����λ��-y
	double  satposz;            //����λ��-z

	double  satvelx;            //�����ٶ�-vx
	double  satvely;            //�����ٶ�-vy
	double  satvelz;            //�����ٶ�-vz
	
	double  satclk;   //�����Ӳ�
	double  satvelc;  //�������� 
};

struct ONEPOCHOFSP3 //һ����Ԫ�������ǵľ�������
{
	int     WeekNumber;         //GPS�ܵ��ܺ�

	double  dWeeksec;           //GPS�ܵ�����

	double  totalWeeksec;       //�ܵ�GPS���룬�������������ڲ�

	COMMONTIME   pT;                 //ͨ��ʱ

	int     Satnum;             //������Ԫ�����ǵ���Ŀ

	struct  ONESATEHP obs[32];  //����ǲ�۲����ݵ�����

};
struct ONESATCLK
{
	int PRN;     //���ǵ�PRN��

	bool   ISExist; //�жϸ����ǵľ��������Ƿ����
	double  satclk;   //�����Ӳ�
	double  satvelc;  //�������� 
};
struct ONEPOCHPRECLK
{
	int     WeekNumber;         //GPS�ܵ��ܺ�

	double  dWeeksec;           //GPS�ܵ�����

	double  totalWeeksec;       //�ܵ�GPS���룬�������������ڲ�

	COMMONTIME   pT;                 //ͨ��ʱ
	
	struct ONESATCLK obs[32];//����һ�����ǵľ����Ӳ�


};
void ReadSp3File(char* filename,struct ONEPOCHOFSP3 *OBSP);
void ReadPreClkFile(char* filename,struct ONEPOCHPRECLK *CLKP);
int FindFitPoint_SP3(const int PRN,GPSTIME OneEpoch,int n,struct ONEPOCHOFSP3 *OBSP, double t[], 
	double  satPos[3][30], double clk[]);//2nΪ��Ͻ���
int FindFitPoint_CLK(GPSTIME OneEpoch,int n,struct ONEPOCHPRECLK *CLKP,struct ONEPOCHPRECLK *FITPOINTCLK);
void Lagrange_SP3(GPSTIME OneEpoch,int PRN,int n,struct ONEPOCHOFSP3 *OBSP,ONESATEHP *PREORBIT_SP3);
int Lagrange_CLK(GPSTIME OneEpoch,int PRN,int n,struct ONEPOCHPRECLK *CLKP,ONESATCLK *PREORBIT_CLK);
void Rotation(int n,int PRN,double dt,ONEPOCHOFSP3 *FITPOINTSP3);
