/****************************************************************************
Ŀ�ģ�    ��Rinex��ʽ�������ж�ȡGPS��GLONASS��������, ���㵼������
          �Ĺ�����Ӳ�

��дʱ�䣺2008.12.4
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "EphProc.h"
#include "RefSys.h"
#include "RTOD_Const.h"
#include "CommonFuncs.h"

extern FILE* FGPSEph;
extern FILE* FGLOEph;

#define GPSEPHTIMELIMIT 7200.0          /* GPS����������ʱ����ֵ */
#define GLOEPHTIMELIMIT 900.0           /* GLONASS������ʱ����ֵ */

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
               IONOPARA* IonPara, GPSEPHREC* GPSEphRecord )
{
    int i;
    char line[100];
    char substr[21];
    int prn;
    GPSTIME GT;
    double LeapSec, dt;
    COMMONTIME CT;

    fseek( FGPSEph, 0, SEEK_SET ); //���ļ�ͷ��ʼ��ȡ���������ļ�

    do{   /*   ��ȡGPS�����ļ�ͷ  */
        
        if( fgets( line, 100, FGPSEph ) == NULL )
        {
            return 0;
        }
        
        if( strncmp( &line[60], "ION ALPHA", 9 ) == 0 )
        {
            for( i=0; i<4; i++ )
            {
                strncpy( substr, &line[2+i*12], 12 );
                substr[12] = '\0';

                IonPara->alpha[i] = atof( substr );
            }
        } 
        else if( strncmp( &line[60], "ION BETA", 8 ) == 0 )
        {
            for( i=0; i<4; i++ )
            {
                strncpy( substr, &line[2+i*12], 12 );
                substr[12] = '\0';
                
                IonPara->beta[i] = atof( substr );
                IonPara->IsValid = true;
            }
        }
        else if( strncmp( &line[60], "LEAP SECONDS", 10) == 0 )
        {
            strncpy( substr, line, 6 );
            substr[6] = '\0';

            LeapSec = atoi( substr );
            SetGPST_UTC( LeapSec );
        }        
        
    }while( strncmp( &line[60],"END OF HEADER", 12 ) != 0 );
    
  /*  ��ʼ��ȡ�۲�ʱ�̵���������  */
    
    do{
        if( fgets( line, 100, FGPSEph ) == NULL )
        {
            break;
        }
        
        sscanf( line, "%d %d %d %d %d %d %lf", &prn, &CT.Year, &CT.Month,&CT.Day,
            &CT.Hour, &CT.Minute, &CT.Second );
        
        CommonTimeToGPSTime( &CT, &GT );
        dt = (GT.Week-Time->Week)*SECPERWEEK + GT.SecOfWeek-Time->SecOfWeek;

        /* ����ȡ�������ǵ�ǰ���Ǻŵ����� */

        if( (Prn==prn) && (fabs(dt)<=GPSEPHTIMELIMIT) )   
        {
            GPSEphRecord->PRN = prn;
            GPSEphRecord->TOC = GT;
            
            strncpy( substr, &line[22], 19 );
            GPSEphRecord->ClkBias = atof( substr );//����ֱ�Ӱ�D-03��ȡΪ1e-3
            strncpy( substr, &line[41], 19 );
            GPSEphRecord->ClkDrift = atof( substr );
            strncpy( substr, &line[60], 19 );
            GPSEphRecord->ClkDriftRate = atof( substr );
            
            /* Orbit line 1  */
            fgets( line, 100, FGPSEph );
            strncpy( substr, &line[3], 19 );
            GPSEphRecord->IODE = atof( substr );
            strncpy( substr, &line[22], 19 );
            GPSEphRecord->Crs = atof( substr );
            strncpy( substr, &line[41], 19 );
            GPSEphRecord->DetlaN = atof( substr );
            strncpy( substr, &line[60], 19 );
            GPSEphRecord->M0 = atof( substr );
            
            /* Orbit line 2 */
            fgets( line, 100, FGPSEph );
            strncpy( substr, &line[3], 19 );
            GPSEphRecord->Cuc = atof( substr );
            strncpy( substr, &line[22], 19 );
            GPSEphRecord->e = atof( substr );
            strncpy( substr, &line[41], 19 );
            GPSEphRecord->Cus = atof( substr );
            strncpy( substr, &line[60], 19 );
            GPSEphRecord->SqrtA = atof( substr );
            
            /* Orbit line 3 */
            fgets( line, 100, FGPSEph );
            strncpy( substr, &line[3], 19 );
            GPSEphRecord->TOE.SecOfWeek = atof( substr );
            strncpy( substr, &line[22], 19 );
            GPSEphRecord->Cic = atof( substr );
            strncpy( substr, &line[41], 19 );
            GPSEphRecord->OMEGA = atof( substr );
            strncpy( substr, &line[60], 19 );
            GPSEphRecord->Cis = atof( substr );
            
            /* Orbit line 4 */
            fgets( line, 100, FGPSEph );
            strncpy( substr, &line[3], 19 );
            GPSEphRecord->i0 = atof( substr );
            strncpy( substr, &line[22], 19 );
            GPSEphRecord->Crc = atof( substr );
            strncpy( substr, &line[41], 19 );
            GPSEphRecord->omega = atof( substr );
            strncpy( substr, &line[60], 19 );
            GPSEphRecord->OMEGADot = atof( substr );
            
            /* Orbit line 5 */
            fgets( line, 100, FGPSEph );
            strncpy( substr, &line[3], 19 );
            GPSEphRecord->iDot = atof( substr );
            strncpy( substr, &line[22], 19 );
            GPSEphRecord->CodesOnL2Channel = atof( substr );
            strncpy( substr, &line[41], 19 );
            GPSEphRecord->TOE.Week = (int)( atof( substr ) );
            
            /* ��Щ������Week�Ƕ�1024ȡģ�������, �ڴ������ */
            
            if( GPSEphRecord->TOE.Week != GPSEphRecord->TOC.Week )
            {
                GPSEphRecord->TOE.Week = GPSEphRecord->TOC.Week;
            }
            strncpy( substr, &line[60], 19 );
            GPSEphRecord->L2PDataFlag = atof( substr );
            
            /* Orbit line 6 */
            fgets( line, 100, FGPSEph );
            strncpy( substr, &line[3], 19 );
            GPSEphRecord->SVAccuracy = atof( substr );
            strncpy( substr, &line[22], 19 );
            GPSEphRecord->SVHealth = atof( substr );
            strncpy( substr, &line[41], 19 );
            GPSEphRecord->TGD = (int)( atof( substr ) );
            strncpy( substr, &line[60], 19 );
            GPSEphRecord->IODC = atof( substr );
            
            /* Orbit line 7 */
            fgets( line, 100, FGPSEph );
            strncpy( substr, &line[3], 19 );
            GPSEphRecord->TransTimeOfMsg = atof( substr );
            if( strlen( line )>= 41 )	
            {
                strncpy( substr, &line[22], 19 );
                GPSEphRecord->FitInterval = atof( substr );
            }
            
            return true;
        }
        else
        {
            for( i=0; i<7; i++ )
            {
                fgets( line, 100, FGPSEph );
            }
        }
        
    } while( feof(FGPSEph) == 0 );
    
   
    return false;
}

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
                 IONOPARA* IonPara, GPSEPHREC* GPSEphRecord )
{
    double dt;

    dt = (Time->Week-GPSEphRecord->TOC.Week) * SECPERWEEK
        + Time->SecOfWeek - GPSEphRecord->TOC.SecOfWeek;

    if( fabs(dt) <= GPSEPHTIMELIMIT )
    {
        return true;            
    }
    else
    {
        if( ReadGPSEph( prn, Time, IonPara, GPSEphRecord ) == true )
        {
            return true;
        }
        else
        {
            return false;            
        }
    }
    
}

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
                    GLOTIMECORR* GloTmCorr, GLONASSEPHREC* GLOEphRecord )
{
    int i;
    char line[100], substr[21];
    COMMONTIME CT;
    int prn;
    GPSTIME GT;
    double  dt;

    fseek( FGLOEph, 0, SEEK_SET );
    
    do{   /*   ��ȡGLONASS�����ļ�ͷ  */
        
        if( fgets( line, 100, FGLOEph ) == NULL )
        {
            return false;
        }
        
        if( strncmp( &line[60], "CORR TO SYSTEM TIME", 19 ) == 0 )
        {
            sscanf( line, "%d %d %d %lf",
                &CT.Year, &CT.Month, &CT.Day, &GloTmCorr->TauC );

            CT.Hour   = 0;
            CT.Minute = 0;
            CT.Second = 0.0;

            CommonTimeToGPSTime( &CT, &GloTmCorr->RefTime );
        } 
        
    }while( strncmp( &line[60],"END OF HEADER", 12 ) != 0 );
    
  /*  ��ʼ��ȡ�۲�ʱ�̵���������  */
    
    do{
        if( fgets( line, 100, FGLOEph ) == NULL )
        {
            break;
        }
        
        sscanf( line, "%d %d %d %d %d %d %lf", &prn, &CT.Year, &CT.Month,&CT.Day,
            &CT.Hour, &CT.Minute, &CT.Second );

        CommonTimeToGPSTime( &CT, &GT );
        dt = (GT.Week-Time->Week)*SECPERWEEK + GT.SecOfWeek-Time->SecOfWeek;

        /* ����ȡ�����������ǵ�ǰʱ�̵�ǰ���ǵ����� */

        if( (Slot==prn) && (fabs(dt)<=GLOEPHTIMELIMIT) )
        {
            GLOEphRecord->SlotNum = prn;
            GLOEphRecord->RefTime.Week = GT.Week;
            GLOEphRecord->RefTime.SecOfWeek = GT.SecOfWeek;
            
            strncpy( substr, &line[22], 19 );
            GLOEphRecord->ClockBias = atof( substr );
            strncpy( substr, &line[41], 19 );
            GLOEphRecord->FreqBias = atof( substr );
            strncpy( substr, &line[60], 19 );
            GLOEphRecord->Tk = atof( substr );
            
            /* Orbit line 1  */
            fgets( line, 100, FGLOEph );
            strncpy( substr, &line[3], 19 );
            GLOEphRecord->Pos[0] = atof( substr ) * 1000.0;
            strncpy( substr, &line[22], 19 );
            GLOEphRecord->Vel[0] = atof( substr ) * 1000.0;
            strncpy( substr, &line[41], 19 );
            GLOEphRecord->Acc[0] = atof( substr ) * 1000.0;
            strncpy( substr, &line[60], 19 );
            GLOEphRecord->Health = (int)( atof( substr ) );
            
            /* Orbit line 2 */
            fgets( line, 100, FGLOEph );
            strncpy( substr, &line[3], 19 );
            GLOEphRecord->Pos[1] = atof( substr ) * 1000.0;
            strncpy( substr, &line[22], 19 );
            GLOEphRecord->Vel[1] = atof( substr ) * 1000.0;
            strncpy( substr, &line[41], 19 );
            GLOEphRecord->Acc[1] = atof( substr ) * 1000.0;
            strncpy( substr, &line[60], 19 );
            GLOEphRecord->FreqNum = atof( substr );
            
            /* Orbit line 3 */
            fgets( line, 100, FGLOEph );
            strncpy( substr, &line[3], 19 );
            GLOEphRecord->Pos[2] = atof( substr ) * 1000.0;
            strncpy( substr, &line[22], 19 );
            GLOEphRecord->Vel[2] = atof( substr ) * 1000.0;
            strncpy( substr, &line[41], 19 );
            GLOEphRecord->Acc[2] = atof( substr ) * 1000.0;
            strncpy( substr, &line[60], 19 );
            GLOEphRecord->InforAge = atof( substr );
            
            return true;
        }
        else
        {
            for( i=0; i<3; i++ )
            {
                fgets( line, 100, FGLOEph );
            }
        }
    } while( feof(FGLOEph) == 0 );
    
    return false;
}    

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
                 GLONASSEPHREC* GLOEphRecord )
{
    double dt;
    
    dt = (Time->Week-GLOEphRecord->RefTime.Week) * SECPERWEEK
        + Time->SecOfWeek - GLOEphRecord->RefTime.SecOfWeek;
    
    if( fabs(dt) <= GLOEPHTIMELIMIT )
    {
        return true;            
    }
    else
    {
        if( ReadGLONASSEph( Slot, Time, GloTmCorr, GLOEphRecord ) == true )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
}

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
                                     double* ClkOft, double* ClkSft )
{
	short  Iterator;                    /* Iterator time less than 10 */
    double dte, dtc;
    
    double dn0,dn,dmk,ek1,ek,ek2,vk,fk,du,dr,di,uk,rk,dik;    // for position
    double ekdot,fkdot,dudot,drdot,didot,ukdot,rkdot,ikdot;   //for velocity
    double v11,v12,ok,okdot;
    double xyk[3], xykdot[3];

    if( CheckGPSEph( Prn, t, IonPara, Eph ) == false )
    {
        return false;
    }
    
    dte = (t->Week-Eph->TOE.Week)*SECPERWEEK + t->SecOfWeek - Eph->TOE.SecOfWeek;
    dtc = (t->Week-Eph->TOC.Week)*SECPERWEEK + t->SecOfWeek - Eph->TOC.SecOfWeek;

    dn0 = sqrt( GM_Earth ) / pow( Eph->SqrtA, 3.0 );
    dn  = dn0 + Eph->DetlaN;		
    dmk = Eph->M0 + dn * dte;
   
    /* ƫ����ǵ������� */

    ek1 = dmk;
	Iterator = 0;
    do{
        ek2 = ek1;
        ek  = dmk + Eph->e * sin(ek2);
        ek1 = ek;
	
		Iterator = Iterator + 1;
		if ( Iterator >= 10 )
		{
			break;
		}

    } while( fabs(ek2-ek)>1e-12 );
 
    ekdot = dn / ( 1.0 - Eph->e * cos(ek) );
    
    /*  �Ӳ������ټ��� */
    
    *ClkOft = Eph->ClkBias + Eph->ClkDrift * dtc + Eph->ClkDriftRate*dtc*dtc;
    *ClkSft = Eph->ClkDrift + 2.0 * Eph->ClkDriftRate * dtc;

    /*  �����ЧӦ���� */

    *ClkOft = *ClkOft - 4.442807633e-10 * Eph->e * sin(ek) * Eph->SqrtA;//�����ɹ㲥������������λ�á��ٶȼ����ȷ�����һ��
    *ClkSft = *ClkSft - 4.442807633e-10 * Eph->e * cos(ek) * Eph->SqrtA * ekdot;
    
    v11 = sqrt( 1 - pow( Eph->e, 2 ) ) * sin(ek);
    v12 = cos(ek) - Eph->e;
    vk  = atan2(v11,v12);			
    fk = vk + Eph->omega;
    
    fkdot = sqrt((1+Eph->e)/(1-Eph->e)) * pow(cos(vk/2)/cos(ek/2),2) * ekdot;
    
    du = Eph->Cuc * cos(2*fk) + Eph->Cus * sin(2*fk);
    dr = Eph->Crc * cos(2*fk) + Eph->Crs * sin(2*fk);
    di = Eph->Cic * cos(2*fk) + Eph->Cis * sin(2*fk);
    
    dudot = 2.0 * ( Eph->Cus*cos(2.0*fk) - Eph->Cuc*sin(2*fk) ) * fkdot;
    drdot = 2.0 * ( Eph->Crs*cos(2.0*fk) - Eph->Crc*sin(2*fk) ) * fkdot;
    didot = 2.0 * ( Eph->Cis*cos(2.0*fk) - Eph->Cic*sin(2*fk) ) * fkdot;
    
    uk  = fk + du;
    rk  = pow( Eph->SqrtA, 2 ) * (1 - Eph->e*cos(ek) ) + dr;
    dik = Eph->i0 + di + Eph->iDot * dte;
    
    ukdot = fkdot + dudot;
    rkdot = pow( Eph->SqrtA, 2 ) * Eph->e * sin(ek) *ekdot + drdot;
    ikdot = didot + Eph->iDot;
    
    xyk[0] = rk * cos(uk);
    xyk[1] = rk * sin(uk);
    
    xykdot[0] = rkdot * cos(uk) - rk * sin(uk) * ukdot;
    xykdot[1] = rkdot * sin(uk) + rk * cos(uk) * ukdot;
    
    ok = Eph->OMEGA + ( Eph->OMEGADot - Omega_WGS ) * dte 
        - Omega_WGS * Eph->TOE.SecOfWeek;
    
    okdot = Eph->OMEGADot - Omega_WGS;
 
    Pos[0]=xyk[0]*cos(ok) - xyk[1]*cos(dik)*sin(ok);
    Pos[1]=xyk[0]*sin(ok) + xyk[1]*cos(dik)*cos(ok);
    Pos[2]=xyk[1]*sin(dik);
    
    Vel[0]=cos(ok)*xykdot[0]-cos(dik)*sin(ok)*xykdot[1]
        -(xyk[0]*sin(ok)+xyk[1]*cos(ok)*cos(dik))*okdot+xyk[1]*sin(ok)*sin(dik)*ikdot;
    Vel[1]=sin(ok)*xykdot[0]+cos(ok)*cos(dik)*xykdot[1]
        +(xyk[0]*cos(ok)-xyk[1]*sin(ok)*cos(dik))*okdot-xyk[1]*cos(ok)*sin(dik)*ikdot;
    Vel[2]=sin(dik)*xykdot[1]+xyk[1]*cos(dik)*ikdot;
    
    return true;
}

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

