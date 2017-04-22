#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ReadObs.h"
#include "RTOD_Const.h"

/***************************************************************************
//
// EmptyEpochObsDataStruct
//
// Ŀ��: ��ʼ��EpochObsData�ṹ��, ��ÿ�ζ�����֮ǰ����
//
// ����:
//
//  Epoch    ����յĹ۲�����

***************************************************************************/

void EmptyEpochObsDataStruct( EpochObsData* Epoch )
{
    int i, j;

    Epoch->EpochFlag = 0;
    Epoch->SatNum    = 0;
    Epoch->Time.Week = 0;
    Epoch->Time.SecOfWeek = 0.0;

    for( i=0; i<MAXCHANNUM; i++ )
    {
        Epoch->SatObs[i].Prn = 0;
        Epoch->SatObs[i].System = UNKS;
        Epoch->SatObs[i].Used   = 0;
        
        for( j=0; j<MAXOBSTYPENUM; j++ )
        {
            Epoch->SatObs[i].Data[j].Availability = 0;
            Epoch->SatObs[i].Data[j].Obs = 0.0;
            Epoch->SatObs[i].Data[j].Type = UNKOBS;
        }

    }

}

/***************************************************************************
//
// EmptyPSPRangeStruct
//
// Ŀ��: ��ʼ��PSPRange�ṹ��, ��ÿ�ζ�����֮ǰ����
//
// ����:
//
//  PSPR    ��PSPRange�ṹ��

***************************************************************************/

void EmptyPSPRangeStruct( PSPRange PSPR[] )
{
	int  i;

	for( i=0; i<32; i++ )
	{
		PSPR[i].CurrNum = 0;
		PSPR[i].CurrTime.Week = 0;
		PSPR[i].CurrTime.SecOfWeek = 0.0;

		PSPR[i].PSC1 = PSPR[i].PSP1 = PSPR[i].PSP2 = 0.0;
		PSPR[i].CPL1 = PSPR[i].CPL2 = 0.0;	

		PSPR[i].GoodC1 = PSPR[i].GoodP1 = PSPR[i].GoodP2 = false;
		PSPR[i].GoodL1 = PSPR[i].GoodL2 = false;

	}

}

/***************************************************************************
//
// ReadObsHead
//
// Ŀ��: ��ȡRinex�ļ�ͷ��Ϣ, �������ؽ��ջ��Ĺ۲�����, ��ʹ���ļ�ͷ�е�
         �κ���Ϣ.
//
// ����:
//
//  fin   �۲������ļ�ָ��
//
// ����ֵ:
// 
// ����ļ���ȡ������, ����0, ���򷵻�1
***************************************************************************/

int ReadObsHead( FILE* fin, OBSTYPELIST* ObsTypeList )
{
    int  i;
    char line[100], str[10], twoline[200];
    
    do{
        
        if( fgets( line, 100, fin )== NULL ) 
        {
            return 0;
        }
        
        if( strncmp( &line[60], "# / TYPES OF OBSERV", 18 ) == 0 )
        {
            strncpy( str, line, 6 );
            ObsTypeList->ObsTypeNum = atoi( str );

            strncpy( twoline, line, 60 );

            if( ObsTypeList->ObsTypeNum > 9 )
            {
                fgets( line, 100, fin );
                strncpy( &twoline[60], &line[6], 54 );
            }
            for( i=0; i<ObsTypeList->ObsTypeNum; i++ )
            { 
                strncpy( str, &twoline[10+6*i], 2 );
                
                if( strncmp( str, "C1", 2 ) == 0 )       ObsTypeList->ObsType[i] = C1;
                else if(strncmp( str, "P1", 2 ) == 0)    ObsTypeList->ObsType[i] = P1;
                else if(strncmp( str, "L1", 2 ) == 0)    ObsTypeList->ObsType[i] = L1;
                else if(strncmp( str, "D1", 2 ) == 0)    ObsTypeList->ObsType[i] = D1;
                else if(strncmp( str, "L2", 2 ) == 0)    ObsTypeList->ObsType[i] = L2;
                else if(strncmp( str, "P2", 2 ) == 0)    ObsTypeList->ObsType[i] = P2;
                else if(strncmp( str, "D2", 2 ) == 0)    ObsTypeList->ObsType[i] = D2;
                else if(strncmp( str, "S1", 2 ) == 0)    ObsTypeList->ObsType[i] = S1;
                else if(strncmp( str, "S2", 2 ) == 0)    ObsTypeList->ObsType[i] = S2;
                else if(strncmp( str, "C2", 2 ) == 0)    ObsTypeList->ObsType[i] = C2;
                else   ObsTypeList->ObsType[i] = UNKOBS;
            }
        } 
    }while( strncmp( &line[60],"END OF HEADER", 12 ) != 0 );
	
    return 1;
}

