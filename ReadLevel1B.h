#pragma once
#include "stdlib.h"
#include <cstring>
#include <math.h>
#include <stdio.h>
#include "satods.h"
//GNV�������
struct ORBIT
{
	GPSTIME gpsTime;
	double sec;
	double x;
	double y;
	double z;
	double dx;
	double dy;
	double dz;
};
//GNV��Թ������
struct D_ORBIT
{
	int week;
	double sec;
	int comSatNum;
	double r;//����rtn
	double t;
	double n;

	double dr;//�ٶ�rtn
	double dt;
	double dn;

	double dx;//��������
	double dy;
	double dz;
	double ddx;//�����ٶ�
	double ddy;
	double ddz;
	double R;//���߳�
};
//��Զ�λ����
struct LCPC_ORBIT
{
	int week;
	double sec;//��J2000��ʼ������
	double x;//�ο�λ�ú��ٶȣ�Ϊ��ת��Ϊrtn
	double y;
	double z;
	double vx;
	double vy;
	double vz;
	double dx;//���������в��GNV��Թ�����бȽϵõ�
	double dy;
	double dz;
	int	satnum;//��Զ�λʱ�ĵ���������
	double sigPosPc;
	double sigPosLc;
	double R;//������Զ�λ��������õĻ��߳�
	double R0;//���ݵ��㶨���������õĻ��߳�
	double DOP;
};
struct KBR
{
	GPSTIME gpsTime;
	double sec;
	double cor_br;
	double br;
	double ltc_br;
	double gc_br;
	int flag;//��ʼ����Ϊ0������Ҫ�������ó�ֵƫ�����Ӧ��ʱ������Ϊ1
};
//�в�
struct KBR_R
{
	double bias;//��ֵƫ��
	int num;
	KBR* kbr;

};
extern struct ORBIT* orbitA;
extern struct ORBIT* orbitB;
extern struct D_ORBIT* d_orbit;
extern struct KBR* kbr;
extern struct LCPC_ORBIT* Lc_orbit;
extern struct D_ORBIT* dOrbitA;
extern struct D_ORBIT* dOrbitB;
extern struct D_ORBIT* dOrbitAB;
void Readorbit(char* filename,ORBIT* orbit,int* num);
void Readkbr(char* filename,KBR* kbr,int* num);
void InitOrbit(ORBIT* orbit);
void InitD_Orbit(D_ORBIT* orbit);
void InitKBR(KBR* kbr);
void InitLc_orbit(LCPC_ORBIT* orbit);
void Out_Residual(LCPC_ORBIT* orbit,int numofepoch,int n_kbr);
int DataCheck(int doy,int year); //����KBR�ļ��۲�ֵ��Ŀ
void assess(int * const numOfEpoch,int * const numOfA,int * const numOfB,EKFSTATE * KFState,FINALORBIT FinalOrb[2]);