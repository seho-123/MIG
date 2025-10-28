/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Package: 
 * File: WSI_types.h
 * Author: ckaczkowski
 * Created: 17 June 2014
 *******************************************************************/

#include <bur/plctypes.h>


#ifndef _WSITYPES_H
#define _WSITYPES_H


#include <..\lib\Statelib.h>
//#include <..\Universal\SuperDefinition.h>


#if 0
#define PARAM_NUM_MAX  		256
#define PARAM_INT_NUM_MAX  	8
#define PARAM_ENUM_NUM_MAX  32
#define PARAM_STR_NUM_MAX   16
//#define PARAM_ENUM_NUM_MAX  8
//#define PARAM_STR_NUM_MAX   8
#define VAR_NUM_MAX    		128
#define ENUM_VAR_MAX   		32
#define INT_VAR_MAX    		32
#define STR_VAR_MAX    		64
//#define SEGM_NUM_MAX   		31
#define SEGM_NUM_MAX   		32
#define TXT_RESOURCES_MAX 	64
#define RES_TXT_LEN 		16
#define PAGES_MAX			64

#endif


#define CALIB_ONE2ONE			0
#define CALIB_2POINTS 			1
#define CALIB_2POINTS_4OFF 		2
#define CALIB_2POINTS_INV 		3
#define CALIB_2POINTS_4OFF_INV	4

#define CALC_SLOPE_BOTH			10



/////////////////////////////////////////////////////
typedef struct AzzNode_typ
{
	float X;
	float Y;
	
} AzzNode_typ;

typedef struct AzzLinSpline
{
	/* VAR_INPUT (analog) */
	unsigned short NbNodes;
	AzzNode_typ* pNodeVec;
	unsigned char Smooth;
	float Rness;
	signed char DisMode;
	float x;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	INT _i;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
	plcbit Update;
} AzzLinSpline_typ;


// New Functions from Paul
typedef struct AzzTON {
  BOOL IN;
  TIME PT;
  BOOL Q;
  TIME ET;
  TIME StartTime;
  BOOL M;
  UDINT Restart;
} AzzTON_typ;

typedef struct AzzMovAvgFlt {
	/* VAR_INPUT (analog) */
	float x;
	unsigned long base;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float y;
	/* VAR (analog) */
	double sum_old;
	//signed long sum_old_dbl_2;
	unsigned long p_xold;
	unsigned short i_xold;
	unsigned long base_old;
	unsigned long bootkey_old;
	/* VAR_INPUT (digital) */
	plcbit enable;
	/* VAR (digital) */
	plcbit enable_old;
} AzzMovAvgFlt_typ;


typedef struct AzzPIDTune{
	float nothing;
  
} AzzPIDTune_typ;

typedef struct AzzPID{
	float i_sum, d_old, e_fbk;
	DINT last_time;
} AzzPID_typ;

typedef struct azzslimpid_par {
	float Y_max;
	float Y_min;
	float Kp;
	float Tn;
	float Tv;
	float Kfbk;
} azzslimpid_par_typ;

typedef struct AzzSlimPID{
	/* VAR_INPUT (analog) */
	float W;
	float X;
	unsigned long request;
	azzslimpid_par_typ* pPar;
	/* VAR_OUTPUT (analog) */
	float e;
	float Y;
	float dt;
	unsigned short status;
	unsigned short addInfo;
	/* VAR (analog) */
	AzzMovAvgFlt_typ MovAvgFlt_inst;
	AzzPIDTune_typ PIDTune_inst;
	AzzPID_typ PID_inst;
	unsigned long requ_old;
	/* VAR_INPUT (digital) */
	plcbit enable;
	/* VAR (digital) */
	plcbit enable_old;
} AzzSlimPID_typ;

typedef struct AzzRamp{
	/* VAR_INPUT (analog) */
	float x;
	float dy_up;
	float dy_down;
	float y_max;
	float y_min;
	float y_set;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float y;
	/* VAR (analog) */
	float dt;
  	float y_old;
	//signed long y_dbl_1;
	//signed long y_dbl_2;
	unsigned long timestore1;
	//unsigned long timestore2;
	//unsigned long getDtForceOff;
	//struct SysInfo sysinfo_inst;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit set;
	/* VAR_OUTPUT (digital) */
	plcbit x_reached;
	plcbit max_limit;
	plcbit min_limit;
} AzzRamp_typ;


