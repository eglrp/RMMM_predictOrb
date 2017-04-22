#include "qualityCheck.h"

const double chisqr[100]={      /* chi-sqr(n) (alpha=0.001) */
	10.8,13.8,16.3,18.5,20.5,22.5,24.3,26.1,27.9,29.6,
	31.3,32.9,34.5,36.1,37.7,39.3,40.8,42.3,43.8,45.3,
	46.8,48.3,49.7,51.2,52.6,54.1,55.5,56.9,58.3,59.7,
	61.1,62.5,63.9,65.2,66.6,68.0,69.3,70.7,72.1,73.4,
	74.7,76.0,77.3,78.6,80.0,81.3,82.6,84.0,85.4,86.7,
	88.0,89.3,90.6,91.9,93.3,94.7,96.0,97.4,98.7,100 ,
	101 ,102 ,103 ,104 ,105 ,107 ,108 ,109 ,110 ,112 ,
	113 ,114 ,115 ,116 ,118 ,119 ,120 ,122 ,123 ,125 ,
	126 ,127 ,128 ,129 ,131 ,132 ,133 ,134 ,135 ,137 ,
	138 ,139 ,140 ,142 ,143 ,144 ,145 ,147 ,148 ,149
};
/*���Զ���ֲ��޳�*/
double LS_PC(const int satNum, int * PRN,int * banPRN,const double * satPos,double * ApriPos,const double * obs
	,double * AprioriClk, PPRESULT * Result)
{
	int i, j, k;                     /*  IteratorΪ���㶨λ��������  */
	int SatNumUsed,Iterator;      /*  ���㶨λʹ�õ������� */
	double ATA[16], InvATA[16];
	double ATB[4];
	double dPos[4];               /* ��λ���  */
	double BPos[MAXCHANNUM];                      /* α�������յĹ۲�ֵ-����ֵ */
	double MeasA[MAXCHANNUM*4], MeasAT[MAXCHANNUM*4];          /* �۲���� [MAXCHANNUM][5] */
	double Weight[MAXCHANNUM];                               /* Ȩ����, ��λ��ֻȡ�Խ���Ԫ�� */
	double Qvv[MAXCHANNUM];                               /* �۲�ֵ��������б������Խ���Ԫ�� */
	double Range;                                 /*  ���ջ��뵼������֮��ľ���  */
	double Coverage,SigmaPos=0.0;
	double Residual[MAXCHANNUM] = {0.0};  /* ��λ�в�  */
	int valid =0;
	Iterator = 0;
	do{
		SatNumUsed = 0;

		for( i=0; i<MAXCHANNUM; i++ )
		{
			valid=1;
			for (j=0;j<12;j++)
			{
				if(PRN[i]==999|PRN[i]==banPRN[j]){
					valid=0;
					break;
				}
			}
			if (valid==1)
			{
				Range = 0.0;
				for( k=0; k<3; k++ )
				{
					Range = Range + pow( satPos[i*3+k]-ApriPos[k], 2.0 );
				}

				Range = sqrt( Range );
				MeasA[SatNumUsed*4+0] = 1.0*( ApriPos[0] - satPos[i*3+0] )/Range;
				MeasA[SatNumUsed*4+1] = 1.0*( ApriPos[1] - satPos[i*3+1] )/Range;
				MeasA[SatNumUsed*4+2] = 1.0*( ApriPos[2] - satPos[i*3+2] )/Range;
				MeasA[SatNumUsed*4+3] = 1.0;             /* ���ջ�GPSϵͳ�Ӳ�ϵ�� */
				Weight[SatNumUsed]    = 1.0;  // * pow( sin(SatMidInfo[i].Elevation), 2.0 );

				BPos[SatNumUsed] =obs[i]-Range-*AprioriClk;
				SatNumUsed++;
			}


		}

		/*  �������  */

		for( k=SatNumUsed; k<MAXCHANNUM; k++)  /* ���������� */
		{
			BPos[k] = 0.0;
			Weight[k] = 0.0;
			for( j=0; j<4; j++ )
			{
				MeasA[k*4+j] = 0.0;
			}
		}

		if( SatNumUsed>=4 )  /* �϶����� */
		{
			MatrixTranspose( SatNumUsed, 4, MeasA, MeasAT );
			MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 4, MeasAT, Weight, MeasA, ATA );
			MatrixInv( 4, ATA, InvATA );

			MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 1, MeasAT, Weight, BPos, ATB );
			MatrixMultiply( 4, 4, 4, 1, InvATA, ATB, dPos );

			*AprioriClk = *AprioriClk + dPos[3];

		}
		for( k=0; k<3; k++ )
		{
			ApriPos[k] = ApriPos[k] + dPos[k];
		}
		Iterator++;
		Coverage = VectDot( 3, 3, dPos, dPos );


	}while ( (Coverage>1E-6) && (Iterator < 10) );
	//ѭ���������߶Ƚ�

	/* ������λ����������  */
	/* ����۲�ֵ�в� */
	MatrixMultiply( SatNumUsed, 4, 4, 1, MeasA, dPos, Residual );//
	for( i=0; i<SatNumUsed; i++ )
	{
		Result->Residual[i] = Residual[i] - BPos[i];
	}

	/* ����۲�ֵ��������Э������ */

	ComputeQvv( SatNumUsed, 4, MeasA, InvATA, MeasAT, Weight, Qvv );

	Result->PDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] );
	Result->GDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] + InvATA[15] );
	Result->HDOP = sqrt( InvATA[0] + InvATA[5] );
	Result->VDOP = sqrt( InvATA[10] );
	Result->TDOP = sqrt( InvATA[15] );
	Result->Iterator = Iterator;
	Result->Coverage = Coverage;
	if(Iterator<10)
	{
		for( i=0; i<SatNumUsed; i++ )
		{
			SigmaPos = SigmaPos + Weight[i]*pow( Result->Residual[i], 2.0 );
		}
		SigmaPos = sqrt( SigmaPos / (SatNumUsed-4) );
	}
	else 
		SigmaPos = 999.0;
	return SigmaPos;
}