/***************************************************************************
//
// ReadEpochObs
//
// Ŀ��: ��ȡһ����Ԫ�Ĺ۲�����
//
// ����:
//
//  fin   �۲������ļ�ָ��
//  Epoch һ����Ԫ�Ĺ۲�����
//
// ����ֵ:
// 
// ����ļ���ȡ������, ����0, ���򷵻�1
***************************************************************************/

int ReadEpochObs( FILE* fin, OBSTYPELIST* ObsTypeList, EpochObsData* Epoch )
{
    int i, j;
    char substr[60], line[100], twoline[200];
    char prnline[120];
    
    COMMONTIME CurrCT;
 
    do{
        if( fgets( line, 100, fin ) == NULL )
        {
            return 0;
        }
        sscanf( &line[26], "%d %d", &Epoch->EpochFlag, &Epoch->SatNum );

        if( Epoch->EpochFlag == 0 )
        {
            break;
        }
        else
        {
            for( i=0; i<Epoch->SatNum; i++ )
            {
                fgets( line, 100, fin );
            }
        }

    }while(1);

    
    sscanf( line, "%d %d %d %d %d %lf", &CurrCT.Year, &CurrCT.Month,
        &CurrCT.Day, &CurrCT.Hour, &CurrCT.Minute, &CurrCT.Second );

    sscanf( &line[26], "%d %d", &Epoch->EpochFlag, &Epoch->SatNum );
    
    CommonTimeToGPSTime ( &CurrCT, &Epoch->Time );
    
    strncpy( prnline, &line[32], 36 );
    
    if( Epoch->SatNum > 12 )
    {
        if( fgets( line, 100, fin ) == NULL )
        {
            return 0;
        }
        strncpy( &prnline[36], &line[32], 36 );
    }
    
    for( i=0; i<Epoch->SatNum; i++ )
    {
        if( fgets( line, 100, fin ) == NULL )
        {
            return 0;
        }
        
        strncpy( twoline, line, 80 );
        
        if( ObsTypeList->ObsTypeNum > 5 )
        {
            if( fgets( line, 100, fin ) == NULL )
            {
                return 0;
            }
            strncpy(&twoline[80], line, 80 );
        }
        
        if( prnline[3*i] == 'G'||prnline[3*i] == 'g'||prnline[3*i] == ' ')
        {
            Epoch->SatObs[i].System = GPS;
        }
        else if( prnline[3*i] == 'R'||prnline[3*i] == 'r' )
        {
            Epoch->SatObs[i].System = GLONASS;
        }

        strncpy( substr, &prnline[3*i+1], 2 );
        substr[2] = '\0';
        Epoch->SatObs[i].Prn    = atoi( substr );
        Epoch->SatObs[i].Used   = 1;             /* ��ʼ������Ϊ���� */
        
        for( j=0; j<ObsTypeList->ObsTypeNum; j++)
        {
            Epoch->SatObs[i].Data[j].Type = ObsTypeList->ObsType[j];
            
            strncpy( substr, &twoline[16*j], 14 );
            substr[14] = '\0';
            Epoch->SatObs[i].Data[j].Obs = atof( substr );

            if( fabs( Epoch->SatObs[i].Data[j].Obs ) < 0.01 )  /* �۲�ֵΪ0��� */
            {
                Epoch->SatObs[i].Data[j].Availability =  false;
            }
            else
            {
                Epoch->SatObs[i].Data[j].Availability =  true;
            }
        }
    }
    
    return 1;    
}
int ReadObsHeadQC( FILE* fin, OBSTYPELIST* ObsTypeList )
{
	int  i;
	char line[100], str[10], twoline[200];

	do{

		if( fgets( line, 100, fin )== NULL ) 
		{
			return 0;
		}

		if( strncmp( &line[60], "# / TYPES OF OBSERV", 18 ) == 0 )
		{
			strncpy( str, line, 6 );
			ObsTypeList->ObsTypeNum = atoi( str );

			strncpy( twoline, line, 60 );

			if( ObsTypeList->ObsTypeNum > 9 )
			{
				fgets( line, 100, fin );
				strncpy( &twoline[60], &line[6], 54 );
			}
			for( i=0; i<ObsTypeList->ObsTypeNum; i++ )
			{ 
				strncpy( str, &twoline[10+6*i], 2 );

				if( strncmp( str, "C1", 2 ) == 0 )       ObsTypeList->ObsType[i] = C1;
				else if(strncmp( str, "P1", 2 ) == 0)    ObsTypeList->ObsType[i] = P1;
				else if(strncmp( str, "L1", 2 ) == 0)    ObsTypeList->ObsType[i] = L1;
				else if(strncmp( str, "D1", 2 ) == 0)    ObsTypeList->ObsType[i] = D1;
				else if(strncmp( str, "L2", 2 ) == 0)    ObsTypeList->ObsType[i] = L2;
				else if(strncmp( str, "P2", 2 ) == 0)    ObsTypeList->ObsType[i] = P2;
				else if(strncmp( str, "D2", 2 ) == 0)    ObsTypeList->ObsType[i] = D2;
				else if(strncmp( str, "S1", 2 ) == 0)    ObsTypeList->ObsType[i] = S1;
				else if(strncmp( str, "S2", 2 ) == 0)    ObsTypeList->ObsType[i] = S2;
				else if(strncmp( str, "C2", 2 ) == 0)    ObsTypeList->ObsType[i] = C2;
				else if(strncmp( str, "Nw", 2 ) == 0)    ObsTypeList->ObsType[i] = Nw;
				else if(strncmp( str, "Sw", 2 ) == 0)    ObsTypeList->ObsType[i] = Sw;
				else   ObsTypeList->ObsType[i] = UNKOBS;
			}
		} 
	}while( strncmp( &line[60],"END OF HEADER", 12 ) != 0 );

	return 1;
}
int ReadEpochObsQC( FILE* fin, OBSTYPELIST* ObsTypeList, EpochObsData* Epoch )
{
	int i, j;
	char substr[60], line[200];
	char prnline[120];
	double tempTime;
	COMMONTIME CurrCT={0};

	do{
		if( fgets( line, 200, fin ) == NULL )
		{
			return 0;
		}
		sscanf( line, "%lf  %d", &tempTime, &Epoch->SatNum );
		Epoch->EpochFlag = 0;	
		if( Epoch->EpochFlag == 0 )
		{
			break;
		}
		else
		{
			for( i=0; i<Epoch->SatNum; i++ )
			{
				fgets( line, 200, fin );
			}
		}

	}while(1);

	SecTimeToCT(tempTime,&CurrCT);
	CommonTimeToGPSTime ( &CurrCT, &Epoch->Time );

	for( i=0; i<Epoch->SatNum; i++ )
	{
		if( fgets( line, 200, fin ) == NULL )
		{
			return 0;
		}
		if( line[1] == 'G'||line[1] == 'g'||line[1] == ' ')
		{
			Epoch->SatObs[i].System = GPS;
		}
		else if( prnline[1] == 'R'||prnline[1] == 'r' )
		{
			Epoch->SatObs[i].System = GLONASS;
		}

		strncpy( substr, &line[2], 2 );
		substr[2] = '\0';
		Epoch->SatObs[i].Prn    = atoi( substr );
		Epoch->SatObs[i].Used   = 1;             /* ��ʼ������Ϊ���� */

		for( j=0; j<ObsTypeList->ObsTypeNum; j++)
		{
			Epoch->SatObs[i].Data[j].Type = ObsTypeList->ObsType[j];

			strncpy( substr, &line[16*j+4], 14 );
			substr[14] = '\0';
			Epoch->SatObs[i].Data[j].Obs = atof( substr );
			strncpy(substr,&line[16*j+4+14],1);
			substr[1] = '\0';
			Epoch->SatObs[i].Data[j].LLI = atof( substr );
			if( fabs( Epoch->SatObs[i].Data[j].Obs ) < 0.01 )  /* �۲�ֵΪ0��� */
			{
				Epoch->SatObs[i].Data[j].Availability =  false;
				Epoch->SatObs[i].Data[j].LLI=9;
			}
			else
			{
				Epoch->SatObs[i].Data[j].Availability =  true;
			}
		}
	}

	return 1;    
}
/***************************************************************************
//
// SmoothPRangeWithPhase
//
// Ŀ��: ���ز���λ�۲�ֵƽ��α��۲�ֵ
//
// ����:
//
//  PSPR  ��λƽ��α����м���
//  Epoch һ����Ԫ�Ĺ۲�����
//
// ע��:
// 
//   ��ʹ��˫Ƶ�۲����ݣ�Ҳ���õ�Ƶα���ƽ�����������ǵ�����������Ӱ�죬
//   ƽ�����ý϶̵�ʱ��Ρ�����ʱ�����ƣ���������ƽ������

//    ƽ���Ĺ۲�ֵ������Ԫ�۲����ݵ�����α�࣬����PSPR��
//    ʹ�ü򵥵�����̽�ⷽ����������λ���ݺ�α�����ݵ�һ���ԡ�
***************************************************************************/

