#include <bur/plctypes.h>

#ifndef _GLOBAL_DEF_H
#define _GLOBAL_DEF_H


#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <..\Lib\WSI_types.h>
#include <..\Lib\LoggerLib.h>

#include <..\Lib\arclink\arclink.h>


// "192.168.10.11"   	TML -> CAN conerter  
// "192.168.10.2"   	Lincoln Power Supply
// "192.168.10.110"  	VNC - Pendant Screen
// "192.168.10.13" 		My Laptop


//#define SOFTWARE_VERSION    "MIG 09 June 2023V" // two new mode : 169 Carbon & 171 Stainless steel

//#define SOFTWARE_VERSION    "MIG 12 June 2023B" // two new mode : 169 Carbon & 171 Stainless steel

//#define SOFTWARE_VERSION    "MIG 14 June 2023H" // ta dziala przy tescie



//#define SOFTWARE_VERSION    "MIG 20 June 2023F" // FINAL GOOD VERSION req.: MIG 04 Nov 2022 new AHC Rel Move.m

//#define SOFTWARE_VERSION    "MIG 21 June 2023Y" // 

//#define SOFTWARE_VERSION    "MIG 22 June 2023H" // Dobrze dziala - sprawdzona z Martin i David

//#define SOFTWARE_VERSION    "MIG 23 June 2023A" // Gas Solenoid in ArcLink

//#define SOFTWARE_VERSION    "MIG 9 Aug 2023C" // Nie dziala - przy stopie bez error WF jechal

//#define SOFTWARE_VERSION    "MIG 9 Aug 2023E" 

//#define SOFTWARE_VERSION    "MIG 10 Aug 2023F" // DObra stabilna wersja
//#define SOFTWARE_VERSION    "MIG 11 Aug 2023F" // drobna poprawka w tekscie na ekranie

//#define SOFTWARE_VERSION    "MIG 23 Oct 2024A"  // weld report added

//#define SOFTWARE_VERSION    "MIG5 14 Nov 2024A"  // TVL indexing from trigger

//#define SOFTWARE_VERSION    "MIG 30 Jan 2025F"  // Ramp for amps in gauging , TVL indexing from trigger

//#define SOFTWARE_VERSION    "MIG 08 Aug 2025K"  // Gouging over current protection

//#define SOFTWARE_VERSION    "MIG 14 Aug 2025A"  // No Gouging over current protection & No repeat bead protection when gouging

//#define SOFTWARE_VERSION    "MIG 04 Sept 2025K"  // Report data display 
//#define SOFTWARE_VERSION    "MIG 05 Sept 2025K"  // Report data display 
#define SOFTWARE_VERSION    "MIG 26 Sept 2025F"  // bypass CheckIndxDone() 


//////////////////////////////////////////
#define SERVO_NUM  6 // number of used servo


#define VISIBLE 	0
#define INVISIBLE 	1

#define MAX_PARAM_NUM 96

#define TravelSpeedAO_Indx 				0
#define ArcVoltsAO_Indx  				1
#define	WireFeedSpeedAO_Indx  			2
#define	AHCCurrentSPAO_Indx  			3  
#define	AHCToleranceAO_Indx  			4
#define	OSCSpeedAO_Indx  				5

#define	Index_Dist_H_Indx  				6
#define Index_Dist_L_Indx				7


#define	Index_Jog_Speed_Indx  			8
#define	Index_Step_Speed_Indx  			9

#define	TVL_Jog_Speed_Indx  			10  
#define WF_Jog_Speed_Indx  				11 

#define	Index_PreDlyTime_Indx  			12
#define	Index_PostDlyTime_Indx  		13

////
#define	OSC_AmplitAO_Indx  				14
#define	OSC_DwellInAO_Indx  			15
#define	OSC_DwellOutAO_Indx  			16
#define	OSC_JogSpeedAO_Indx  			17
#define	OSC_StepDistAO_Indx  			18

/////
#define	AHC_JogSpeedAO_Indx  			19
#define AHC_Response_Indx  				20
#define AHC_Integrator_Indx  			21


////////////
#define TVL_Calibr_Vel_Low				25
#define TVL_Calibr_Vel_High				26
#define TVL_Calibr_Dist					27
////////////
#define WF_Calibr_Vel_Low				30
#define WF_Calibr_Vel_High				31
#define WF_Calibr_Time					32



/////// Ramps
#define TVL_Ramp						50			
#define TVL_Return_Speed				51	

#define WF_Ramp							52
#define TVL_Skip_Speed					53

////  GAS

#define GAS_PrePurge					54
#define GAS_PostPurge					55

////  Weld Mode

