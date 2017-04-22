/****************************************************************************
Ŀ�ģ�    LEOSAOD�����������

��дʱ�䣺2008.12.10
�汾:     V1.1
��Ȩ��    �人��ѧ

�޸ļ�¼��
2009.3.11    ����̫����ѹ�㶯�������Ʋ���������Crϵ��,
�޸��˳�ʱ���ж�����µĹ�������
2009.5.10    ��������ģ�͸�ΪEGM2008���ɸĽ�������ģ�;��ȡ�

2009.8.3    ���ʹ�õ�Ƶ���ݽ���ʵʱ���죬��Ҫ�޸�������������
1. GetOneSatPseudoRange�����У���P2��Ϊ����������û�б�ǵķ���
2. ��ComputeGPSSatOrbitAtSignalTrans�е�Tgd���ϣ�������߶��쾫�ȡ�

****************************************************************************/
#pragma once
#include "Satods.h"
#include "RTOD.h"
#include "CommonFuncs.h"
FILE* FGPSEph;
FILE* FGLOEph;


void initFace()
{
	printf("\n        ********************************************************************\n");
	printf("        *                                                                  *\n");
	printf("        *                  LEO Spacecraft Spaceborne-GNSS                  *\n");
	printf("        *      Autonomous Orbit flying formation Determination Software    *\n");
	printf("        *                          LEOSAOD(DF)v1.1                         *\n");
	printf("        *                                                                  *\n");
	printf("        *                         Developed by WHU                         *\n");
	printf("        *                                                                  *\n");
	printf("        *                                   All Copyright reserved!        *\n");
	printf("        *                                                                  *\n");
	printf("        ********************************************************************\n\n\n");
}
void CheckDOY( int &year, int &doy )
{
	int days = 365;

	if( year%4 == 0 )
	{
		days++;
	}

	if( doy > days )
	{
		doy = doy - days;
		year++;
	}
}
void brushFile()
{
	//��һ�μ���ˢ��
	FILE* brush=fopen("OutFile\\graceA.txt","w+");
	fclose(brush);
	brush=fopen("OutFile\\graceB.txt","w+");
	fclose(brush);
	brush=fopen("OutFile\\line.txt","w+");
	fclose(brush);
	brush=fopen("OutFile\\state.txt","w+");
	fclose(brush);
	brush=fopen("OutFile\\cov.txt","w+");
	fclose(brush);
	brush=fopen("OutFile\\Out_obs.txt","w+");
	fclose(brush);
	brush=fopen("OutFile\\cycleSlip.txt","w+");
	fclose(brush);
	//��һ�μ����ļ�ˢ�½���
}
void InitPreOrbit(const int StartDOY,const int StartYear)
{
	int n_kbr;
	Lc_orbit=(LCPC_ORBIT*)malloc(17330*sizeof(struct LCPC_ORBIT));
	orbitB=(ORBIT*)malloc(17330*sizeof(struct ORBIT));
	orbitA=(ORBIT*)malloc(17330*sizeof(struct ORBIT));
	d_orbit=(D_ORBIT*)malloc(17330*sizeof(struct D_ORBIT));
	kbr=(KBR*)malloc(17330*sizeof(struct KBR));
	dOrbitA=(D_ORBIT*)malloc(17330*sizeof(struct D_ORBIT));
	dOrbitAB=(D_ORBIT*)malloc(17330*sizeof(struct D_ORBIT));
	dOrbitB=(D_ORBIT*)malloc(17330*sizeof(struct D_ORBIT));
	n_kbr=DataCheck(StartDOY,StartYear);//��ȡ����Ĳο�ֵ
	//���������ʼ��
	for (int index=0;index<17330;index++)
		InitLc_orbit(Lc_orbit+index);
	//��������ʼ��
	for (int index=0;index<17330;index++)
		InitD_Orbit(dOrbitA+index);
	for (int index=0;index<17330;index++)
		InitD_Orbit(dOrbitB+index);
	for (int index=0;index<17330;index++)
		InitD_Orbit(dOrbitAB+index);
}
void freeAll()
{
	free(Lc_orbit);
	free(orbitB);
	free(orbitA);
	free(d_orbit);
	free(dOrbitA);
	free(dOrbitB);
	free(kbr);
	free(dOrbitAB);
}
void initFile(char * Satname_A, char * Satname_B, char * EphName, const int StartDOY, const int StartYear,
	char * ObsFileName_A, char * ObsFileName_B, char * GPSEphFileName, char * ResFileName_A, char * ResFileName_B,
	char * RsdFileName_A, char * RsdFileName_B, EKFSTATE KFState)
{
	//sprintf( ObsFileName_A, "InputFile\\%s%04d.%02do", Satname_A, StartDOY*10, StartYear );
	sprintf( ObsFileName_A, "InputFile\\%s%04d.%02do", Satname_A, StartDOY*10, StartYear );
	sprintf( ObsFileName_B, "InputFile\\%s%04d.%02do", Satname_B, StartDOY*10, StartYear );
	sprintf( GPSEphFileName, "InputFile\\%4s%04d.%02dn", EphName, StartDOY*10, StartYear );
	sprintf( ResFileName_A, "InputFile\\%s%04d.%02ds", Satname_A, StartDOY*10, StartYear );
	sprintf( ResFileName_B, "InputFile\\%s%04d.%02ds", Satname_B, StartDOY*10, StartYear );
	sprintf( RsdFileName_A, "OutFile\\graceA.txt_%d_%d",(int)KFState.Step,KFState.Para[0].m_a);
	sprintf( RsdFileName_B, "OutFile\\graceB.txt_%d_%d",(int)KFState.Step,KFState.Para[0].m_a);
}
void ReadAntFile(const char antexFile[],pcvs_t * pcvs)
{
	pcvs->n=0;
    pcvs->nmax=0;
    readantex(antexFile, pcvs);
}
int main( int args,char * argv[] )
{
	int  i,flag;
	time_t start, endt;
	double   transXYZV[6];//Ϊ��ת��ΪRTN
	double   dXYZ[3];
	double   dRTN[3];
	/* ��ʱ��ʵʱ���������Ҫ���ļ�Ŀ¼�ȱ�����Ϣ */
	char ObsFileName_A[100],ObsFileName_B[100];
	char GPSEphFileName[100];
	char ResFileName_A[100],ResFileName_B[100],RsdFileName_A[100],RsdFileName_B[100];
	//char OrbFileName_A[100],OrbFileName_B[100];

	/* �����������ݽṹ */
	GPSEPHREC   Record_A[32],Record_B[32];

	/* �۲����ݽṹ */
	OBSTYPELIST ObsTypeList_A,ObsTypeList_B;
	EpochObsData Epoch_A,Epoch_B;
	PSPRange     PSPR_B[32],PSPR_A[32];    /* ���ڴ洢��λƽ��α����м���,������GPS */

	/* ���㶨λ���ݽṹ */
	SATMIDRESULT SatMidInfo_A[24],SatMidInfo_B[24];
	

	/* EKF������ */
	EKFSTATE KFState;
	
	/* ����ѧ������ֲ��� */
	double   h;             
	SCState  CurrStat[2];              /* �ڲ�����Ҫ�Ĳ��� */
	double	 OiState[108]={0.0};

	/* ���չ����� */
	char Satname_A[5], Satname_B[5],EphName[5];
	int  StartYear, StartDOY, EndYear, EndDOY;
	FINALORBIT FinalOrb[2];            /* ����Ĺ��ƽ������˲��ֵ�Ȳ��� */

	/*���۲��ֽṹ*/
	int numOfEpoch=0;
	int numOfA=0,numOfB=0;//��ǰA��B��GFZ��ֵ����

	double xA_pre[6]={0.0},xB_pre[6]={0.0};//ǰһ����ֵ�����Ϊ��Kalman�˲����������
	initFace();

	printf("\nPlease input EPH abbr name (four alphabet: brdc)...\n" );
	//scanf("%s", EphName );
	sprintf(EphName,"brdc");
	printf("\nPlease input the year and DOY for first obs data...\n" );
	StartYear=10;StartDOY=152;//StartDOY=152;
	EndYear=10;EndDOY=152;//EndDOY=161;
	
	//ˢ�½���ļ�,��ʼ���ṹ��
	brushFile();
	initKFState(&KFState);
	EmptySATMIDRESULTStruct( 24, SatMidInfo_A);
	EmptySATMIDRESULTStruct( 24, SatMidInfo_B);
	EmptyEpochObsDataStruct( &Epoch_A);
	EmptyEpochObsDataStruct( &Epoch_B);
	if(args!=3)
	{
		return 0;
	}
	else
	{
		KFState.Step = str2num(argv[1],0,4);
		KFState.Para[0].m_a = str2num(argv[2],0,4);
		KFState.Para[0].n_a = str2num(argv[2],0,4);
		KFState.Para[1].m_a = str2num(argv[2],0,4);
		KFState.Para[1].n_a = str2num(argv[2],0,4);
		//KFState.wuchaMod = str2num(argv[2],0,4);
	}

	time( &start );
	
	sprintf(Satname_A,"GPSA");
	sprintf(Satname_B,"GPSB");

	initFile(Satname_A, Satname_B, EphName, StartDOY, StartYear, ObsFileName_A, ObsFileName_B,GPSEphFileName,
		ResFileName_A,ResFileName_B,RsdFileName_A,RsdFileName_B, KFState);
	
	/************************************************************************/
	/* ��ȡ�ļ�*/
	/************************************************************************/
	FILE* FRESIDUAL_A= fopen( RsdFileName_A, "wt" );
	FILE* FRESIDUAL_B= fopen( RsdFileName_B, "wt" );
	FILE* Fout_A  = fopen( ResFileName_A, "a+" );
	FILE* Fout_B  = fopen( ResFileName_B, "a+" );
	/*
	FILE* Fobs_A= fopen( ObsFileName_A, "rt" );  
	FILE* Fobs_B= fopen( ObsFileName_B, "rt" );  
	*/
	
	/*��ȡ���������ļ�,GPS������λ���ĸ����ļ�*/
	/*
	PRE_EPH=(ONEPOCHOFSP3*)malloc(96*3*sizeof(ONEPOCHOFSP3));
	ReadSp3File("InputFile\\igs15861.sp3",PRE_EPH);
	ReadSp3File("InputFile\\igs15862.sp3",PRE_EPH);
	ReadSp3File("InputFile\\igs15863.sp3",PRE_EPH);
	//��ȡ������λ�����ļ�
	ReadAntFile("InputFile\\igs05_1627.atx",&pcvs);		//����GPS��
	//ReadAntFile("InputFile\\igs08_1758.atx",&pcvs);     //����beidou�����ǵģ���ȡ��Ҫ�����ж�
	
	if( (FGPSEph = fopen( GPSEphFileName, "rt" )) == NULL )
	{
		printf( "Cannot open %s GPS ephem file. \n", GPSEphFileName );
		return 0;
	}
	
	if( ReadObsHead( Fobs_A, &ObsTypeList_A ) ==0 )
	{
		printf( "Some problems in Reading the header of Obs file.\n" );
		return 0;
	}
	if( ReadObsHead( Fobs_B, &ObsTypeList_B ) ==0 )
	{
		printf( "Some problems in Reading the header of Obs file.\n" );
		return 0;
	}
	*/
	//���ܵ͹����ǹ����ʼ��
	int initFlag=0;
	double xA_pre_ECI[6],xB_pre_ECI[6];
	numOfEpoch=0;
	numOfA=0,numOfB=0;//��ǰA��B��GFZ��ֵ����
	InitPreOrbit(StartDOY,StartYear);
	if(orbitA[0].gpsTime.Week==orbitB[0].gpsTime.Week&&
		fabs(orbitA[0].gpsTime.SecOfWeek-orbitB[0].gpsTime.SecOfWeek)<1E-5)
	{
	
		KFState.Time.Week=orbitA[12].gpsTime.Week;
		KFState.Time.SecOfWeek=orbitA[12].gpsTime.SecOfWeek;
		xA_pre[0]=orbitA[12].x;
		xA_pre[1]=orbitA[12].y;
		xA_pre[2]=orbitA[12].z;
		xA_pre[3]=orbitA[12].dx;
		xA_pre[4]=orbitA[12].dy;
		xA_pre[5]=orbitA[12].dz;

		xB_pre[0]=orbitB[12].x;
		xB_pre[1]=orbitB[12].y;
		xB_pre[2]=orbitB[12].z;
		xB_pre[3]=orbitB[12].dx;
		xB_pre[4]=orbitB[12].dy;
		xB_pre[5]=orbitB[12].dz;
		ICRF_ITRF_GPST( MJD_J2000,  &KFState.Time, false, xA_pre_ECI, xA_pre);//ת������ϵ
		ICRF_ITRF_GPST( MJD_J2000,  &KFState.Time, false, xB_pre_ECI, xB_pre);//ת������ϵ
	}
	else
		return 0;
	do{
		double temptest=fmod(KFState.Time.SecOfWeek,86400.0);
		//if(fabs(KFState.Time.SecOfWeek-604770.0)<1E-5)
			//system("pause");
		//if( ReadEpochObs( Fobs_A, &ObsTypeList_A, &Epoch_A ) == 0||ReadEpochObs( Fobs_B, &ObsTypeList_B, &Epoch_B ) == 0 )//�������򿪹۲��ļ�ʧ�ܣ����ȡ��һ������
		if(fabs(fmod(KFState.Time.SecOfWeek,86400.0))<1E-5&&initFlag==1)//������λ�ñȽϵ�һ������һ����Ԫ�����ȡ��һ���ļ�
		{
			
			//�����������
			char baselineFile[60]="",baseVSFile[60]="",ASpeedFilePath[60]="",BSpeedFilePath[60]="";
			sprintf(baselineFile,"OutFile\\line.txt_%d_%d",(int)KFState.Step,KFState.Para[0].m_a);
			sprintf(baseVSFile,"OutFile\\speed.txt_%d_%d",(int)KFState.Step,KFState.Para[0].m_a);
			sprintf(ASpeedFilePath,"OutFile\\graceA_speed.txt_%d_%d",(int)KFState.Step,KFState.Para[0].m_a);
			sprintf(BSpeedFilePath,"OutFile\\graceB_speed.txt_%d_%d",(int)KFState.Step,KFState.Para[0].m_a);
			FILE * lineFile=fopen(baselineFile,"a+");
			FILE * speedFile=fopen(baseVSFile,"a+");
			FILE * ASpeedFile=fopen(ASpeedFilePath,"a+");
			FILE * BSpeedFile=fopen(BSpeedFilePath,"a+");
			double outDx,outDy,outDz,outDR;
			double outDDx,outDDy,outDDz,outDDR;
			for(i=0;i<numOfEpoch;i++){
				outDx=Lc_orbit[i].dx-dOrbitAB[i].dx;
				outDy=Lc_orbit[i].dy-dOrbitAB[i].dy;
				outDz=Lc_orbit[i].dz-dOrbitAB[i].dz; 
				outDR=sqrt(outDx*outDx+outDy*outDy+outDz*outDz);
				//������λ������ļ�
				fprintf( lineFile, " %10d %10d %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f\n",
					Lc_orbit[i].week,(int)(Lc_orbit[i].sec+0.5), outDx,outDy,outDz,
					dOrbitAB[i].r,dOrbitAB[i].t,dOrbitAB[i].n ,outDR);
				outDDx=Lc_orbit[i].vx-dOrbitAB[i].ddx;
				outDDy=Lc_orbit[i].vy-dOrbitAB[i].ddy;
				outDDz=Lc_orbit[i].vz-dOrbitAB[i].ddz; 
				outDDR=sqrt(outDDx*outDDx+outDDy*outDDy+outDDz*outDDz);
				//�������ٶ�����ļ�
				fprintf(speedFile, "%10d%10d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",
					Lc_orbit[i].week,(int)(Lc_orbit[i].sec+0.5), outDDx*1000,outDDy*1000,outDDz*1000,
					dOrbitAB[i].dr*1000,dOrbitAB[i].dt*1000,dOrbitAB[i].dn*1000 ,outDDR*1000);
			}
			fclose(lineFile);
			fclose(speedFile);
			//�����������
			for(i=0;i<numOfEpoch;i++){
				//���A��λ������ļ�
				outDR=sqrt(dOrbitA[i].dx*dOrbitA[i].dx+dOrbitA[i].dy*dOrbitA[i].dy
					+dOrbitA[i].dz*dOrbitA[i].dz);
				fprintf( FRESIDUAL_A,"%10d%10d%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f\n",
					dOrbitA[i].week,(int)(dOrbitA[i].sec+0.5), dOrbitA[i].dx,dOrbitA[i].dy,dOrbitA[i].dz,
					dOrbitA[i].r,dOrbitA[i].t,dOrbitA[i].n,outDR);
				//���A���ٶ�����ļ�
				outDDR=sqrt(dOrbitA[i].ddx*dOrbitA[i].ddx+dOrbitA[i].ddy*dOrbitA[i].ddy
					+dOrbitA[i].ddz*dOrbitA[i].ddz);
				fprintf( ASpeedFile,"%10d%10d%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f\n",
					dOrbitA[i].week,(int)(dOrbitA[i].sec+0.5), dOrbitA[i].ddx*1000,dOrbitA[i].ddy*1000,
					dOrbitA[i].ddz*1000,dOrbitA[i].dr*1000,dOrbitA[i].dt*1000,dOrbitA[i].dn*1000,outDDR*1E3);
				//���B��λ������ļ�
				outDR=sqrt(dOrbitB[i].dx*dOrbitB[i].dx+dOrbitB[i].dy*dOrbitB[i].dy
					+dOrbitB[i].dz*dOrbitB[i].dz);
				fprintf( FRESIDUAL_B,"%10d%10d%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f\n",
					dOrbitB[i].week,(int)(dOrbitB[i].sec+0.5),
					dOrbitB[i].dx,dOrbitB[i].dy,dOrbitB[i].dz,
					dOrbitB[i].r,dOrbitB[i].t,dOrbitB[i].n,outDR);
				//���B���ٶ�����ļ�
				outDDR=sqrt(dOrbitB[i].ddx*dOrbitB[i].ddx+dOrbitB[i].ddy*dOrbitB[i].ddy
					+dOrbitB[i].ddz*dOrbitB[i].ddz);
				fprintf( BSpeedFile,"%10d%10d%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f%12.3f\n",
					dOrbitB[i].week,(int)(dOrbitB[i].sec+0.5), dOrbitB[i].ddx*1E3,dOrbitB[i].ddy*1E3,dOrbitB[i].ddz*1E3,
					dOrbitB[i].dr*1E3,dOrbitB[i].dt*1E3,dOrbitB[i].dn*1E3,outDDR*1E3);
			}
			
			//Out_Residual(Lc_orbit,numOfEpoch,n_kbr);
			freeAll();
			/*
			fclose( Fobs_A);
			fclose( Fobs_B);
			fclose( FGPSEph );
			*/
			fclose( Fout_A );
			fclose( Fout_B );
			fclose( FRESIDUAL_A );
			fclose( FRESIDUAL_B );
			fclose( ASpeedFile);
			fclose( BSpeedFile);
			//���۽���
			time( &endt );    			
			printf("\nColapsed time: %lf\n", difftime( endt, start ) );
			//�ڶ���
			StartDOY++;
			CheckDOY( StartYear, StartDOY );
			if( StartYear>=EndYear && StartDOY>EndDOY ) break;

			//���³�ʼ����ǰ���Ѿ�free��
			//���ܵ͹����ǹ����ʼ��
			numOfEpoch=0;
			numOfA=0,numOfB=0;//��ǰA��B��GFZ��ֵ����
			InitPreOrbit(StartDOY,StartYear);
		
			initFile(Satname_A, Satname_B, EphName, StartDOY, StartYear, ObsFileName_A, ObsFileName_B,GPSEphFileName,
				ResFileName_A,ResFileName_B,RsdFileName_A,RsdFileName_B, KFState);

			printf( "Now processing %s......\n", ObsFileName_A );
			printf( "Now processing %s......\n", ObsFileName_B );
			/*
			Fout_A  = fopen( ResFileName_A, "wt" );
			Fout_B  = fopen( ResFileName_B, "wt" );

			if( (Fobs_A = fopen( ObsFileName_A, "rt" )) == NULL )
			{
				printf( "Cannot open GPSA Obs file. \n " );
				break;
			}
			if( (Fobs_B = fopen( ObsFileName_B, "rt" )) == NULL )
			{
				printf( "Cannot open GPSB Obs file. \n " );
				break;
			}
			if( (FGPSEph = fopen( GPSEphFileName, "rt" )) == NULL )
			{
				printf( "Cannot open %s GPS ephem file. \n", GPSEphFileName );
				break;
			}
			*/
			FRESIDUAL_A  = fopen( RsdFileName_A, "a+" );
			FRESIDUAL_B  = fopen( RsdFileName_B, "a+" );

			/* �۲��ļ���ȡ */
			/*
			if( ReadObsHead( Fobs_A, &ObsTypeList_A ) ==0 )
			{
				printf( "Some problems in Reading the header of ObsA file.\n" );
				break;
			}
			if( ReadObsHead( Fobs_B, &ObsTypeList_B ) ==0 )
			{
				printf( "Some problems in Reading the header of ObsB file.\n" );
				break;
			}
		
			ReadEpochObs( Fobs_A, &ObsTypeList_A, &Epoch_A );
			ReadEpochObs( Fobs_B, &ObsTypeList_B, &Epoch_B );
			*/
		}

		/*
		
		alignEpoch(Fobs_A, &ObsTypeList_A, &Epoch_A,Fobs_B, &ObsTypeList_B, &Epoch_B);//��ʼ���ɹ��󣬶�׼�󣬿���ֱ���ø�����������£���Ϊ�����ò���׼	
		
		/************************************************************************/
		//��Ϊǰ��۲�ֵ�Ѿ���׼���˲�Ҳ��׼����������A�۲�ֵ��ʱ���жϼ���

		//ע��KFState���ڹ���ϵ�µ��������꣬����ȥ��ת��ֱ�����Ľ���Ƿ���ͬ

		//��������ǰһ����Ԫassess����ֵ����Ϊ��ֵ
		initFlag=1;
		
		KFStateUpdate(xA_pre_ECI,xB_pre_ECI,&KFState);	
		for(int k=0;k<6;k++)
		{
			OiState[k] = KFState.StateA[k];
			OiState[k+54] = KFState.StateB[k];
		}
		ABTimeUpdate(OiState,&KFState);
		KFState.SatNumUsed[0]=0;
		KFState.SatNumUsed[1]=0;
		KFState.comSatNumUsed=0;
		assess(&numOfEpoch,&numOfA,&numOfB,&KFState,FinalOrb);
		for(int k=0;k<6;k++)
		{
			xA_pre_ECI[k] = KFState.StateA[k]; 
			xB_pre_ECI[k] = KFState.StateB[k]; 
		}
		/*
		//A�Ǳ��ָ߾��ȣ���B��Ԥ��
		double delta[6];

		xA_pre[0] = orbitA[numOfA].x;
		xA_pre[1] = orbitA[numOfA].y;
		xA_pre[2] =	orbitA[numOfA].z;
		xA_pre[3] = orbitA[numOfA].dx;
		xA_pre[4] = orbitA[numOfA].dy;
		xA_pre[5] = orbitA[numOfA].dz;
		ICRF_ITRF_GPST( MJD_J2000,  &KFState.Time, false, xA_pre_ECI, xA_pre);//ת������ϵ

		for(int k=0;k<6;k++)
		{
			delta[k] = KFState.StateB[k] - KFState.StateA[k];
			xB_pre_ECI[k] = xA_pre_ECI[k] + delta[k]; 
		}*/
		//��������JPL�����������˲����и���,ȥ�����ǲ鿴���ھ��ȱ仯
	}while(1);
	//ֻ�ܴ���һ��ģ��������Ϊ�ڴ��������
	freeKFState(KFState);
	time( &endt );    

	printf("\nColapsed time: %12.3f\n", difftime( endt, start ) );
	//free(PRE_EPH);
	printf( "Enter a key to exit...\n" );
//	getchar();
	//system("pause");
	return 0;

}
void KFStateUpdate(double * xA_pre,double * xB_pre,EKFSTATE *  KFState)
{

	PhaseCentToMassCent( false, &KFState->CentBias[0], xA_pre);    /* ת��Ϊ��λ���� */
	PhaseCentToMassCent( false, &KFState->CentBias[3], xB_pre);    /* ת��Ϊ��λ���� */
	for(int i=0;i<6;i++)
	{
		KFState->StateA[i] = xA_pre[i];
		KFState->StateB[i] = xB_pre[i];
	}
	getRelPos(KFState->StateA,KFState->StateB,KFState->StateRel);
}