typedef struct AzzLimScal {
  REAL x, x1, y1, x2, y2, y;
} AzzLimScal_typ;

typedef struct AzzScal{
  REAL x, x1, y1, x2, y2, y;
} AzzScal_typ;


//////////////////////////////////////////
///////////////////////////////////////////////////////////



typedef struct LogItem_typ{

	signed long timeStamp;
	signed short status; 
	unsigned long value; 
	plcstring  txt[32];
	
}LogItem_typ;

typedef struct WarnItem_typ{
	signed short status; 
	plcstring  txt[32];
	
}WarnItem_typ;

typedef struct ERROR_Mgr_typ{
	SINT Error;
	unsigned short lastItemIndx;	
	unsigned short history[128];
	LogItem_typ logs[128];
	UINT warnings;

}ERROR_Mgr_typ;



/*********************************************************************/

/***********   VECTOR STATE        ***************************************/

/*********************************************************************/


#define vectSt_Mode_OFF 0
#define vectSt_Mode_Idle 1
#define vectSt_Mode_Hold 2
#define vectSt_Mode_Home 3
#define vectSt_Mode_Pos 4
#define vectSt_Mode_Vel 5
#define vectSt_Mode_Torq 6
#define vectSt_Mode_Volt 7
#define vectSt_Mode_PT 	 8
#define vectSt_Move_Abs  9
#define vectSt_Move_Rel  10  
#define vectSt_Mode_Custom 11
#define vectSt_Mode_Drive_Independant 12


/******   Generic Vector Command to TML *************/
#define VECT_UpdateSignFilter		1	
#define VECT_ClearSignFilter		2
#define VECT_FullSignFilter			3
#define VECT_Set_Gain				4
#define VECT_Set_Gain_Zero			5
#define VECT_Set_PID_Param			6
#define VECT_Set_SATP				7
#define VECT_Set_SATP_Zero			8

#define VECT_Set_Param_0			10
#define VECT_Set_Param_1			11
#define VECT_Set_Param_2			12
#define VECT_Set_Param_3			13
#define VECT_Set_Param_4			14
#define VECT_Set_Param_5			15
#define VECT_Set_Param_6			16
#define VECT_Set_Param_7			17
#define VECT_Set_MaxError			18

#define VECT_Set_CmdWord			20


#define VECT_OSC_Start_Cmd			40
#define VECT_OSC_Stop_Cmd			41
#define VECT_OSC_Update_Cmd			42
#define VECT_OSC_Stop2_Cmd			43


#define VECT_Move_Abs_Cmd			22

#define VECT_Move_Rel_Cmd			23
#define VECT_Move_Stop_Cmd			24

#define VECT_ARC_Stop_Cmd			25
#define VECT_ARC_Start_Cmd			26

#define VECT_ZERO_AMPS_Cmd			27
#define VECT_DEFAULT_AMPS_Cmd		28

#define VECT_RESTART_SRV_Cmd		29

#define VECT_Set_KPP				30
#define VECT_Set_KPP_Zero			31

#define VECT_Upd_CACC				32
#define VECT_Upd_CSPD				33


#define VECT_Set_Act_Pos			35

#define VECT_Set_OUT0				50
#define VECT_Rst_OUT0				51
#define VECT_Set_OUT1				52
#define VECT_Rst_OUT1				53
#define VECT_Set_OUT2				54
#define VECT_Rst_OUT2				55
#define VECT_Set_OUT3				56
#define VECT_Rst_OUT3				57
#define VECT_Set_OUT4				58
#define VECT_Rst_OUT4				59

#define VECT_Set_FFL				60

#define VECT_Refresh_Mode			70


#define VECT_Set_OSC_Max 		80
#define VECT_Set_OSC_Min 		81
#define VECT_Set_OSC_Time		82
#define VECT_Set_OSC_DwellIn 	83
#define VECT_Set_OSC_DwellOut 	84
#define VECT_Set_OSC_Steer		85 
#define VECT_Set_OSC_Cmd_Param 	86
#define VECT_Set_OSC_Jog_Volt	87
#define VECT_Set_OSC_Jog_Speed	88
#define VECT_Set_OSC_Jog_Step	89
#define VECT_Set_OSC_Amplit 	90
#define VECT_Set_OSC_AllParam 	91
#define VECT_Set_OSC_Home 		92
#define VECT_Set_All_Run_Param 	93

