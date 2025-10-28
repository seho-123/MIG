#ifndef _GLOBAL_DECL_H
#define _GLOBAL_DECL_H

#include <bur/plctypes.h>

#include <..\GlobalDef.h>

//#include <..\Lib\arclink\Robot.h>

// For TML comunnication over Ethernet
_GLOBAL RS_Channel_typ TML_Msg_Chan[2] , TML_Data_Chan[2] , UDP_Connect_Send[2];
_GLOBAL USINT Kick_TML_Cmd;
_GLOBAL Arc_Link_typ ArcLink;

_GLOBAL ESAB_typ ESAB_PS;

_GLOBAL plcbit UDP_Ready ,UDP_Restart;

//////////////

_GLOBAL_RETAIN Mach_Config_typ MachCfg;

_GLOBAL plcbit Purge;
_GLOBAL plcbit OSCJog;
_GLOBAL plcbit OSCToggle;
_GLOBAL plcbit GAS_Cmd;
_GLOBAL SINT INDX_Step_Cmd;
_GLOBAL SINT TVL_Step_Cmd;
_GLOBAL plcbit SKIP_Weld_Cmd;
_GLOBAL_RETAIN struct ReversingRelay TravelSpeed;
_GLOBAL struct ReversingRelay TravelJog;
_GLOBAL struct Sequence Seq;
_GLOBAL struct ReversingRelay IndexJog;
_GLOBAL struct ReversingRelay IndexExtJog;
_GLOBAL struct ReversingRelay AHCJog;
_GLOBAL struct ReversingRelay WFJog;
_GLOBAL plcbit pelcoMsgReady;
_GLOBAL unsigned char pelcoPreset;
_GLOBAL unsigned char pelcoCommand;
_GLOBAL unsigned char _pelcoCommand;
_GLOBAL unsigned char pelcoAddress;
_GLOBAL float Slider;
_GLOBAL unsigned char RockerButton;
_GLOBAL unsigned char ToggleSwitch;
_GLOBAL plcbit X20ModuleOK[14];
_GLOBAL plcbit X67ModuleOK[3];
_GLOBAL unsigned short ModulesOK;
_GLOBAL plcbit ESTOP;
_GLOBAL unsigned short visESTOP;
_GLOBAL unsigned char visPage;
_GLOBAL unsigned char visChangePage;
_GLOBAL unsigned short visMaint;
_GLOBAL plcbit SequenceRun_Out;
_GLOBAL plcbit SequenceRun_WF;
_GLOBAL plcbit SequenceRun_OSC;
_GLOBAL plcbit computerPower[2];
_GLOBAL float visTravelSpeed;
_GLOBAL unsigned char cameraCommand;
_GLOBAL_RETAIN plcbit permMemInit;
_GLOBAL unsigned char ledLevel;
_GLOBAL unsigned char cameraPicked;
_GLOBAL plcbit cameraPower;
_GLOBAL float _TravelSpeed;
_GLOBAL unsigned short DisplayInfo;
_GLOBAL plcstring SoftwareVersion[81];
_GLOBAL plcbit MoveEnable;
_GLOBAL USINT LocalJogMode;
_GLOBAL plcbit MachActive;
_GLOBAL TouchAction   Screen_touch;
_GLOBAL plcbit ErrorFromMaster_flag;

// From Head

_GLOBAL HEAD_typ Head;

//  For Files saving

_GLOBAL  SM_obj Screen_sm;
_GLOBAL  SCREEN_obj Screen;
_GLOBAL  FILEMGR_obj FileMgr;
_GLOBAL  FILEOUT_IF File_out;
_GLOBAL  FILEIN_IF File_in;
_GLOBAL XML_Method_typ XMLFile;
_GLOBAL_RETAIN unsigned char ROM_USB;
_GLOBAL_RETAIN plcstring RecipeName[81];

/////////////////////////////////////////////////////
/// For HMI only
_GLOBAL plcbit SetDefault;
_GLOBAL plcbit SetDefault_TVL_Calibr;
_GLOBAL plcbit SetDefault_WF_Calibr;
_GLOBAL plcbit SetDefault_AMPS_Calibr;
_GLOBAL UINT Page_Step;


