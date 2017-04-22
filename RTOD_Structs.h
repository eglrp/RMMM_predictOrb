/****************************************************************************
Ŀ�ģ�    ����LEOSAOD�����Ҫ�����нṹ��
��дʱ�䣺2008.21.42
�汾:     V1.1
��Ȩ��    �人��ѧ
****************************************************************************/
#pragma once 
#ifndef _RTOD_STRUCTS_H_
#define _RTOD_STRUCTS_H_

#define ADIMENSION   15 /*position,velocity,GPS clk,GLONASS clk,clksft,Cd,Cr,tao,W*/
#define RELDIMENSION 36 /*position,velocity,GPS rel clk,Cd, Cr,W,L1ģ����12��,����ģ����12��*/
#define MAXCHANNUM 12
#define MAXOBSTYPENUM 11
#define TT_TAI   32.184
#define GPST_TAI -19.0
#define MaxValiEpochNum 20
#define MAXOBSNUMDAY 8640
//   LEMTA_1  the wavelength of the primary carry frequency of GPS.
#define	LEMTA_1		0.190293672798         //��λΪ��
//   LEMTA_2  the wavelength of the secondary carry frequency.
#define LEMTA_2		0.2442102134           //��λΪ��
/* ��������ϵͳ���� */
enum GNSSSys { UNKS, GPS, GLONASS, GALILEO, GEO, COMPASS };

/* �۲��������Ͷ��� */
enum OBSTYPE  { UNKOBS, C1, P1, L1, D1, C2, P2, L2, D2, C5, L5, D5, S1, S2,Nw,Sw };  


typedef struct _COMMON_TIME_   /* ͨ��ʱ�䶨�� */
{
	unsigned short Year;
	unsigned short Month;
	unsigned short Day;
	unsigned short Hour;
	unsigned short Minute;
	double Second;
} COMMONTIME;

typedef struct _GPS_TIME_              /* GPSʱ�䶨�� */
{
	unsigned short Week;          
	double         SecOfWeek;     
} GPSTIME;

typedef struct _MJD_TIME_             /* �������� */
{
	int Days;             
	double FracDay;      
} MJDTIME;

/* ������ת�����ṹ�嶨�� */
typedef struct _EOP_PARA_
{
	double Mjd;          /* �������� */
	double x;            /* ���Ʋ���x[����] */
	double y;            /* ���Ʋ���y[����] */
	double dUT1;         /* UT1-UTC [s]     */
	double LeapSec;      /* ��ǰʱ�̵�������[s] */
	char   Status;       /* ��ʼ����־ */
} EOPPARA;


/***************************************************************************
DYNMODELPARA

Ŀ��:  ���嶯��ѧģ��ʹ�õĲ���

***************************************************************************/
typedef struct _DYNAMICMODEL_
{
	int     n_a, m_a;   /* Degree and order of gravity field */
	double  Cd;         /* Coefficient of atmospheric drag */
	double  Cr;         /* Coefficient of solar radiation pressure */
	double  Mass;       /* Mass of spacecraft [kg] */
	double  Area_H;     /* surface area of spacecraft Cross-section[m^2]*/
	double  Area_R;     /* surface area of spacecraft solar radiation pressure[m^2]*/
	double  GM;         /* Gravitational coefficient [m^3/s^2] */
	double  R_ref;      /* Reference radius [m]   */
} DYNMODELPARA;

typedef struct _IONO_CORRECT_PARA_
{
	double alpha[4];        /* ��������ģ�Ͳ��� alpha */
	double beta[4];         /* ��������ģ�Ͳ��� beta  */
	bool   IsValid;         /* ��������������ЧΪtrue */
} IONOPARA;

typedef struct _GLONASS_TIMESYSTEM_CORRECTION_
{
	double  TauC;
	GPSTIME RefTime;
} GLOTIMECORR;


typedef struct _GPS_EPH_RECORD_
{
	short       PRN;
	GPSTIME  	TOC;
	double		ClkBias;
	double		ClkDrift;
	double		ClkDriftRate;
	double		IODE;
	double		Crs;
	double		DetlaN;
	double		M0;
	double		Cuc;
	double		e;
	double		Cus;
	double		SqrtA;
	GPSTIME	    TOE;
	double		Cic;
	double		OMEGA;
	double		Cis;
	double		i0;
	double		Crc;
	double		omega;
	double		OMEGADot;
	double		iDot;
	double		CodesOnL2Channel;
	double		L2PDataFlag;
	double		SVAccuracy;
	double		SVHealth;
	double		TGD;
	double		IODC;
	double		TransTimeOfMsg;
	double		FitInterval;
	double		Spare1;
	double		Spare2;
} GPSEPHREC;