void SmoothPRangeWithPhase( PSPRange PSPR[], EpochObsData* Epoch )
{
	int i,j;
	int Prn;

	double l1, l2;
	double dt;

	bool bl1, bl2;  /* ���۲����ݵĿ����� */

	double dP, dL;
	bool   ResetFlagC1, ResetFlagP1, ResetFlagP2;

	for( i=0; i<Epoch->SatNum; i++ )
	{
		/* ��ȷ����ǰ�۲����ݵ�������Ϣ */

		Prn = Epoch->SatObs[i].Prn;
		if( Epoch->SatObs[i].System != GPS )
		{
			continue;   /* GLONASS��ƽ��  */
		}
		
		/* ��ȡ��ǰ��Ԫ�����ǵ�L1��L2���� */

		for( j=0; j<MAXOBSTYPENUM; j++ )
		{
			if( Epoch->SatObs[i].Data[j].Type == L1 )
			{
				l1 = Epoch->SatObs[i].Data[j].Obs;
				bl1 = false;
				if( Epoch->SatObs[i].Data[j].Availability == 1 )
				{
					bl1 = true;
				}
			}
			else if( Epoch->SatObs[i].Data[j].Type == L2 )
			{
				l2 = Epoch->SatObs[i].Data[j].Obs;
				bl2 = false;
				if( Epoch->SatObs[i].Data[j].Availability == 1 )
				{
					bl2 = true;
				}
			}
			else
			{
				continue;
			}
		}

		/* �Ը����ǽ�����λƽ������ÿ�����ǽ��зֱ�ƽ�� */

		dt = ( Epoch->Time.Week - PSPR[Prn-1].CurrTime.Week ) * SECPERWEEK
			+ ( Epoch->Time.SecOfWeek - PSPR[Prn-1].CurrTime.SecOfWeek );

		if( dt > 3600.0 )   /* ����3600�룬������,��������  */
		{
			PSPR[Prn-1].CurrTime = Epoch->Time;

			PSPR[Prn-1].CPL1 = l1;
			PSPR[Prn-1].GoodL1 = bl1;

			PSPR[Prn-1].CPL2 = l2;
			PSPR[Prn-1].GoodL2 = bl2;

			ResetPhaseSmoothor( &Epoch->SatObs[i], &PSPR[Prn-1] );
		}
		else   /* ����ƽ��  */
		{
			for( j=0; j<MAXOBSTYPENUM; j++ )
			{
				if( Epoch->SatObs[i].Data[j].Type == C1 
					&& Epoch->SatObs[i].Data[j].Availability == 1
					&& PSPR[Prn-1].GoodC1 == true )
				{
					ResetFlagC1 = false;
					dP = Epoch->SatObs[i].Data[j].Obs - PSPR[Prn-1].PSC1;
					dL = WAVELENGTHL1 * ( l1 - PSPR[Prn-1].CPL1 );
					if( fabs( dP - dL ) < 10.0 )
					{
						PSPR[Prn-1].PSC1 = Epoch->SatObs[i].Data[j].Obs / (PSPR[Prn-1].CurrNum+1) +
							PSPR[Prn-1].CurrNum*( PSPR[Prn-1].PSC1 + dL )/(PSPR[Prn-1].CurrNum+1);

						Epoch->SatObs[i].Data[j].Obs = PSPR[Prn-1].PSC1;
					}
					else  /* ����ƽ���� */
					{
						ResetFlagC1 = true;
					}
				}
				else if( Epoch->SatObs[i].Data[j].Type == P1 
					&& Epoch->SatObs[i].Data[j].Availability == 1
					&& PSPR[Prn-1].GoodP1 == true )
				{
					ResetFlagP1 = false;
			
					dP = Epoch->SatObs[i].Data[j].Obs - PSPR[Prn-1].PSP1;
					dL = WAVELENGTHL1 * ( l1 - PSPR[Prn-1].CPL1 );
					if( fabs( dP - dL ) < 10.0 )
					{
						PSPR[Prn-1].PSP1 = Epoch->SatObs[i].Data[j].Obs / (PSPR[Prn-1].CurrNum+1) +
							PSPR[Prn-1].CurrNum*( PSPR[Prn-1].PSP1 + dL )/(PSPR[Prn-1].CurrNum+1);
						
						Epoch->SatObs[i].Data[j].Obs = PSPR[Prn-1].PSP1;
					}
					else  /* ����ƽ���� */
					{
						ResetFlagP1 = true;
					}
				}
				else if( Epoch->SatObs[i].Data[j].Type == P2 
					&& Epoch->SatObs[i].Data[j].Availability == 1
					&& PSPR[Prn-1].GoodP2 == true )
				{
					ResetFlagP2 = false;
					dP = Epoch->SatObs[i].Data[j].Obs - PSPR[Prn-1].PSP2;
					dL = WAVELENGTHL2 * ( l2 - PSPR[Prn-1].CPL2 );
					if( fabs( dP - dL ) < 10.0 )
					{
						PSPR[Prn-1].PSP2 = Epoch->SatObs[i].Data[j].Obs / (PSPR[Prn-1].CurrNum+1) +
							PSPR[Prn-1].CurrNum*( PSPR[Prn-1].PSP2 + dL )/(PSPR[Prn-1].CurrNum+1);
						
						Epoch->SatObs[i].Data[j].Obs = PSPR[Prn-1].PSP2;
					}
					else  /* ����ƽ���� */
					{
						ResetFlagP2 = true;
					}
				}
				else
				{
					continue;
				}
			}

			if( ResetFlagC1 || ResetFlagP1 || ResetFlagP2 )
			{
				PSPR[Prn-1].CurrTime = Epoch->Time;
				
				PSPR[Prn-1].CPL1 = l1;
				PSPR[Prn-1].GoodL1 = bl1;
				
				PSPR[Prn-1].CPL2 = l2;
				PSPR[Prn-1].GoodL2 = bl2;
				
				ResetPhaseSmoothor( &Epoch->SatObs[i], &PSPR[Prn-1] );

			}
			else
			{
				PSPR[Prn-1].CurrNum = PSPR[Prn-1].CurrNum + 1;
				PSPR[Prn-1].CPL1 = l1;
				PSPR[Prn-1].GoodL1 = bl1;
				
				PSPR[Prn-1].CPL2 = l2;
				PSPR[Prn-1].GoodL2 = bl2;

			}
		}

	}

}

