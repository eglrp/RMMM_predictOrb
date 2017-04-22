#include "DataProcess.h"
int GenCommomObs(EpochObsData * Epoch_A,EpochObsData * Epoch_B,Common11Obs * tempComObs)
{
	int i,j,k;
	int commonObsNum=0;
	tempComObs->Time.Week		=Epoch_A->Time.Week;
	tempComObs->Time.SecOfWeek	=Epoch_A->Time.SecOfWeek;
	for(i=0;i<Epoch_A->SatNum;i++)
		for(j=0;j<Epoch_B->SatNum;j++)
		{
			if(Epoch_A->SatObs[i].System!=GPS||Epoch_A->SatObs[i].Used==0)//used==0��ʾ�߶Ƚ�С��10�ȣ�used==-1��ʾα��ֲ�춨
				break;
			if(Epoch_B->SatObs[j].System!=GPS||Epoch_B->SatObs[j].Used==0)//
				continue;
			if(Epoch_A->SatObs[i].Prn==Epoch_B->SatObs[j].Prn)
			{
				tempComObs->comobs[commonObsNum].PRN=Epoch_A->SatObs[i].Prn;
				for(k=0;k<3;k++)
					tempComObs->comobs[commonObsNum].satPos[k]=Epoch_A->SatObs[i].satPos[k];
				/*�ɵ��㶨λ����ĸ߶ȽǴ������ģ����Ƚϵ�*/
				tempComObs->comobs[commonObsNum].elevation[0]=Epoch_A->SatObs[i].elevation;
				tempComObs->comobs[commonObsNum].elevation[1]=Epoch_B->SatObs[j].elevation;
				for(k=0;k<MAXOBSTYPENUM;k++){
					//A��
					if(Epoch_A->SatObs[i].Data[k].Availability==1&&Epoch_A->SatObs[i].Data[k].Type==C1
						&&Epoch_A->SatObs[i].Data[k].LLI!=9)
						tempComObs->comobs[commonObsNum].CA1=Epoch_A->SatObs[i].Data[k].Obs;
					if(Epoch_A->SatObs[i].Data[k].Availability==1&&Epoch_A->SatObs[i].Data[k].Type==P1
						&&Epoch_A->SatObs[i].Data[k].LLI!=9)
						tempComObs->comobs[commonObsNum].P11=Epoch_A->SatObs[i].Data[k].Obs;
					if(Epoch_A->SatObs[i].Data[k].Availability==1&&Epoch_A->SatObs[i].Data[k].Type==P2
						&&Epoch_A->SatObs[i].Data[k].LLI!=9)
						tempComObs->comobs[commonObsNum].P12=Epoch_A->SatObs[i].Data[k].Obs;
					if(Epoch_A->SatObs[i].Data[k].Availability==1&&Epoch_A->SatObs[i].Data[k].Type==L1){
						tempComObs->comobs[commonObsNum].L11=Epoch_A->SatObs[i].Data[k].Obs;
						tempComObs->comobs[commonObsNum].LLI_L1A=Epoch_A->SatObs[i].Data[k].LLI;
					}
					if(Epoch_A->SatObs[i].Data[k].Availability==1&&Epoch_A->SatObs[i].Data[k].Type==L2){
						tempComObs->comobs[commonObsNum].L12=Epoch_A->SatObs[i].Data[k].Obs;
						tempComObs->comobs[commonObsNum].LLI_L2A=Epoch_A->SatObs[i].Data[k].LLI;
					}
					if(Epoch_A->SatObs[i].Data[k].Availability==1&&Epoch_A->SatObs[i].Data[k].Type==S1)
						tempComObs->comobs[commonObsNum].S11=Epoch_A->SatObs[i].Data[k].Obs;
					if(Epoch_A->SatObs[i].Data[k].Availability==1&&Epoch_A->SatObs[i].Data[k].Type==S2)
						tempComObs->comobs[commonObsNum].S12=Epoch_A->SatObs[i].Data[k].Obs;
					//B��
					if(Epoch_B->SatObs[j].Data[k].Availability==1&&Epoch_B->SatObs[j].Data[k].Type==C1
						&&Epoch_B->SatObs[j].Data[k].LLI!=9)
						tempComObs->comobs[commonObsNum].CA2=Epoch_B->SatObs[j].Data[k].Obs;
					if(Epoch_B->SatObs[j].Data[k].Availability==1&&Epoch_B->SatObs[j].Data[k].Type==P1
						&&Epoch_B->SatObs[j].Data[k].LLI!=9)
						tempComObs->comobs[commonObsNum].P21=Epoch_B->SatObs[j].Data[k].Obs;
					if(Epoch_B->SatObs[j].Data[k].Availability==1&&Epoch_B->SatObs[j].Data[k].Type==P2
						&&Epoch_B->SatObs[j].Data[k].LLI!=9)
						tempComObs->comobs[commonObsNum].P22=Epoch_B->SatObs[j].Data[k].Obs;
					if(Epoch_B->SatObs[j].Data[k].Availability==1&&Epoch_B->SatObs[j].Data[k].Type==L1){
						tempComObs->comobs[commonObsNum].L21=Epoch_B->SatObs[j].Data[k].Obs;
						tempComObs->comobs[commonObsNum].LLI_L1B=Epoch_B->SatObs[j].Data[k].LLI;
					}
					if(Epoch_B->SatObs[j].Data[k].Availability==1&&Epoch_B->SatObs[j].Data[k].Type==L2){
						tempComObs->comobs[commonObsNum].L22=Epoch_B->SatObs[j].Data[k].Obs;
						tempComObs->comobs[commonObsNum].LLI_L2B=Epoch_B->SatObs[j].Data[k].LLI;
					}
					if(Epoch_B->SatObs[j].Data[k].Availability==1&&Epoch_B->SatObs[j].Data[k].Type==S1)
						tempComObs->comobs[commonObsNum].S21=Epoch_B->SatObs[j].Data[k].Obs;
					if(Epoch_B->SatObs[j].Data[k].Availability==1&&Epoch_B->SatObs[j].Data[k].Type==S2)
						tempComObs->comobs[commonObsNum].S22=Epoch_B->SatObs[j].Data[k].Obs;
				}
				if(fabs(tempComObs->comobs[commonObsNum].CA1)>0.1&&fabs(tempComObs->comobs[commonObsNum].CA2)>0.1){
						tempComObs->comobs[commonObsNum].CAApplyEpoch=1;
						tempComObs->comobs[commonObsNum].dCA=tempComObs->comobs[commonObsNum].CA2-tempComObs->comobs[commonObsNum].CA1;
				}
				if(fabs(tempComObs->comobs[commonObsNum].L11)>0.1&&fabs(tempComObs->comobs[commonObsNum].L21)>0.1){
						tempComObs->comobs[commonObsNum].L1ApplyEpoch=1;
						tempComObs->comobs[commonObsNum].dL1=tempComObs->comobs[commonObsNum].L21-tempComObs->comobs[commonObsNum].L11;
				}
				if(fabs(tempComObs->comobs[commonObsNum].L12)>0.1&&fabs(tempComObs->comobs[commonObsNum].L22)>0.1){
						tempComObs->comobs[commonObsNum].L2ApplyEpoch=1;
						tempComObs->comobs[commonObsNum].dL2=tempComObs->comobs[commonObsNum].L22-tempComObs->comobs[commonObsNum].L12;
				}
				if(fabs(tempComObs->comobs[commonObsNum].P11)>0.1&&fabs(tempComObs->comobs[commonObsNum].P21)>0.1){
						tempComObs->comobs[commonObsNum].P1ApplyEpoch=1;
						tempComObs->comobs[commonObsNum].dP1=tempComObs->comobs[commonObsNum].P21-tempComObs->comobs[commonObsNum].P11;
				}
				if(fabs(tempComObs->comobs[commonObsNum].P12)>0.1&&fabs(tempComObs->comobs[commonObsNum].P22)>0.1){
						tempComObs->comobs[commonObsNum].P2ApplyEpoch=1;
						tempComObs->comobs[commonObsNum].dP2=tempComObs->comobs[commonObsNum].P22-tempComObs->comobs[commonObsNum].P12;
				}
				tempComObs->comobs[commonObsNum].flag=0;//��ʼ����Ϊ����������
				tempComObs->comobs[commonObsNum].used=1;//��ʼ��Ϊ�����Ǵֲ�
				//��۲�ֵ��ּ���ֲ�
				if ( fabs(tempComObs->comobs[commonObsNum].P11-tempComObs->comobs[commonObsNum].P12) > 30.0||
					fabs(tempComObs->comobs[commonObsNum].P21-tempComObs->comobs[commonObsNum].P22) > 30.0
					)
				{
					tempComObs->comobs[commonObsNum].P2ApplyEpoch= 0;							//P2�дֲ�
				}
				if ( fabs(tempComObs->comobs[commonObsNum].P11-tempComObs->comobs[commonObsNum].CA1)>10.0||
					fabs(tempComObs->comobs[commonObsNum].P21-tempComObs->comobs[commonObsNum].CA2)>10.0)
				{
					tempComObs->comobs[commonObsNum].P1ApplyEpoch= 0;							//P1�дֲ�
				}
				if (Epoch_A->SatObs[i].Used==-1||Epoch_B->SatObs[j].Used==-1)			//���ǰ��α��̽��Ϊ�ֲ�
				{
					tempComObs->comobs[commonObsNum].used=-1;
				}
				
				if(tempComObs->comobs[commonObsNum].L1ApplyEpoch==1&&tempComObs->comobs[commonObsNum].L2ApplyEpoch==1
					&&tempComObs->comobs[commonObsNum].P1ApplyEpoch==1&&tempComObs->comobs[commonObsNum].P2ApplyEpoch==1)
					commonObsNum++;//Ŀǰֻ����P1��P2��L1��L2�۲�ֵ��������һ�������Ϊ��ͬ�۲�����
				else
					EmptyOneSat11Obs(&tempComObs->comobs[commonObsNum]);
				break;
			}
		}
		tempComObs->ComSatNum=commonObsNum;
		return 1;
}
void copyOneSat11Obs(OneSat11Obs * target,OneSat11Obs * src)
{
	int i=0;
	target->used		=	src->used;
	for(i=0;i<2;i++)
		target->elevation[i]=src->elevation[i];
	for(i=0;i<3;i++)
		target->satPos[i]=	src->satPos[i];
	target->map[0]		=	src->map[0];
	target->map[1]		=	src->map[1];
	target->CA1			=	src->CA1;
	target->CA2			=	src->CA2;
	target->CAApplyEpoch=	src->CAApplyEpoch;
	target->dCA			=	src->dCA;
	target->dL1			=	src->dL1;
	target->dL2			=	src->dL2;
	target->dP1			=	src->dP1;
	target->dP2			=	src->dP2;
	target->elevation[0]=	src->elevation[0];
	target->elevation[1]=	src->elevation[1];
	target->flag		=	src->flag;
	target->L11			=	src->L11;
	target->L12			=	src->L12;
	target->L1ApplyEpoch=	src->L1ApplyEpoch;
	target->L21			=	src->L21;
	target->L22			=	src->L22;
	target->L2ApplyEpoch=	src->L2ApplyEpoch;
	target->P11			=	src->P11;
	target->P12			=	src->P12;
	target->P1ApplyEpoch=	src->P1ApplyEpoch;
	target->P21			=	src->P21;
	target->P22			=	src->P22;
	target->P2ApplyEpoch=	src->P2ApplyEpoch;
	target->PRN			=	src->PRN;
	target->S11			=	src->S11;
	target->S12			=	src->S12;
	target->S21			=	src->S21;
	target->S22			=	src->S22;
	//MW����̽���м����
	target->Nw_SD		=	src->Nw_SD;
	target->sigNw_SD	=	src->sigNw_SD;
	target->Nw_num_SD	=	src->Nw_num_SD;
	//MW����̽���м����
	target->Nw_A		=	src->Nw_A;
	target->sigNw_A		=	src->sigNw_A;
	target->Nw_num_A	=	src->Nw_num_A;
	//
	target->Nw_B		=	src->Nw_B;
	target->sigNw_B		=	src->sigNw_B;
	target->Nw_num_B	=	src->Nw_num_B;
}
void copyCommon11Obs(Common11Obs * targetComObs,Common11Obs * srcComObs)
{
	int i;
	targetComObs->ComSatNum		=	srcComObs->ComSatNum;
	targetComObs->Time.SecOfWeek=	srcComObs->Time.SecOfWeek;
	targetComObs->Time.Week		=	srcComObs->Time.Week;
	for(i=0;i<srcComObs->ComSatNum;i++){
		copyOneSat11Obs(&targetComObs->comobs[i],&srcComObs->comobs[i]);
	}
	for (i=srcComObs->ComSatNum;i<MAXCHANNUM;i++)
	{
		EmptyOneSat11Obs(&targetComObs->comobs[i]);
	}
}