typedef struct _GLONASS_EPH_RECORD
{
	short   SlotNum;          /*  Slot number in sat. constellation */
	GPSTIME RefTime;         /*  Epoch of ephemerides (UTC) */
	double  ClockBias;       /*  SV clock bias (sec) (-TauN)  */
	double  FreqBias;      /* SV relative frequency bias (+GammaN) */
	double  Tk;     /* message frame time (0 <= Tk < 86400 sec of day UTC)*/
	double  Pos[3];       /*  Satellite position [m]  */
	double  Vel[3];       /*  Satellite  velocity[m/s] */
	double  Acc[3];       /*  Satellite  velocity[m/s^2] */
	int     Health;       /*  health (0=OK) */
	double  FreqNum;      /*  frequency number (1-24) */
	double  InforAge;     /*  Age of oper. information (days) */
} GLONASSEPHREC;


/*  ����۲�ֵ���͵��б�˳��  */
typedef struct _OBS_TYPE_LIST_
{
	int ObsTypeNum;
	OBSTYPE ObsType[MAXOBSTYPENUM];
} OBSTYPELIST;

/* �۲����ݼ������� */
typedef struct _OBSDATA_
{
	bool     Availability; /*  �Ƿ���� */
	OBSTYPE  Type;     /*  �۲���������  */
	double   Obs;      /*  �۲�ֵ        */
	int		 LLI;		/*���ݱ�ʶ����1Ϊ������9Ϊ�ֲ�*/
} ObsData;

/*  ÿ�����ǵĹ۲����ݶ���  */
typedef struct _SATOBSDATA_
{
	short           Prn;
	GNSSSys         System;
	ObsData         Data[MAXOBSTYPENUM];     
	short           Used;            /* û������Ϊ0, ����Ϊ1, �ֲ�Ϊ-1, ��ʼ��Ϊ1 */ 
	double			satPos[3];			 /*�ع�ϵ���������꣬���ﱣ�����B��������ģ���ΪB�ǰ�A�ǵĸ���*/
	double			elevation;			/*���Ǹ߶Ƚ�*/
	double			Ion;				/*��б������ӳ�*/
	double			wValue;				/*w������*/
} SatObsData;

/*  ÿ����Ԫ�Ĺ۲����ݶ���  */
typedef struct _EPOCHOBSDATA_
{
	GPSTIME           Time;
	short             EpochFlag;
	short             SatNum;
	SatObsData        SatObs[MAXCHANNUM];
} EpochObsData;

typedef struct ONESATTA11OBS{
	int		index_satlist;	//satlist������
	int		used;			//�������Ƿ����
	int		flag;			//����۲�ֵ�Ƿ���������-1Ϊ����������0Ϊ������������1Ϊ������
	int     PRN;			//���ǵ�Prn��
	double  CA1;			//��׼վ��CA��
	double	CA2;			//����վ��CA��
	double  P11;			//����վ��P1��
	double  P21;			//����վ��P1��
	double  P12;			//����վ��P2��
	double  P22;			 //����վ��P2��
	//�����
	double  S11;			//1������
	double  S21;			//2������
	double  S12;			//1������
	double  S22;			//2������
	//L1�ز�
	double  L11;			//1������
	double  L21;			//2������

	//L2�ز�
	double  L12;			//1������
	double  L22;			//2������
	int	    L1ApplyEpoch;   //L1---�۲����ݵ�ʹ�ñ�־
	int	    L2ApplyEpoch;   //L2---�۲����ݵ�ʹ�ñ�־
	int     CAApplyEpoch;   //CA---�۲����ݵ�ʹ�ñ�־
	int	    P1ApplyEpoch;   //P1---�۲����ݵ�ʹ�ñ�־
	int	    P2ApplyEpoch;   //P2---�۲����ݵ�ʹ�ñ�־
	//����۲�ֵ
	double	dL1;			//����L1�۲�ֵ,��λ����
	double	dL2;			//����L2�۲�ֵ,��λ����
	double  dCA;			//����CA�۲�ֵ
	double	dP1;			//����P1�۲�ֵ
	double	dP2;			//����P2�۲�ֵ
	double  satPos[3] ;     //����λ������
	double  elevation[2];   //elevation1---��׼վ�ĸ߶ȽǱ�־  elevation2---����վ�ĸ߶ȽǱ�־,��λ�ǻ���rad
	double	map[2];			//��׼վ������վ��ͶӰ����
	//��������̽���м����
	double	Nw_SD;				//����ģ����
	double	sigNw_SD;			//����ģ���ȵķ���
	int		Nw_num_SD;			//����ģ������Ԫ��
	//�ǲ�����̽���м����
	double  Nw_A;				//A�ǿ���ģ����
	double	sigNw_A;			//A�ǿ���ģ���ȵķ���
	int		Nw_num_A;			//A�ǿ���ģ������Ԫ��
	double  Nw_B;				//A�ǿ���ģ����
	double	sigNw_B;			//A�ǿ���ģ���ȵķ���
	int		Nw_num_B;			//A�ǿ���ģ������Ԫ��
	//LLI����̽����
	int	   LLI_L1A;				//A��L1Ƶ��LLI
	int	   LLI_L2A;				//A��L2Ƶ��LLI
	int	   LLI_L1B;				//B��L1Ƶ��LLI
	int	   LLI_L2B;				//B��L2Ƶ��LLI
}OneSat11Obs;
/*  ÿ����Ԫ�Ĺ�ͬ�۲�ֵ  */
typedef struct _COMMONOBS_
{
	GPSTIME           Time;//�����ǵ�ʱ��
	short             ComSatNum;
	int				  PRN[MAXCHANNUM];
	OneSat11Obs       comobs[MAXCHANNUM];
}Common11Obs;
struct ONESATDDBIASE							//�ѹ̶���˫��ģ����
{
	