void ResetPhaseSmoothor( SatObsData* data, PSPRange* PSPR )
{
	int j;
	double c1, p1, p2;
	
	bool bc1, bp1, bp2;  /* ���۲����ݵĿ����� */
	PSPR->CurrNum = 1;
	
	for( j=0; j<MAXOBSTYPENUM; j++ )
	{
		if( data->Data[j].Type == C1 )
		{
			c1 = data->Data[j].Obs;
			bc1 = false;
			if( data->Data[j].Availability == 1 )
			{
				bc1 = true;
			}
			PSPR->PSC1 = c1;
			PSPR->GoodC1 = bc1;
		}
		else if( data->Data[j].Type == P1 )
		{
			p1 = data->Data[j].Obs;
			bp1 = false;
			if( data->Data[j].Availability == 1 )
			{
				bp1 = true;
			}
			PSPR->PSP1 = p1;
			PSPR->GoodP1 = bp1;
		}
		else if( data->Data[j].Type == P2 )
		{
			p2 = data->Data[j].Obs;
			bp2 = false;
			if( data->Data[j].Availability == 1 )
			{
				bp2 = true;
			}
			PSPR->PSP2 = p2;
			PSPR->GoodP2 = bp2;
		}
		else
		{
			continue;
		}
	}
}
/***************************************************************************
//
// CreateDopplerObs
//
// Ŀ��: ���ز���λ���Ĳ�ֽ��Ʒ�����Doppler�۲�ֵ, ֻ������CHAMP��������,����û��
�����չ۲�ֵ
// ����:
//
//
//  Epoch[3]         ������Ԫ�Ĺ۲�����
//
// ����ֵ:
// 
// ����ļ���ȡ������, ����0, ���򷵻�1
***************************************************************************/