#define VECT_Set_CACC			100
#define VECT_Get_APOS			101
#define VECT_STOP				102
#define VECT_EINT				103

#define VECT_Cmd_SAP			110
#define VECT_Cmd_CPA			111
#define VECT_Cmd_CACC			112
#define VECT_Cmd_CPOS			113

#define VECT_Cmd_MIG_OSC		120

#define VECT_Read_MCR			130

#define VECT_UPD_Cmd			135
#define VECT_Move_Abs_Param		136



typedef struct STATE_VECTOR_typ
{ 	
	//plcbit OK;
	float x; 	// value
	float x2; 	// value
	float vel; 	// first derivative
	float acc; 	// second derivative
	float amp; 	// current amps
	float volt; // volts
	float softLimits[2]; // software limits
	//DINT Center;
	float Center;
	signed short Status;
	signed short Mode;
	
}STATE_VECTOR_typ;

typedef struct StateVectorProxy_typ{
	plcstring Name[16];
	UINT Indx;
   /* VAR_INPUT (analog) */	
   UINT simul;
   UINT testMode;
   UINT xCMD;
   UINT cmdWord;
   float fParam[8];
   DINT Param[8];
   unsigned short MCRReg;
   unsigned long StatusReg;
   USINT xCmd_busy;
   float InVal;	
   float maxVel;
   float maxAcc;
   unsigned short curStep;
   signed short Mode;
   /* VAR_OUTPUT (analog) */
   STATE_VECTOR_typ OutVect;
   signed short Status;
   STATE_VECTOR_typ FbkVect;
   DINT actPosDINT;
   INT FbkWarning;
   UDINT VirStatus;
   
	
}StateVectorProxy_typ;


/******* Files Operation***************/

#define PRG_FILE_EXT ".prg"
#define PRG_NO_NEXT "None"



#define F_READ 				1
#define F_WRITE 			2
#define F_DELETE			3
#define F_FREE_MEM 			4

#define F_LOAD_CURCFG		5
#define F_SAVE_CURCFG		6
#define F_SAVE_AS_CURCFG	7

#define F_RENAME			8
#define F_COPY				9
#define F_GET_NEXT_FILE		10	
#define F_SET_NEXT_FILE		11	
#define F_READ_FILE_TO_TMP 	12
#define F_SAVE_AS_TMP 		13

#define F_READ_DIR 			20
#define F_READ_FILE_INF 	21
#define F_READ_DIR_FILELIST 22

#define F_OPEN_ONLY 		30
#define F_CREATE_ONLY 		31
#define F_WRITE_ONLY 		32
#define F_CLOSE_ONLY 		33

#define F_READ_FILES_CHAIN  40
#define F_EDIT_CHAIN  		41

#define F_SAVE_CURCFG_BIN		45
#define F_SAVE_AS_CURCFG_BIN	46
#define F_SAVE_SETUP_BIN		47
#define F_SAVE_AS_FULL_BIN	48

#define F_MODE_BINARY		0
#define F_MODE_XML			1
#define F_MODE_BINARY_FULL	2

typedef struct FILE_RW_typ
{

	USINT CMD;
	plcbit Done;
	plcbit Error;
	plcbit FileLocked;
	plcbit NewFileLoaded;
	USINT Mode;

	//plcstring varName[32];
	plcstring fileName[64];
	plcstring fileNewName[64];
	
	plcstring deviceName[32];
	plcstring targetDeviceName[32];

	plcstring MsgText[128];
	//FILE_SCHEDULE_typ FileSchedule;
	
	
	//CurCfg_typ curCfgTemp;
	//fiDIR_READ_DATA  FileInfo[1];

	TON_typ Tmr;
	

	UDINT pMem;
	UDINT Len;
	UDINT Offset;

	//XML_RW_HS_INT_typ _int;


}FILE_RW_typ;


typedef struct Calib_Param_typ
{	
	UINT num;
	float val[8];
	
} Calib_Param_typ;