	int nonRefPrn;								//�ǲο����ǵ�PRNֵ

	int     ddF1;							//˫���ز�L1�ѹ̶���ģ����
	int     ddF2;							//˫���ز�L2�ѹ̶���ģ����
	
	//SATPOSXYZ  satpos;							//��ǰ�ĸ��ǵ�λ��

};
struct DDPSEUDOOBS                      //˫��۲����ݽṹ��
{
	int				refPrn;			   //�ο����ǵ�PRNֵ
	int				DDObsNum;         //˫����ģ���ȵ���Ŀ����һ����ComSatNum-1
    ONESATDDBIASE satddobs[MAXCHANNUM]; //˫��۲�����
};
/*  �ز���λƽ��α����м�ṹ�嶨��  */
typedef struct _PHASESMOOTHPSEUDORANGE_
{
	short       CurrNum;   /* ��ǰƽ����Ԫ���� */
	GPSTIME     CurrTime;  /* ��ǰ��Ԫ��GPSʱ�� */

	double      PSC1;      /* ��ǰ��Ԫ��CA��ƽ�����  */
	double      PSP1;      /* ��ǰ��Ԫ��P1��ƽ�����  */
	double      PSP2;      /* ��ǰ��Ԫ��P2��ƽ�����  */
	double      CPL1;      /* ��ǰ��Ԫ��L1  */
	double      CPL2;      /* ��ǰ��Ԫ��L2  */
	bool        GoodC1;    /* ��ǰ��ԪCA��ƽ��״̬��true or false */
	bool        GoodP1;    /* ��ǰ��ԪP1��ƽ��״̬��true or false */
	bool        GoodP2;    /* ��ǰ��ԪP2��ƽ��״̬��true or false */
	bool        GoodL1;    /* ��ǰ��ԪL1���ݵ�������true or false */
	bool        GoodL2;    /* ��ǰ��ԪL2���ݵ�������true or false */
} PSPRange;


typedef struct _SPACECRAFT_STATE_
{
	GPSTIME   Mjd_GPS;     
	double    Pos[6];   /* position and velocity  */
	double    Acc[3];
} SCState;


/* 
���㶨λ�������б�
*/
typedef struct _SATLIST_PP_
{
	GNSSSys System;
	short   Prn;
	int     Status;        /*  �������ǵ�״̬: 1Ϊ����, 0Ϊ������/������/�߶Ƚǵ�,
						   -1Ϊ��ǰ���麬�дֲ�, -2Ϊ�������дֲ�*/
	int		index;			/*����ڹ۲�ֵ������*/
} SATLIST;

/* ÿ����Ԫ���㶨λ�Ͳ��ٵĽ�����侫��ָ�� */
typedef struct _EPOCH_PP_RESULT_
{
	double Position[3];
	double PosBLH[3];
	double Velocity[3];
	double VelNEU[3];
	double RcvClkOft[2];         /* 0 ΪGPS�Ӳ�, 1ΪGLONASS�Ӳ�  */
	double MatrixFromBLHtoNEU[9];
	double RcvClkSft;
	double PDOP;
	double GDOP;
	double HDOP;
	double VDOP;
	double TDOP;
	double SigmaPos;
	double SigmaVel;
	double Residual[MAXCHANNUM];     /* α��۲�ֵ�в� */
	SATLIST SatList[MAXCHANNUM];     /* ���㶨λ�����б�, �͹۲�ֵ˳�򱣳�һ�� */
	int    Iterator;                 /* ���㶨λ�������� */
	double Coverage;                 /* ���㶨λ�������� */
	bool   IsSuccess;                /* ���㶨λ�Ƿ�ɹ�, 1Ϊ�ɹ�, 0Ϊʧ�� */
	int    SatNumUsed;				/*�õ�GPS������*/
} PPRESULT;