double LSRel_PC(const int satNum, int * PRN,int * banPRN,const double * satPos,const double * StateInECEFA,const double * obs
	,double * AprioriPosRel,double * AprioriClk, PPRESULT * Result)
{
	int i, j, k;                     /*  IteratorΪ���㶨λ��������  */
	int SatNumUsed,Iterator;      /*  ���㶨λʹ�õ������� */
	double ATA[16], InvATA[16];
	double ATB[4];
	double dPos[4];               /* ��λ���  */
	double BPos[MAXCHANNUM];                      /* α�������յĹ۲�ֵ-����ֵ */
	double MeasA[MAXCHANNUM*4], MeasAT[MAXCHANNUM*4];          /* �۲���� [MAXCHANNUM][5] */
	double Weight[MAXCHANNUM];                               /* Ȩ����, ��λ��ֻȡ�Խ���Ԫ�� */
	double Qvv[MAXCHANNUM];                               /* �۲�ֵ��������б������Խ���Ԫ�� */
	double RangeA,RangeB;                                 /*  ���ջ��뵼������֮��ľ���  */
	double Coverage,SigmaPos=0.0;
	double Residual[MAXCHANNUM] = {0.0};  /* ��λ�в�  */
	int valid =0;
	Iterator = 0;
	do{
		SatNumUsed = 0;

		for( i=0; i<MAXCHANNUM; i++ )
		{
			valid=1;
			for (j=0;j<12;j++)
			{
				if(PRN[i]==999|PRN[i]==banPRN[j]){
					valid=0;
					break;
				}
			}
			if (valid==1)
			{
				RangeA = 0.0;
				RangeB = 0.0;
				for( k=0; k<3; k++ )
				{
					RangeA = RangeA + pow( satPos[i*3+k]-StateInECEFA[k], 2.0 );
					RangeB = RangeB + pow( satPos[i*3+k]-StateInECEFA[k]-AprioriPosRel[k], 2.0 );
				}

				RangeA = sqrt( RangeA );
				RangeB = sqrt( RangeB );
				MeasA[SatNumUsed*4+0] = 1.0*( AprioriPosRel[0] + StateInECEFA[0] - satPos[i*3+0] )/RangeB;
				MeasA[SatNumUsed*4+1] = 1.0*( AprioriPosRel[1] + StateInECEFA[1] - satPos[i*3+1] )/RangeB;
				MeasA[SatNumUsed*4+2] = 1.0*( AprioriPosRel[2] + StateInECEFA[2] - satPos[i*3+2] )/RangeB;
				MeasA[SatNumUsed*4+3] = 1.0;             /* ���ջ�GPSϵͳ�Ӳ�ϵ�� */
				Weight[SatNumUsed]    = 1.0;  // * pow( sin(SatMidInfo[i].Elevation), 2.0 );

				BPos[SatNumUsed] =obs[i]-(RangeB-RangeA)-*AprioriClk;
				SatNumUsed++;
			}


		}

		/*  �������  */

		for( k=SatNumUsed; k<MAXCHANNUM; k++)  /* ���������� */
		{
			BPos[k] = 0.0;
			Weight[k] = 0.0;
			for( j=0; j<4; j++ )
			{
				MeasA[k*4+j] = 0.0;
			}
		}

		if( SatNumUsed>=4 )  /* �϶����� */
		{
			MatrixTranspose( SatNumUsed, 4, MeasA, MeasAT );
			MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 4, MeasAT, Weight, MeasA, ATA );
			MatrixInv( 4, ATA, InvATA );

			MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 1, MeasAT, Weight, BPos, ATB );
			MatrixMultiply( 4, 4, 4, 1, InvATA, ATB, dPos );

			*AprioriClk = *AprioriClk + dPos[3];

		}
		for( k=0; k<3; k++ )
		{
			AprioriPosRel[k] = AprioriPosRel[k] + dPos[k];
		}
		Iterator++;
		Coverage = VectDot( 3, 3, dPos, dPos );


	}while ( (Coverage>1E-6) && (Iterator < 10) );
	//ѭ���������߶Ƚ�

	/* ������λ����������  */
	/* ����۲�ֵ�в� */
	MatrixMultiply( SatNumUsed, 4, 4, 1, MeasA, dPos, Residual );//
	for( i=0; i<SatNumUsed; i++ )
	{
		Result->Residual[i] = Residual[i] - BPos[i];
	}

	/* ����۲�ֵ��������Э������ */

	ComputeQvv( SatNumUsed, 4, MeasA, InvATA, MeasAT, Weight, Qvv );

	Result->PDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] );
	Result->GDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] + InvATA[15] );
	Result->HDOP = sqrt( InvATA[0] + InvATA[5] );
	Result->VDOP = sqrt( InvATA[10] );
	Result->TDOP = sqrt( InvATA[15] );
	Result->Iterator = Iterator;
	Result->Coverage = Coverage;
	if(Iterator<10)
	{
		for( i=0; i<SatNumUsed; i++ )
		{
			SigmaPos = SigmaPos + Weight[i]*pow( Result->Residual[i], 2.0 );
		}
		SigmaPos = sqrt( SigmaPos / (SatNumUsed-4) );
	}
	else 
		SigmaPos = 999.0;
	return SigmaPos;
}