typedef struct CurCalib_typ
{	
	UINT num;
	float TVL_Lo_Limit;
	float TVL_Hi_Limit;
	Calib_Param_typ Points[8];
	DINT GenSetup[8];
	float GenFloatValue[8];
	DINT  GenDintValue[8];		
} CurCalib_typ;





/***********************************************/

/********			 Generic Motor 	    **********/

/***********************************************/
typedef struct GEN_MTR_msc_type
{	

		AzzRamp_typ Ramp;		
		
		AzzSlimPID_typ PID;
		azzslimpid_par_typ PIDParameters;
	

		/*
		LCRRamp_typ Ramp;		
		
		LCRSlimPID_typ PID;
		lcrslimpid_par_typ PIDParameters;
		*/

		DINT Pos;
		INT _pos;
		float _vel;
		float _amp;
		float _volt;
		DINT deltaPos;


		//float kVel;
		//float kAcc;
		float kPos;

		
} GEN_MTR_msc_type;	


typedef struct GEN_MTR_obj
{

	UDINT pState;
	plcstring Name[16];
	USINT Disable;
	USINT xCmd;
	float actPos;
	float actVel;
	float fParam[2];

	struct Hrdw
	{	
		UINT *pPeriodDurationPWM;
		INT *pPulseWidthPWM;
		BOOL *pClearError;
		INT *pEncFbk;
		INT *pAnalogFbk;
		
		
	}Hrdw;	
	
	struct Cfg{
		DINT units;
		float enc_dir;
		float mtr_dir;
		float rev_scale;
		//float loopPeriod;
				
	}Cfg;

	GEN_MTR_msc_type msc;

	/*
	struct msc
	{	

		LCRRamp_typ Ramp;
		//LCRPT2o_typ Trajectory;
		//HCRFIFO_typ FIFODelay;
		//float FIFODelay[FIFO_DLY+1];
		
		LCRSlimPID_typ PID;
		lcrslimpid_par_typ PIDParameters;

		DINT Pos;
		INT _pos;
		float _vel;
		float _amp;
		float _volt;
		DINT deltaPos;

		//float kVel;
		//float kAcc;
		float kPos;

		
	} msc;	
	*/

}GEN_MTR_obj;

///////////////////////////////////////////////////////////

typedef struct Cfg_DC2DIRMTR_obj{

	float MtrDir;
	BOOL *pQ1_output;
	BOOL *pQ2_output;
	BOOL *pQ3_output;
	BOOL *pQ4_output;
	
}Cfg_DC2DIRMTR_obj;

typedef	struct msc_DC2DIRMTR_obj
{		
	UINT sw_step;
	
} msc_DC2DIRMTR_obj;	

typedef struct DC2DIRMTR_obj
{
	UDINT pState;
	plcstring Name[16];
	USINT Disable;
	USINT xCmd;	
	float fParam[2];
	Cfg_DC2DIRMTR_obj Cfg;
	msc_DC2DIRMTR_obj msc;

}DC2DIRMTR_obj;

typedef struct TML_OUTPUTS_Cfg_obj{

	float MtrDir;
	UINT TML_Vect_Indx;
	UINT TML_Indx;
	UINT Out1_Indx;
	UINT Out2_Indx;
	

}TML_OUTPUTS_Cfg_obj;

typedef	struct TML_OUTPUTS_msc_obj
{		
	UINT sw_step;
	
} TML_OUTPUTS_msc_obj;

typedef struct TML_OUTPUTS_obj
{
	UDINT pState;
	plcstring Name[16];
	USINT Disable;
	USINT xCmd;	
	float fParam[2];
	TML_OUTPUTS_Cfg_obj Cfg;
	TML_OUTPUTS_msc_obj msc;

}TML_OUTPUTS_obj;


//////////////////////////////////////////////////////////


/***********************************************/

/****        B&R 	Screen   						****/

/************************************************/

 //*===COLORS===*/
#define TRANSP						0xFF
#define GREEN						10
#define DARK_GREEN					2
#define BLUE						1
#define BABY_BLUE					11
#define TEAL						168
#define RED							0x33
#define YELLOW						46
#define BLACK						0
#define WHITE						15
#define GRAY						7
#define DARK_NAVY					219
#define DARK_GRAY					8
#define LIGHT_GRAY					227
#define CYAN						196
#define MAGENTA						21
#define BROWN						86
#define LIGHT_BLUE					52