//��ʧ���������������¹۲⣬��ʼ��ģ�����뷽��ģ��
//�����ʼ��ģ���������˲�����λ�ã��ܼӿ�����������Ҳ�����ܵ�Ӱ�죬���λ�þ�ȷ�������Լ�����λ����ƫ��
void initialAllAmb(double * Mat, EKFSTATE * EkfState)
{
	int i,j;
	double rhoA,rhoB,dx,dy,dz,dtr;
	double B[48]={0.0},Dl[12]={0.0};
	double stateInECEFRel[6]={0.0};
	/************************************************************************/
	/* ��ʼ�������ֵ                                                       */
	/************************************************************************/
	dtr=EkfState->StateRel[6];
	MatrixMultiply( 3, 3, 3, 1, Mat, EkfState->StateA, EkfState->StateInECEFA );//ÿ���۲�ֵ���¶�Ҫת���ع�ϵ��
	MatrixMultiply( 3, 3, 3, 1, Mat, EkfState->StateRel, stateInECEFRel );
	getBpos(EkfState->StateInECEFA,stateInECEFRel,EkfState->StateInECEFB);
	/************************************************************************/
	/* ��ʼ������                                                           */
	/************************************************************************/
	for(i=0;i<12;i++){
		EkfState->StateRel[12+i]=0.0;
		EkfState->StateRel[24+i]=0.0;
		for(j=0;j<RELDIMENSION;j++)
		{
			EkfState->CovRel[(12+i)+RELDIMENSION*j]=0.0;		//��i��L1ģ������ȫ��Ϊ0.0;
			EkfState->CovRel[(12+i)*RELDIMENSION+j]=0.0;		//��i��L1ģ������ȫ��Ϊ0.0;
			EkfState->CovRel[(24+i)+RELDIMENSION*j]=0.0;		//��i������ģ������ȫ��Ϊ0.0;
			EkfState->CovRel[(24+i)*RELDIMENSION+j]=0.0;		//��15+i��ȫ��Ϊ0.0;
		}
	}
	for(i=0;i<EkfState->CurComObs.ComSatNum;i++)
	{
		dx	=	EkfState->StateInECEFA[0]-EkfState->CurComObs.comobs[i].satPos[0];
		dy	=	EkfState->StateInECEFA[1]-EkfState->CurComObs.comobs[i].satPos[1];
		dz	=	EkfState->StateInECEFA[2]-EkfState->CurComObs.comobs[i].satPos[2];
		rhoA=	sqrt(dx*dx+dy*dy+dz*dz);
		dx=		EkfState->StateInECEFB[0]-EkfState->CurComObs.comobs[i].satPos[0];
		dy=		EkfState->StateInECEFB[1]-EkfState->CurComObs.comobs[i].satPos[1];
		dz=		EkfState->StateInECEFB[2]-EkfState->CurComObs.comobs[i].satPos[2];
		rhoB=	sqrt(dx*dx+dy*dy+dz*dz);
		EkfState->PRN[i]		=	EkfState->CurComObs.comobs[i].PRN;
		//ģ���ȳ�ʼ��
		EkfState->StateRel[24+i] = ( EkfState->CurComObs.comobs[i].dL1 - EkfState->CurComObs.comobs[i].dL2 )
										   - LEMTA_NL/LEMTA_WL * ( EkfState->CurComObs.comobs[i].dP1/LEMTA_1 + EkfState->CurComObs.comobs[i].dP2/LEMTA_2 );
		EkfState->StateRel[12+i]				=	1.0/(1.0-RATIO2)*EkfState->CurComObs.comobs[i].dL1*LEMTA_1-RATIO2/(1.0-RATIO2)*EkfState->CurComObs.comobs[i].dL2*LEMTA_2
											   -(rhoB-rhoA)-dtr;//����N=(l-Bx)/lemta����˷����ʼ��ͨ������
		//����λ�ó�ʼ��LC									   
		/*EkfState->State[15+i]				=	1.0/(1.0-RATIO)*EkfState->CurComObs.comobs[i].dL1-RATIO/(1.0-RATIO)*EkfState->CurComObs.comobs[i].dL2
											   -(rhoB-rhoA)/LEMTA_NL-dtr/LEMTA_NL-EkfState->State[EKFDIMENSION+15+i]*RATIO/(1.0-RATIO);
		����λ�ó�ʼ��MW��L1*/
		//ģ���ȷ����ʼ��
		EkfState->CovRel[(12+i)*RELDIMENSION+(12+i)]=SIGL1;//ģ���ȳ�ʼ��Ϊ3��
		EkfState->CovRel[(24+i)*RELDIMENSION+(24+i)]=SIGMW;//ģ���ȳ�ʼ��Ϊ3��
		//�����ʼ������۲����
		B[i*4+0] = dx/rhoB;
		B[i*4+1] = dy/rhoB;
		B[i*4+2] = dz/rhoB;
		B[i*4+3] = 1.0;
		Dl[i] = Me_dLC;
		//MW̽�������м������ʼ��
		EkfState->CurComObs.comobs[i].Nw_SD = ( EkfState->CurComObs.comobs[i].dL1 - EkfState->CurComObs.comobs[i].dL2 )
									     - LEMTA_NL/LEMTA_WL*( EkfState->CurComObs.comobs[i].dP1/LEMTA_1 + EkfState->CurComObs.comobs[i].dP2/LEMTA_2 );
		EkfState->CurComObs.comobs[i].Nw_num_SD = 1;
		EkfState->CurComObs.comobs[i].sigNw_SD	 = MW_SIG;
		//MW�ǲ�ģ���ȳ�ʼ��
		EkfState->CurComObs.comobs[i].Nw_A = ( EkfState->CurComObs.comobs[i].L11 - EkfState->CurComObs.comobs[i].L12 )
			- LEMTA_NL/LEMTA_WL*( EkfState->CurComObs.comobs[i].P11/LEMTA_1 + EkfState->CurComObs.comobs[i].P12/LEMTA_2 );
		EkfState->CurComObs.comobs[i].Nw_num_A = 1;
		EkfState->CurComObs.comobs[i].sigNw_A	 = MW_SIG;
		EkfState->CurComObs.comobs[i].Nw_B = ( EkfState->CurComObs.comobs[i].L21 - EkfState->CurComObs.comobs[i].L22 )
			- LEMTA_NL/LEMTA_WL*( EkfState->CurComObs.comobs[i].P21/LEMTA_1 + EkfState->CurComObs.comobs[i].P22/LEMTA_2 );
		EkfState->CurComObs.comobs[i].Nw_num_B = 1;
		EkfState->CurComObs.comobs[i].sigNw_B	 = MW_SIG;
	}
	//initialAmbsCov(B,Dl,EkfState,EkfState->Sigma[1]*EkfState->Sigma[1]);
	
}
void initialAmbsCov(const double B[48],const double Dl[12],EKFSTATE * EkfState,const double sigma)
{
	int i,j;
	double Qxx[16]={0.0},BT[48],BQ[48]={0.0},TempQ[144]={0.0};
	/************************************************************************/
	/* �����ʼ��ͨ����������QNN=Dl+B*QXX*BT                            */
	/************************************************************************/
	//���Qxx
	for(i=0;i<4;i++){
		for(j=0;j<4;j++)
		{
			Qxx[i*4+j]=EkfState->CovRel[i*RELDIMENSION+j];
		}
	}
	//���TempQ=B*QXX*BT
	MatrixMultiply(EkfState->CurComObs.ComSatNum,4,4,4,B,Qxx,BQ);
	MatrixTranspose(EkfState->CurComObs.ComSatNum,4,B,BT);
	MatrixMultiply(EkfState->CurComObs.ComSatNum,4,4,EkfState->CurComObs.ComSatNum,BQ,BT,TempQ);
	MatrixMultiplyk(EkfState->CurComObs.ComSatNum,EkfState->CurComObs.ComSatNum,TempQ,sigma);
	MatrixAddition3(EkfState->CurComObs.ComSatNum,Dl,TempQ);
	//����QNN
	for (i=0;i<EkfState->CurComObs.ComSatNum;i++)
	{
		for (j=0;j<EkfState->CurComObs.ComSatNum;j++)
		{
			EkfState->CovRel[(i+12)*RELDIMENSION+j+12]=TempQ[i*EkfState->CurComObs.ComSatNum+j];
		}
	}
}
/****************************************************************************
������:deletePRNCov
Ŀ��:ɾ������Ϊindex������ģ�������Э����
˼·;ͨ����index��ʼ���Ƚ������ս����һ����0
����:
		index		��Ҫɾ�������������������е�����
		ComSatNum	���������е���������
		Cov			�˲���Э����
���:
		Cov			ɾ��Ŀ��������Ϣ���Э����
****************************************************************************/
void deletePRNCov(int index,int ComSatNum,double * Cov)
{
	int i,j;
	//���и���
	for(i=index;i<ComSatNum-1;i++)
	{
		for(j=0;j<RELDIMENSION;j++)
		{
			Cov[(i+12)*RELDIMENSION+j]	=	Cov[(i+13)*RELDIMENSION+j];//L1ģ����Э�����и���
			Cov[(i+24)*RELDIMENSION+j]	=	Cov[(i+25)*RELDIMENSION+j];//����ģ����Э�����и���
		}
	}
	//���и���
	for(i=index;i<ComSatNum-1;i++)
	{
		for(j=0;j<RELDIMENSION;j++)
		{		
			Cov[j*RELDIMENSION+(i+12)]=Cov[j*RELDIMENSION+(i+13)];		//L1ģ����Э�����и���
			Cov[j*RELDIMENSION+(i+24)]=Cov[j*RELDIMENSION+(i+25)];		//����ģ����Э�����и���
		}
	}
	//Ȼ������һ�����ǵ�ģ����Э����������0
	j=ComSatNum-1;
	for(i=0;i<RELDIMENSION;i++){
		Cov[(12+j)*RELDIMENSION+i]=0.0;		//L1ģ����Э��������0
		Cov[(12+j)+RELDIMENSION*i]=0.0;		//L1ģ����Э��������0
		Cov[(24+j)*RELDIMENSION+i]=0.0;		//����ģ����Э��������0		
		Cov[(24+j)+RELDIMENSION*i]=0.0;		//����ģ����Э��������0
	}
}
/****************************************************************************
������:deletePRN
Ŀ��:ɾ������Ϊindex������L1�����ģ���ȡ�ģ�������Э����
˼·;ͨ����index��ʼ��������һ������ǰһ�����ݣ����ս����һ����0
����:
		index		��Ҫɾ�������������������е�����
		preComObs	ǰһ����Ԫ����ǰ�˲�״̬�����������м������Ϣ
		EkfState	�˲��ܽṹ�壬Ϊ�˵������е�ģ����������Э����
���:
		preComObs	ɾ��Ŀ��������Ϣ���preComObs
		EkfState	ɾ��Ŀ��������Ϣ���EkfState
****************************************************************************/
void deletePRN(int index,Common11Obs * preComObs,EKFSTATE * EkfState)
{
	int i,j;
	for(i=index;i<preComObs->ComSatNum-1;i++){
		copyOneSat11Obs(&preComObs->comobs[i],&preComObs->comobs[i+1]);
		EkfState->StateRel[12+i] = EkfState->StateRel[13+i];		//L1ģ����
		EkfState->StateRel[24+i] = EkfState->StateRel[25+i];		//����ģ����
	}
	EkfState->StateRel[12+preComObs->ComSatNum-1] = 0.0;
	EkfState->StateRel[24+preComObs->ComSatNum-1] = 0.0;
	//��0���һ��ģ���ȵ������Ϣ
	j=preComObs->ComSatNum-1;
	EmptyOneSat11Obs(&preComObs->comobs[j]);
	//ɾ�����ӦЭ����Cov
	deletePRNCov(index,preComObs->ComSatNum,EkfState->CovRel);
	
	preComObs->ComSatNum=preComObs->ComSatNum-1;
}
//��ͬһ��Ԫ���������н��е������ú����ǽ��������ǵ�����
//ע��Ҫ��ģ���Ⱥ�Э����ͬʱ����
void SwapOneSat11Obs(OneSat11Obs * target,OneSat11Obs * src)
{
	OneSat11Obs tempSatObs;				//�м����
	copyOneSat11Obs(&tempSatObs,target);
	copyOneSat11Obs(target,src);
	copyOneSat11Obs(src,&tempSatObs);
}
void getBpos(double * StateA,double * relState,double * StateB)/*ǰ6�����*/
{
	for (int i=0;i<6;i++)
	{
		StateB[i] = StateA[i] + relState[i];
	}
}
void getRelPos(double * StateA,double * StateB,double * relState)
{
	for (int i=0;i<6;i++)
	{
		relState[i] = StateB[i] - StateA[i];
	}
}
//���ڷ������������³��ֵ����ǳ�ʼ��ģ������Э����
void initialOneAmb(int index, double * Mat,EKFSTATE * EkfState,OneSat11Obs * obs)
{
	int i;
	double dx,dy,dz,rhoA,rhoB,dtr;
	double stateInECEFRel[6]={0.0};
	/************************************************************************/
	/* ��ʼ�������ֵ                                                       */
	/************************************************************************/
	dtr=EkfState->StateRel[6];
	MatrixMultiply( 3, 3, 3, 1, Mat, EkfState->StateA, EkfState->StateInECEFA );//ÿ���۲�ֵ���¶�Ҫת���ع�ϵ��
	MatrixMultiply( 3, 3, 3, 1, Mat, EkfState->StateRel, stateInECEFRel );
	getBpos(EkfState->StateInECEFA,stateInECEFRel,EkfState->StateInECEFB);
	/************************************************************************/
	/* ��ʼ������                                                           */
	/************************************************************************/
	for(i=0;i<RELDIMENSION;i++)
	{
		//L1ģ����
		EkfState->CovRel[(12+index)*RELDIMENSION+i]=0.0;
		EkfState->CovRel[(12+index)+RELDIMENSION*i]=0.0;
		//����ģ����
		EkfState->CovRel[(24+index)*RELDIMENSION+i]=0.0;
		EkfState->CovRel[(24+index)+RELDIMENSION*i]=0.0;
	}
	//ģ���ȳ�ʼ��
	dx	=	EkfState->StateInECEFA[0]-obs->satPos[0];
	dy	=	EkfState->StateInECEFA[1]-obs->satPos[1];
	dz	=	EkfState->StateInECEFA[2]-obs->satPos[2];
	rhoA=	sqrt(dx*dx+dy*dy+dz*dz);
	dx	=	EkfState->StateInECEFB[0]-obs->satPos[0];
	dy	=	EkfState->StateInECEFB[1]-obs->satPos[1];
	dz	=	EkfState->StateInECEFB[2]-obs->satPos[2];
	rhoB=	sqrt(dx*dx+dy*dy+dz*dz);
	EkfState->StateRel[24+index] = (obs->dL1-obs->dL2)-LEMTA_NL/LEMTA_WL*(obs->dP1/LEMTA_1+obs->dP2/LEMTA_2);

											
	EkfState->StateRel[12+index] = 1.0/(1.0-RATIO2)*obs->dL1*LEMTA_1-RATIO2/(1.0-RATIO2)*obs->dL2*LEMTA_2
								-(rhoB-rhoA)-dtr;
	
	/*EkfState->StateRel[12+index]				=1.0/(1.0-RATIO)*obs->dL1-RATIO/(1.0-RATIO)*obs->dL2
											   -(rhoB-rhoA)/LEMTA_NL-dtr/LEMTA_NL-EkfState->State[EKFDIMENSION+15+index]*RATIO/(1.0-RATIO);
	*/
	EkfState->CovRel[(12+index)+RELDIMENSION*(12+index)] = SIGL1;//��ʼ��ģ���ȷ���Ϊ��
	EkfState->CovRel[(24+index)+RELDIMENSION*(24+index)] = SIGMW;//��ʼ��ģ���ȷ���Ϊ��
	//����̽��ģ���ȳ�ʼ��
	obs->Nw_SD	=(obs->dL1-obs->dL2)
			-LEMTA_NL/LEMTA_WL*(obs->dP1/LEMTA_1+obs->dP2/LEMTA_2);
	obs->Nw_num_SD=1;
	obs->sigNw_SD=MW_SIG;
	//�ǲ�����̽��ģ���ȳ�ʼ��
	obs->Nw_A	=(obs->L11-obs->L12)
		-LEMTA_NL/LEMTA_WL*(obs->P11/LEMTA_1+obs->P12/LEMTA_2);
	obs->Nw_num_A=1;
	obs->sigNw_A=MW_SIG;

	obs->Nw_B	=(obs->L21-obs->L22)
		-LEMTA_NL/LEMTA_WL*(obs->P21/LEMTA_1+obs->P22/LEMTA_2);
	obs->Nw_num_B=1;
	obs->sigNw_B=MW_SIG;
}
//��������̽��������
int CySlipDetection_SD(double * Mat, Common11Obs * preComObs,Common11Obs * curComObs,EKFSTATE * EkfState)
{
	int i,j,isSearch,flag;
	int search[12]={0};
	char slipFilePath_GF[60]="OutFile\\cycleSlip.GF-SD";
	char slipFilePath_MW[60]="OutFile\\cycleSlip.MW-SD";
	char gfFilePath_SD[60]="OutFile\\gf-SD.txt";
	char mwFilePath_SD[60]="OutFile\\mw-SD.txt";
	FILE * cycleSlipFile_GF=fopen(slipFilePath_GF,"a+");
	FILE * cycleSlipFile_MW=fopen(slipFilePath_MW,"a+");
	FILE * gfFile_SD=fopen(gfFilePath_SD,"a+");
	FILE * mwFile_SD=fopen(mwFilePath_SD,"a+");
	int isHaveSlip_GF=0,isHaveSlip_MW=0;
	double tempLC=0.0,tempMW=0.0;
	/*����ʱ��ͷ*/
	double deltT=curComObs->Time.SecOfWeek-preComObs->Time.SecOfWeek+(curComObs->Time.Week-preComObs->Time.Week)*86400.0;
	if(preComObs->ComSatNum==0||fabs(deltT)>INTERVAL){
		copyCommon11Obs(&EkfState->CurComObs,curComObs);
		initialAllAmb(Mat,EkfState);
		fclose(cycleSlipFile_GF);
		fclose(cycleSlipFile_MW);
		fclose(gfFile_SD);
		fclose(mwFile_SD);
		return 0;//��һ����Ԫ������̽������
	}
	//TRACE("test");
	for(i=0;i<preComObs->ComSatNum;i++)
	{
		isSearch=0;
		for(j=0;j<curComObs->ComSatNum;j++)
		{
			if(preComObs->comobs[i].PRN==curComObs->comobs[j].PRN)
			{
				flag=0;
				isSearch=1;
				SwapOneSat11Obs(&curComObs->comobs[i],&curComObs->comobs[j]);
				search[i]=1;
				flag=GFDetection_SD(&preComObs->comobs[i],&curComObs->comobs[i], &tempLC);
				fprintf(gfFile_SD,"%12.3f %4d %12.3f\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempLC);
				if(flag==1){
					fprintf(cycleSlipFile_GF,"%12.3f %4d\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempLC);
					isHaveSlip_GF=1;
				}
				flag=MWDetection_SD(&preComObs->comobs[i],&curComObs->comobs[i], &tempMW);
				fprintf(mwFile_SD,"%12.3f %4d %12.3f\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempMW);
				if(flag==1){
					fprintf(cycleSlipFile_MW,"%12.3f %4d %14.3f\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempMW);
					isHaveSlip_MW=1;
				}
				break;
			}
		}
		if(isSearch==0)
		{
			deletePRN(i,preComObs,EkfState);//��preComObs->comobs[i]��ɾ�������ǣ��Լ�ɾ��Ekf�и����ǵ������Ϣ
			i--;
		}
	}
	for(i=0;i<curComObs->ComSatNum;i++)
	{
		if(curComObs->comobs[i].flag==1&&(curComObs->comobs[i].PRN==preComObs->comobs[i].PRN))
			initialOneAmb(i, Mat,EkfState,&curComObs->comobs[i]);
		if(search[i]==0){
			curComObs->comobs[i].flag=2;
			initialOneAmb(i, Mat,EkfState,&curComObs->comobs[i]);
		}
	}
	/*�����˲��е�ǰ�۲�ֵ*/
	copyCommon11Obs(preComObs,curComObs);

	fclose(cycleSlipFile_GF);
	fclose(cycleSlipFile_MW);
	fclose(gfFile_SD);
	fclose(mwFile_SD);
	return 1;
}
int CySlipDetection_LLI(double *Mat,Common11Obs * preComObs,Common11Obs * curComObs,EKFSTATE * EkfState){

	int i,j,isSearch,flag;
	int search[12]={0};
	int isHaveSlip_GF=0,isHaveSlip_MW=0;
    double tempMW,tempLC; 
	double deltT=curComObs->Time.SecOfWeek-preComObs->Time.SecOfWeek+(curComObs->Time.Week-preComObs->Time.Week)*86400.0;
	if(preComObs->ComSatNum==0||fabs(deltT)>INTERVAL){
		copyCommon11Obs(&EkfState->CurComObs,curComObs);
		initialAllAmb(Mat,EkfState);
		return 0;//��һ����Ԫ������̽������
	}
	//TRACE("test");
	for(i=0;i<preComObs->ComSatNum;i++)
	{
		isSearch=0;
		for(j=0;j<curComObs->ComSatNum;j++)
		{
			if(preComObs->comobs[i].PRN==curComObs->comobs[j].PRN)
			{
				flag=0;
				isSearch=1;
				SwapOneSat11Obs(&curComObs->comobs[i],&curComObs->comobs[j]);
				search[i]=1;
				if(curComObs->comobs[i].LLI_L1A==1||curComObs->comobs[i].LLI_L2A==1
					||curComObs->comobs[i].LLI_L1B==1||curComObs->comobs[i].LLI_L2B==1)
					curComObs->comobs[i].flag = -1;
				break;
			}
		}
		if(isSearch==0)
		{
			deletePRN(i,preComObs,EkfState);//��preComObs->comobs[i]��ɾ�������ǣ��Լ�ɾ��Ekf�и����ǵ������Ϣ
			i--;
		}
	}
	for(i=0;i<curComObs->ComSatNum;i++)
	{
		if(curComObs->comobs[i].flag==1&&(curComObs->comobs[i].PRN==preComObs->comobs[i].PRN))
			initialOneAmb(i, Mat,EkfState,&curComObs->comobs[i]);
		if(search[i]==0){
			curComObs->comobs[i].flag=2;
			initialOneAmb(i, Mat,EkfState,&curComObs->comobs[i]);
		}
	}
	/*�����˲��е�ǰ�۲�ֵ*/
	copyCommon11Obs(preComObs,curComObs);
	return 1;
}
int CySlipDetection_ND(double * Mat, Common11Obs * preComObs,Common11Obs * curComObs,EKFSTATE * EkfState)
{
	int i,j,isSearch,flag;
	int search[12]={0};
	char slipFilePath_GF[60]="OutFile\\cycleSlip.GF";
	char slipFilePath_MW[60]="OutFile\\cycleSlip.MW";
	char gfFilePath_ND[60]="OutFile\\gf-ND.txt";
	char mwFilePath_ND[60]="OutFile\\mw-ND.txt";
	FILE * cycleSlipFile_GF=fopen(slipFilePath_GF,"a+");
	FILE * cycleSlipFile_MW=fopen(slipFilePath_MW,"a+");
	FILE * gfFile_ND=fopen(gfFilePath_ND,"a+");
	FILE * mwFile_ND=fopen(mwFilePath_ND,"a+");
	int isHaveSlip_GF=0,isHaveSlip_MW=0;
	double tempLC[2]={0.0},tempMW[2]={0.0};
	/*����ʱ��ͷ*/
	double deltT=curComObs->Time.SecOfWeek-preComObs->Time.SecOfWeek+(curComObs->Time.Week-preComObs->Time.Week)*86400.0;
	if(preComObs->ComSatNum==0||fabs(deltT)>INTERVAL){
		copyCommon11Obs(&EkfState->CurComObs,curComObs);
		initialAllAmb(Mat,EkfState);
		fclose(cycleSlipFile_GF);
		fclose(cycleSlipFile_MW);
		fclose(gfFile_ND);
		fclose(mwFile_ND);
		return 0;//��һ����Ԫ������̽������
	}
	//TRACE("test");
	for(i=0;i<preComObs->ComSatNum;i++)
	{
		isSearch=0;
		for(j=0;j<curComObs->ComSatNum;j++)
		{
			if(preComObs->comobs[i].PRN==curComObs->comobs[j].PRN)
			{
				flag=0;
				isSearch=1;
				SwapOneSat11Obs(&curComObs->comobs[i],&curComObs->comobs[j]);
				search[i]=1;
				flag=GFDetection_ND(&preComObs->comobs[i],&curComObs->comobs[i], tempLC);
				fprintf(gfFile_ND,"%12.3f %4d %12.3f %12.3f\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempLC[0],tempLC[1]);
				if(flag==1){
					fprintf(cycleSlipFile_GF,"%12.3f %4d %12.3f %12.3f\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempLC[0],tempLC[1]);
				}
				flag=MWDetection_ND(&preComObs->comobs[i],&curComObs->comobs[i],tempMW);
				fprintf(mwFile_ND,"%12.3f %4d %12.3f %12.3f\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempMW[0],tempMW[1]);
				if(flag==1){
					fprintf(cycleSlipFile_MW,"%12.3f %4d %12.3f %12.3f\n",curComObs->Time.SecOfWeek,preComObs->comobs[i].PRN,tempMW[0],tempMW[1]);
				}
				break;
			}
		}
		if(isSearch==0)
		{
			deletePRN(i,preComObs,EkfState);//��preComObs->comobs[i]��ɾ�������ǣ��Լ�ɾ��Ekf�и����ǵ������Ϣ
			i--;
		}
	}
	for(i=0;i<curComObs->ComSatNum;i++)
	{
		if(curComObs->comobs[i].flag==1&&(curComObs->comobs[i].PRN==preComObs->comobs[i].PRN))
			initialOneAmb(i, Mat,EkfState,&curComObs->comobs[i]);
		if(search[i]==0){
			curComObs->comobs[i].flag=2;
			initialOneAmb(i, Mat,EkfState,&curComObs->comobs[i]);
		}
	}
	/*�����˲��е�ǰ�۲�ֵ*/
	copyCommon11Obs(preComObs,curComObs);
	fclose(cycleSlipFile_GF);
	fclose(cycleSlipFile_MW);
	fclose(gfFile_ND);
	fclose(mwFile_ND);
	return 1;
}
/******************************************
COMBINED����̽������
�ο�:����ƽ��ʿ���ģ�����GPS�͹����Ƕ������ۼ������о�
����:
���:
******************************************/
int combDetection()
{
	return 1;
}
/******************************************
�����в�̽��������-����۲�ֵ
����:
		����iǰһ����ԪdL1�۲�ֵ��dL2�۲�ֵ
		����i�ĵ�ǰ��ԪdL1�۲�ֵ��dL2�۲�ֵ
�����
		������ʶflag
		-1:		���ݲ���ȫ��̽������ʧ��
		 0:		δ��������
		 1:		��������
******************************************/
int GFDetection_SD(OneSat11Obs * PreObs,OneSat11Obs * CurObs, double * dGF)
{
	double delta_GF;
	if(fabs(PreObs->dL1)>0.1&&fabs(PreObs->dL2)>0.1&&
		fabs(CurObs->dL1)>0.1&&fabs(CurObs->dL2)>0.1)
	{
		delta_GF=LEMTA_1*(CurObs->dL1-PreObs->dL1)+LEMTA_2*(PreObs->dL2-CurObs->dL2);//�ز��۲�ֵ��λΪ��
		*dGF=delta_GF;
		if(fabs(delta_GF)>GF_THRES)				//����gf_thres����Ϊ������
		{
			CurObs->flag=1;
		}
		return CurObs->flag;
	}
	return -1;
}
/******************************************
�����в�̽��������-�ǲ�۲�ֵ
����:
		����iǰһ����ԪdL1�۲�ֵ��dL2�۲�ֵ
		����i�ĵ�ǰ��ԪdL1�۲�ֵ��dL2�۲�ֵ
�����
		������ʶflag
		-1:		���ݲ���ȫ��̽������ʧ��
		 0:		δ��������
		 1:		��������
******************************************/
int GFDetection_ND(OneSat11Obs * PreObs,OneSat11Obs * CurObs, double * dGF)
{
	double delta_GF;
	double preL1,preL2,curL1,curL2;
	double gflimitA=GF_THRES,gflimitB=GF_THRES;
	/*A��*/
	preL1=PreObs->L11;preL2=PreObs->L12;
	curL1=CurObs->L11;curL2=CurObs->L12;
	delta_GF=LEMTA_1*(curL1-preL1)+LEMTA_2*(preL2-curL2);//�ز��۲�ֵ��λΪ��
	dGF[0]=fabs(delta_GF);
	if(fabs(delta_GF)>gflimitA)				//����gf_thres����Ϊ������
	{
		CurObs->flag=1;
	}
	
	/*B��*/
	preL1=PreObs->L21;preL2=PreObs->L22;
	curL1=CurObs->L21;curL2=CurObs->L22;
	delta_GF=LEMTA_1*(curL1-preL1)+LEMTA_2*(preL2-curL2);//�ز��۲�ֵ��λΪ��
	dGF[1]=fabs(delta_GF);
	if(fabs(delta_GF)>gflimitB)				//����gf_thres����Ϊ������
	{
		CurObs->flag=1;
	}
	return CurObs->flag;
}
/***************************************
//MW��̽������-����۲�ֵ
����:
		����i��ǰһ��Ԫ��dL1,dL2,dP1,dP2�۲�ֵ
		����i�ĵ�ǰ��Ԫ��dL1,dL2,dP1,dP2�۲�ֵ
�����
		������ʶflag
		-1:		���ݲ���ȫ��̽������ʧ��,���ɹ�ͬ����ʱ�����ݽ��й�Ҫ�󣬱���������ȫ
		 0:		δ��������
		 1:		��������
****************************************/
int MWDetection_SD(OneSat11Obs * PreObs,OneSat11Obs * CurObs, double * tempMW)
{
	double temp,preSig2;
	temp=(CurObs->dL1-CurObs->dL2)-LEMTA_NL/LEMTA_WL*(CurObs->dP1/LEMTA_1+CurObs->dP2/LEMTA_2);
	*tempMW=fabs(temp-PreObs->Nw_SD);
	preSig2=PreObs->sigNw_SD*PreObs->sigNw_SD;
	if(fabs(temp-PreObs->Nw_SD)<(4*PreObs->sigNw_SD))
	{
		CurObs->Nw_num_SD=PreObs->Nw_num_SD+1;
		CurObs->Nw_SD=PreObs->Nw_SD+1.0/CurObs->Nw_num_SD*(temp-PreObs->Nw_SD);
		CurObs->sigNw_SD=sqrt(preSig2+1.0/CurObs->Nw_num_SD*((temp-PreObs->Nw_SD)*(temp-PreObs->Nw_SD)-preSig2));
		CurObs->flag	=0;
	}
	else
	{
		CurObs->flag	=1;
		CurObs->Nw_SD	=temp;
		CurObs->sigNw_SD=MW_SIG;
	}
	return CurObs->flag;
}
/***************************************
//MW��̽������-�ǲ�۲�ֵ
����:
		����i��ǰһ��Ԫ��dL1,dL2,dP1,dP2�۲�ֵ
		����i�ĵ�ǰ��Ԫ��dL1,dL2,dP1,dP2�۲�ֵ
�����
		������ʶflag
		-1:		���ݲ���ȫ��̽������ʧ��
		 0:		δ��������
		 1:		��������
****************************************/
int MWDetection_ND(OneSat11Obs *  PreObs,OneSat11Obs *  CurObs,double * tempMW)
{
	double temp,preSig2;
	/*A������̽��*/
	temp=(CurObs->L11-CurObs->L12)-LEMTA_NL/LEMTA_WL*(CurObs->P11/LEMTA_1+CurObs->P12/LEMTA_2);
	preSig2=PreObs->sigNw_A*PreObs->sigNw_A;
	tempMW[0]=fabs(temp-PreObs->Nw_A);
	if(fabs(temp-PreObs->Nw_A)<(4*PreObs->sigNw_A))
	{
		CurObs->Nw_num_A=PreObs->Nw_num_A+1;
		CurObs->Nw_A=PreObs->Nw_A+1.0/CurObs->Nw_num_A*(temp-PreObs->Nw_A);
		CurObs->sigNw_A=sqrt(preSig2+1.0/CurObs->Nw_num_A*((temp-PreObs->Nw_A)*(temp-PreObs->Nw_A)-preSig2));
	}
	else
	{
		CurObs->flag	=1;
		CurObs->used	=-1;
		CurObs->Nw_A	=temp;
		CurObs->sigNw_A	=MW_SIG;
	}
	
	/*B������̽��*/
	
	temp=(CurObs->L21-CurObs->L22)-LEMTA_NL/LEMTA_WL*(CurObs->P21/LEMTA_1+CurObs->P22/LEMTA_2);
	preSig2=PreObs->sigNw_B*PreObs->sigNw_B;
	tempMW[1]=fabs(temp-PreObs->Nw_B);
	if(fabs(temp-PreObs->Nw_B)<(4*PreObs->sigNw_B))
	{
		CurObs->Nw_num_B=PreObs->Nw_num_B+1;
		CurObs->Nw_B=PreObs->Nw_B+1.0/CurObs->Nw_num_B*(temp-PreObs->Nw_B);
		CurObs->sigNw_B=sqrt(preSig2+1.0/CurObs->Nw_num_B*((temp-PreObs->Nw_B)*(temp-PreObs->Nw_B)-preSig2));
	}
	else
	{
		CurObs->flag	=1;
		CurObs->used	=-1;
		CurObs->Nw_B	=temp;
		CurObs->sigNw_B	=MW_SIG;
	}
	
	return CurObs->flag;
}
//���ø߶ȽǼ�������ͶӰ����
double getMap(double radEle)
{
	double map=0.0;
	//Learģ��
	double sinE=sin(radEle);
	map=2.037/(sqrt(sinE*sinE+0.076)+sinE);
	return map;
}

//************************************
// ��������:  relEKFilter_LP
// ����˵��:  α����Բ�������
// ����:      RJ
// ʱ��:   	  2016/11/25
// ����ֵ:    void
// ����:      double Mat[9]
// ����:      EKFSTATE * KFState
//************************************
int relEKFilter_PC(double Mat[9],EKFSTATE * KFState)
{
	int    i, j, k;
	double stateInECEFRel[6]={0.0};
	double H[RELDIMENSION];	/* �۲ⷽ�����Ի�ϵ������ */
	double RangeA,RangeB;				/* ���ջ��뵼������֮��ľ������ֵ */
	double dRange,dtr;				/*�������ֵ*/
	double dPos_A[3],dPos_B[3];				/* ���ջ������뵼������λ��֮�� */
	double Adtr,AECF[3], ARangeB; /*��ǰ״̬����������״̬������Ϣʹ��*/
	double OC_LP;
	double OC_LP0[12],OC_LP1[12];
	int    satNumWhole=0;
	FILE * fPC;
	fPC=fopen( "OutFile\\LPOC.txt", "a+" );
	fprintf( fPC, "%12.3f\n", KFState->Time.SecOfWeek );
	//--------------------------------------------------------------------------------
	//����в�
	//CopyArray( 2*EKFDIMENSION, ApriState, KFState->State );
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateA, KFState->StateInECEFA );//ÿ���۲�ֵ���¶�Ҫת���ع�ϵ��
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, stateInECEFRel );
	getBpos( KFState->StateInECEFA, stateInECEFRel, KFState->StateInECEFB);
	//��ǰ״̬����ǰ�в���
	CopyArray(3,AECF,KFState->StateInECEFB);
	Adtr=KFState->StateRel[6];
	//��ͬ������
	for(i=0;i<KFState->CurComObs.ComSatNum;i++)
	{
		if(KFState->CurComObs.comobs[i].used!=1)
			continue;
		fprintf( fPC, "%12d",KFState->CurComObs.comobs[i].PRN );
		satNumWhole++;
	}
	fprintf(fPC,"\n");
	//--------------------------------------------------------------------------------
	if ( satNumWhole<4 )
	{
		return 0;
	}
	
	for(i=0;i<KFState->CurComObs.ComSatNum;i++)
	{
		MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, stateInECEFRel );
		getBpos( KFState->StateInECEFA, stateInECEFRel, KFState->StateInECEFB);
		for( j=0; j<RELDIMENSION; j++ )
		{
			H[j]	= 0.0;
		}
		//������Բ�������ǰ������Զ�λ���߿�������Ϣ�ֲ�̽���ֶ�
		if(KFState->CurComObs.comobs[i].used == 1 )
		{
			RangeA=0.0;RangeB=0.0;
			for(k=0;k<3;k++){
				dPos_A[k]=(KFState->StateInECEFA[k]-KFState->CurComObs.comobs[i].satPos[k]);
				RangeA+=dPos_A[k]*dPos_A[k];
				dPos_B[k]=(KFState->StateInECEFB[k]-KFState->CurComObs.comobs[i].satPos[k]);
				RangeB+=dPos_B[k]*dPos_B[k];
			}

			RangeA	=	sqrt(RangeA);
			RangeB	=	sqrt(RangeB);
			dRange	=	RangeB-RangeA;
			dtr		=	KFState->StateRel[6];
			OC_LP	=	1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP1 - RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP2
							-dRange-dtr;
			//λ�ù۲���󣬹۲�ֵ�ع�ϵ�µ�
			H[0]	= 1.0 * ( dPos_B[0] * Mat[0] + dPos_B[1] * Mat[3] + dPos_B[2] * Mat[6] )/RangeB;
			H[1]	= 1.0 * ( dPos_B[0] * Mat[1] + dPos_B[1] * Mat[4] + dPos_B[2] * Mat[7] )/RangeB;
			H[2]	= 1.0 * ( dPos_B[0] * Mat[2] + dPos_B[1] * Mat[5] + dPos_B[2] * Mat[8] )/RangeB;
				//�Ӳ�۲����
			H[6]	= 1.0;
			EKFMeasureUpdateRel(OC_LP, 1, H, KFState );	//Lp��������
		}
	}
	KFState->ApriSigma[1] = 0.0;
	KFState->PostSigma[1] = 0.0;
	//--------------------------------------------------------------------------------
	//�в�
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, stateInECEFRel );
	getBpos( KFState->StateInECEFA, stateInECEFRel, KFState->StateInECEFB);
	satNumWhole=0;
	for(i=0;i<KFState->CurComObs.ComSatNum;i++)
	{
		if(KFState->CurComObs.comobs[i].used!=1)
			continue;

		RangeA=0.0;RangeB=0.0;ARangeB=0.0;

		for(k=0;k<3;k++){
			RangeA = RangeA+pow(KFState->StateInECEFA[k]-KFState->CurComObs.comobs[i].satPos[k],2.0);
			RangeB = RangeB+pow(KFState->StateInECEFB[k]-KFState->CurComObs.comobs[i].satPos[k],2.0);
			ARangeB= ARangeB+pow(AECF[k]-KFState->CurComObs.comobs[i].satPos[k],2.0);
		}

		RangeA=sqrt(RangeA);
		RangeB=sqrt(RangeB);
		ARangeB=sqrt(ARangeB);
		dtr		=KFState->StateRel[6];
		OC_LP0[satNumWhole]	=1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP1-
			RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP2
			-(ARangeB-RangeA)-Adtr;
		OC_LP1[satNumWhole]	=1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP1-
			RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP2
			-(RangeB-RangeA)-dtr;
		KFState->ApriSigma[1] = KFState->ApriSigma[1] + pow( OC_LP0[satNumWhole], 2.0);
		KFState->PostSigma[1] = KFState->PostSigma[1] + pow( OC_LP1[satNumWhole], 2.0 );

		satNumWhole++;
	}
	if( satNumWhole > 0 )
	{
		KFState->ApriSigma[1] = sqrt( KFState->ApriSigma[1] / satNumWhole );
		KFState->PostSigma[1] = sqrt( KFState->PostSigma[1] / satNumWhole );
	}
	else
	{
		KFState->ApriSigma[1] = 999.0;
		KFState->PostSigma[1] = 999.0;
	}
	outRes(satNumWhole,fPC,OC_LP0,OC_LP1,KFState);
	outResEKFPC(satNumWhole,OC_LP0,OC_LP1,KFState);
	KFState->SatNumUsed[1] = satNumWhole;
	KFState->sigPosPC=KFState->PostSigma[1];
	EKFConvergencyCheck(1,KFState);
	return satNumWhole;
	//--------------------------------------------------------------------------------
}

//************************************
// ��������:  relEKFilter_LC
// ����˵��:  �ز���Բ�������
// ����:      RJ
// ʱ��:   	  2016/11/25
// ����ֵ:    void
// ����:      double Mat[9]
// ����:      EKFSTATE * KFState
//************************************
int relEKFilter_LC(double Mat[9],EKFSTATE * KFState)
{
	int    i, j, k;
	double stateInECEFRel[6]={0.0};
	double AL1[12]={0.0};
	double AW[12]={0.0};	
	double H[RELDIMENSION];	/* �۲ⷽ�����Ի�ϵ������ */
	double RangeA,RangeB;				/* ���ջ��뵼������֮��ľ������ֵ */
	double dRange,dtr;				/*�������ֵ*/
	double dPos_A[3],dPos_B[3];				/* ���ջ������뵼������λ��֮�� */
	double AECF[3], ARangeB,Adtr; /*��ǰ״̬����������״̬������Ϣʹ��*/
	double OC_Lc;
	double OC_LC0[12],OC_LC1[12];
	double LC_PC0=0.0,LC_PC1=0.0;//�˲�ǰ���޵�����ز���ȥ�޵����α�࣬��ֵΪ0˵��������ģ���ȴ�����ȷ
	FILE   *fLC=fopen( "OutFile\\LCOC.txt", "a+" );
	char   lc_str[60];
	int    satNumWhole=0;
	
	fprintf( fLC, "%12.3f\n", KFState->Time.SecOfWeek );

	//--------------------------------------------------------------------------------
	//����в�
	//CopyArray( 2*EKFDIMENSION, ApriState, KFState->State );
	//traceMat(RELDIMENSION,RELDIMENSION,KFState->CovRel);
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateA, KFState->StateInECEFA );//ÿ���۲�ֵ���¶�Ҫת���ع�ϵ��
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, stateInECEFRel );
	getBpos( KFState->StateInECEFA, stateInECEFRel, KFState->StateInECEFB);
	CopyArray(3,AECF,KFState->StateInECEFB);
	Adtr=KFState->StateRel[6];
	for (i=0;i<12;i++)
	{
		AL1[i] = KFState->StateRel[12+i];
		AW[i] = KFState->StateRel[24+i];
	}

	for(i=0;i<KFState->CurComObs.ComSatNum;i++)
	{
		if(KFState->CurComObs.comobs[i].flag!=0||KFState->CurComObs.comobs[i].used!=1)
			continue;
		fprintf(fLC,"%12d",KFState->CurComObs.comobs[i].PRN);
		satNumWhole++;
	}
	//--------------------------------------------------------------------------------
	fprintf( fLC, "\n" );
	if ( satNumWhole<4 )
	{
		fclose(fLC);
		return 0;
	}
	
	for(i=0;i<KFState->CurComObs.ComSatNum;i++)
	{
		for( j=0; j<RELDIMENSION; j++ )
		{
			H[j]	= 0.0;
		}
		//������Բ�������ǰ������Զ�λ���߿�������Ϣ�ֲ�̽���ֶ�
		if(KFState->CurComObs.comobs[i].flag==0&&KFState->CurComObs.comobs[i].used==1)
		{
			MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, stateInECEFRel );
			getBpos( KFState->StateInECEFA, stateInECEFRel, KFState->StateInECEFB);
			RangeA=0.0;RangeB=0.0;
			for(k=0;k<3;k++){
				dPos_A[k]=(KFState->StateInECEFA[k]-KFState->CurComObs.comobs[i].satPos[k]);
				RangeA+=dPos_A[k]*dPos_A[k];
				dPos_B[k]=(KFState->StateInECEFB[k]-KFState->CurComObs.comobs[i].satPos[k]);
				RangeB+=dPos_B[k]*dPos_B[k];
			}

			RangeA	=	sqrt(RangeA);
			RangeB	=	sqrt(RangeB);
			dRange	=	RangeB-RangeA;
			dtr		=	KFState->StateRel[6];
			OC_Lc	=	1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL1*LEMTA_1 - RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL2*LEMTA_2
						-dRange-dtr-KFState->StateRel[12+i];//�����ģ����
			//LEMTA_NL*KFState->State[15+i]-Hwl*KFState->State[15+i+EKFDIMENSION];
			/*
			Obs_MW	=LEMTA_WL*(KFState->CurComObs.comobs[i].dL1			-	KFState->CurComObs.comobs[i].dL2)-
					 LEMTA_NL*(KFState->CurComObs.comobs[i].dP1/LEMTA_1	+	KFState->CurComObs.comobs[i].dP2/LEMTA_2);
			OC_MW	=Obs_MW-LEMTA_WL*KFState->StateRel[24+i];
			OC_G1	=1.0/2*(LEMTA_1*KFState->CurComObs.comobs[i].dL1+KFState->CurComObs.comobs[i].dP1)
						-dRange-dtr-1.0/2*LEMTA_1*KFState->StateRel[12+i];
			Obs_MW_OC=Obs_MW/LEMTA_WL-KFState->StateRel[24+i];
			*/
			//λ�ù۲���󣬹۲�ֵ�ع�ϵ�µ�
			H[0]	= 1.0 * ( dPos_B[0] * Mat[0] + dPos_B[1] * Mat[3] + dPos_B[2] * Mat[6] )/RangeB;
			H[1]	= 1.0 * ( dPos_B[0] * Mat[1] + dPos_B[1] * Mat[4] + dPos_B[2] * Mat[7] )/RangeB;
			H[2]	= 1.0 * ( dPos_B[0] * Mat[2] + dPos_B[1] * Mat[5] + dPos_B[2] * Mat[8] )/RangeB;
			//Lcģ���ȹ۲����
				//�Ӳ�۲����
			H[6]	= 1.0;
			H[12+i]	= 1.0;
			//H[12+i] = LEMTA_NL;
			//H[24+i] = Hwl;	//����ģ���ȹ۲�ϵ��
			EKFMeasureUpdateRel(OC_Lc, Me_dLC, H, KFState );//Lc��������
			/*
			H[12+i]				= LEMTA_1/2.0;
			H[24+i]				= 0.0;
			EKFMeasureUpdate( OC_G1, 0.71, H, KFState );//G1��������
			H[24+i]=1.0*LEMTA_WL;
			EKFMeasureUpdate( OC_MW, 1.01, H, KFState );//MW��������,ע�����ﵥλ��m����������
			*/
		}
	}
	KFState->ApriSigma[1] = 0.0;
	KFState->PostSigma[1] = 0.0;
	
	//--------------------------------------------------------------------------------
	//�в�
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, stateInECEFRel );
	getBpos( KFState->StateInECEFA, stateInECEFRel, KFState->StateInECEFB);
	satNumWhole=0;
	for(i=0;i<KFState->CurComObs.ComSatNum;i++)
	{
		if(KFState->CurComObs.comobs[i].flag!=0||KFState->CurComObs.comobs[i].used!=1)
			continue;

		RangeA=0.0;RangeB=0.0;ARangeB=0.0;

		for(k=0;k<3;k++){
			RangeA = RangeA+pow(KFState->StateInECEFA[k]-KFState->CurComObs.comobs[i].satPos[k],2.0);
			RangeB = RangeB+pow(KFState->StateInECEFB[k]-KFState->CurComObs.comobs[i].satPos[k],2.0);
			ARangeB= ARangeB+pow(AECF[k]-KFState->CurComObs.comobs[i].satPos[k],2.0);
		}

		RangeA = sqrt(RangeA);
		RangeB = sqrt(RangeB);
		ARangeB= sqrt(ARangeB);
		dtr	   = KFState->StateRel[6];
		OC_LC0[satNumWhole] = 1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL1*LEMTA_1 - RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL2*LEMTA_2
			-(ARangeB-RangeA)-Adtr-AL1[i];//�����ģ����
		OC_LC1[satNumWhole] = 1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL1*LEMTA_1 - RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL2*LEMTA_2
							- (RangeB-RangeA)-dtr-KFState->StateRel[12+i];//�����ģ����
		LC_PC0+= 1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL1*LEMTA_1 - RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL2*LEMTA_2
			-AL1[i]-(1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP1-RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP2);
		LC_PC1+= 1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL1*LEMTA_1 - RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dL2*LEMTA_2
			-KFState->StateRel[12+i]-(1.0/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP1-RATIO2/(1.0-RATIO2)*KFState->CurComObs.comobs[i].dP2);
		//LEMTA_NL*KFState->State[12+i]-Hwl*KFState->State[24+i];
		KFState->ApriSigma[1] = KFState->ApriSigma[1] + pow( OC_LC0[satNumWhole], 2.0);
		KFState->PostSigma[1] = KFState->PostSigma[1] + pow( OC_LC1[satNumWhole], 2.0 );
		satNumWhole++;
	}
	//--------------------------------------------------------------------------------
	if( satNumWhole > 0 )
	{
		KFState->ApriSigma[1] = sqrt( KFState->ApriSigma[1] / satNumWhole );
		KFState->PostSigma[1] = sqrt( KFState->PostSigma[1] / satNumWhole );
	}
	else
	{
		KFState->ApriSigma[1] = 999.0;
		KFState->PostSigma[1] = 999.0;
	}
	//�������
	outResAmbSD(KFState->Time.SecOfWeek,LC_PC0,LC_PC1);
	outRes(satNumWhole,fLC,OC_LC0,OC_LC1,KFState);
	outResEKFLC(satNumWhole,OC_LC0,OC_LC1,KFState);
	KFState->SatNumUsed[1] = satNumWhole;
	KFState->sigPosLC=KFState->PostSigma[1];
	EKFConvergencyCheck(1,KFState);
	return satNumWhole;
	
}