_GLOBAL UINT PageBtnPress[16];
_GLOBAL UINT EditBox_Vis;
_GLOBAL UINT EditBox_Indx;
_GLOBAL UINT Inc_Plus_Btn_Vis;
_GLOBAL UINT Inc_Minus_Btn_Vis; 
_GLOBAL UINT Fwd_Limit_Vis, Rev_Limit_Vis;


_GLOBAL UINT EndOfBead_Stop;
_GLOBAL UINT Sequence_Halt_Cmd;


_GLOBAL UINT AccessLevel;

_GLOBAL plcbit HardwInit_Done;

_GLOBAL Analog_Out_Module_typ Analog_Out_Mod[MAX_PARAM_NUM];
_GLOBAL_RETAIN struct PermMem_typ permMem;
_GLOBAL_RETAIN struct CurCalib_typ CurCalib;
_GLOBAL_RETAIN DINT OLD_MTR_POS[TML_SERVO_NUM];


_GLOBAL UINT NewPageReq_Cmd;
_GLOBAL EditBox_typ EditBox;

_GLOBAL USINT Travel_JOG_Step;

_GLOBAL USINT Travel_JOG_Step_FWD_Cmd , Travel_JOG_Step_REV_Cmd;

_GLOBAL USINT INDX_Limit_Bypass_Btn;

_GLOBAL plcbit LED_One_Blink_Req;

////////////////////////////////////////////
////  For Calibration
_GLOBAL plcstring HMI_Calibr_Intruct_TXT[128];
_GLOBAL UINT HMI_Calibr_Indx;
_GLOBAL USINT HMI_Calibr_Start_Stop , HMI_Calibr_New_User_Input;
_GLOBAL USINT HMI_Calibr_Type_Select;
_GLOBAL USINT HMI_Calibr_NewStart_Btn , HMI_Calibr_Verify_Btn , HMI_Calibr_Resume_Btn;
_GLOBAL USINT HMI_Calibr_Step;
_GLOBAL UINT HMI_Calibr_StartBtn_vis , HMI_Calibr_StopBtn_vis ;//, HMI_Calibr_New_User_Input_vis  ;
_GLOBAL UINT HMI_Calibr_Slop0_vis , HMI_Calibr_Slop1_vis;
_GLOBAL UINT HMI_Calibr_CalculateBtn_vis , HMI_Calibr_SaveBtn_vis , HMI_Calibr_User_Input_vis;
_GLOBAL UINT HMI_Calibr_UserInput_Lock;
_GLOBAL UINT HMI_Calibr_Resume_Btn_vis;
_GLOBAL UINT HMI_Calibr_Step_Done[8];
_GLOBAL UINT HMI_Step_Color[16];
_GLOBAL UINT HMI_Step_Vis[16];
_GLOBAL float HMI_Calibr_Meas_Value;
_GLOBAL Calib_Param_typ TempCalibr;
_GLOBAL float HMI_Calibr_User_Inputs[8]; 
_GLOBAL float HMI_Calibr_Correct, HMI_Calibr_Error;
_GLOBAL float HMI_Calibr_Slop , HMI_Calibr_Bias; 
_GLOBAL float HMI_Calibr_Slop2 , HMI_Calibr_Bias2; 

_GLOBAL UINT HMI_AXIS_Disable[6];


_GLOBAL UINT HoldThisAxError;

_GLOBAL float Calibr_Error[8];
_GLOBAL UINT HMI_Calibr_What_vis[8];

// TVL
_GLOBAL float TVL_Calibr_Speed;
_GLOBAL float TVL_Calibr_Distance;
_GLOBAL float TVL_Act_Pos;
_GLOBAL USINT TVL_Start_Err;
_GLOBAL USINT ESAB_Tmo_Err;
_GLOBAL float TVL_Last_Pos;
_GLOBAL Jog_Direction TVL_Last_Dir;


// INDX
_GLOBAL float INDX_Act_Pos;
_GLOBAL float INDX_Home_Pos;
_GLOBAL plcbit INDX_SetHome_Btn;
_GLOBAL float INDX_Last_Pos;

_GLOBAL plcbit WatchIndx;
_GLOBAL UINT ProcError_vis;
_GLOBAL UINT ProcError_Indx;



// AHC 
_GLOBAL plcbit AHC_SetHome_Btn;
_GLOBAL float AHC_Scaled_Pos;
_GLOBAL float AHC_MaxPos;
_GLOBAL float AHC_StartPos;
_GLOBAL float AHC_Scaled_MaxPos;
_GLOBAL float AHC_Scaled_StartPos;
_GLOBAL float AHC_Scaled_Fact;