typedef struct SCREEN_obj
{  
   
	unsigned short SetPage;
	unsigned short ReadPage;
	unsigned short GotoScreen;
	unsigned short ReturnScreen;
	unsigned short Index;
	unsigned short NavMenu;
	unsigned short Update;
	plcbit btn_YES;
	plcbit btn_NO;
	plcbit btn[32];
	plcbit btnBar[32];
	plcbit btnMenu[10];
	plcbit btnSave;
	plcbit btnIndex;
	unsigned short vis[32];
	unsigned short visBar[32];
	unsigned short visSave;
	unsigned short colorBackground;
	unsigned short color[8];
	unsigned short colorBar[8];
	unsigned short colorSave;
	signed long Param[10];
	float fParam[10];
	unsigned short Language;
	unsigned short Units;
	unsigned short AngleUnits;
	float UnitsFactor;
	float UnitsOffset;
	unsigned long LifeSign;
	USINT AccessLevel;
	unsigned short menubtnColor[10];
  } SCREEN_obj;	


/*
  typedef struct Screen_obj
{	unsigned short SetPage;
	unsigned short ReadPage;
	unsigned short GotoScreen;
	unsigned short ReturnScreen;
	unsigned short Index;
	unsigned short NavMenu;
	unsigned short Update;
	plcbit btn_YES;
	plcbit btn_NO;
	plcbit btnDefault;
	plcbit btn[32];
	plcbit btnGo2PrevScrn;
	plcbit RecipeModified;
	plcbit btnMoment[9];
	plcbit btnBar[32];
	plcbit btnMenu[20];
	plcbit btnSave;
	plcbit btnIndex;
	unsigned short vis[32];
	unsigned short visBar[32];
	unsigned short visSave;
	unsigned short colorBackground;
	unsigned short color[8];
	unsigned short colorBar[8];
	unsigned short colorSave;
	signed long Param[32];
	float fParam[20];
	plcstring stringParam[81];
	struct Pad_typ fPad[13];
	unsigned short fPadIndx;
	unsigned short ParamIndx;
	unsigned short fPad_vis;
	plcbit fPadUpdate;
	unsigned short Language;
	unsigned short LifeSign;
	unsigned short AccessLevel;
	unsigned short menubtnColor[10];
	unsigned char encIndex;
	unsigned char servoIndex;
}
*/
///////////////////////////////////////////////////////////


/***********************************************/

/****        Technosoft Servos					****/

/************************************************/


// Commands definition
#define TML_IDLE 0
#define TML_RESTART 1
// 2 - 13 reserved for TML_RESTART
#define TML_WAIT_FOR_DOWNLOAD 15
// 14 reserved for TML_RESTART
#define TML_HOLD_MODE 17
// 15 - 19 reserved for TML_HOLD_MODE
#define TML_POS_MODE 20
// 21 - 29 reserved for TML_POS_MODE
#define TML_VEL_MODE 30
// 31 - 39 reserved for TML_VEL_MODE
#define TML_TORQ_MODE 40
// 41 - 49 reserved for TML_TORQ_MODE
#define TML_VOLT_MODE 50
// 51 - 59 reserved for TML_VOLT_MODE
#define TML_PT_MODE 60
// 61 - 79 reserved for TML_PT_MODE
#define TML_CUST_MODE 80
// 81 - 89 reserved for TML_PT_MODE
#define TML_OSC_CUST_CMD 90
// 90 - 99 reserved for TML_PT_MODE
#define TML_DOWNL			100	

/// New Osc

#define TML_SET_P_Max 		110
#define TML_SET_P_Min 		111
#define TML_SET_P_Time		112
#define TML_SET_P_DwellIn 	113
#define TML_SET_P_DwellOut 	114
#define TML_SET_P_Steer		115 
#define TML_SET_P_Jog		117
#define TML_SET_P_Amplit 	118
#define TML_SET_Cmd_Word 	119


#define TML_SET_Cmd_Param 	120
#define TML_SET_Home 		121
#define TML_SET_Home2 		122