bool ComputeGPSOrbitAndClockPartInfo( const int Prn, const GPSTIME* t,
                                     GPSEPHREC* Eph, IONOPARA* IonPara, 
                                     double Pos[3], double* ClkOft )
{
	short Iterator;
    double dte, dtc;
    
    double dn0,dn,dmk,ek1,ek,ek2,vk,fk,du,dr,di,uk,rk,dik;    // for position
    double v11,v12,ok;
    double xyk[2];

    if( CheckGPSEph( Prn, t, IonPara, Eph ) == false ) 
    {
        return false;
    }

    dte = (t->Week-Eph->TOE.Week)*SECPERWEEK + t->SecOfWeek - Eph->TOE.SecOfWeek;
    dtc = (t->Week-Eph->TOC.Week)*SECPERWEEK + t->SecOfWeek - Eph->TOC.SecOfWeek;
  
    dn0 = sqrt( GM_Earth ) / pow( Eph->SqrtA, 3.0 );
    dn  = dn0 + Eph->DetlaN;		
    dmk = Eph->M0 + dn * dte;
    
    /* ƫ����ǵ������� */
    
	Iterator = 0;
    ek1 = dmk;
    do{
        ek2 = ek1;
        ek  = dmk + Eph->e * sin(ek2);
        ek1 = ek;

		Iterator = Iterator + 1;
		if ( Iterator >= 10 )
		{
			break;
		}

    } while(fabs(ek2-ek)>1e-12);
    
    /*  �Ӳ������ټ��� */
    
    *ClkOft = Eph->ClkBias + Eph->ClkDrift * dtc + Eph->ClkDriftRate*dtc*dtc;
    
    /*  �����ЧӦ���� */
    
    *ClkOft = *ClkOft - 4.442807633e-10 * Eph->e * sin(ek) * Eph->SqrtA;
    
    v11 = sqrt( 1 - pow( Eph->e, 2 ) ) * sin(ek);
    v12 = cos(ek) - Eph->e;
    vk  = atan2(v11,v12);			
    fk = vk + Eph->omega;
    
    du = Eph->Cuc * cos(2*fk) + Eph->Cus * sin(2*fk);
    dr = Eph->Crc * cos(2*fk) + Eph->Crs * sin(2*fk);
    di = Eph->Cic * cos(2*fk) + Eph->Cis * sin(2*fk);
    
    uk  = fk + du;
    rk  = pow( Eph->SqrtA, 2 ) * (1 - Eph->e*cos(ek) ) + dr;
    dik = Eph->i0 + di + Eph->iDot * dte;
    
    xyk[0] = rk * cos(uk);
    xyk[1] = rk * sin(uk);
    
    ok = Eph->OMEGA + ( Eph->OMEGADot - Omega_WGS ) * dte 
        - Omega_WGS * Eph->TOE.SecOfWeek;
    
    Pos[0]=xyk[0]*cos(ok) - xyk[1]*cos(dik)*sin(ok);
    Pos[1]=xyk[0]*sin(ok) + xyk[1]*cos(dik)*cos(ok);
    Pos[2]=xyk[1]*sin(dik);
    
    return true;
}

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
                      double dY[6] )
{
    int i;
    double GM_N, Pos_N[3], R_N;
    double R = sqrt( VectDot( 3, 3, Pos, Pos ) );
    double tmp;

    GM_N = GM_PZ90 / pow(R,2.0);
    R_N  = R_PZ90 / R;
    
    for( i=0; i<3; i++ )
    {
        dY[i] = Vel[i];
        Pos_N[i] = Pos[i]/R;
    }
    
    tmp = 1.5*Geopo_J02*GM_N*pow(R_N,2.0)*(1.0-5.0*pow(Pos_N[2],2.0));
	//Ϊʲô���ٶ�Ҫ��ô��?����������ϵ��ԭ����
    dY[3] = -GM_N*Pos_N[0] 
        - tmp*Pos_N[0] + pow(Omega_PZ90,2.0)*Pos[0] + 2.0*Omega_PZ90*Vel[1] + Acc[0];
    
    dY[4] = -GM_N*Pos_N[1]
        - tmp*Pos_N[1] + pow(Omega_PZ90,2.0)*Pos[1] - 2.0*Omega_PZ90*Vel[0] + Acc[1];
    
    dY[5] = -GM_N*Pos_N[2] - 1.5*Geopo_J02*GM_N*pow(R_N,2.0)*(3.0-5.0*pow(Pos_N[2],2.0)) 
        *Pos_N[2] + Acc[2];

}

                      
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
                             double Pos[3], double Vel[3] )
{
    int sign, i;                      /* dT�ķ���, ����ʱ��ǰ������� */
    double dT, Step, h;               // ���ֲ���[s], �������Step, �ֲ�����.
    double r0[3], v0[3], Y[5][6];     /* r0, v0 �洢����һ�����м��� */
    double r[3],  v[3];               /*  ÿ�λ��ֵ��м��� */
    GPSTIME CurrTime;

    if( CheckGLOEph( Slot, t, GloTmCorr, Eph ) == false )
    {
        return false;
    }
    
	if( Eph->Health != 0 )   /* ���ǲ����� */
	{
		return false;
	}

    CurrTime.Week = Eph->RefTime.Week;
    CurrTime.SecOfWeek = Eph->RefTime.SecOfWeek;

    h= 120.0;

    dT = (t->Week-CurrTime.Week)*SECPERWEEK + t->SecOfWeek - CurrTime.SecOfWeek;
   
    if( dT >= 0.0 )
    {
        sign = 1;
    }
    else
    {
        sign = -1;
    }
  
    for( i=0; i<3; i++ )
    {
        r0[i] = Eph->Pos[i];
        v0[i] = Eph->Vel[i];
    }
    
    do{
        if( fabs(dT) > h )  /* ���ֲ���Ϊ120s */
        {
            Step = h * sign;    /*  ���û��ֲ��� */
        }
        else
        {
            Step = dT;
        }

        for( i=0; i<3; i++ )
        {
            r[i] = r0[i];
            v[i] = v0[i];
        }
        ComputeRightFunc( r, v, Eph->Acc, Y[0] );
        
        /*   compute right function at the second time */        

        for( i=0; i<3; i++ )
        {
            v[i] = v0[i] + Y[0][3+i] * Step / 4.0;
            r[i] = r0[i] + Y[0][i]   * Step / 4.0;
        }
        ComputeRightFunc( r, v, Eph->Acc, Y[1] );
        
        /*   compute right function at the third time */
        
        
        for( i=0; i<3; i++ )
        {
            r[i] = r0[i] + (Y[0][i]  *3/32 + Y[1][i]  *9/32.0) * Step;
            v[i] = v0[i] + (Y[0][3+i]*3/32 + Y[1][3+i]*9/32.0) * Step;
        }
        ComputeRightFunc( r, v, Eph->Acc, Y[2] );
        
        /*   compute right function at the fourth time */
        
        
        for( i=0; i<3; i++ )
        {
            r[i] = r0[i] + (Y[0][i]  *1932/2197.0 - Y[1][i]  *7200/2197.0
                + Y[2][i]  *7296/2197.0 )* Step;
            v[i] = v0[i] + (Y[0][3+i]*1932/2197.0 - Y[1][3+i]*7200/2197.0
                + Y[2][3+i]*7296/2197.0 )* Step;
        }
        ComputeRightFunc( r, v, Eph->Acc, Y[3] );
        
        /*   compute right function at the fourth time */
        
        
        for( i=0; i<3; i++ )
        {
            r[i] = r0[i] + (Y[0][i]*439/216.0 - Y[1][i]*8.0
                + Y[2][i]*3680/513.0 - Y[3][i]*845/4104.0 )* Step;
            v[i] = v0[i] + (Y[0][3+i]*439/216.0 - Y[1][3+i]*8.0
                + Y[2][3+i]*3680/513.0 - Y[3][3+i]*845/4104.0 )* Step;
        }
        ComputeRightFunc( r, v, Eph->Acc, Y[4] );
        
        /*  ���ֽ��  */
        
        CurrTime.SecOfWeek = CurrTime.SecOfWeek + Step;
        if( CurrTime.SecOfWeek < 0.0 )    /*  ���ܼ�� */
        {
            CurrTime.Week = CurrTime.Week - 1;
            CurrTime.SecOfWeek = CurrTime.SecOfWeek + SECPERWEEK;
        }
        else if( CurrTime.SecOfWeek > SECPERWEEK )
        {
            CurrTime.Week = CurrTime.Week + 1;
            CurrTime.SecOfWeek = CurrTime.SecOfWeek - SECPERWEEK;
        }
        
        for( i=0; i<3; i++ )
        {
            r0[i] = r0[i] + ( Y[0][i]*25/216 + Y[2][i]*1408/2565
                + Y[3][i]*2197/4104 - Y[4][i]/5 ) * Step;
            v0[i] = v0[i] + ( Y[0][i+3]*25/216 + Y[2][i+3]*1408/2565
                + Y[3][i+3]*2197/4104 - Y[4][i+3]/5 ) * Step;
        }
   
        dT = (t->Week-CurrTime.Week)*SECPERWEEK + t->SecOfWeek - CurrTime.SecOfWeek;
        
    }while( fabs(dT) > 1E-8 );

    for( i=0; i<3; i++ )
    {
        Pos[i] = r0[i];
        Vel[i] = v0[i];
    }

    return true;    
}


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
bool ComputeGLONASSSClockInfo( const int Slot, const GPSTIME* t,
                             GLONASSEPHREC* Eph, GLOTIMECORR* GloTmCorr,
                             double* ClkOft, double* ClkSft )
{
    double dt;

    if( CheckGLOEph( Slot, t, GloTmCorr, Eph ) == false )
    {
        return false;
    }
 
	if( Eph->Health != 0 )   /* ���ǲ����� */
	{
		return false;
	}
   
    dt = (t->Week - Eph->RefTime.Week) * SECPERWEEK 
        + t->SecOfWeek - Eph->RefTime.SecOfWeek;

    *ClkOft = Eph->ClockBias + Eph->FreqBias * dt;

    *ClkSft = Eph->FreqBias;

    return true;
}