int CreateDopplerObs( EpochObsData Epoch[3] )
{
	int i, j;
	int prn;
	double dt1, dt2;
	double l2, l1;

	dt1 = (Epoch[1].Time.Week-Epoch[0].Time.Week)*SECPERWEEK 
		+ Epoch[1].Time.SecOfWeek - Epoch[0].Time.SecOfWeek;
	dt2 = (Epoch[2].Time.Week-Epoch[1].Time.Week)*SECPERWEEK 
		+ Epoch[2].Time.SecOfWeek - Epoch[1].Time.SecOfWeek;

	if( fabs( dt2 - dt1 ) > 0.1 )
	{
		return 0;
	}

	for( i=0; i<Epoch[1].SatNum; i++ )
	{
		if( Epoch[1].SatObs[i].System == GPS )
		{
			prn = Epoch[1].SatObs[i].Prn;
			for( j=0; j<MAXOBSTYPENUM; j++ )
			{
				if( Epoch[1].SatObs[i].Data[j].Type == UNKOBS )
				{
					if( GetEpochSatL1( GPS, prn, &Epoch[0], &l1 ) && 
						GetEpochSatL1( GPS, prn, &Epoch[2], &l2) )
					{
						Epoch[1].SatObs[i].Data[j].Type = D1;
						Epoch[1].SatObs[i].Data[j].Obs = (l1-l2)/(dt2+dt1);
						Epoch[1].SatObs[i].Data[j].Availability = true;
						break;
					}
					
				}
			}
		}
				
	}

	return 1;
}