// New MIG Oscillator
#define TML_Cmd_SAP 		130
#define TML_Cmd_CPA			131
#define TML_Cmd_MODE_PP		132
#define TML_Cmd_TUM1 		133
#define TML_Cmd_CACC		134
#define TML_Cmd_CSPD		135
#define TML_Cmd_CPOS		136
#define TML_Cmd_UPD			137

///////////////////////////////
#define TML_KPP				140
#define TML_KIP				141
#define TML_KIP_Zero		145
#define TML_KPP_Zero		146
#define TML_SATP			150
#define TML_SATP_Zero		151

#define TML_UpdateFilter	160	
#define TML_ClearFilter		161	
#define TML_FullFilter		162
#define TML_SetGain			170

#define TML_SetGain_Zero	173



#define TML_MOVE_REL_CMD 175
// 175 - 190 reserved for TML_MOVE_REL_CMD

#define TML_RESET_ONLY_CMD 185
#define TML_FAULTR		   186


#define TML_ARC_CUST_CMD 190



#define TML_SET_OUT0_CMD 200
#define TML_RST_OUT0_CMD 201
#define TML_SET_OUT1_CMD 202
#define TML_RST_OUT1_CMD 203
#define TML_SET_OUT2_CMD 204
#define TML_RST_OUT2_CMD 205
#define TML_SET_OUT3_CMD 206
#define TML_RST_OUT3_CMD 207
#define TML_SET_OUT4_CMD 208
#define TML_RST_OUT4_CMD 209

#define TML_AXIS_ON_CMD  210
#define TML_AXIS_OFF_CMD 211

#define TML_UPD			212 
#define TML_EINT		213 

#define TML_MOVE_ABS_CMD 220

#define TML_READ_MCR 229


#define TML_UPD_CACC 230
#define TML_UPD_CSPD 231

// New MIG Oscillator Profile
#define TML_Cmd_OSC_Proc 235

#define TML_MOVE_ABS_PARM 245


////////


#define TML_DOWNL_DONE	222	
#define TML_ERROR			255

// Status Mask
#define ERR_CANbus_MSK 			0x0001
#define ERR_ShortCircuit_MSK 	0x0002
#define ERR_InvalidSetup_MSK 	0x0004
#define ERR_Cntrl_MSK 			0x0008
#define ERR_RS232_MSK 			0x0010

//#define SREG_Fault_MSK 				0x80000000
#define SREG_Fault_MSK 				0x80000000

#define SREG_MotionCompleted_MSK 	0x00000400


//  Enumeration for new style OSC Parameters
#define V_Param_Cmd_Param	0
#define V_Param_Time 		1
#define V_Param_DwellIn 	2
#define V_Param_DwellOut 	3


#define V_fParam_Amplit 	0
#define V_fParam_Max 		1 
#define V_fParam_Min 		2
#define V_fParam_Jog		3
#define V_fParam_Steer 		4
#define V_fParam_HomePos 	5


//  Enumeration for new MIG OSC Parameters

#define V_fParam_Cmd_SAP		0
#define V_fParam_Cmd_CPOS 		1
#define V_fParam_Cmd_CACC 		2
#define V_fParam_Cmd_CSPD 		3

//  Enumeration for new MIG OSC Profile

#define V_fParam_MIG_OSC_Amplit		0
#define V_fParam_MIG_OSC_Speed 		1
#define V_fParam_MIG_OSC_Acc		2
#define V_fParam_MIG_OSC_DwellIn 	3
#define V_fParam_MIG_OSC_DwellOut	4


//  Enumeration for Rel Move Parameters

#define V_fParam_Rel_CACC		0
#define V_fParam_Rel_CSPD 		1
#define V_fParam_Rel_CPOS 		2



/////////////////////////////////////////
/******  TML STatus enumeration    *****/