int PPRTOD_LC(Common11Obs * CurObs,EKFSTATE * KFState, PPRESULT * Result, double * Mat)
{
	int i, j, k, Iterator;                     /*  IteratorΪ���㶨λ��������  */
	int SatNumUsed,GPSSatNum;      /*  ���㶨λʹ�õ�������, GPS������ */
	short Prn;
	short PosKind;            /* GPS:0, GLO:1, GPS&GLO:2  */		
	int    PRValid;                            /* �۲�ֵ��α������Ժ�α��۲�ֵ */
	double PRange;
	double AprioriPosRel[3], AprioriClk[2];
	double RangeA,RangeB, Ion, Height;                                 /*  ���ջ��뵼������֮��ľ���  */
	double BPos[MAXCHANNUM];                      /* α�������յĹ۲�ֵ-����ֵ */
	double MeasA[MAXCHANNUM*5], MeasAT[MAXCHANNUM*5];          /* �۲���� [MAXCHANNUM][5] */
	double Weight[MAXCHANNUM];                               /* Ȩ����, ��λ��ֻȡ�Խ���Ԫ�� */
	double Qvv[MAXCHANNUM];                               /* �۲�ֵ��������б������Խ���Ԫ�� */
	double QLL[MAXCHANNUM*MAXCHANNUM]={0.0};			/*ƽ���۲�ֵ��Э������*/
	double ATA[25], InvATA[25];
	double ATB[5];
	double dPos[5];               /* ��λ���  */
	double Residual[MAXCHANNUM] = {0.0};  /* ��λ�в�  */
	int outlier=1;							/*��һ��̽��ֲ�*/
	double	OrbitAccuracy=0.0;
	PosKind=0;
	
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateA, KFState->StateInECEFA );//ÿ���۲�ֵ���¶�Ҫת���ع�ϵ��
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, AprioriPosRel );
	CopyArray(2,AprioriClk,&KFState->StateRel[6]);
	for(i=0;i<3;i++)
	{
		OrbitAccuracy = OrbitAccuracy + pow( KFState->CovRel[i*RELDIMENSION+i], 2.0 );
	}
	OrbitAccuracy=sqrt(OrbitAccuracy);
	Iterator = 0;
	
		do{
				SatNumUsed = 0;
				GPSSatNum  = 0;
	
				for( i=0; i<CurObs->ComSatNum; i++ )
				{
					if(CurObs->comobs[i].flag==0)
					{
					
						RangeA = 0.0;
						RangeB = 0.0;
						for( k=0; k<3; k++ )
						{
							RangeA = RangeA + pow( KFState->StateInECEFA[k]-CurObs->comobs[i].satPos[k], 2.0 );
							RangeB = RangeB + pow( KFState->StateInECEFA[k]+AprioriPosRel[k]-CurObs->comobs[i].satPos[k], 2.0 );
						}
	
						RangeA = sqrt( RangeA );
						RangeB = sqrt( RangeB );
						MeasA[SatNumUsed*5+0] = 1.0*( AprioriPosRel[0] + KFState->StateInECEFA[0] - CurObs->comobs[i].satPos[0] )/RangeB;
						MeasA[SatNumUsed*5+1] = 1.0*( AprioriPosRel[1] + KFState->StateInECEFA[1] - CurObs->comobs[i].satPos[1] )/RangeB;
						MeasA[SatNumUsed*5+2] = 1.0*( AprioriPosRel[2] + KFState->StateInECEFA[2]- CurObs->comobs[i].satPos[2] )/RangeB;
						MeasA[SatNumUsed*5+3] = 1.0;             /* ���ջ�GPSϵͳ�Ӳ�ϵ�� */
						MeasA[SatNumUsed*5+4] = 0.0;             /* ���ջ�GLONASSϵͳ�Ӳ�ϵ�� */
						Weight[SatNumUsed]    = 1.0;  // * pow( sin(SatMidInfo[i].Elevation), 2.0 );
	
						BPos[SatNumUsed] =1.0/(1.0-RATIO2)*CurObs->comobs[i].dL1*LEMTA_1-RATIO2/(1.0-RATIO2)*CurObs->comobs[i].dL2*LEMTA_2
							-(RangeB-RangeA)-AprioriClk[0]-LEMTA_NL*KFState->StateRel[12+i]-Hwl*KFState->StateRel[24+i];
						Result->SatList[SatNumUsed].Status=1;
						Result->SatList[SatNumUsed].index =i;
						CurObs->comobs[i].index_satlist=SatNumUsed;
						SatNumUsed++;
					
					}
				}
				/*  �������  */
	
				for( k=SatNumUsed; k<MAXCHANNUM; k++)  /* ���������� */
				{
					BPos[k] = 0.0;
					Weight[k] = 0.0;
					for( j=0; j<5; j++ )
					{
						MeasA[k*5+j] = 0.0;
					}
				}
	
				if( SatNumUsed>=4 )  /* ������GLONASS���� */
				{
					PosKind = 0;
					/*  ���۲����任ΪMAXCHANNUM*4, ����GLONASS�����Ӳ���� */
	
					for( i=0; i<SatNumUsed; i++ )
					{
						MeasA[i*4+0] = MeasA[i*5+0];
						MeasA[i*4+1] = MeasA[i*5+1];
						MeasA[i*4+2] = MeasA[i*5+2];
						MeasA[i*4+3] = MeasA[i*5+3];
					}
	
					MatrixTranspose( SatNumUsed, 4, MeasA, MeasAT );
					MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 4, MeasAT, Weight, MeasA, ATA );
					MatrixInv( 4, ATA, InvATA );
	
					MatrixMultiply3( 4, SatNumUsed, SatNumUsed, 1, MeasAT, Weight, BPos, ATB );
					MatrixMultiply( 4, 4, 4, 1, InvATA, ATB, dPos );
	
					AprioriClk[0] = AprioriClk[0] + dPos[3];
	
				}
				else
				{
					trace("\nCannot position because of only %2d satellites at %10.3f epoch.\n",SatNumUsed, CurObs->Time.SecOfWeek );
					break;
				}
	
				for( k=0; k<3; k++ )
				{
					AprioriPosRel[k] = AprioriPosRel[k] + dPos[k];
				}
				Iterator++;
				Result->Iterator = Iterator;
				Result->Coverage = VectDot( 3, 3, dPos, dPos );
	
			}while ( (Result->Coverage>1E-6) && (Iterator < 10) );
			//ѭ���������߶Ƚ�
			Result->SatNumUsed = SatNumUsed;
			CopyArray(3,Result->Position,AprioriPosRel);
			CopyArray( 2, Result->RcvClkOft, AprioriClk );
	
	
			/* ������λ����������  */
			/* ����۲�ֵ�в� */
	
			MatrixMultiply( SatNumUsed, 4, 4, 1, MeasA, dPos, Residual );//
			for( i=0; i<SatNumUsed; i++ )
			{
				Residual[i] = Residual[i] - BPos[i];
			}
	
			/* ����۲�ֵ��������Э������ */
			ComputeQvv( SatNumUsed, 4, MeasA, InvATA, MeasAT, Weight, Qvv );
			/* ����ƽ���۲�ֵ��Э������*/
			ComputeQLL( SatNumUsed, 4, MeasA, InvATA, MeasAT, Weight, QLL);
			Result->PDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] );
			Result->GDOP = sqrt( InvATA[0] + InvATA[5] + InvATA[10] + InvATA[15] );
			Result->HDOP = sqrt( InvATA[0] + InvATA[5] );
			Result->VDOP = sqrt( InvATA[10] );
			Result->TDOP = sqrt( InvATA[15] );
	
			if( SatNumUsed > 5 ) /* ����5�����ǲż��㶨λ�����, ��ǿ�ɿ��� */
			{
				Result->SigmaPos = 0.0;
	
				for( j=0, i=0; i<CurObs->ComSatNum; i++ )
				{
					if( Result->SatList[i].Status == 1 )
					{
						Result->Residual[i] = Residual[j];
						Result->SigmaPos = Result->SigmaPos + Weight[i]*pow( Residual[i], 2.0 );
						j++;
					}
				}
				Result->SigmaPos = sqrt( Result->SigmaPos / (SatNumUsed-4) );
			}
			else
			{
				Result->SigmaPos = 999.99;
			}
	
			/* ���ֲ�̽�� */
	
			if( SatNumUsed > 5 )			//����۲�ֵ���ڵ���2ʱ���ܽ��дֲ��⣬��W����ֻ�е��۲�ֵ����ʱ���ܽ���
			{
				//CheckPostResidual_W( CurObs->ComSatNum, Me_dLC, Residual, Qvv , Result );//������ֵ�ֱ�Ϊw������ֵ��۲�ֵ����
			}
			/*RJ-2016-9-20
			**��ģ����Ϊ�˰ѵ��㶨λ������ֲ��������Ϊ������
			*/
			for(i=0;i<CurObs->ComSatNum;i++)
			{
				if(CurObs->comobs[i].flag==0&&Result->SatList[CurObs->comobs[i].index_satlist].Status<0){
					CurObs->comobs[i].flag=1;
					//���³�ʼ��ģ����
				}
			}

	if( Result->Coverage < 1E-6 && Result->Iterator < 5 &&
		Result->PDOP < 8.0 && Result->SigmaPos < 5.0 && SatNumUsed > 5 )
	{
		Result->IsSuccess = true;
	}
	else
	{
		Result->IsSuccess = false;
	}

	return SatNumUsed;
}
int PPRTOD_PC(Common11Obs * CurObs,EKFSTATE * KFState, PPRESULT * Result, double * Mat)
{
	int i, j, k,SatNumUsed=0;                     /*  IteratorΪ���㶨λ��������  */
	double StateInECEFA[3];
	double AprioriPosRel[3], AprioriClk;
	double apriRes=0.0,posRes=0.0;
	double RangeA,RangeB;
	int    bestBan=999;
	int    PRN[MAXCHANNUM]={999};//ֻ�е�һ��Ϊ999������Ķ���0
	int    banPRN[MAXCHANNUM]={999};
	double satPos[3*MAXCHANNUM]={0.0};
	double obs[MAXCHANNUM]={0.0};
	int    valFlag=0;
	FILE * apriRes_file,*posRes_file;
	for(i=0;i<MAXCHANNUM;i++)
	{
		PRN[i] = 999;
		banPRN[i] = 999;
	}
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateA, KFState->StateInECEFA );//ÿ���۲�ֵ���¶�Ҫת���ع�ϵ��
	MatrixMultiply( 3, 3, 3, 1, Mat, KFState->StateRel, AprioriPosRel );
	AprioriClk=KFState->StateRel[6];
	for (i=0;i<CurObs->ComSatNum;i++)
	{
		RangeA=0.0;RangeB=0.0;
		if (CurObs->comobs[i].used==1)
		{
			for (j=0;j<3;j++)
			{
				satPos[SatNumUsed*3+j]=CurObs->comobs[i].satPos[j];
				RangeA+=pow(KFState->StateInECEFA[j]-CurObs->comobs[i].satPos[j],2.0);
				RangeB+=pow(KFState->StateInECEFA[j]+AprioriPosRel[j]-CurObs->comobs[i].satPos[j],2.0);
			}
			RangeA=sqrt(RangeA);
			RangeB=sqrt(RangeB);
			obs[SatNumUsed]=1.0/(1.0-RATIO2)*CurObs->comobs[i].dP1-RATIO2/(1.0-RATIO2)*CurObs->comobs[i].dP2;
			apriRes=apriRes+obs[SatNumUsed]-(RangeB-RangeA)-AprioriClk;
			PRN[SatNumUsed]=CurObs->comobs[i].PRN;
			SatNumUsed++;
		}
	}
	apriRes=apriRes/SatNumUsed;
	apriRes_file=fopen("OutFile\\apriRes.txt","a+");
	fprintf(apriRes_file,"%14.3f%14.3f\n",KFState->Time.SecOfWeek,apriRes);
	fclose(apriRes_file);
	if (SatNumUsed<5)
	{
		Result->IsSuccess = false;
		Result->SigmaPos=999.0;
		Result->SatNumUsed=0;
		return 0;
	}
	/*LEGE̽���ά�ֲ�*/
	Result->SigmaPos=LSRel_PC(SatNumUsed,PRN,banPRN,satPos,KFState->StateInECEFA,obs,AprioriPosRel,&AprioriClk, Result);
	valFlag=valResult(Result->SigmaPos,5,SatNumUsed,4);
	if (valFlag==1)
	{
		Result->SatNumUsed=SatNumUsed;
	}
	else
	{
		Raim_fde1(&SatNumUsed,PRN,banPRN,satPos,KFState->StateInECEFA,obs,AprioriPosRel,&AprioriClk,Result);		
	}
	valFlag=valResult(Result->SigmaPos,5,SatNumUsed,4);
	CopyArray(3,Result->Position,AprioriPosRel);
	Result->RcvClkOft[0]=AprioriClk;
	/*RJ-2016-9-20
	**��ģ����Ϊ�˰ѵ��㶨λ������ֲ��������Ϊ������
	*/
	for(i=0;i<CurObs->ComSatNum;i++)
	{
		for (j=0;j<MAXCHANNUM;j++)
		{
			if(CurObs->comobs[i].PRN==banPRN[j])
				CurObs->comobs[i].used=-2;
		}			
	}
	/*������󷽲�*/
	SatNumUsed=0;
	for (i=0;i<CurObs->ComSatNum;i++)
	{
		RangeA=0.0;RangeB=0.0;
		if (CurObs->comobs[i].used==1)
		{
			for (j=0;j<3;j++)
			{
				satPos[SatNumUsed*3+j]=CurObs->comobs[i].satPos[j];
				RangeA+=pow(KFState->StateInECEFA[j]-CurObs->comobs[i].satPos[j],2.0);
				RangeB+=pow(KFState->StateInECEFA[j]+AprioriPosRel[j]-CurObs->comobs[i].satPos[j],2.0);
			}
			RangeA=sqrt(RangeA);
			RangeB=sqrt(RangeB);
			obs[SatNumUsed]=1.0/(1.0-RATIO2)*CurObs->comobs[i].dP1-RATIO2/(1.0-RATIO2)*CurObs->comobs[i].dP2;
			posRes=posRes+obs[SatNumUsed]-(RangeB-RangeA)-AprioriClk;
			PRN[SatNumUsed]=CurObs->comobs[i].PRN;
			SatNumUsed++;
		}
	}
	posRes=posRes/SatNumUsed;
	posRes_file=fopen("OutFile\\posRes.txt","a+");
	fprintf(posRes_file,"%14.3f%14.3f\n",KFState->Time.SecOfWeek,posRes);
	fclose(posRes_file);
	if( Result->Coverage < 1E-6 && Result->Iterator < 5 &&
		Result->PDOP < 8.0 && Result->SigmaPos < 5.0 && SatNumUsed > 4 )//��������6�����Ǹ�Ϊ5��
	{
		Result->IsSuccess = true;
	}
	else
	{
		Result->IsSuccess = false;
	}

	return SatNumUsed;
}
//************************************
// Method:    EKFTimeUpdateAB
// FullName:  EKFTimeUpdateAB
// Access:    public 
// Returns:   void
// Qualifier: ABTimeUpdate����������Ӻ���,�����˲���ʱ�����
// Parameter: double OIState[108]
// Parameter: EKFSTATE * KFState
//************************************
void EKFTimeUpdateRel( double OIState[54], EKFSTATE* KFState )
{
	//����cov����  |A C|
	//             |D B|
	
	int i,j;
	double StateB[RELDIMENSION]={0.0};
	double STM[RELDIMENSION*RELDIMENSION] = {0.0};
	double Q[RELDIMENSION*RELDIMENSION] = {0.0};
	double TmpMat[RELDIMENSION*RELDIMENSION], STMT[RELDIMENSION*RELDIMENSION];
	double KFTmp[RELDIMENSION*RELDIMENSION];
	//���������Ƿ�������
	CopyArray(RELDIMENSION,StateB,KFState->StateRel);
	//���ٶȺ�λ���⸳ֵ��StateB
	CopyArray(6,StateB,OIState);						//ֱ�����û����������
	/************************************************************************/
	/* ʱ�����ǰ��Э����                                                   */
	/************************************************************************/
	for(i=0;i<RELDIMENSION;i++)
	{
		for(j=0;j<RELDIMENSION;j++)
		{
			KFTmp[i*RELDIMENSION+j]=KFState->CovRel[i*RELDIMENSION+j];
		}
	}
	/************************************************************************/
	/* B��+����ģ���ȵĹ���������ע��StateB��Ϊ�˼������������ת�ƾ���     */
	/************************************************************************/
	UdProcessNoiseCovRTN4rel( KFState->Tao[1], KFState->Step, KFState->Sigma[1], StateB, Q);
	
	FormStateTransMatrixFromDynRTN4rel( KFState->Tao[1], KFState->Step,StateB, &OIState[6], STM );
	
	MatrixMultiply( RELDIMENSION, RELDIMENSION, RELDIMENSION,RELDIMENSION, 
		STM, KFTmp, TmpMat );
	MatrixTranspose( RELDIMENSION, RELDIMENSION, STM, STMT );
	MatrixMultiply( RELDIMENSION, RELDIMENSION, RELDIMENSION, RELDIMENSION,
		TmpMat, STMT, KFTmp );
	MatrixAddition2( RELDIMENSION, RELDIMENSION, Q, KFTmp ); 
	for(i=0;i<RELDIMENSION;i++)
	{
		for(j=0;j<RELDIMENSION;j++)
		{
			KFState->CovRel[i*RELDIMENSION+j]=KFTmp[i*RELDIMENSION+j];
		}
	}
	/************************************************************************/
	/* Э���������ϣ��������ǲ������ٶȶ�״̬�Ĳ���                       */
	/************************************************************************/
	DMCrtn4Rel( KFState->Tao[1], KFState->Step, StateB );			//B�ǵĲ���
	//��Ϊ����״̬ת��ֻ�����󷽲�ת�ƣ�λ���ٶ�ת���ǵ������ֵģ����������Ҫ����
	CopyArray(RELDIMENSION,KFState->StateRel,StateB);//��Ϊ���������״̬�ĳ��ٶ�λ����������˸���
	CopyArray(RELDIMENSION,KFState->StateB,StateB);//��Ϊ���������״̬�ĳ��ٶ�λ����������˸���
}
void outResAmbSD(const double sow,const double LC_PC0,const double LC_PC1)
{
	FILE * fp;
	fp=fopen("OutFile\\LC-PC","a+");
	fprintf(fp,"%14.3f%14.3f%14.3f\n",sow,LC_PC0,LC_PC1);
	fclose(fp);
}
void outRes(int satNumWhole,FILE *fLC,double *OC_LC0,double *OC_LC1,EKFSTATE * KFState)
{
	int i;
	for (i=0;i<satNumWhole;i++)
	{
		fprintf(fLC,"%12.3f",OC_LC0[i]);
	}
	fprintf(fLC,"%12.3f\n",KFState->ApriSigma[1]);
	for (i=0;i<satNumWhole;i++)
	{
		fprintf(fLC,"%12.3f",OC_LC1[i]);
	}
	fprintf(fLC,"%12.3f\n",KFState->PostSigma[1]);
	fclose(fLC);
}
void outResEKFPC(int satNumWhole,double *OC_PC0,double *OC_PC1,EKFSTATE * KFState)
{
	int i;
	double apriRes=0.0,posRes=0.0;
	FILE *apriRes_file,*posRes_file;
	apriRes_file=fopen("OutFile\\apriRes_EKF_PC.txt","a+");
	posRes_file=fopen("OutFile\\posRes_EKF_PC.txt","a+");
	for (i=0;i<satNumWhole;i++)
	{
		apriRes+=OC_PC0[i];
		posRes+=OC_PC1[i];
	}
	fprintf(apriRes_file,"%12.3f%12.3f\n",KFState->Time.SecOfWeek,apriRes/satNumWhole);
	fprintf(posRes_file,"%12.3f%12.3f\n",KFState->Time.SecOfWeek,posRes/satNumWhole);
	fclose(apriRes_file);
	fclose(posRes_file);
}
void outResEKFLC(int satNumWhole,double *OC_LC0,double *OC_LC1,EKFSTATE * KFState)
{
	int i;
	double apriRes=0.0,posRes=0.0;
	FILE *apriRes_file,*posRes_file;
	apriRes_file=fopen("OutFile\\apriRes_EKF_LC.txt","a+");
	posRes_file=fopen("OutFile\\posRes_EKF_LC.txt","a+");
	for (i=0;i<satNumWhole;i++)
	{
		apriRes+=OC_LC0[i];
		posRes+=OC_LC1[i];
	}
	fprintf(apriRes_file,"%12.3f%12.3f\n",KFState->Time.SecOfWeek,apriRes/satNumWhole);
	fprintf(posRes_file,"%12.3f%12.3f\n",KFState->Time.SecOfWeek,posRes/satNumWhole);
	fclose(apriRes_file);
	fclose(posRes_file);
}