#define StanBy_Time						56
#define Weld_Pinch						57
#define Weld_Trim						58
#define Weld_UltimArc					59


#define AMPS_Calibr_Low					60
#define AMPS_Calibr_High				61

#define BURN_BACK_Tm					62

#define WF_SlowSpeed					63

#define AHC_Retrack_Dist  				64

#define GOUG_Volts  				    65
#define GOUG_Amps 				    	66

#define ARC_Start_Tmo					67
#define ARC_NotActive_Level				68

#define OSC_OFF_Tm						69

#define GOUG_Amps_Init 				    70
#define GOUG_Amps_Ramp 				    71

#define GOUG_Amps_Limit 				72
#define GOUG_Amps_Limit_Tm 				73

#define Wire_Dim 						74

#define	LastPermMem_Indx  				75

/////////////////////////////////////////////////
#define CALIBR_WF_Indx					0
#define CALIBR_AMPS_Indx				1
#define CALIBR_TVL_Indx					2

///////////////////////
#define WF_Calibr_Indx			0   // speed
#define AMPS_Calibr_Indx		1   // Amp feedback
#define TVL_Calibr_Indx			2   // speed

#define Index_Calibr_Indx		3   // distance
#define OSC_Calibr_Indx			4	// distance
#define ArcVolts_Calibr_Indx	5   // Volts to Power Supply


///////////////////////////
#define mm2inch 0.03936996
#define in_min_2_mm_s 25.4/60.0
#define in_2_mm 25.4


/////////   IO mapping
// Inputs

#define ESTOP_status 5

#define STOP_RST_BTN_LOC 6
#define STOP_RST_BTN 7

#define JOG1_BTN 9
#define JOG2_BTN 10

#define RESET_BTN_LOC 11


//outputs
#define ENABLE_out 3
#define RESET_ESR_Btn_out 4



typedef struct HEAD_typ
{	
	plcbit Remote_SW;
	plcbit LocalJog_FWD;
	plcbit LocalJog_REV;
	
} HEAD_typ;


/////////////////////

typedef struct Mach_Config_typ
{	SINT OSCon;
	SINT IndxDir;
	SINT Indxon;
	SINT IndxExt;
	SINT AHC_Auto;
	SINT WeldMode;
	SINT Weld;
	UINT Weld_mode;
	SINT TorchLastPos;
	SINT TestMode;
	SINT TorchIdleDeg;
	SINT ContinSingle;
	SINT GougingON;
	SINT AHC_BackStep;
	SINT GougHighJogSpeed;
	SINT STOP_TVL_After_SKIP;
	SINT TrigStep_INDX_TVL;
	SINT GougAutAHC;
	SINT spare[13];
	 
} Mach_Config_typ;


typedef struct Analog_Out_Mem_typ
{	float Value;
	float H_Incr;
	float L_Incr;
} Analog_Out_Mem_typ;

typedef struct PermMem_typ
{	
	//float TVL_Lo_Limit;
	//float TVL_Hi_Limit;
	struct Analog_Out_Mem_typ Analog_Out[MAX_PARAM_NUM];
} PermMem_typ;



//////////////
typedef struct RS_Channel_typ{
	USINT Status;
	USINT Data[32];
	UINT Len;
	UINT _len_prev;
	UDINT Cntr;
	UDINT Trash_Cntr;
	UINT Msg_ID;
	UINT Msg_Error;
	
}RS_Channel_typ;



typedef struct EditBox_typ
{	plcbit H_Incr_Plus_Btn;
	plcbit L_Incr_Plus_Btn;
	plcbit H_Incr_Minus_Btn;
	plcbit L_Incr_Minus_Btn;
	plcbit Return_Btn;

	USINT Cmd;
	
} EditBox_typ;

typedef struct Analog_Out_Module_typ{
	plcstring Name[81];
	float Min_Val;
	float Max_Val;
	//LCRLimScal_typ LimScale;

}Analog_Out_Module_typ;


////  FOR TML Configuration

#define VECTORS_NUM		6
#define TML_SERVO_NUM 	16
#define TML_MGR_NUM 	2

#define TML_AX_TVL 			0
#define TML_AX_OSC 			1
#define TML_AX_AHC 			2
#define TML_AX_WF			3
#define TML_AX_INDX 		4
#define TML_AX_INDX_EXT		5

#define VECT_AX_TVL 		0
#define VECT_AX_OSC 		1
#define VECT_AX_AHC 		2
#define VECT_AX_WF			3
#define VECT_AX_INDX 		4
#define VECT_AX_INDX_EXT	5


////////////////////////////////

// NEW ADDED is below