typedef enum TML_STAT_Enum
{	
	TML_STAT_DRIVE_ERR = 	-4,
	TML_STAT_VER_ERR   =	-3,
	TML_STAT_INIT  	   =	-2,
	TML_HEARTBEAT_ERR  = 	-1,
	TML_STAT_DEAD  	   =	0,
	TML_STAT_READY 	   =	1,
	TML_STAT_IDLE 	   =	2,
	TML_STAT_HOLD_INIT 	   =	3,
	TML_STAT_HOLD_OK 	   =	4,
	TML_STAT_POS_MODE_INIT  =	5,
	TML_STAT_POS_MODE_OK    =	6,
	TML_STAT_VEL_MODE_INIT  =	7,
	TML_STAT_VEL_MODE_OK 	=	8,
	TML_STAT_TORQ_MODE_INIT =	9,
	TML_STAT_TORQ_MODE_OK 	=	10,
	TML_STAT_VOLT_MODE_INIT =	11,
	TML_STAT_VOLT_MODE_OK 	=	12,
	TML_STAT_PT_MODE_INIT 	=	13,
	TML_STAT_PT_MODE_OK 	=	14,
	TML_STAT_CUSTOM_INIT 	   =	15,
	TML_STAT_CUSTOM_OK 	   =	16,
	TML_STAT_DRIVE_SELF_CNTRL 	   =	17,
	
}TML_STAT_Enum;	

typedef struct TML_typ
{	
	StateVectorProxy_typ* pState; 
	plcstring Name[16];
	USINT Disable;
	unsigned long  ProgramVersion;
	unsigned long  FirmwareVersion;
	
	float PosFBK;
	USINT xCMD;
	TML_STAT_Enum Status;
	unsigned long StatusReg;
	unsigned short ErrorReg;
	unsigned short MCRReg;	
	plcbit Error;
	plcbit EnableInp;
	unsigned short AnInp[2];
	plcbit DigInp[4];
	plcbit DigOut_stat[4];
	plcbit EStop;

	plcbit HoldErrorReg;
	
	// DigInp[2] = LSP
	// DigInp[3] = LSN
	// AnInp[0] = REF
	// AnInp[1] = FDBK
	struct _cfg{
		DINT units;
		float rev_motor;
		float rev_scale;
		float actPos2_scale;
		UINT  absFbk;
		DINT absLastPos;
		DINT *pAbsLastPos;
		float loopPeriod;
		float maxVel;
		float maxAcc;
		float signFilter;
		//DINT  satp;
		DINT  satpMax;
		DINT  kip;
		DINT  kipMax;
		DINT  kpp;
		DINT  kppMax;
		DINT  SATSMax;
		plcstring programName[32];
		float HeartBeat;
		float PVTPeriod;
		UINT MtrType;
		//float sftLimitPos;
		//float sftLimitNeg;
		UINT NotAutoDownload;
		UDINT Appl_ID_adr;
		unsigned short AnalLimitPos;
		unsigned short AnalLimitNeg;
		
	}_cfg;
	
	struct _int{
		DINT _pos;
		DINT _deltaPos;
		UINT _posCnt;
		float _floatPos;
		float _vel;
		float _amp;
		float _volt;
		float kVel;
		float kAcc;
		float kPos;
		float kVolt;
		UINT PVTPeriod;
		SINT Init;
		float startPos;
		UINT restartCnt;
		UINT cnt;
		//UINT msgProcess;
		UINT KickCntr;
		DINT const_PT;
		TON_typ dly_Tmr;
			
	}_int;
	
} TML_typ;


typedef struct TML_CHAN_HRDW_typ
{
	USINT AxisNum;
	
	USINT TXCountReadBack;
	//
	USINT RXDataSize0;
	USINT RXCount;
	USINT buff[4];
	UDINT RXIdent0;
	UINT  RXData0Word0[4];
	//	
	USINT TXCount;
	USINT TXDataSize;
	UDINT TXIdent;
	UINT  TXDataWord0[4];
	
	USINT _oldRxCount;
	USINT _indx;

	TON_typ DlyTmr;

}TML_CHAN_HRDW_typ;

typedef struct TML_Axis_typ
{	unsigned short AxisID;
	unsigned long  Version;  
	unsigned long  ProgramVersion;
	unsigned long  FirmwareVersion;
	USINT  Flag;
	UINT  AxisState;
	unsigned short DriveStatusRegisterL;
	unsigned short DriveStatusRegisterH;
	unsigned short DriveErrorRegister;
	unsigned short PVTstatus;
	unsigned short DataLow;
	unsigned short DataHigh;
	unsigned short VarAddress;
	unsigned short SATS;
	signed long	   curPos;	
	signed long	   _curPos;
	signed long	   _lastPos;
	unsigned short pointer;
	short deltaAPOS;
	//unsigned long  CmdWord;
	unsigned short PointCounter;
	TON_typ HeartBeat;
	TON_typ InitTmr;
	
	
	
} TML_Axis_typ;