/* ��λ����(�������㶨λ���˲���λ)��Ҫ�õ���ÿ�����ǵ��м������ */
typedef struct _EPOCH_MIDLLE_RESULT_FOR_ONE_SV_
{
	double SatPos[3];
	double SatVel[3];
	double Elevation;
	double SatClkOft;
	double SatClkSft;
	double Relativity;
	double IonoCorr;
	double TropCorr;
} SATMIDRESULT;

typedef struct _APRIORI_LEO_ORBIT_CLK_
{
	double LeoPos[3];
	double LeoClk[2];

	double OrbitAccuracy;      /* PDOP*Sigma */
	int    Validate;           /* SatNum>=5, Coverage, Accuracy etc. */
} APRIORBIT; 


typedef struct _FINAL_RESULT_OUTPUT_STRUCT_
{
	COMMONTIME GT;              /* �����Ӧ��GPSʱ��, ��ͨ��ʱ��ʽ��� */
	double     Mjd_UTC;         /* �����Ӧ��UTCʱ��, �Լ������ո�ʽ��� */
	double     ECF_Orb[6];      /* �˲����������λ�ú��ٶ�,��WGS84�ع�ϵ */
	double     ECI_Orb[6];      /* �˲����������λ�ú��ٶ�,��J2000.0����ϵ */
	double     MeanElements[6]; /* ƽ����� a,e,i,OMEGA,omega,M */
	double     OscuElements[6]; /* ˲ʱ��� */
	double     FirstMeanElements[6]; /* ��һ�������ƽ�� */
	double     FirstOscuElements[6]; /* ��һ�������˲�� */
	double     da;                   /* ������仯�� */
	double     c1, c2, c3;
	double     Sg0;                  /* �������κ���ʱ */
	double     Cov[6];               /* J2000����ϵ�˲�����ľ���ָ�� */

	bool       Valid;               /* ���ת������Ч��, ƽ������Ƿ���ȷ */
} FINALORBIT;
typedef struct _EKF_STATE_INFOMATION_
{
	double			Sigma[2],Step,wuchaMod;
	double			CentBias[6];								/*	��λƫ��  A3B3*/
	double			Tao[2];										/*	A��B��Tao*/
	IONOPARA		IonPara;
	APRIORBIT		AprioriState[2];							/*�ع�ϵ�µ���������*/
	DYNMODELPARA	Para[2];
	SCState			IntSate[4];									/*A2B2*/
	GPSTIME			Time;										/* Time       */
	double			StateA[ADIMENSION];							/*�ο���A��״̬*/
	double			StateB[RELDIMENSION];						/*B��״̬��ʱ�����ʱ��*/
	double			CovA[ADIMENSION*ADIMENSION];				/*�ο���A�ķ���*/
	double			StateRel[RELDIMENSION];						/*���״̬*/
	double			CovRel[RELDIMENSION*RELDIMENSION];			/*���״̬�ķ���*/
	double			ApriSigma[2];								/* ������Ϣ�ı�׼�� A1B1*/
	double			PostSigma[2];								/* �������º�ı�׼�� A1B1*/
	double			sigPosPC;
	double			sigPosLC;
	double			StateInECEFA[6];							/* �ع�ϵ�µ��˲�λ��״̬ */
	double			StateInECEFB[6];							/* �ع�ϵ�µ��˲�λ��״̬ */
	int				SatNumUsed[2];
	int				comSatNumUsed;
	short			KFConvergency[2];							/* 0��ʾû������,1Ϊ����,2Ϊ��ɢ  */
	short			IsInitial[2];								/* 0��ʾ�˲�û�г�ʼ����1Ϊ�Ѿ���ʼ�����������ʱʹ�� A1B1*/
	int				PRN[MAXCHANNUM];						    /*����Ƶ��ģ����ͨ��������˳��Ӧ��һ��δ��־��Ϊ-999*/
	Common11Obs		CurComObs;									/*��ǰ��Ԫ�Ĺ�ͬ�۲����ݣ�����α����ز�ԭʼ�۲�ֵ*/ 
	Common11Obs     *wholeComObs;								/*һ���ܵĹ۲�ֵ*/
	//int				ComEpochNum	;								/*��������Ԫ��*/
	int				eleMask;                                    /*�߶Ƚ���ֵ*/
	DDPSEUDOOBS     ddOBS;										/*����ģ���ȹ۲�ֵ   rj 20160727*/
} EKFSTATE;

#endif