/***************************************************************************
//
// GetEpochSatL1
//
// Ŀ��: ��ȡ���ǵ�L1�ز���λ�۲�ֵ, ��ʱ����, ���������ز���λ���Ĳ�ֽ��Ʒ�
         ����Doppler�۲�ֵ

***************************************************************************/

bool GetEpochSatL1( GNSSSys sys, short prn, EpochObsData* data, double* obs )
{
	int i, j;

	for( i=0; i<data->SatNum; i++ )
	{
		if( data->SatObs[i].System == sys && data->SatObs[i].Prn == prn )
		{
			for( j=0; j<MAXOBSTYPENUM; j++ )
			{
				if( data->SatObs[i].Data[j].Type == L1 )
				{
					*obs = data->SatObs[i].Data[j].Obs;
					return data->SatObs[i].Data[j].Availability;
				}
			}
		}		
	}

	return false;
}

 
/***************************************************************************
//
// GetOneSatPseudoRange
//
// Ŀ��: ��ȡĳ�����ǵ�α��۲�ֵ������˫Ƶ��ʹ����Ϲ۲�ֵ��C1+P2������Ƶ����C1
//
// ����:
 
	data       ���ǵĹ۲�����
	pr         α��۲�ֵ
	Ion        ˫Ƶα����ϼ���ĵ�������ֵ, ����ʱ���ڼ��鵥Ƶ�����ģ�͵�
	           ����Ч��, ��������ȡ���ò������ó�����C1Ƶ�εĵ�����ӳ�
	
//
// ����ֵ:
// 
// �������1, ��ƵC1�룻����2��˫Ƶ���α�࣬����0����ʾ�дֲ�
***************************************************************************/

int GetOneSatPseudoRange( GNSSSys Sys, SatObsData* data, double* pr, double *Ion )
{
	int i;
	double c1, p2;
	int ValidC1 = 0, ValidP2 = 0;
	int Val = 0;

	if( data->Used != 1 )
	{
		return 0;
	}
	for( i=0; i<MAXOBSTYPENUM; i++ )
	{
		if( (data->Data[i].Type == P1)
			&& ( data->Data[i].Availability == true ) )
		{
			ValidC1 = 1;
			c1 = data->Data[i].Obs;
		}
		if( ( data->Data[i].Type == P2 )
			&& ( data->Data[i].Availability == true ) )
		{
			ValidP2 = 1;
			p2 = data->Data[i].Obs;
		}
	}

	if( (ValidC1==1) && (ValidP2==1) )
	{
		if( fabs( p2 - c1 ) <= 30.0 )  /* P2�дֲ��Ϊ������ӳٲ��ᳬ��30m */
		{
			if( Sys == GPS )
			{
				*pr = ( FG12Ratio*FG12Ratio * c1 - p2 ) / ( FG12Ratio*FG12Ratio - 1.0 ); //˫Ƶ�޵�������
				*Ion = c1 - *pr;
			}
			else if( Sys == GLONASS )
			{
				printf("����glonass�������!");
				system("pause");
			}
			Val = 2;
		}
		else
		{
			Val = 0;
		}
	}
	else if( ValidC1 == 1 )
	{
		*pr = c1;
		*Ion = 0.0;
		Val = 1;
	}
	else
	{
		Val = 0;
	}

	return Val;
}