////////////////////////////////

#define mm2inch 0.03936996
#define in_min_2_mm_s 25.4/60.0
#define in_2_mm 25.4


/*******************************/

#define MODE_NONE 	0
#define MODE_TVL 	1
#define MODE_OSC 	2
#define MODE_AHC 	3
#define MODE_WF 	4
#define MODE_INDX 	5
#define MODE_INDX_EXT 	6


/*******************************/
/*         ALARMS  			****/
/*******************************/


// Alarms Class
#define ERROR_SETUP 		-2
#define ERROR_FATAL 		-1
#define ERROR_MOTOR  		1
#define ERROR_MOTOR_INIT  	2
#define ERROR_TIG_PROC  	3
#define ERROR_COOL_GAS  	4
#define ERROR_TVL  			5
#define ERROR_PENDANT  		6
#define ERROR_ESTOP  		10

// Alarms items
#define ERROR_NONE 0

#define ERROR_DRV_01 1 
#define ERROR_DRV_11 11 
#define ERROR_DRV_21 21
#define ERROR_DRV_31 31
#define ERROR_DRV_41 41

// Warnings 
#define WARN_AVC_LIMITS  0
#define WARN_OSC_LIMITS  1


////   Moved from GlobalTyp.h (ST)

typedef enum Jog_Direction
{	Stop = 0,
	Forward = 1,
	Reverse = 2
} Jog_Direction;

typedef struct ReversingRelay
{	enum Jog_Direction IN;
	//plcbit OUT[4];
	//plcbit Forward;
	//plcbit Reverse;
	//plcbit Stop;
} ReversingRelay;

typedef struct Sequence
{	plcbit _start;
	plcbit _stop;
	plcbit Start_in;
	plcbit Stop_in;
	plcbit Start_out;
	plcbit Stop_out;
	struct TOF startTimer;
	struct TOF stopTimer;
	unsigned short State;
} Sequence;

typedef struct Indexer_typ
{	plcbit Run;
	plcbit Limit_Forward;
	plcbit Limit_Reverse;
	plcbit Direction;
	plcbit Sequence;
	plcbit Start;
	signed char ManualStep;
	float* Speed_Forward;
	float* Speed_Reverse;
	float* Speed_Jog;
	signed short Speed;
	unsigned char state;
	float* Reverse_time;
	float* Forward_time;
	float* PreDly_time;
	float* PostDly_time;
	struct TON timer;
	unsigned short period;
} Indexer_typ;


typedef struct IO_typ{

	plcbit Inp[12];

	plcbit Out[12];

}IO_typ;

/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

typedef struct ESAB_CMD_typ{

	
plcbit WeldON;
plcbit QuickStop;

plcbit WF_Jog_FWD;
plcbit WF_Jog_REV;

plcbit JogHighSpeed;

plcbit InputArea;

plcbit UpdateParam;



}ESAB_CMD_typ;

typedef struct ESAB_Fbk_typ{

	UINT HeartBeat;
	
	USINT  Status;
	USINT  StatusMSB;
	plcbit ReadyToStart;
	plcbit Error;
	plcbit InputAreaActive;

	USINT SetRegulType;	

	float SetVoltage;
	float SetAmps;
	float SetWF;
	float SetTVL;
	float Req_WF_Speed;
	float Req_TVL_Speed;

	float actVolts;
	float actAmps;
	float actHeat;
	float actWF;
	
	USINT ErrorNum;
	UINT ErrorCode;
	USINT ErrorSubCode;
	USINT ErrorNodeID;

	USINT NewErrorCode;
	

}ESAB_Fbk_typ;

typedef struct ESAB_Param_typ{

	float Voltage;
	float Amps;
	USINT RegulType;
	

}ESAB_Param_typ;

typedef struct ESAB_Msc_typ{

	USINT 	_recev_counts;
	USINT 	_sent_counts;

	plcbit _weldON;
	plcbit _quickStop;

	plcbit _inputArea;

	float _voltage;
	float _amps;
	USINT _regulType;

	plcbit _wF_Jog_FWD;
	plcbit _wF_Jog_REV;

	plcbit _jogHighSpeed;


	USINT _heartBeat;

	USINT _errorNum;

	TON_typ Main_Tmr;
	TON_typ Cmd_Hrbt_Tmr;

}ESAB_Msc_typ;

typedef struct ESAB_typ{
	USINT step;
	
	ESAB_CMD_typ Cmd;
	ESAB_Param_typ Param;
	ESAB_Fbk_typ Fbk;
	ESAB_Msc_typ Msc;
	
}ESAB_typ;



///////////////////////////////////////////////////
#endif