typedef struct TML_MGR_typ
{
	UDINT pSendData[2]; // two channels
	UDINT pRecData[2]; // two channels
	TML_CHAN_HRDW_typ hrdw[2];
	UDINT Firmware[2];
	USINT BlockTrans[2];
	TML_Axis_typ *pAxis[2];
	TML_typ *pTML[2];
	UDINT pLog;


}TML_MGR_typ; 



///////////////////////////////////////////////////////////




typedef struct SerialPortDriver{
	 FRM_xopen_typ	 Open;							 
	 XOPENCONFIG	 OpenConfig;						 
	 FRM_gbuf_typ	 GetBuffer; 			 
	 FRM_rbuf_typ	 Release;					  
	 FRM_robuf_typ	 ReleaseOutput; 		  
	 FRM_write_typ	 Write; 			  
	 FRM_read_typ	 Read;	 
	 FRM_mode_typ	 Mode;
	 FRM_close_typ	 Close;
		 
	 UINT			 StatusOpen;
	 UINT			 StatusWrite;
	 UINT			 StatusRead;
	 UINT			 StatusGetBuffer;
	 UINT			 StatusReleaseOutputBuffer; 
	 UINT			 StatusMode; 
	 UINT			 StatusClose; 
 
	 UDINT			 Ident; 							  
	 UDINT* 		 pSendBuffer;
	 UDINT* 		 pReadBuffer; 
	 UINT			 SendLength;
	 UINT			 ReadLength;
	 UINT			 l; 
 
	 UINT			 txRequest;
	 UINT			 txBufRequest;
	 UINT			 txDataLen;
	 UINT			 rxReady;
	 UINT			 txMessageCounter;
	 UINT			 rxMessageCounter;
	 UINT			 txError;
	 UINT			 rxError;
	 UINT			 initError;
 
	 STRING 		 sDevice[32];
	 STRING 		 sMode[64]; 			 
	 STRING 		 sModeNew[64];
	 USINT			 _parity;
 
	 plcstring		 txData[256];
	 plcstring		 rxData[256];
	 STRING 		 sInput[64];
						 
 }SerialPortDriver_typ;
 /***************************************/

 



/////////////////////////////////////////////////////
///   New Path Generator created March 2019 by CK
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////

typedef struct HCRMoveParamAzz_type
{
	float t_Jolt;
	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;
} HCRMoveParamAzz_type;


typedef struct segment_type{

	float s;
	float a;
	float v;
	
}segment_type;


typedef struct HCRSPFazz_Internal_type
{
	plcstring statusTxt[81];
	UINT step;
	
	float S0;
	float S1;
	float S2;
	float S3;

	float _s_set;
	float _v_set;
	float _a_set;
	UINT _uSec;
	UINT uSec;	
	UINT indx;

	plcbit _enable;
	plcbit _Stop;
	plcbit _Set_home;
	plcbit _Init;
	plcbit _Brake;

	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;

	float TaskPeriod;

	segment_type segm[4];
	
}HCRSPFazz_Internal_type;

typedef struct HCRMoveParaAzz_type
{
	float t_Jolt;
	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;
} HCRMoveParaAzz_type;


typedef struct HCRSPFazz_type
{
	/* VAR_INPUT (analog) */
	//struct HCRMovePara_typ* pMove_Para;
	struct  HCRMoveParamAzz_type* pMove_Para;
	float s_home;
	float s_end;
	/* VAR_OUTPUT (analog) */
	float s_set;
	float v_set;
	float a_set;
	float elapseTime;
	signed char MoveDir;
	signed char ApproachDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRSPFazz_Internal_type Intr;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Stop;
	plcbit Set_home;
	plcbit Init;
	plcbit Brake;
	/* VAR_OUTPUT (digital) */
	plcbit Reached;
} HCRSPFazz_type;


#endif