double LEGE(GPSTIME Time, const int satNum, int * PRN,int * banPRN,const double * satPos,const double * StateInECEFA,const double * obs
	,double * AprioriPosRel,double * AprioriClk, PPRESULT * Result)
{
	int i,j;
	int SatNumUsed=satNum;
	int bestBan=999;
	double bestSigma0,bestSigma1,tempSigma=999.0;
	bestSigma0=LSRel_PC(SatNumUsed,PRN,banPRN,satPos,StateInECEFA,obs,AprioriPosRel,AprioriClk, Result);
	for (i=0;i<SatNumUsed-5;i++)
	{
		bestSigma1=999.0;
		for (j=0;j<SatNumUsed;j++)
		{
			banPRN[i]=PRN[j];
			tempSigma=LSRel_PC(SatNumUsed-i-1,PRN,banPRN,satPos,StateInECEFA,obs,AprioriPosRel,AprioriClk, Result);
			banPRN[i]=999;
			if (tempSigma<bestSigma1)
			{
				bestSigma1=tempSigma;
				bestBan=PRN[j];
			}
		}
		//������޳�һ�����Ǻ���ƣ����޳���õ�
		if (bestSigma0>bestSigma1)
		{
			banPRN[i]=bestBan;
		}
		//����޳��󷴶�����ֹͣ�޳���
		else
		{
			banPRN[i]=999;
			break;
		}
		//����޳��󣬸��Ƶ�һ���̶ȣ���Ҳֹͣ�޳�
		if (bestSigma0>3*bestSigma1)
		{
			FILE * lege=fopen("OutFile\\lege.txt","a+");
			fprintf(lege,"%14.3f,%14.3f\n",Time.SecOfWeek,bestSigma1);
			fclose(lege);
			SatNumUsed=SatNumUsed-i-1;//�޳����������
			Result->SigmaPos=LSRel_PC(SatNumUsed,PRN,banPRN,satPos,StateInECEFA,obs,AprioriPosRel,AprioriClk, Result);
			Result->SatNumUsed = SatNumUsed;
			break;
		}
		//û�дﵽ����̶�������޳���
		else{
			bestSigma0=bestSigma1;
		}
	}
	//�޳���ֻʣ��5������ʱ����ԭ���������Ǽ���
	if(i==SatNumUsed-5)
	{
		for (j=0;j<MAXCHANNUM;j++)
		{
			banPRN[j]=999;
		}
		Result->SigmaPos=LSRel_PC(SatNumUsed,PRN,banPRN,satPos,StateInECEFA,obs,AprioriPosRel,AprioriClk, Result);
		Result->SatNumUsed = SatNumUsed;
	}
	return Result->SigmaPos;
}
/****************************************************************************
Raim_fde1

Ŀ��:
����: GPSTIME Time
����: const int satNum
����: int * PRN
����: int * banPRN
����: const double * satPos
����: const double * StateInECEFA
����: const double * obs
����: double * AprioriPosRel
����: double * AprioriClk
����: PPRESULT * Result
****************************************************************************/
double Raim_fde1(int * satNum, int * PRN,int * banPRN,const double * satPos,const double * StateInECEFA,const double * obs
	,double * AprioriPosRel,double * AprioriClk, PPRESULT * Result)
{
	int i,j;
	int SatNumUsed=*satNum;
	int bestBan=999;
	double bestSigma1,tempSigma=999.0;
	if(SatNumUsed>=6)
	{
		for (j=0;j<SatNumUsed;j++)
		{
			banPRN[0]=PRN[j];
			tempSigma=LSRel_PC(SatNumUsed-1,PRN,banPRN,satPos,StateInECEFA,obs,AprioriPosRel,AprioriClk, Result);
			banPRN[0]=999;
			if (j==0)
			{
				bestSigma1=tempSigma;
				continue;
			}
			if (tempSigma<bestSigma1)
			{
				bestSigma1=tempSigma;
				bestBan=PRN[j];
			}
		}
		banPRN[0]=bestBan;
		SatNumUsed=SatNumUsed-1;//�޳����������
		Result->SigmaPos=LSRel_PC(SatNumUsed,PRN,banPRN,satPos,StateInECEFA,obs,AprioriPosRel,AprioriClk, Result);
		*satNum=SatNumUsed;
	}
	else{
		Result->SigmaPos=999.0;
		Result->SatNumUsed=0;
	}
	return Result->SigmaPos;
}
//Raim_spp
void Raim_spp(EpochObsData * Epoch)
{
	int i,j;
	int SatNumUsed=0;
	int bestBan=999;
	double bestSigma1,tempSigma=999.0;
}
/****************************************************************************
valResult

Ŀ��:
����: sigmaPos			��������
����: simga0			��ǰ�����  //��۲������йأ���Ϊ��ǰ�����õ��ǵ�λȨ
����: int nv			�۲�ֵ��
����: int nx			���Ʋ�������
****************************************************************************/
int valResult(const double sigmaPos,const double sigma0,int nv,int nx)
{
	double VTPV=0.0,GDop=0.0;
	if (nv>nx)
	{
		VTPV=sigmaPos * sigmaPos*(nv-nx)/(sigma0*sigma0);
		if(VTPV>chisqr[nv-nx-1]){
			printf("chi-square error nv=%d vv=%.1f cs=%.1f",nv,VTPV,chisqr[nv-nx-1]);
			return 0;
		}
	}
	return 1;
}