_GLOBAL plcbit ObserveAHC_MaxPos;


// WF
//_GLOBAL SINT  WF_Calibr_Cmd;
_GLOBAL float WF_Calibr_Speed;
_GLOBAL float WF_Calibr_Distance;

_GLOBAL float AMPS_Calibr_Val;


/////////////

_GLOBAL IO_typ Main_IO;
_GLOBAL plcbit ENABLE_sign;
_GLOBAL plcbit STANDBY_sign;

_GLOBAL plcbit RESET_ESR_Btn; // HMI
_GLOBAL UINT RESET_ESR_Btn_vis;


_GLOBAL plcbit Robot_Cmd_RUN , Robot_Cmd_RUN1;


_GLOBAL plcbit HardRestart_Mode;

////////////////////////////////////////////

_GLOBAL SINT OSCJog_Cmd;
///  new 
_GLOBAL ERROR_Mgr_typ ErrorMgr;

_GLOBAL plcbit HardwReady;
_GLOBAL plcbit ReadyToOperate;

_GLOBAL UINT HardInit_Step;

_GLOBAL LOGMGR_obj	LogMgr ;
_GLOBAL UDINT LogID; // _VAR_RETAIN;
_GLOBAL StateVectorProxy_typ VectorStateMach[VECTORS_NUM];
_GLOBAL TML_MGR_typ TML_mgr[TML_MGR_NUM];
_GLOBAL TML_typ TML_MTR_INST[TML_SERVO_NUM] ;
_GLOBAL TML_Axis_typ AxisCh[TML_SERVO_NUM];
_GLOBAL float Glbl_TML_CMD_Delay;


_GLOBAL BOOL TML_MTR_CUT_POWER , TML_STOP_SEND_ERROR;
_GLOBAL BOOL KK_HoldEnable;
_GLOBAL plcbit StopStateMachineEvaluation;

_GLOBAL plcbit ResetErrorInTML;

_GLOBAL UINT TVL_Step;
_GLOBAL UINT OSC_Step;
_GLOBAL UINT INDX_Step;
_GLOBAL UINT AHC_Step;
_GLOBAL UINT WF_Step;

_GLOBAL float OSC_Steer;

_GLOBAL plcbit WeldCalibr_Mode;

_GLOBAL TON_typ GAS_ON_Tmr , GAS_OFF_Tmr; 
_GLOBAL UINT LED_CMD;

_GLOBAL AzzMovAvgFlt_typ  ActCurrent_Filter;
_GLOBAL float ActCurrrentFbk;
_GLOBAL float ActCurrrentFbk_Calib[2];
_GLOBAL float HMI_Act_Amps;
//_GLOBAL float AHCResponde_Calib[2];
_GLOBAL USINT AHC_Mode;
_GLOBAL float AHC_Move_Target;
_GLOBAL plcbit  AHC_Move_Cmd;


_GLOBAL INT VoltCmd_Out , AmpsCmd_Out;

_GLOBAL AzzRamp_typ AHC_Ramp;
_GLOBAL AzzRamp_typ TVL_Jog_Ramp , INDX_Jog_Ramp , WF_Volt_Ramp , OSC_Jog_Ramp , WF_Run_Ramp ;
_GLOBAL AzzSlimPID_typ AHC_PID;
_GLOBAL azzslimpid_par_typ AHC_PIDParameters;
_GLOBAL plcstring HMI_Error_Txt[32];

_GLOBAL AzzRamp_typ ESAB_Amp_Ramp; 

// Just for compatibilty with UniverslCntroller TML_Server
_GLOBAL FILE_RW_typ File_RW; // Dummy structure - do not use 
_GLOBAL float ABS_TVL_Pos;
_GLOBAL plcbit ProjectConfig_Hrdw_ERROR;
_GLOBAL plcbit KK_DoNotChangeTMLProgram; // Must be 1
_GLOBAL plcbit KK_TML_PowerON ;

_GLOBAL USINT KK_TVL_Test_step;


_GLOBAL plcstring ErrorMgr_Txt[SERVO_NUM][64];
_GLOBAL plcstring DriveVer_Txt[SERVO_NUM][64];


#endif
