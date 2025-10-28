
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

#include <..\Lib\Statelib.c>
#include <..\Lib\LoggerLib.c>
#include <..\Lib\WSI_lib.c>


_LOCAL TON_typ Init_Tmr;
_LOCAL TON_typ OSC_Tmr;
_LOCAL TON_typ TVL_Tmr;
_LOCAL TON_typ TVL_DT_Tmr;
_LOCAL TON_typ WF_Tmr;
_LOCAL TON_typ WF_DT_Tmr;
_LOCAL TON_typ AHC_Tmr;
_LOCAL TON_typ INDX_Tmr;
_LOCAL TON_typ TVL_StatusReg_Tmr;
_LOCAL TON_typ AHC_StatusReg_Tmr;
_LOCAL TON_typ WF_StatusReg_Tmr;
_LOCAL TON_typ OSC_StatusReg_Tmr;

_LOCAL TON_typ Cyclic_Tmr;
_LOCAL TON_typ Evalut_Pos_Tmr;

///  Amps Limit protection
_LOCAL plcbit _ESAB_PS_Cmd_WeldON;
_LOCAL USINT AmpsLimit_Step;
_LOCAL TON_typ AmpsLimit_Tmr;

_LOCAL  LCRMovAvgFlt_typ  filterLCRMovAvgFlt;



//////////////////////////////

_LOCAL float StatusReg_Tm;

_LOCAL plcbit AHC_Test_Dir;

plcbit _INDX_EXT_DigInp2;

//#define OLD_AHC 1


void TVL_Func();
void OSC_Func();
void INDX_Func();
void AHC_Func();
void WF_Func();
void WF_Func_Torq();
void WF_Func_Volt();

void Calculate_OSC_Param(float S);
void Calculate_MIG_OSC_Param(float S);

float Calculate_Acc(   float v , float t);
float AHC_Cntrl(USINT mode);

plcbit WatchTransError(USINT mode);
plcbit CheckAxisOn(TML_Axis_typ * ax);
void CalculateParamForABSCmove();

void Gaug_Max_Amp_Protection();




void MotorCfg();

//_LOCAL UINT FAULT_RST_Loop_step;
_LOCAL UINT Init_Indx , FaultRst_Indx;
_LOCAL UINT Tml_Estop_Tmr[SERVO_NUM];
UDINT _VirStatus[SERVO_NUM];
plcbit _SequenceRun_Out , _OSCon , _OSCJog; 

_LOCAL UINT ETH_Try_Again_Cnt , ETH_Try_Again_Cnt_act , ETH_Try_Again_Cnt_act_Max , ETH_Try_Again_Cnt_Max;

_LOCAL USINT UpdateProfile;


_LOCAL float HMI_AHC_Error;
_LOCAL float HMI_AHC_Volt_Error;

_LOCAL float AHC_Error;
_LOCAL float AHC_Volt_Error;
_LOCAL float AHC_SetPoint;
_LOCAL float AHC_Margin;
_LOCAL float AHC_Integr;

_LOCAL float OSC_Len2Deg;



_LOCAL plcbit IND_Repeat_Init;
_LOCAL USINT TVL_Repeat_Init_Cnt;

///////////////////////////

#define TVL_JOG_MAX 	240.0/60.0
#define WF_JOG_MAX 		500.0
#define OSC_JOG_MAX 	20.0
#define INDX_JOG_MAX 	2.33/2.0
#define AHC_JOG_MAX 	400.0
#define OSC_JOG_TOGGLE_MAX 		50.0


#define AHC_Respond_Legacy_MAX  	500.0

#define AHC_Gain_PLC_MAX  	50.0
#define AHC_Integr_PLC_MAX  1.0

//////////////////////////
// For OSC
_LOCAL float  KK_OSC_Time , KK_OSC_Amplit;
_LOCAL float KK_ACC_ratio;
_LOCAL TIME KK_Loop_Time;
_LOCAL plcbit KK_OSC_Test;

_LOCAL plcbit KK_Stop;
_LOCAL plcbit KK_HaltProcess;

_LOCAL float  KK_WF_Run_Scale;

_LOCAL plcbit KK_DoNotStopOnError;
_LOCAL plcbit KK_Aply_Estop;


_LOCAL plcbit KK_StartWF;

/////////////////////////

float _OSC_Freq ,_OSC_Amplit , _ACC_ratio , _DwellIn , _DwellOut;
float _TVL_Ramp, _TVL_Return_Speed , _TVL_Vel;
float _WF_Ramp , _WF_Vel;
float _AHC_Integrator, _AHC_Response;

_LOCAL plcbit KK_Reset_Seq;
_LOCAL plcbit KK_ESTOP_Fake;

_LOCAL plcbit _ESTOP;
_LOCAL float INDX_Proc_Dly;
_LOCAL float Reset_Proc_Dly;
plcbit _estopSign;

_LOCAL plcbit FirstBootUp;
plcbit _TravelJog_IN;

////////
float _TravelSpeedAO; 
float _TVL_Return_Speed;
float _Travel_Skip_Speed;

Jog_Direction _TravelSpeed_IN;	
float _TravelRampAO;
plcbit _SKIP_Weld_Cmd;
plcbit _MoveEnable2;
///////////////

plcbit _sequenceRun_WF;

plcbit  _rESET_STOP_Btns;

_LOCAL plcbit RESET_STOP_Btns;


_LOCAL float WF_Stop_Ramp;

_LOCAL plcbit _observeAHC_MaxPos;
///////////////////////

_LOCAL plcbit JOG_AHC_IN , JOG_AHC_OUT;

_LOCAL plcbit JOG_AHC_OUT_prot;


///////////////////////

_LOCAL TON_typ WatchEthernet_Tmr;

_LOCAL float WF_InVal , OSC_InVal , TVL_InVal , INDX_InVal;


_LOCAL float AHC_Gain_MAX;
_LOCAL float AHC_Integr_MAX;
_LOCAL float AHC_VAL_MAX;
_LOCAL plcbit KK_Run_AHC_PID;

_LOCAL plcbit KK_Test;

_LOCAL float AHC_Move_Dist;


void _INIT ProgramInit(void)
{int i;

	MachCfg.GougAutAHC = 0;

	/////////////////////////

	
	/* Parameters for moving average filter */
	filterLCRMovAvgFlt.enable = 1;
	filterLCRMovAvgFlt.base	= 100;

	//////////////////////

	StatusReg_Tm = 1.5;
	
	FirstBootUp = 0; // not used

	AHC_Move_Dist = 10.0;

	TimerStart(&Evalut_Pos_Tmr, 0.1);

	AHC_Scaled_Fact = -4.0/1052.0;

	KK_Reset_Seq = 1; // 
	KK_ESTOP_Fake = 0;

	INDX_Proc_Dly = 1.0;
	//Reset_Proc_Dly = 0.0;
	Reset_Proc_Dly = 0.05; // important 28 June 2022

	WF_Stop_Ramp = 0.3;

	KK_Run_AHC_PID = 0;
	AHC_VAL_MAX = 400.0;	

#ifdef OLD_AHC
	AHC_Gain_MAX = 40;
	AHC_Integr_MAX  = 0.02;
#else
	AHC_Gain_MAX = 10;
	AHC_Integr_MAX  = 0.01;
#endif

	KK_DoNotStopOnError = 0;
	KK_Aply_Estop = 0;

	
	KK_WF_Run_Scale = 0.052;

	

	KK_HoldEnable = 0;	
	HardRestart_Mode = 1;
	KK_HaltProcess = 0;
	

	ETH_Try_Again_Cnt_Max = 6;

	HardInit_Step = 1;
	
	MoveEnable = 1;
	KK_ACC_ratio = 0.0;	

	OSC_Len2Deg = 360.0 / ((M_PI * 2.0) * 10.5);

	if(TravelSpeed.IN == Stop )
		TravelSpeed.IN = Forward;

	//////////////////////////////

	

	TML_mgr[0].pLog = TML_mgr[0].pLog;
	VectorStateMach[0].fParam[0] = VectorStateMach[0].fParam[0];
	TML_MTR_INST[0].AnInp[0] = TML_MTR_INST[0].AnInp[0] ;
	AxisCh[0].AxisID = 0;
	LogMgr.Cmd.Refresh = LogMgr.Cmd.Refresh;

	/***********************************************************/	
	//LogTxt(&LogMgr , "Start TML Cfg %d " , 1);
	LoggerInit(&LogMgr, &LogID, "LogBook1", 2000);
	///////////////////////////////////////
	/***********************************************************/

	
	MotorCfg();
	VectorStateMach[VECT_AX_OSC].cmdWord = 1;	

	TVL_Step = 0;
	OSC_Step = 0;
	INDX_Step = 0;
	AHC_Step = 0;
	WF_Step = 0;
	
	///////////////////////////////////////
	/// AHC PID parameters

	
	ActCurrent_Filter.enable = 1;
	ActCurrent_Filter.base = 100;

	
	ActCurrrentFbk_Calib[0] = 0.11;

	
	AHC_Mode = 3; // Classic PID in Drive	

	AHC_Ramp.dy_up = 1000.0;
	AHC_Ramp.dy_down = 1000.0;
	AHC_Ramp.y_max = 500.0;
	AHC_Ramp.y_min = 0.0;
	AHC_Ramp.enable = 1;

	
	TVL_Jog_Ramp.dy_up = 0.25;
	TVL_Jog_Ramp.dy_down = 4.0;
	TVL_Jog_Ramp.y_max = 1.0;
	TVL_Jog_Ramp.y_min = 0.0;
	TVL_Jog_Ramp.enable = 1;
	
	INDX_Jog_Ramp.dy_up = 0.5;
	INDX_Jog_Ramp.dy_down = 4.0;
	INDX_Jog_Ramp.y_max = 1.0;
	INDX_Jog_Ramp.y_min = 0.0;
	INDX_Jog_Ramp.enable = 1;	

	OSC_Jog_Ramp.dy_up = 0.25;
	OSC_Jog_Ramp.dy_down = 1000.0;
	OSC_Jog_Ramp.y_max = 1.0;
	OSC_Jog_Ramp.y_min = -1.0;
	OSC_Jog_Ramp.enable = 1;
	AzzRamp(&OSC_Jog_Ramp);
	OSC_Jog_Ramp.set = 0.05;

	WF_Volt_Ramp.dy_up = 100.0;
	WF_Volt_Ramp.dy_down = 20.0;
	WF_Volt_Ramp.y_max = 36.0;
	WF_Volt_Ramp.y_min = -36.0;
	WF_Volt_Ramp.enable = 1;
	WF_Volt_Ramp.x = 0.0;
	WF_Volt_Ramp.y_set = 0.0;
	WF_Volt_Ramp.set = 1;
	AzzRamp(&WF_Volt_Ramp);
	WF_Volt_Ramp.set = 0;	

	WF_Run_Ramp.dy_up = 1.0;
	WF_Run_Ramp.dy_down = 5.0;
	//WF_Run_Ramp.dy_down = 0.5;
	WF_Run_Ramp.y_max = 1.0;
	WF_Run_Ramp.y_min = 0.0;
	WF_Run_Ramp.enable = 1;	
	WF_Run_Ramp.y_set = 0.0;
	WF_Run_Ramp.set = 1;	
	AzzRamp(&WF_Run_Ramp);
	WF_Run_Ramp.set = 0;	

	AHC_PIDParameters.Kp = 30.0;
	AHC_PIDParameters.Tn = 0.0;
	AHC_PIDParameters.Tv = 0.0;
	AHC_PIDParameters.Kfbk = 0.0;
	AHC_PIDParameters.Y_max = 1000.0;
	AHC_PIDParameters.Y_min = -1000.0;

	AHC_PID.pPar = &AHC_PIDParameters;

	AHC_PID.request = 3 ; // Read parameters from PIDParameters 

	///
	LocalJogMode = LocalJogMode;
	Head.LocalJog_FWD = Head.LocalJog_FWD;
	Head.LocalJog_REV = Head.LocalJog_REV;

	ESAB_PS.step = ESAB_PS.step;

}

void _CYCLIC ProgramCyclic(void)
{int i;
USINT status , status1;


	////  Joging from Pendant

	if(MachCfg.GougingON){

		ESAB_PS.Cmd.WF_Jog_FWD = Main_IO.Inp[JOG1_BTN];
		ESAB_PS.Cmd.WF_Jog_REV = Main_IO.Inp[JOG2_BTN];

		JOG_AHC_IN = Main_IO.Inp[JOG1_BTN];
		JOG_AHC_OUT = Main_IO.Inp[JOG2_BTN];

		// Wyrzucone bo nie mozna ominac
		/*
		if(JOG_AHC_OUT_prot)
			JOG_AHC_OUT = 1;
		
		
		Gaug_Max_Amp_Protection();
		*/
	
	
	}else{

		JOG_AHC_IN = Main_IO.Inp[JOG1_BTN];
		JOG_AHC_OUT = Main_IO.Inp[JOG2_BTN];

	}
	//Gaug_Max_Amp_Protection();
	/////////////////////

	if(SequenceRun_Out || (TravelJog.IN != Stop) )
		TimerStart(&Evalut_Pos_Tmr, 0.5);
	
	TimerExecute(&Evalut_Pos_Tmr);

	if(Evalut_Pos_Tmr.Q){
		
		switch(TravelSpeed.IN){

			case  Forward:

				if((TML_MTR_INST[TML_AX_TVL].PosFBK - CurCalib.TVL_Hi_Limit) > -0.125){
					TravelSpeed.IN = (TravelSpeed.IN == Forward)? Reverse:Forward;
					//ProcError_Indx = 3;
				}

			break;

			case  Reverse:	

				if((TML_MTR_INST[TML_AX_TVL].PosFBK - CurCalib.TVL_Lo_Limit) < 0.125){
					TravelSpeed.IN = (TravelSpeed.IN == Forward)? Reverse:Forward;
					//ProcError_Indx = 3;
				}

			break;
		}	
	}
	 

	switch(MachCfg.WeldMode){

		case 1: // Weld Up

			MoveEnable = (TravelSpeed.IN == Forward)? 1:0;

		break;
		
		case 2: // Weld Down

			MoveEnable = (TravelSpeed.IN == Reverse)? 1:0;

		break;		
		
		default:

			MachCfg.WeldMode = 0;
			MoveEnable = 1;

		break;

	}
	
	///////////////////////////
		
	////////////////////////////////

	LoggerSrv(&LogMgr);

	///////////////////////////////
	// Temporary ESTOP evalution
	
	_ESTOP = 1;
	//ESTOP = 1;
	status = 0;
	for(i = 0 ; i < SERVO_NUM ; i++){

		if(TML_MTR_INST[i].Disable)
			continue;
			
		if(TML_MTR_INST[i].Error){
			status = 1;

		}		
		else if(AxisCh[i].AxisState){

			TML_MTR_INST[i].EStop = 1;
			Tml_Estop_Tmr[FaultRst_Indx] = 0;
			_ESTOP = 0; 

		}			

		if(Tml_Estop_Tmr[i]< 1000)
			Tml_Estop_Tmr[i]++;
		
		if(!AxisCh[i].AxisState && (Tml_Estop_Tmr[i]>= 1000))		
			TML_MTR_INST[i].EStop = 0;
			
		
	}

	/////////////////////////////////////////////////////////
	ESTOP = Main_IO.Inp[ESTOP_status] && !KK_ESTOP_Fake;
	
	Main_IO.Out[ENABLE_out] = (ENABLE_sign && !STANDBY_sign)  || KK_HoldEnable;

	RESET_STOP_Btns = ((RESET_ESR_Btn || Main_IO.Inp[RESET_BTN_LOC] 
			|| Main_IO.Inp[STOP_RST_BTN] || Main_IO.Inp[STOP_RST_BTN_LOC]) && !STANDBY_sign) ;

	if(!ESTOP){

		ESAB_PS.Cmd.QuickStop = 1;
				
		Main_IO.Out[RESET_ESR_Btn_out] = RESET_STOP_Btns;
		
		RESET_ESR_Btn_vis = VISIBLE;
	}
	else{


		if(!_rESET_STOP_Btns && RESET_STOP_Btns){

			ProcError_vis = INVISIBLE;
			ProcError_Indx = 0;
			if((HardInit_Step == 100) || (HardInit_Step < 50) ){
			//if(HardInit_Step == 100){						

				ProcError_vis = INVISIBLE;
				AckAllErrors(&ErrorMgr);
				memset(ErrorMgr_Txt[0] , 0 , sizeof(ErrorMgr_Txt));		
				HardInit_Step = 1;	
	
			}

		}	

		Main_IO.Out[RESET_ESR_Btn_out] = 0;
		RESET_ESR_Btn_vis = INVISIBLE;
	}

	_rESET_STOP_Btns = RESET_STOP_Btns;
	
	///////////////////////////////////////////////

	
	if(!ESTOP || FirstBootUp){
		
		if(_estopSign)	
			LogTxt(&LogMgr, "Estop Pressed");

		if(FirstBootUp){

			if(HardInit_Step < 500)
				HardInit_Step = 500;

		}
		else if(HardInit_Step < 200)
			HardInit_Step = 200;	

	}
	else if(!_estopSign)
		LogTxt(&LogMgr, "Estop Clear");
	
	_estopSign = ESTOP;
	//////////////////////////////////


	
	if(!HardwReady)
		ReadyToOperate = 0;
	
	switch(HardInit_Step){


		case 0:
			
			HardwReady = 0;	
			Seq.State = 300;

			ENABLE_sign = 0;
			
	
		break;

		case 1:

			HardwReady = 0;
			ENABLE_sign = 1;

			ArcLink.robot.Cntrl.Fbk.Error = 0;

			ErrorFromMaster_flag = 0;
		
			if(FirstBootUp)
				Seq.State = 303;
			else	
				Seq.State = 300;
			
			if(ESTOP && Main_IO.Out[ENABLE_out]){

				if(KK_Reset_Seq)
					Init_Indx = 0;
				else
					Init_Indx = SERVO_NUM-1;

				TVL_Step = 0;
				OSC_Step = 0;
				INDX_Step = 0;
				AHC_Step = 0;
				WF_Step = 0;


				strcpy(HMI_Error_Txt , "");

				for(i = 0 ; i < VECTORS_NUM ; i++){ 

					VectorStateMach[i].Mode = vectSt_Mode_OFF;			
					VectorStateMach[i].FbkVect.Mode = 100; // unknown
				}	

				LogTxt(&LogMgr, "Reset TML Errors");
				
				TimerStart(&Init_Tmr, 0.2);
				WatchEthernet_Tmr.IN = 0;
				
				ETH_Try_Again_Cnt = 0;
				ETH_Try_Again_Cnt_act_Max = 0;
				ETH_Try_Again_Cnt_act = 0;
				
				LED_CMD = 255;

				TVL_Start_Err = 0;

				HardInit_Step = 4;
			}

		break;
		 

		case 4:

			WatchTransError(1);
			
			if(!TimerIsTimeOut(&Init_Tmr))
				break;

			
			if(TML_MTR_INST[Init_Indx].Disable){

				TimerStart(&Init_Tmr, 0.01);
				WatchEthernet_Tmr.IN = 0;
				ETH_Try_Again_Cnt = 0;
				HardInit_Step++;

			}
			else if(TML_MTR_INST[Init_Indx]._int.Init && !HardRestart_Mode){

				TML_Msg_Chan[1].Msg_ID = 0;
				TML_Data_Chan[1].Msg_ID = 0;

				TML_Msg_Chan[1].Msg_Error = 0;
				TML_Data_Chan[1].Msg_Error = 0;

				TML_Msg_Chan[1].Trash_Cntr = 0;
				TML_Data_Chan[1].Trash_Cntr = 0;
			
				TML_MTR_INST[Init_Indx].xCMD = TML_RESET_ONLY_CMD;//;TML_RESET_ONLY_CMD; TML_FAULTR				

				if(Reset_Proc_Dly > 0.0)
					TimerStart(&Init_Tmr, Reset_Proc_Dly);
				else
					TimerStart(&Init_Tmr, 0.01);

				HardInit_Step++;
			

			}
			else{
				
				TML_MTR_INST[Init_Indx].xCMD = TML_RESTART;
				TML_MTR_INST[Init_Indx]._int.Init = 0;	
				TML_MTR_INST[Init_Indx].ErrorReg = 0;
				TML_MTR_INST[Init_Indx].Error = 0;
				TML_MTR_INST[Init_Indx].EStop = 0;
						
				TimerStart(&Init_Tmr, 0.1);
				WatchEthernet_Tmr.IN = 0;
				ETH_Try_Again_Cnt = 0;
				HardInit_Step++;

			}

			

		break;


		case 5:

			if(!TimerIsTimeOut(&Init_Tmr))
				break;

			WatchTransError(1);

			if((TML_MTR_INST[Init_Indx]._int.Init 
				&& (TML_MTR_INST[Init_Indx].xCMD == TML_IDLE))
				//|| TML_MTR_INST[Init_Indx].Error
				//|| TimerIsTimeOut(&Init_Tmr) 
				|| TML_MTR_INST[Init_Indx].Disable){


				if(!KK_HaltProcess){ 


					switch(Init_Indx){

						case TML_AX_TVL:
				
							if(HMI_AXIS_Disable[TML_AX_TVL] == 0)
								TVL_Step = 1;

						break;

						case TML_AX_OSC:
							
							if(HMI_AXIS_Disable[TML_AX_OSC] == 0)
								OSC_Step = 1;

						break;

						case TML_AX_INDX:
						case TML_AX_INDX_EXT:
						
							if((HMI_AXIS_Disable[TML_AX_INDX] == 0)
								||(HMI_AXIS_Disable[TML_AX_INDX_EXT] == 0))									
								INDX_Step = 1;

						break;	
						

						case TML_AX_AHC:
							
							if(HMI_AXIS_Disable[TML_AX_AHC] == 0)
								AHC_Step = 1;
						break;

						case TML_AX_WF:
							
							if(HMI_AXIS_Disable[TML_AX_WF] == 0)
								WF_Step = 1;
						break;

					}
				}


				if(KK_Reset_Seq)
					Init_Indx++;
				else
					Init_Indx--;
				
				if(Init_Indx >= SERVO_NUM){
					LogTxt(&LogMgr, "Restart Process");
					TimerStart(&Init_Tmr, 0.05);
					WatchEthernet_Tmr.IN = 0;
					ETH_Try_Again_Cnt = 0;
					HardInit_Step = 10;				
				}
				else{
					TimerStart(&Init_Tmr, 0.2);
					HardInit_Step = 4;
				}	
				
			}

		break;

		
		case 10:

			Seq.State = 300;

			WatchTransError(1);

			if(!TimerIsTimeOut(&Init_Tmr))
				break;

			HardRestart_Mode = 0;
			FirstBootUp = 0;

			HardwReady = 1;

			TimerStart(&TVL_Tmr, 0.01);
			TimerStart(&OSC_Tmr, 0.01);
			//TimerStart(&INDX_Tmr, 0.01);
			TimerStart(&AHC_Tmr, 0.01);
			TimerStart(&WF_Tmr, 0.01);

			
			WatchEthernet_Tmr.IN = 0;
			ETH_Try_Again_Cnt = 0;
			TimerStart(&Init_Tmr, 20.0);
			HardInit_Step++;
			
		
		break;

		case 11:

			WatchTransError(1);
			
			Seq.State = 300;			

			if(((TVL_Step >= 10) || (TVL_Step == 0) )
				&& ((OSC_Step >= 5) || (OSC_Step == 0) )
				&& ((AHC_Step >= 10) || (AHC_Step == 0) )
				&& ((INDX_Step >= 10) || (INDX_Step == 0))
				&& ((WF_Step >= 10) || (WF_Step == 0) )){

				
				for(i = 0 ; i < SERVO_NUM ; i++)
					TML_MTR_INST[i].HoldErrorReg = 0;

				ReadyToOperate = 1;
				Seq.State = 0;
				TML_MTR_CUT_POWER = 0;

				LogTxt(&LogMgr, "Machine Ready");

				WatchEthernet_Tmr.IN = 0;
				ETH_Try_Again_Cnt = 0;		

				GAS_ON_Tmr.IN = 0;
				TimerStart(&GAS_OFF_Tmr, 0.3);
								
				TimerStart(&Init_Tmr, 2.5);	

				ESAB_PS.Fbk.NewErrorCode = 0;
				
				HardInit_Step = 50;
			}
			if(TimerIsTimeOut(&Init_Tmr)){

				xsprintf(HMI_Error_Txt,"Axis Init Error" );

				LogTxt(&LogMgr, HMI_Error_Txt);

			 	//ENABLE_sign = 0;vv                      
			 	ENABLE_sign = 1;
				
				GAS_OFF_Tmr.PT = (int) (permMem.Analog_Out[GAS_PostPurge].Value * 1000.0 +0.5);					
				GAS_OFF_Tmr.IN = 1;
				GAS_ON_Tmr.IN = 0;

				if(ReadyToOperate && HardwReady)
					INDX_Step_Cmd = 0;

				

				EndOfBead_Stop = 0;
				Sequence_Halt_Cmd = 0;	

				SKIP_Weld_Cmd = 0;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;				

				
				Seq.State = 0;

				TimerStart(&Init_Tmr, 0.5);	
				
				HardInit_Step = 51;

			}
				
		break;		

		case 50:  // Harwdare OK - watch for any error but not ESTOP

			if(Init_Tmr.Q){
				Init_Tmr.IN = 0;
				Init_Tmr.Q = 0;

				if(Head.Remote_SW)
					LED_CMD = 10;
				else
					LED_CMD = 20;
			}

			if(KK_Aply_Estop)
				ENABLE_sign = !TML_MTR_CUT_POWER;
			
			status = 0;

			HoldThisAxError = 0xFFFF;

			
			for(i = 0 ; i < SERVO_NUM ; i++){ //SERVO_NUM


				if(!TML_MTR_INST[i].Disable && (HoldThisAxError == 0xFFFF)){

					if(TML_MTR_INST[i].Error 
						|| !TML_MTR_INST[i]._int.Init
						|| (TML_MTR_INST[i].xCMD == TML_ERROR)){

							xsprintf(HMI_Error_Txt," Drive %s Er.: %x " , TML_MTR_INST[i].Name , (UDINT)TML_MTR_INST[i].ErrorReg);							
							TML_MTR_INST[i].HoldErrorReg = 1;
							HoldThisAxError = i;
							status = 1;	

					}
				}
			}
			

			///////////////////////////////////////////

			if(TVL_Start_Err > 0){				
				status = 1;
				xsprintf(HMI_Error_Txt,"TML Gen. Err. %d" , (UDINT)TVL_Start_Err );
				//LogTxt(&LogMgr, HMI_Error_Txt);
				

			}

			if(MachCfg.Weld){

				if(ESAB_Tmo_Err > 0){				
					status = 1;
					xsprintf(HMI_Error_Txt,"ESAB Com. Err. %d" , (UDINT)ESAB_Tmo_Err );
					//LogTxt(&LogMgr, HMI_Error_Txt);
					

				}
			}
			else
				ESAB_Tmo_Err = 0;

			
			
			///////////////////////////////////////////
			if(MachCfg.Weld){

				if(MachCfg.GougingON){

					if(ESAB_PS.step < 3){
						
						status = 1;
						xsprintf(HMI_Error_Txt,"ESAB Com. Error");
						LogTxt(&LogMgr, HMI_Error_Txt);
					}
			
					else if(ESAB_PS.Fbk.NewErrorCode != 0){
						status = 1;
						xsprintf(HMI_Error_Txt,"ESAB Err.: %d" , (UDINT)ESAB_PS.Fbk.NewErrorCode);
						LogTxt(&LogMgr, HMI_Error_Txt);

					}	
					

				}
				else{
					
					if(!ArcLink.robot.isMapped){				
						status = 1;
						xsprintf(HMI_Error_Txt,"Arclink Error" );

					}
					else  if(ArcLink.robot.Cntrl.Fbk.Error > 0){
						status = 1;
						xsprintf(HMI_Error_Txt,"Lincoln Err.: %d" , ArcLink.robot.Cntrl.Fbk.Status);
					}
					else if(ErrorFromMaster_flag)
						status = 1;
					
					else if(!MachCfg.GougingON){ // NOT Gouging

						if((AccessLevel == 0)
							&& TML_MTR_INST[TML_AX_WF].DigInp[0]){ // Wire Break
							status = 1;
							xsprintf(HMI_Error_Txt,"Wire Break" );

						}
						
					}

				}
				
			}

			/////////////////////////////////////

			if((TML_Msg_Chan[1].Msg_Error != 0) || (TML_Data_Chan[1].Msg_Error != 0)){
					
				status = 1;
				xsprintf(HMI_Error_Txt,"Eth. Noise Err. %d" , TML_Data_Chan[1].Msg_Error);

			}
			

			if(WatchTransError(0))
				status = 1;
			
			 if(status || TML_MTR_CUT_POWER){	

			 	LogTxt(&LogMgr, HMI_Error_Txt);

			 	//HardwReady = 0;
			 	
			 	//ENABLE_sign = 0;                      
			 	//ENABLE_sign = 1;

				// Motors with Brakes
				//TML_MTR_INST[TML_AX_INDX].xCMD = TML_RESET_ONLY_CMD;
				//TML_MTR_INST[TML_AX_INDX].xCMD = TML_AXIS_OFF_CMD;
			 	//VectorStateMach[VECT_AX_INDX].Mode = vectSt_Mode_OFF;			
				//VectorStateMach[VECT_AX_INDX].FbkVect.Mode = 100; // unknown

			 	//VectorStateMach[VECT_AX_INDX].Mode = vectSt_Mode_Idle;
				//VectorStateMach[VECT_AX_INDX_EXT].Mode = vectSt_Mode_Idle;
				INDX_Step = 100;

				
				//VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_OFF;			
				//VectorStateMach[VECT_AX_TVL].FbkVect.Mode = 100; // unknown
				//TML_MTR_INST[TML_AX_TVL].xCMD = TML_AXIS_OFF_CMD;
				//VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Idle;
				TVL_Step = 100;

				///

				//VectorStateMach[VECT_AX_WF].Mode = vectSt_Mode_OFF;			
				//VectorStateMach[VECT_AX_WF].FbkVect.Mode = 100; // unknown
				//TML_MTR_INST[TML_AX_WF].xCMD = TML_AXIS_OFF_CMD;
				VectorStateMach[VECT_AX_WF].Mode = vectSt_Mode_Idle;
				WF_Step = 0;

				//VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_OFF;			
				//VectorStateMach[VECT_AX_AHC].FbkVect.Mode = 100; // unknown
				//TML_MTR_INST[TML_AX_AHC].xCMD = TML_AXIS_OFF_CMD;
				VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Idle;				
				AHC_Step = 0;

			
				///

				SequenceRun_Out = 0;
				Robot_Cmd_RUN = 0;
				SequenceRun_WF = 0;	
				
				EndOfBead_Stop = 0;
				Sequence_Halt_Cmd = 0;

				INDX_Step_Cmd = 0;

				Seq.State = 0;

				/////////////////////////////
				
				
				GAS_OFF_Tmr.PT = (int) (permMem.Analog_Out[GAS_PostPurge].Value * 1000.0 +0.5);					
				GAS_OFF_Tmr.IN = 1;
				GAS_ON_Tmr.IN = 0;

				//if(ReadyToOperate && HardwReady)
					//INDX_Step_Cmd = 0;

				

				EndOfBead_Stop = 0;
				Sequence_Halt_Cmd = 0;	

				SKIP_Weld_Cmd = 0;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;	

				ESAB_PS.Cmd.QuickStop = 1;
				ESAB_PS.Cmd.WeldON = 0;
				

				TimerStart(&Init_Tmr, 2.0);				
				
				HardInit_Step++;

			}		
			

		break;	

		case 51:

			Seq.State = 301;

			if(TimerIsTimeOut(&Init_Tmr)
				|| ((TVL_Step == 200) && (INDX_Step == 200) )){
				
				TimerStart(&Init_Tmr, 3.0);

				if(!KK_DoNotStopOnError)
					ENABLE_sign = 0;

				LED_CMD = 30;
				
				HardInit_Step = 100;
			}

		break;

		case 100: // Hardare error (Not Estop) - wait for error clearing 

			if(TimerIsTimeOut(&Init_Tmr)){				
				ENABLE_sign = 1;				
			}
			
			Seq.State = 301;

			status = 1;
			for(i = 0 ; i < SERVO_NUM ; i++){
			
				if(TML_MTR_INST[i].Error )
					status = 0;
				else if(!(TML_MTR_INST[i]._int.Init || TML_MTR_INST[i].Disable))
					status = 0;
			}

			
			if(!ArcLink.robot.isMapped){				
				status = 0;
				

			}
			else if(ArcLink.robot.Cntrl.Fbk.Error > 0){
				status = 0;
				
			}
			else if(ErrorFromMaster_flag)
				status = 0;

			
			else if(!MachCfg.GougingON && MachCfg.Weld && (AccessLevel < 9)
				&& TML_MTR_INST[TML_AX_WF].DigInp[0]) // Wire Break
				status = 0;

			if(TVL_Start_Err)
				status = 0;

			if(ESAB_Tmo_Err)
				status = 0;
						
			if(status){
			
				//xsprintf(HMI_Error_Txt,"Robot is ready to RESTART" );
				
				//Seq.State = 0;
				//HardwReady = 1;
				//ENABLE_sign = 1;
								
				//HardInit_Step = 1;
				HardInit_Step = 101;
			}

		break;



		case 200: // THIS is state for ESTOP fault

				Seq.State = 302;
				HardwReady = 0;

				for(i = 0 ; i < SERVO_NUM ; i++){

					VectorStateMach[i].Mode = vectSt_Mode_OFF;				

				}

				TVL_Step = 0;
				OSC_Step = 0;
				INDX_Step = 0;
				AHC_Step = 0;
				WF_Step = 0;

				ENABLE_sign = 0;

				LogTxt(&LogMgr, "Eth: %d %d" ,ETH_Try_Again_Cnt , ETH_Try_Again_Cnt_act_Max);

				HardInit_Step ++;

		break;


		case 201: // Wait for ESTOP clear

			if(ESTOP){
				
				ENABLE_sign = 1;
				TimerStart(&Init_Tmr, 1.5);
				HardInit_Step ++;
			}


		break;


		case 202:

			if(TimerIsTimeOut(&Init_Tmr))
				//HardInit_Step = 0;
				HardInit_Step = 1;

		break;


		case 500:			

			
			if(ESTOP || FirstBootUp){

				Seq.State = 303;

				TimerStart(&Init_Tmr, 1.0);
				HardInit_Step ++;
			}
			else
				Seq.State = 302;


		break;


		case 501:

			if(TimerIsTimeOut(&Init_Tmr)){

				
				//for(i = 0 ; i < SERVO_NUM ; i++)
					//TML_MTR_INST[i].xCMD = TML_RESTART;

				TML_MTR_INST[TML_AX_TVL].xCMD = TML_RESTART;

				TimerStart(&Init_Tmr, 5.0);

				FirstBootUp = 0;
				HardInit_Step ++;
			
			}

		break;


		case 502:

				
			if(TimerIsTimeOut(&Init_Tmr))			
				HardInit_Step = 1;

		break;
		


		default:


		break;

	}

	///////////////////////////////////////////
	
	ActCurrent_Filter.x = ArcLink.robot.Cntrl.Fbk.curArcAmp;
	AzzMovAvgFlt(&ActCurrent_Filter);

	ActCurrrentFbk = getFromCalib(ActCurrent_Filter.y, &CurCalib.Points[AMPS_Calibr_Indx] , CALIB_2POINTS_INV);
	
	if(HardwReady){
		
		TVL_Func();
		OSC_Func();
		INDX_Func();
		AHC_Func(); AHC_Mode = 1; // PID with window on PLC
		WF_Func();
		//WF_Func_Volt();
		
	}
	
	///////
	TimerExecute(&Init_Tmr);
	TimerExecute(&Cyclic_Tmr);
	TimerExecute(&WatchEthernet_Tmr);
	_SequenceRun_Out = SequenceRun_Out;

}

void _EXIT ProgramExit(void)
{

}

void TVL_Func(){
float f , x;

	if (TML_MTR_INST[TML_AX_TVL]._int.Init &&
		 (TML_MTR_INST[TML_AX_TVL]._cfg.absFbk != 2))
		OLD_MTR_POS[TML_AX_TVL] = VectorStateMach[VECT_AX_TVL].actPosDINT;	

	TVL_Act_Pos = TML_MTR_INST[TML_AX_TVL].PosFBK;

	
	if(WatchIndx){		
		TVL_Last_Pos = TVL_Act_Pos;

	}
	
	//////////////////////////
	if(TravelJog.IN != Stop)
		TVL_Jog_Ramp.x = 1.0;
		
	else if(TVL_Jog_Ramp.y != 0.0){

		TVL_Jog_Ramp.x = 0.0;
		TVL_Jog_Ramp.y_set = 0.0;
		TVL_Jog_Ramp.set = 1;

	}
	//////////////////////////

	AzzRamp(&TVL_Jog_Ramp);
	TVL_Jog_Ramp.set = 0;

	switch(TVL_Step){


		case 0: // IDLE		 

		break;

		case 1:

			if(!CheckAxisOn(&AxisCh[TML_AX_TVL]))
				break;			
				
			
			TVL_Step = 2;			
			
		break;

		case 2:

			//if(!CheckAxisOn(&AxisCh[TML_AX_TVL]))
				//break;			

			if(!VectorStateMach[VECT_AX_TVL].xCMD 
				&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){

				VectorStateMach[VECT_AX_TVL].InVal = 0.0;
				VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Vel;

				TimerStart(&TVL_DT_Tmr, 0.1);
				TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm * 2.0);
				TVL_Repeat_Init_Cnt = 0;
				
				TVL_Step = 3;

			}				
			
		break;

		case 3:

			if(TVL_StatusReg_Tmr.Q){

				TVL_Start_Err = 5;

			}
			else if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode == vectSt_Mode_Vel) {				

				//if(!VectorStateMach[VECT_AX_TVL].xCMD 
					//&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){

				if(!TML_MTR_INST[TML_AX_TVL].xCMD){
					
					TML_MTR_INST[TML_AX_TVL].xCMD = TML_READ_MCR; //VECT_Read_MCR	
				
					//VectorStateMach[VECT_AX_TVL].xCMD = VECT_Read_MCR;
					TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm * 2.0);
					TVL_Repeat_Init_Cnt = 0;
					TVL_Step++;

				}

			}			

			else if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode != vectSt_Mode_Vel)
				break;			
			

		break;	

		
		case 4:

			if((VectorStateMach[VECT_AX_TVL].MCRReg & 0x0700) == 0x0300)			
					TVL_Step = 10;

			else if(TVL_StatusReg_Tmr.Q)
				TVL_Start_Err = 6;				
			

		break;	
		

		case 10:
			

			VectorStateMach[VECT_AX_TVL].InVal = 0.0;

			
			if((TVL_Calibr_Speed >0.0 )
				&& (TVL_Calibr_Distance != 0.0)){
			
				TVL_Step = 40;
				
				break;
			}	


			/*
			if(	(_TravelSpeedAO != permMem.Analog_Out[TravelSpeedAO_Indx].Value)
				|| (_TVL_Return_Speed != permMem.Analog_Out[TVL_Return_Speed].Value)
				|| (_Travel_Skip_Speed != permMem.Analog_Out[TVL_Skip_Speed].Value)			
				|| (_TravelSpeed_IN != TravelSpeed.IN)
				|| (_TravelRampAO = !permMem.Analog_Out[TVL_Ramp].Value)
				|| (_SKIP_Weld_Cmd != SKIP_Weld_Cmd)
				|| (_MoveEnable2 != MoveEnable)
				|| TVL_StatusReg_Tmr.Q){


				if(!VectorStateMach[VECT_AX_TVL].xCMD 
						&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){							
							
					CalculateParamForABSCmove();
					VectorStateMach[VECT_AX_TVL].xCMD = VECT_Move_Abs_Param;					

					TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm * 4);
					_TravelSpeedAO = permMem.Analog_Out[TravelSpeedAO_Indx].Value;
					_TVL_Return_Speed = permMem.Analog_Out[TVL_Return_Speed].Value;
					_Travel_Skip_Speed = permMem.Analog_Out[TVL_Skip_Speed].Value;			
					_TravelSpeed_IN = TravelSpeed.IN;
					_TravelRampAO = permMem.Analog_Out[TVL_Ramp].Value;
					_SKIP_Weld_Cmd = SKIP_Weld_Cmd;
					_MoveEnable2 = MoveEnable;

				}

			};
			*/
			
			if(SequenceRun_Out
				&& (permMem.Analog_Out[TravelSpeedAO_Indx].Value > 0.0)){
				
				if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode  != vectSt_Mode_Drive_Independant)
					VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Drive_Independant;
				
				else {					

					CalculateParamForABSCmove();
					
					if(TravelSpeed.IN != Stop){ 
						
						_TravelSpeedAO = permMem.Analog_Out[TravelSpeedAO_Indx].Value;
						
						if(!VectorStateMach[VECT_AX_TVL].xCMD 
							&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){
							
							VectorStateMach[VECT_AX_TVL].xCMD = VECT_Move_Abs_Cmd;

							TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm);
							
							TVL_Step = 15;

						}
						
					}

				}			

			}
			else if((TVL_Step_Cmd != 0)){
				
				
				if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode  != vectSt_Mode_Hold)
					VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Hold;
				
				else {				 
				
					VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CSPD] = permMem.Analog_Out[Index_Step_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX;
					VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CACC] = VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CSPD] * 10.0;

					//if((INDX_Step_Cmd > 0) && !(TML_MTR_INST[tml_indx].ErrorReg &0x0040)){
					if((TravelSpeed.IN == Forward) && !(TML_MTR_INST[TML_AX_TVL].ErrorReg &0x0040)){

						if(Seq.State == 0)							
							VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_H_Indx].Value;
						else	
							VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_L_Indx].Value ;
						
						if(TVL_Step_Cmd < 0)
							VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS] = -VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS];

						if(fabsf(VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS]) < 0.01)
							TVL_Step_Cmd = 0;
						
						else{
							
							VectorStateMach[VECT_AX_TVL].xCMD = VECT_Move_Rel_Cmd;						
							TVL_Step = 50;	
						}
					}
					//else if((INDX_Step_Cmd < 0) && !(TML_MTR_INST[tml_indx].ErrorReg &0x0080)){
					else if((TravelSpeed.IN == Reverse) && !(TML_MTR_INST[TML_AX_TVL].ErrorReg &0x0080)){

						if(Seq.State == 0)
							VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_L_Indx].Value ;
						else	
							VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_H_Indx].Value;

						if(TVL_Step_Cmd < 0)
							VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS] = -VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS];
						
						if(fabsf(VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS]) < 0.01)
							TVL_Step_Cmd = 0;
						
						else{
							
							VectorStateMach[VECT_AX_TVL].xCMD = VECT_Move_Rel_Cmd;						
							TVL_Step = 50;	
						}
					}
					else
						TVL_Step_Cmd = 0;
					 
				}
			}	

			else if((Seq.State == 0)
				|| (Seq.State == 200)) { // Jogging

				if(TravelJog.IN != Stop){

					if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode  != vectSt_Mode_Vel){
						VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Vel;
						
					}else {

						VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Cmd_CACC] =  Calculate_Acc(700.0/60.0 , 0.2);
						VectorStateMach[VECT_AX_TVL].xCMD = VECT_Set_CACC;	
					
						TimerStart(&TVL_DT_Tmr, 2.5);
						TVL_Step = 11;

					}
				}
				
			}
			
				
			
	
		break;

		case 11:			
							

			if(TravelJog.IN == Forward)				
				VectorStateMach[VECT_AX_TVL].InVal = permMem.Analog_Out[TVL_Jog_Speed_Indx].Value/100.0 * TVL_JOG_MAX * TVL_Jog_Ramp.y;
				
			
			else if(TravelJog.IN == Reverse)
				VectorStateMach[VECT_AX_TVL].InVal = -permMem.Analog_Out[TVL_Jog_Speed_Indx].Value/100.0 * TVL_JOG_MAX  * TVL_Jog_Ramp.y;
				
			else{
				
				VectorStateMach[VECT_AX_TVL].InVal = 0.0;
				//TVL_Step = 10; // zmienione 10 Mar 2023
				TVL_Step = 1; // wrocilo 6 June 23
			}	
			

			if(TimerIsTimeOut(&TVL_DT_Tmr)){				

				if((VectorStateMach[VECT_AX_TVL].FbkVect.x > (CurCalib.TVL_Hi_Limit - 0.5))
					|| (VectorStateMach[VECT_AX_TVL].FbkVect.x < (CurCalib.TVL_Lo_Limit + 0.5)))
					break;

				
				if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode  != vectSt_Mode_Drive_Independant){
					VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Drive_Independant;
					break;
				}
				
				
				f = permMem.Analog_Out[TVL_Jog_Speed_Indx].Value/100.0 * TVL_JOG_MAX;;
				
				VectorStateMach[VECT_AX_TVL].fParam[0] =  f * 1.0; // ACC			
				VectorStateMach[VECT_AX_TVL].fParam[1] = f; // CSPD

											
				switch(TravelJog.IN){

					default:
						VectorStateMach[VECT_AX_TVL].InVal = 0.0;
					break;

					case  Forward:								
						VectorStateMach[VECT_AX_TVL].Param[0] = (DINT)(CurCalib.TVL_Hi_Limit * TML_MTR_INST[TML_AX_TVL]._int.kPos);
					break;

					case  Reverse:													
						VectorStateMach[VECT_AX_TVL].Param[0] = (DINT)(CurCalib.TVL_Lo_Limit * TML_MTR_INST[TML_AX_TVL]._int.kPos);
					break;
				}	

				if(!VectorStateMach[VECT_AX_TVL].xCMD 
					&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){
					
					VectorStateMach[VECT_AX_TVL].xCMD = VECT_Move_Abs_Cmd;
					_TravelJog_IN = (TravelJog.IN != Stop);
					TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm);
					TVL_Step ++;

				}

			

			}
			else if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode  != vectSt_Mode_Vel){
				VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Vel;
				break;
			}


		break;

		case 12:

			if(((TravelJog.IN != Stop) && !_TravelJog_IN)
				|| Main_IO.Inp[STOP_RST_BTN]
				|| Main_IO.Inp[STOP_RST_BTN_LOC]){

				VectorStateMach[VECT_AX_TVL].cmdWord = 1;	
				VectorStateMach[VECT_AX_TVL].xCMD = VECT_Set_CmdWord;				

				TVL_Step = 14;

			}
			else if(!(VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400)
					|| TVL_StatusReg_Tmr.Q){
					
				//VectorStateMach[VECT_AX_TVL].xCMD = VECT_Move_Abs_Cmd; // Powtarzam 	
				TVL_Step ++;
			}
			_TravelJog_IN = (TravelJog.IN != Stop);
			
		break;

		case 13:


			if(((TravelJog.IN != Stop) && !_TravelJog_IN)
				|| Main_IO.Inp[STOP_RST_BTN]
				|| Main_IO.Inp[STOP_RST_BTN_LOC]
				|| KK_Stop){

				KK_Stop = 0;

				VectorStateMach[VECT_AX_TVL].cmdWord = 1;	
				VectorStateMach[VECT_AX_TVL].xCMD = VECT_Set_CmdWord;


				TVL_Step++;

			}
			else if(VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400){

				TravelSpeed.IN = (TravelSpeed.IN == Forward)? Reverse:Forward;
				
				TVL_Step++;
			}

			_TravelJog_IN = TravelJog.IN;
			
		break;


		case 14:

			if(VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400){
				//VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Vel;
				TVL_Step = 1;
				//TVL_Step = 10;
			}

		break;

		/////////////////////////////////////////////////////////////////////////////////////////
		case 15:

			if(!SequenceRun_Out){

				VectorStateMach[VECT_AX_TVL].cmdWord = 1;	
				VectorStateMach[VECT_AX_TVL].xCMD = VECT_Set_CmdWord;

				SequenceRun_Out = 0;	

				TVL_Step = 1;
				//TVL_Step = 10;

			}
			else if(TVL_StatusReg_Tmr.Q){
			
				//TVL_Start_Err = 1;

			}
			else if(!(VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400)
				&& ((VectorStateMach[VECT_AX_TVL].MCRReg & 0x2701) == 0x2701)){
				
				//if(!VectorStateMach[VECT_AX_TVL].xCMD
					//&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){
					
				if(!TML_MTR_INST[TML_AX_TVL].xCMD){
					
					TML_MTR_INST[TML_AX_TVL].xCMD = TML_READ_MCR; //VECT_Read_MCR
					

					VectorStateMach[VECT_AX_TVL].MCRReg = 0x0;	
					//VectorStateMach[VECT_AX_TVL].xCMD = VECT_Read_MCR;

					LED_One_Blink_Req = 1;
					
					TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm);	
					TVL_Step ++;
				}
						
				
			}
			
			
		break;

		case 16:


			if(!SequenceRun_Out){

				if(!VectorStateMach[VECT_AX_TVL].xCMD
					&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){
					
					VectorStateMach[VECT_AX_TVL].cmdWord = 1;	
					VectorStateMach[VECT_AX_TVL].xCMD = VECT_Set_CmdWord;

					SequenceRun_Out = 0;
					TVL_Step = 1;
					//TVL_Step = 10;
					//TVL_Step = 17; // to czasami nie dziala
					TimerStart(&TVL_StatusReg_Tmr, 1.5);
					

				}

			}

			else if(TVL_StatusReg_Tmr.Q){	


				LED_One_Blink_Req = 1;

				if(!TML_MTR_INST[TML_AX_TVL].xCMD){
					
					

				//if(!VectorStateMach[VECT_AX_TVL].xCMD
					//&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){

					
					if((VectorStateMach[VECT_AX_TVL].MCRReg & 0x2701) == 0x2701){
						VectorStateMach[VECT_AX_TVL].MCRReg = 0x0;
						TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm * 4.0);
						//VectorStateMach[VECT_AX_TVL].xCMD = VECT_Read_MCR;
						TML_MTR_INST[TML_AX_TVL].xCMD = TML_READ_MCR; //VECT_Read_MCR
						
					}else{
						
						TVL_Start_Err = 3;
					}
					
				}
			}
			else if(VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400){

				TravelSpeed.IN = (TravelSpeed.IN == Forward)? Reverse:Forward;

				SequenceRun_Out = 0;	
				//SKIP_Weld_Cmd = 0;
				TVL_Step = 1;
				//TVL_Step = 10;
			}
			
			else if((_TravelSpeedAO != permMem.Analog_Out[TravelSpeedAO_Indx].Value)
					 || (_SKIP_Weld_Cmd != SKIP_Weld_Cmd)){
					 
					_TravelSpeedAO = permMem.Analog_Out[TravelSpeedAO_Indx].Value;
					_SKIP_Weld_Cmd = SKIP_Weld_Cmd;

					if(MoveEnable){
						CalculateParamForABSCmove();
						VectorStateMach[VECT_AX_TVL].xCMD = VECT_Upd_CACC;
					}
						

			}
			
		break;


		case 17:

			if((VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400)
				|| TVL_StatusReg_Tmr.Q){

				//TravelSpeed.IN = (TravelSpeed.IN == Forward)? Reverse:Forward;

				SequenceRun_Out = 0;	
				//SKIP_Weld_Cmd = 0;
				TVL_Step = 1;
				//TVL_Step = 10;
			}

		break;

		/////////////////////////////////////////////////////////////////

		case 20:

			VectorStateMach[VECT_AX_TVL].InVal = TVL_Calibr_Speed/60.0;

			if(TVL_Calibr_Speed == 0.0)
				TVL_Step = 1;	


		break;

		///////////

		case 40:

			if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode != vectSt_Mode_Drive_Independant){
				VectorStateMach[VECT_AX_TVL].InVal = 0.0;
				VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Drive_Independant;
				
			}else
				TVL_Step ++;


		break;


		case 41:
			
				f = TVL_Calibr_Speed / 60.0;		
			
				VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CSPD] = f;
				VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CACC] = VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CSPD] * 10.0;
				VectorStateMach[VECT_AX_TVL].fParam[V_fParam_Rel_CPOS] = TVL_Calibr_Distance ; // Distance;
				
				VectorStateMach[VECT_AX_TVL].xCMD = VECT_Move_Rel_Cmd;
				TimerStart(&TVL_StatusReg_Tmr, StatusReg_Tm);
				
				
				TVL_Step ++;
		break;


		case 42:

				if(TVL_Calibr_Speed == 0.0){

					VectorStateMach[VECT_AX_TVL].cmdWord = 1;	
					VectorStateMach[VECT_AX_TVL].xCMD = VECT_Set_CmdWord;

					TVL_Calibr_Distance = 0.0;	

					TVL_Step = 1;

				}
				else if(!(VectorStateMach[VECT_AX_TVL].StatusReg & 0x02000400)
					|| TVL_StatusReg_Tmr.Q)
					TVL_Step ++;
					
		break;

		case 43:

			 if(TVL_Calibr_Speed == 0.0){

				VectorStateMach[VECT_AX_TVL].cmdWord = 1;	
				VectorStateMach[VECT_AX_TVL].xCMD = VECT_Set_CmdWord;

				TVL_Calibr_Distance = 0.0;	

				TVL_Step = 1;

			}

			 else if(VectorStateMach[VECT_AX_TVL].StatusReg & 0x02000400){ // movement stopped

				TVL_Calibr_Distance = 0.0;

				TVL_Step = 1;

			  }


		break;

		//////////////////////////////////////
		
		case 50:

			if((TVL_Step_Cmd > 0) && (TML_MTR_INST[TML_AX_TVL].ErrorReg &0x0040)){
				TimerStart(&INDX_Tmr, 0.5);
				TVL_Step = 1;

			}
			else if((TVL_Step_Cmd < 0) && (TML_MTR_INST[TML_AX_TVL].ErrorReg &0x0080)){
				TimerStart(&TVL_Tmr, 0.5);
				TVL_Step = 1;
			}	
			
			else if(!(VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400)){

				TVL_Step ++;
			}
			
		break;

		case 51:			

			if((TVL_Step_Cmd > 0) && (TML_MTR_INST[TML_AX_TVL].ErrorReg &0x0040)){
				TimerStart(&TVL_Tmr, 0.5);
				TVL_Step = 1;

			}
			else if((TVL_Step_Cmd < 0) && (TML_MTR_INST[TML_AX_TVL].ErrorReg &0x0080)){
				TimerStart(&TVL_Tmr, 0.5);
				TVL_Step = 1;
			}	
			
			else if(TVL_Step_Cmd == 0){

				TVL_Step = 1;
			}
			else if(VectorStateMach[VECT_AX_TVL].StatusReg &0x02000400){

				TVL_Step_Cmd = 0;
				TVL_Step = 1;
			}
			
			
		break;


		///////////////////////////////////////
		case 100:
			
			if(VectorStateMach[VECT_AX_TVL].FbkVect.Mode != vectSt_Mode_Hold)
				VectorStateMach[VECT_AX_TVL].Mode = vectSt_Mode_Hold;

			else if(!TML_MTR_INST[TML_AX_TVL].xCMD){
				TML_MTR_INST[TML_AX_TVL].xCMD = TML_AXIS_OFF_CMD;
				TVL_Step++;
			}	


		break;

		case 101:

			if(!(TML_MTR_INST[TML_AX_TVL].StatusReg & 0x8000))
				TVL_Step = 200;

		break;


		///////////////////////////////////

		
		default:

			//TVL_Step = 0;

		break;

	}

	TimerExecute(&TVL_Tmr);
	TimerExecute(&TVL_DT_Tmr);
	TimerExecute(&TVL_StatusReg_Tmr);

}

			

						


void OSC_Func(){
	////////////////

	// Store last Position
	if (TML_MTR_INST[TML_AX_OSC]._int.Init &&
		 (TML_MTR_INST[TML_AX_OSC]._cfg.absFbk != 2))
		OLD_MTR_POS[TML_AX_OSC] = VectorStateMach[VECT_AX_OSC].actPosDINT;	

	///////////////

	

	switch(OSC_Step){

		case 0: // IDLE		 

		break;

		case 1:

			if(!CheckAxisOn(&AxisCh[TML_AX_OSC]))
				break;

			if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
				
				VectorStateMach[VECT_AX_OSC].InVal = 0;
				VectorStateMach[VECT_AX_OSC].Mode = vectSt_Mode_Vel;	

				OSCJog = 0;
				OSCToggle = 0;

				OSC_Step++;

			}
			

		break;
/*
		case 2:

			if(VectorStateMach[VECT_AX_OSC].FbkVect.Mode == vectSt_Mode_Vel
				&& !TML_MTR_INST[TML_AX_OSC].xCMD){
								
				OSC_Step++;
			}


		break;

		case 2:
			
			VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Vel;
			VectorStateMach[VECT_AX_AHC].InVal = 0.0;

			AHC_Move_Cmd = 0;
			
			AHC_Step++;
			

		break;
*/

		case 2:

		 	if(VectorStateMach[VECT_AX_OSC].FbkVect.Mode == vectSt_Mode_Vel) {				

				//if(!VectorStateMach[VECT_AX_TVL].xCMD 
					//&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){

				if(!TML_MTR_INST[TML_AX_OSC].xCMD){
					
					TML_MTR_INST[TML_AX_OSC].xCMD = TML_READ_MCR; //VECT_Read_MCR	
				
					//VectorStateMach[VECT_AX_TVL].xCMD = VECT_Read_MCR;
					TimerStart(&OSC_StatusReg_Tmr, StatusReg_Tm * 2.0);
					OSC_Step++;

				}

			}			

			else if(VectorStateMach[VECT_AX_OSC].FbkVect.Mode != vectSt_Mode_Vel)
				break;			
			

		break;	

		case 3:

			OSC_Step = 5;
			break;
			

			if((VectorStateMach[VECT_AX_OSC].MCRReg & 0x0700) == 0x0300)			
					OSC_Step = 5;

			else if(OSC_StatusReg_Tmr.Q)
				TVL_Start_Err = 20;				
			

		break;	
/*
		case 3:

			if(VectorStateMach[VECT_AX_OSC].StatusReg & 0x00008000){ // AXIS ON

				TimerStart(&OSC_Tmr, 0.5);

				if(MachCfg.TorchLastPos == 0)
					MachCfg.TorchLastPos = 1;
				
				OSC_Step = 5;
				
			}

		break;
*/
		case 5:

			if(!TimerIsTimeOut(&OSC_Tmr))
				break;

			if(MachCfg.TorchLastPos < 0){

				OSCToggle = 1;
				OSC_Step = 22;


			}
			else if(OSCToggle){				

				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CSPD] = OSC_JOG_TOGGLE_MAX * (permMem.Analog_Out[OSC_JogSpeedAO_Indx].Value/100.0);
				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CACC] = VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CSPD] * 5.0;
				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CPOS] = (MachCfg.TorchIdleDeg > 0)?  -90.0:90.0;
		
				VectorStateMach[VECT_AX_OSC].xCMD = VECT_Move_Rel_Cmd;

				TimerStart(&OSC_Tmr, 0.5);

				OSC_Step = 20;

			}
			//else if((SequenceRun_Out && MachCfg.OSCon && MoveEnable)
			else if((SequenceRun_OSC && MachCfg.OSCon && MoveEnable)
				|| OSCJog || KK_OSC_Test){

				if(OSCToggle){ // First untaggle torch
					OSCToggle = 0;
					break;

				}
				Calculate_MIG_OSC_Param(permMem.Analog_Out[OSC_AmplitAO_Indx].Value * OSC_Len2Deg);

				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_MIG_OSC_Amplit] = permMem.Analog_Out[OSC_AmplitAO_Indx].Value /2.0 * OSC_Len2Deg ;

				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_MIG_OSC_DwellIn] = permMem.Analog_Out[OSC_DwellInAO_Indx].Value * 1000.0;
				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_MIG_OSC_DwellOut] = permMem.Analog_Out[OSC_DwellOutAO_Indx].Value * 1000.0;

				VectorStateMach[VECT_AX_OSC].cmdWord = 1;

				VectorStateMach[VECT_AX_OSC].xCMD = VECT_Cmd_MIG_OSC;

				
				_OSC_Amplit = permMem.Analog_Out[OSC_AmplitAO_Indx].Value;
				_OSC_Freq  = permMem.Analog_Out[OSCSpeedAO_Indx].Value;
				_DwellIn = permMem.Analog_Out[OSC_DwellInAO_Indx].Value;
				_DwellOut = permMem.Analog_Out[OSC_DwellOutAO_Indx].Value;

				OSC_Step = 10;

			}			
			else if(OSCJog_Cmd != 0){

				VectorStateMach[VECT_AX_OSC].xCMD = VECT_Refresh_Mode;
				OSC_InVal = 0;
				OSC_Step ++;

			}
	

		break;

		case 6:

			if(VectorStateMach[VECT_AX_OSC].xCmd_busy)
				break;
			
			if(VectorStateMach[VECT_AX_OSC].FbkVect.Mode != vectSt_Mode_Vel)
				VectorStateMach[VECT_AX_OSC].Mode = vectSt_Mode_Vel;

			else{

				switch(OSCJog_Cmd){

					case 0:

						VectorStateMach[VECT_AX_OSC].InVal = 0.0;
						TimerStart(&OSC_Tmr, 0.05);	

						VectorStateMach[VECT_AX_OSC].InVal = 0.0;
		
						OSC_Jog_Ramp.x = OSC_Jog_Ramp.y_set;			
						OSC_Jog_Ramp.set = 1;
						
						OSC_Step = 5;	
							
					break;

					case 1:
						OSC_Jog_Ramp.x = 1.0;
						VectorStateMach[VECT_AX_OSC].InVal = OSC_Jog_Ramp.y * OSC_JOG_MAX * (permMem.Analog_Out[OSC_JogSpeedAO_Indx].Value/100.0);
					
					break;

					case -1:
						OSC_Jog_Ramp.x = 1.0;
						VectorStateMach[VECT_AX_OSC].InVal = -OSC_Jog_Ramp.y *  OSC_JOG_MAX * (permMem.Analog_Out[OSC_JogSpeedAO_Indx].Value/100.0);

					break;


				}

			}
		
		break;	
		

		case 10:

			//if(!((SequenceRun_Out &&  MachCfg.OSCon)|| OSCJog || KK_OSC_Test)){
			if(!((SequenceRun_OSC &&  MachCfg.OSCon)|| OSCJog || KK_OSC_Test)){	

				if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
					VectorStateMach[VECT_AX_OSC].cmdWord = 0;	
					VectorStateMach[VECT_AX_OSC].xCMD = VECT_Set_CmdWord;
					TimerStart(&OSC_Tmr, 1.0);	
					OSC_Step = 1;

				}
			}
			else if((_OSC_Amplit != permMem.Analog_Out[OSC_AmplitAO_Indx].Value)
				|| (_OSC_Freq != permMem.Analog_Out[OSCSpeedAO_Indx].Value)
				||(_DwellIn != permMem.Analog_Out[OSC_DwellInAO_Indx].Value)
				||(_DwellOut != permMem.Analog_Out[OSC_DwellOutAO_Indx].Value)
				){	

					if((VectorStateMach[VECT_AX_OSC].StatusReg & 0x02000400)){

						TimerStart(&OSC_Tmr, 0.2);
						//OSC_Step = 5;
						OSC_Step = 1;

					}

			}	
			
		break;

		//// Toggle
		case 20:

			if(TimerIsTimeOut(&OSC_Tmr)){
				//OSC_Step = 5;
				OSC_Step = 1;
			}
			else if(!(VectorStateMach[VECT_AX_OSC].StatusReg &0x02000400)){			
							
				OSC_Step ++;
			}
			
		break;

		case 21:
			
			if(VectorStateMach[VECT_AX_OSC].StatusReg & 0x02000400){

				MachCfg.TorchLastPos = -MachCfg.TorchLastPos;  
				OSC_Step ++;
			}
			

		break;	

		case 22:

			if(!OSCToggle){
			
				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CSPD] = OSC_JOG_TOGGLE_MAX * (permMem.Analog_Out[OSC_JogSpeedAO_Indx].Value/100.0);
				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CACC] = VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CSPD] * 5.0;
				VectorStateMach[VECT_AX_OSC].fParam[V_fParam_Rel_CPOS] = (MachCfg.TorchIdleDeg > 0)?  90.0:-90.0;

				VectorStateMach[VECT_AX_OSC].xCMD = VECT_Move_Rel_Cmd;

				OSC_Step ++;

			}

		break;	

		case 23:

			if(!(VectorStateMach[VECT_AX_OSC].StatusReg &0x02000400)){

				MachCfg.TorchLastPos = -MachCfg.TorchLastPos; 
				
				OSC_Step ++;
			}
			
		break;

		case 24:

			if(VectorStateMach[VECT_AX_OSC].StatusReg &0x02000400){

				TimerStart(&OSC_Tmr, 1.5);
				OSC_Step = 1;
			}

		break;	
		
		default:

			//OSC_Step = 0;

		break;

	}

	AzzRamp(&OSC_Jog_Ramp);
	TimerExecute(&OSC_Tmr);
	TimerExecute(&OSC_StatusReg_Tmr);
		
}




 
void INDX_Func(){ 

USINT vect_indx ,vect_ext_indx , tml_indx , tml_ext_indx ; 
	

	if(MachCfg.IndxExt){

		vect_indx = VECT_AX_INDX_EXT;
		tml_indx = TML_AX_INDX_EXT;	

		vect_ext_indx = VECT_AX_INDX;
		tml_ext_indx = TML_AX_INDX;

		
		if(IndexJog.IN != 0){	


			switch(IndexJog.IN){

				default:
					VectorStateMach[vect_ext_indx].InVal = 0.0;
				break;

				case  Forward:											
					VectorStateMach[vect_ext_indx].InVal = permMem.Analog_Out[Index_Jog_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX ;
				break;

				case  Reverse:						
					VectorStateMach[vect_ext_indx].InVal = -permMem.Analog_Out[Index_Jog_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX ;
				break;
			}
	
		}
		else
			VectorStateMach[vect_ext_indx].InVal = 0.0;	
	
	}
	else{	

		vect_indx = VECT_AX_INDX;
		tml_indx = TML_AX_INDX;

		vect_ext_indx = VECT_AX_INDX_EXT;
		tml_ext_indx = TML_AX_INDX_EXT;


		if(IndexExtJog.IN != 0){	


			switch(IndexExtJog.IN){

				default:
					VectorStateMach[vect_ext_indx].InVal = 0.0;
				break;

				case  Forward:											
					VectorStateMach[vect_ext_indx].InVal = permMem.Analog_Out[Index_Jog_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX ;
				break;

				case  Reverse:						
					VectorStateMach[vect_ext_indx].InVal = -permMem.Analog_Out[Index_Jog_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX ;
				break;
			}
	
		}
		else
			VectorStateMach[vect_ext_indx].InVal = 0.0;	

		//////////////////////////////////////
		//// Indexing with Limits switch

		if(!TML_MTR_INST[TML_AX_INDX_EXT].DigInp[2] &&  _INDX_EXT_DigInp2){
			
				if(MachCfg.Indxon){ // Indexing 
	
					if(MachCfg.TrigStep_INDX_TVL == 0){
						
						if((INDX_Step_Cmd == 0)
							&& (INDX_Step == 10)){
								
							if(MachCfg.IndxDir > 0)
								INDX_Step_Cmd = 1;
							else
								INDX_Step_Cmd = -1;

						}
							
						
					}else {

						if((TVL_Step_Cmd == 0)
							&& (TVL_Step == 10)){
								
							if(MachCfg.IndxDir > 0)
								TVL_Step_Cmd = 1;
							else
								TVL_Step_Cmd = -1;

						}
					}
				}
		}


		_INDX_EXT_DigInp2 = TML_MTR_INST[TML_AX_INDX_EXT].DigInp[2];
		//////////////////////////////////////////

			
	}

	
	
	if(INDX_SetHome_Btn){
		INDX_SetHome_Btn = 0;
		INDX_Home_Pos = TML_MTR_INST[tml_indx].PosFBK;
		
	}
	INDX_Act_Pos = TML_MTR_INST[tml_indx].PosFBK - INDX_Home_Pos;

	if(WatchIndx)
		INDX_Last_Pos = INDX_Act_Pos;


	////////////////////////////////////////////////
	
	//////////////////////////
	if(IndexJog.IN != Stop)
		INDX_Jog_Ramp.x = 1.0;
		
	else if(INDX_Jog_Ramp.y != 0.0){

		INDX_Jog_Ramp.x = 0.0;
		INDX_Jog_Ramp.y_set = 0.0;
		INDX_Jog_Ramp.set = 1;

	}
	AzzRamp(&INDX_Jog_Ramp);
	INDX_Jog_Ramp.set = 0;
	//////////////////////////

	switch(INDX_Step){

		case 0: // IDLE		 

		break;

		case 1:

			
			IndexExtJog.IN = Stop;			

			if(MachCfg.IndxExt){
				IND_Repeat_Init = 1;
				
				TimerStart(&INDX_Tmr, 0.1);	 // To jest wazne aby nie krocej
				//TML_MTR_INST[tml_indx].xCMD = TML_RESTART;

				INDX_Step_Cmd = 0;

				INDX_Step++;

				break;

			}

			else if(!CheckAxisOn(&AxisCh[tml_indx])) // Nie wiem czemu to nie dziala przy ext a dzial przy int
				break;

			///////////////////////////////////////////
			if(!MachCfg.IndxExt){ // For Limit switch Index Trigger
			
				if(!TML_MTR_INST[TML_AX_INDX_EXT].xCMD)
					TML_MTR_INST[TML_AX_INDX_EXT].xCMD = TML_SET_OUT3_CMD;
				else
					break;

			}	
			////////////////////////////////////////////////////////////

			TimerStart(&INDX_Tmr, 0.01);	
			
			INDX_Step_Cmd = 0;

			INDX_Step++;

			
			
		break;

		case 2:	


			if(!TimerIsTimeOut(&INDX_Tmr))
				break;


			if(IND_Repeat_Init){
				IND_Repeat_Init = 0;				
				TML_MTR_INST[tml_indx].xCMD = TML_RESTART;

				
			}
			
			
			INDX_Step++;
				
			
		break;

		case 3:

			if(!TML_MTR_INST[tml_indx]._int.Init)
				break;

			

			if(VectorStateMach[vect_indx].FbkVect.Mode  == vectSt_Mode_Hold){ //
				
				VectorStateMach[vect_indx].Mode = vectSt_Mode_Vel;
				VectorStateMach[vect_indx].InVal = 0.0;
				INDX_Step_Cmd = 0;			
				//INDX_Step++;
				if((IndexJog.IN == Stop))						
					INDX_Step = 10;	

			}
			else
				VectorStateMach[vect_indx].Mode = vectSt_Mode_Hold;

			
			

		break;


		case 10:

	
			if(VectorStateMach[vect_indx].xCmd_busy)
				break;

			

			if(IndexJog.IN != 0){
			
				if(VectorStateMach[vect_indx].FbkVect.Mode != vectSt_Mode_Vel){
					VectorStateMach[vect_indx].Mode = vectSt_Mode_Vel;
					VectorStateMach[vect_indx].InVal = 0.0;
				}
				else {

					switch(IndexJog.IN){

						default:
							VectorStateMach[vect_indx].InVal = 0.0;
						break;

						case  Forward:	

							if((TML_MTR_INST[tml_indx].ErrorReg &0x0040)){
								INDX_Step = 20;
								break;
							}							
							VectorStateMach[vect_indx].InVal = permMem.Analog_Out[Index_Jog_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX * INDX_Jog_Ramp.y;
						break;

						case  Reverse:	
							if((TML_MTR_INST[tml_indx].ErrorReg &0x0080)){
								INDX_Step = 20;
								break;
							}
							VectorStateMach[vect_indx].InVal = -permMem.Analog_Out[Index_Jog_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX * INDX_Jog_Ramp.y;
						break;
					}

				}

			}
			
			else if((INDX_Step_Cmd != 0)){
				
				
				if(VectorStateMach[vect_indx].FbkVect.Mode  != vectSt_Mode_Hold)
					VectorStateMach[vect_indx].Mode = vectSt_Mode_Hold;
				
				else {				 
				
					VectorStateMach[vect_indx].fParam[V_fParam_Rel_CSPD] = permMem.Analog_Out[Index_Step_Speed_Indx].Value / 100.0  *  INDX_JOG_MAX;
					VectorStateMach[vect_indx].fParam[V_fParam_Rel_CACC] = VectorStateMach[vect_indx].fParam[V_fParam_Rel_CSPD] * 10.0;

					//if((INDX_Step_Cmd > 0) && !(TML_MTR_INST[tml_indx].ErrorReg &0x0040)){
					if((TravelSpeed.IN == Forward) && !(TML_MTR_INST[tml_indx].ErrorReg &0x0040)){

						if(Seq.State == 0)							
							VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_H_Indx].Value;
						else	
							VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_L_Indx].Value ;
						
						if(INDX_Step_Cmd < 0)
							VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS] = -VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS];

						if(fabsf(VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS]) < 0.01)
							INDX_Step_Cmd = 0;
						
						else{
							
							VectorStateMach[vect_indx].xCMD = VECT_Move_Rel_Cmd;						
							INDX_Step = 15;	
						}
					}
					//else if((INDX_Step_Cmd < 0) && !(TML_MTR_INST[tml_indx].ErrorReg &0x0080)){
					else if((TravelSpeed.IN == Reverse) && !(TML_MTR_INST[tml_indx].ErrorReg &0x0080)){

						if(Seq.State == 0)
							VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_L_Indx].Value ;
						else	
							VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS] = permMem.Analog_Out[Index_Dist_H_Indx].Value;

						if(INDX_Step_Cmd < 0)
							VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS] = -VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS];
						
						if(fabsf(VectorStateMach[vect_indx].fParam[V_fParam_Rel_CPOS]) < 0.01)
							INDX_Step_Cmd = 0;
						
						else{
							
							VectorStateMach[vect_indx].xCMD = VECT_Move_Rel_Cmd;						
							INDX_Step = 15;	
						}
					}
					else
						INDX_Step_Cmd = 0;
					 
				}
			}
			else {
				
				if(VectorStateMach[vect_indx].FbkVect.Mode != vectSt_Mode_Vel)
					VectorStateMach[vect_indx].Mode = vectSt_Mode_Vel;

				VectorStateMach[vect_indx].InVal = 0.0;

			}			



		break;


		case 15:

			if((INDX_Step_Cmd > 0) && (TML_MTR_INST[tml_indx].ErrorReg &0x0040)){
				TimerStart(&INDX_Tmr, 0.5);
				INDX_Step = 1;

			}
			else if((INDX_Step_Cmd < 0) && (TML_MTR_INST[tml_indx].ErrorReg &0x0080)){
				TimerStart(&INDX_Tmr, 0.5);
				INDX_Step = 1;
			}	
			
			else if(!(VectorStateMach[vect_indx].StatusReg &0x02000400)){

				INDX_Step ++;
			}
			
		break;

		case 16:			

			if((INDX_Step_Cmd > 0) && (TML_MTR_INST[tml_indx].ErrorReg &0x0040)){
				TimerStart(&INDX_Tmr, 0.5);
				INDX_Step = 2;

			}
			else if((INDX_Step_Cmd < 0) && (TML_MTR_INST[tml_indx].ErrorReg &0x0080)){
				TimerStart(&INDX_Tmr, 0.5);
				INDX_Step = 2;
			}	
			
			else if(INDX_Step_Cmd == 0){

				INDX_Step = 2;
			}
			else if(VectorStateMach[vect_indx].StatusReg &0x02000400){

				INDX_Step_Cmd = 0;
				INDX_Step = 2;
			}
			
			
		break;


		case 20:

			if(IndexJog.IN == Stop){
				TimerStart(&INDX_Tmr, 0.01);
				INDX_Step_Cmd = 0;
				INDX_Step = 2;

			}

		break;


		case 100:
			
			if(VectorStateMach[vect_indx].FbkVect.Mode != vectSt_Mode_Hold)
				VectorStateMach[vect_indx].Mode = vectSt_Mode_Hold;

			else if(!TML_MTR_INST[TML_AX_INDX].xCMD){
				TML_MTR_INST[TML_AX_INDX].xCMD = TML_AXIS_OFF_CMD;
				INDX_Step++;
			}	


		break;

		case 101:

			if(!(TML_MTR_INST[TML_AX_INDX].StatusReg & 0x8000))
				INDX_Step = 200;

		break;

		
		default:

			//INDX_Step = 0;

		break;

	}

	TimerExecute(&INDX_Tmr);

}
void AHC_Func(){

	
	// Store last Position
	if(TML_MTR_INST[TML_AX_AHC]._int.Init &&
	 (TML_MTR_INST[TML_AX_AHC]._cfg.absFbk != 2))
		OLD_MTR_POS[TML_AX_AHC] = VectorStateMach[VECT_AX_AHC].actPosDINT;	

	if(AHC_SetHome_Btn){
		AHC_SetHome_Btn = 0;

		if(!(MoveEnable || Seq.State)
			|| (MachCfg.WeldMode == 0)
			|| MachCfg.GougingON){
			AHC_MaxPos = -3.40282e+038;
			AHC_StartPos = -3.40282e+038;
			AxisCh[TML_AX_AHC]._lastPos -= AxisCh[TML_AX_AHC].curPos; // This make current zero
		}

	}

	if(!MachCfg.AHC_BackStep){
		AHC_MaxPos = -3.40282e+038;
		AHC_StartPos = -3.40282e+038;
	}

	/////////////////////////////////////////////////////////////
	if(ObserveAHC_MaxPos){

		if(!_observeAHC_MaxPos){			
			AHC_MaxPos = -3.40282e+038;
			//AHC_StartPos = TML_MTR_INST[TML_AX_AHC].PosFBK;
		}
		

		if(TML_MTR_INST[TML_AX_AHC].PosFBK > AHC_MaxPos)
			AHC_MaxPos = TML_MTR_INST[TML_AX_AHC].PosFBK;	

	} 
	_observeAHC_MaxPos = ObserveAHC_MaxPos;	

	AHC_Scaled_Pos = AHC_Scaled_Fact * TML_MTR_INST[TML_AX_AHC].PosFBK;
	AHC_Scaled_MaxPos =  AHC_MaxPos * AHC_Scaled_Fact;
	AHC_Scaled_StartPos =  AHC_StartPos * AHC_Scaled_Fact;

	///////////////////////////////////////////////////////////////
	
	//////////////////////////////////////
	if(AHC_Mode < 2){ // Legacy && PLC with window
	
		AHC_Margin = permMem.Analog_Out[AHCToleranceAO_Indx].Value;

		AHC_SetPoint = permMem.Analog_Out[AHCCurrentSPAO_Indx].Value;
	}
	else{

		AHC_SetPoint = permMem.Analog_Out[AHCCurrentSPAO_Indx].Value;
	}

	
	AHC_Error = ActCurrrentFbk - AHC_SetPoint;	
	HMI_AHC_Error = AHC_Error;	

	AHC_Volt_Error = -permMem.Analog_Out[ArcVoltsAO_Indx].Value;
	HMI_AHC_Volt_Error = AHC_Volt_Error;
	/////////////////////////////////////

	if(AHC_PID.enable){
			
		if(AHC_PID.request == 3 || AHC_PID.request == 4)	// Read and Write Parameters request
			AHC_PID.request = 0;	
	}


	switch(AHC_Step){


		case 0: // IDLE		 

		break;

		case 1:

			if(!TimerIsTimeOut(&AHC_Tmr))
				break;

			if(!CheckAxisOn(&AxisCh[TML_AX_AHC]))
				break;
			

			AHCJog.IN = Stop;

			AHC_Step++;		

		break;

		case 2:

			if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){
			
				VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Vel;
				VectorStateMach[VECT_AX_AHC].InVal = 0.0;

				AHC_Move_Cmd = 0;
				
				AHC_Step++;

			}
			

		break;


		case 3:

		 	if(VectorStateMach[VECT_AX_AHC].FbkVect.Mode == vectSt_Mode_Vel) {				

				//if(!VectorStateMach[VECT_AX_TVL].xCMD 
					//&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){

				if(!TML_MTR_INST[TML_AX_AHC].xCMD){
					
					TML_MTR_INST[TML_AX_AHC].xCMD = TML_READ_MCR; //VECT_Read_MCR	
				
					//VectorStateMach[VECT_AX_TVL].xCMD = VECT_Read_MCR;
					TimerStart(&AHC_StatusReg_Tmr, StatusReg_Tm * 2.0);
					AHC_Step++;

				}

			}			

			else if(VectorStateMach[VECT_AX_AHC].FbkVect.Mode != vectSt_Mode_Vel)
				break;			
			

		break;	

		
		case 4:

			if((VectorStateMach[VECT_AX_AHC].MCRReg & 0x0700) == 0x0300)			
					AHC_Step ++;

			else if(AHC_StatusReg_Tmr.Q)
				TVL_Start_Err = 10;				
			

		break;	

		

		case 5:

			if(VectorStateMach[VECT_AX_AHC].FbkVect.Mode  == vectSt_Mode_Vel){

				if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){

				
					VectorStateMach[VECT_AX_AHC].fParam[V_fParam_Cmd_CACC] = Calculate_Acc( 850.0 , 0.1);						
				
					VectorStateMach[VECT_AX_AHC].xCMD = VECT_Set_CACC;
					
					AHC_Step ++;

				}
			}

		break;


		case 6:

				if(VectorStateMach[VECT_AX_AHC].xCmd_busy)
					break;

				if(AHC_Mode == 3){	
							
					TML_MTR_INST[TML_AX_AHC]._cfg.kpp = permMem.Analog_Out[AHC_Response_Indx].Value/100.0 * TML_MTR_INST[TML_AX_AHC]._cfg.kppMax;
					TML_MTR_INST[TML_AX_AHC]._cfg.kip = permMem.Analog_Out[AHC_Integrator_Indx].Value/100.0 * TML_MTR_INST[TML_AX_AHC]._cfg.kipMax;

					VectorStateMach[VECT_AX_AHC].xCMD = VECT_Set_PID_Param;
					

				}else{

					AHC_PIDParameters.Kp = permMem.Analog_Out[AHC_Response_Indx].Value/100.0 * AHC_Gain_PLC_MAX;
					AHC_PIDParameters.Tn = permMem.Analog_Out[AHC_Integrator_Indx].Value/100.0 * AHC_Integr_PLC_MAX;
					AHC_PID.request = 3 ; // Read parameters from PIDParameters 

				}				
				

				_AHC_Response = permMem.Analog_Out[AHC_Response_Indx].Value;
				_AHC_Integrator = permMem.Analog_Out[AHC_Integrator_Indx].Value;
						

				AHC_PID.enable = 1;
				AHC_Step = 10;

		break;


		case 10:

			
			if(VectorStateMach[VECT_AX_AHC].FbkVect.Mode != vectSt_Mode_Vel){
				//VectorStateMach[VECT_AX_AHC].InVal = 0.0;
				VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Vel;
				break;

			}

			if(AHC_Move_Cmd){				

				AHC_Step = 50;		
						 
			}
			else if((SequenceRun_Out && MachCfg.AHC_Auto && MoveEnable && !SKIP_Weld_Cmd && !MachCfg.GougingON) 
				|| KK_Run_AHC_PID){		


				if(MachCfg.Weld || KK_Run_AHC_PID){ // Weld Mode
									
					if(AHCJog.IN){
						AHC_Integr = 0.0;
						AHC_PID.enable = 0;				
						
						switch(AHCJog.IN){ 

							default:
								VectorStateMach[VECT_AX_AHC].InVal = 0.0;
							break;

							case  Forward:		
								VectorStateMach[VECT_AX_AHC].InVal = permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100.0 * AHC_JOG_MAX;
							break;

							case  Reverse:		
								VectorStateMach[VECT_AX_AHC].InVal = -permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100 * AHC_JOG_MAX;
							break;
						}

					}
					//else if((ArcLink.robot.Cntrl.Fbk.Status & Arc_Active) || KK_Run_AHC_PID){
					else if((Seq.State == 11) || KK_Run_AHC_PID){	

						if(ArcLink.robot.Cntrl.Fbk.Status & (Arc_Active))
							VectorStateMach[VECT_AX_AHC].InVal = AHC_Cntrl(AHC_Mode);	
						
						else {
							VectorStateMach[VECT_AX_AHC].InVal = 0.0;
							AHC_Integr = 0.0;
							AHC_PID.enable = 0;	
						}

					}
					
					
				}
				else { // Test Mode


					
					if(AHCJog.IN){
						AHC_Integr = 0.0;
						AHC_PID.enable = 0;				
						
						switch(AHCJog.IN){ 

							default:
								VectorStateMach[VECT_AX_AHC].InVal = 0.0;
							break;

							case  Forward:		
								VectorStateMach[VECT_AX_AHC].InVal = permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100.0 * AHC_JOG_MAX;
							break;

							case  Reverse:		
								VectorStateMach[VECT_AX_AHC].InVal = -permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100 * AHC_JOG_MAX;
							break;
						}

					}
					else if(MachCfg.AHC_Auto && (!MachCfg.Weld) && (!MachCfg.GougingON)){ // Test Mode

						AHC_Test_Dir = !AHC_Test_Dir;
						AHC_Step = 40;
					}
						
				}			

				
			}			
			else{
				
				AHC_Integr = 0.0;
				AHC_PID.enable = 0;		

				if(JOG_AHC_IN)
					VectorStateMach[VECT_AX_AHC].InVal = permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100.0 * AHC_JOG_MAX;
				
				else if (JOG_AHC_OUT)
					VectorStateMach[VECT_AX_AHC].InVal = -permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100 * AHC_JOG_MAX;
				
				else {
				
					switch(AHCJog.IN){ 

						default:
							VectorStateMach[VECT_AX_AHC].InVal = 0.0;
						break;

						case  Forward:		
							VectorStateMach[VECT_AX_AHC].InVal = permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100.0 * AHC_JOG_MAX;
						break;

						case  Reverse:		
							VectorStateMach[VECT_AX_AHC].InVal = -permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100 * AHC_JOG_MAX;
						break;
					}

				}

			}


		break;


		case 20:		

			VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Pos;
			VectorStateMach[VECT_AX_AHC].InVal = TML_MTR_INST[TML_AX_AHC].AnInp[1]/ TML_MTR_INST[TML_AX_AHC]._int.kPos;

			AHC_Step++;

		break;

		case 21:
	
			if(VectorStateMach[VECT_AX_AHC].FbkVect.Mode == vectSt_Mode_Pos){

				AHC_Ramp.y_set = TML_MTR_INST[TML_AX_AHC].AnInp[1] * ActCurrrentFbk_Calib[0];

				AHC_Ramp.x = permMem.Analog_Out[AHCCurrentSPAO_Indx].Value;
				AHC_Ramp.set = 1;
				AzzRamp(&AHC_Ramp);
				AHC_Ramp.set = 0;

				AHC_PID.enable = 1;
				
				AHC_Step++;

			}				

		break;

		case 22:

			if((_AHC_Response != permMem.Analog_Out[AHC_Response_Indx].Value)
				|| (_AHC_Integrator != permMem.Analog_Out[AHC_Integrator_Indx].Value)){				

				if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){

					TML_MTR_INST[TML_AX_AHC]._cfg.kpp = permMem.Analog_Out[AHC_Response_Indx].Value/100.0 * TML_MTR_INST[TML_AX_AHC]._cfg.kppMax;
					TML_MTR_INST[TML_AX_AHC]._cfg.kip = permMem.Analog_Out[AHC_Integrator_Indx].Value/100.0 * TML_MTR_INST[TML_AX_AHC]._cfg.kipMax;

					_AHC_Response = permMem.Analog_Out[AHC_Response_Indx].Value;
					_AHC_Integrator = permMem.Analog_Out[AHC_Integrator_Indx].Value;
						
					VectorStateMach[VECT_AX_AHC].xCMD = VECT_Set_PID_Param;
					break;

				}

			}			

			if(SequenceRun_Out && MachCfg.Weld && MachCfg.AHC_Auto && !AHCJog.IN){					
				VectorStateMach[VECT_AX_AHC].InVal = AHC_Ramp.y / ActCurrrentFbk_Calib[0] / TML_MTR_INST[TML_AX_AHC]._int.kPos;
			}
			else{

				VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Idle;
				AHC_Step ++;

			}

		break;

		case 23:

			if(VectorStateMach[VECT_AX_AHC].FbkVect.Mode == vectSt_Mode_Idle)
				AHC_Step = 10;

		break;

		///////////////////////////////////////////////////////////////////

		
		case 40:

			

			if(AHC_Test_Dir)
				VectorStateMach[VECT_AX_AHC].InVal = -0.2 * AHC_JOG_MAX;
			else
				VectorStateMach[VECT_AX_AHC].InVal = 0.2 * AHC_JOG_MAX;

			TimerStart(&AHC_Tmr , 0.2);

			AHC_Step++;

		break;


		case 41:

			if(!(SequenceRun_Out && MachCfg.AHC_Auto && MoveEnable)
				|| (AHCJog.IN != Stop)){	

				VectorStateMach[VECT_AX_AHC].InVal = 0.0;
				AHC_Step = 10;

			}
			else if(TimerIsTimeOut(&AHC_Tmr)){

				VectorStateMach[VECT_AX_AHC].InVal = 0.0;

				TimerStart(&AHC_Tmr , 1.0);

				AHC_Step++;
			}
			
		break;

		
		case 42:
		
			if(!(SequenceRun_Out && MachCfg.AHC_Auto && MoveEnable)
				|| (AHCJog.IN != Stop)){	
				
				VectorStateMach[VECT_AX_AHC].InVal = 0.0;
				AHC_Step = 10;

			}
			else if(TimerIsTimeOut(&AHC_Tmr)){
				AHC_Test_Dir = !AHC_Test_Dir;
				AHC_Step = 40;
			}
			
		break;

		////////////////////////////////////////////////////////

		case 50:
			
			if(VectorStateMach[VECT_AX_AHC].FbkVect.Mode != vectSt_Mode_Drive_Independant){
				VectorStateMach[VECT_AX_AHC].InVal = 0.0;
				VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Drive_Independant;
				
			}else
				AHC_Step ++;

		break;				

		case 51:			
			
			VectorStateMach[VECT_AX_AHC].fParam[V_fParam_Rel_CSPD] = permMem.Analog_Out[AHC_JogSpeedAO_Indx].Value/100.0 * AHC_JOG_MAX;
			VectorStateMach[VECT_AX_AHC].fParam[V_fParam_Rel_CACC] = VectorStateMach[VECT_AX_AHC].fParam[V_fParam_Rel_CSPD] * 5.0;
			VectorStateMach[VECT_AX_AHC].Param[0] = (DINT)(AHC_Move_Target * TML_MTR_INST[TML_AX_AHC]._int.kPos);
			VectorStateMach[VECT_AX_AHC].xCMD = VECT_Move_Abs_Cmd;
			AHC_Step ++;

		break;

		case 52:

			if(((TML_MTR_INST[TML_AX_AHC].PosFBK < AHC_Move_Target) &&   !TML_MTR_INST[TML_AX_AHC].DigInp[0])
				&&   ((TML_MTR_INST[TML_AX_AHC].PosFBK > AHC_Move_Target) &&   !TML_MTR_INST[TML_AX_AHC].DigInp[1]))				
				AHC_Step = 100;

			else if(!(VectorStateMach[VECT_AX_AHC].StatusReg &0x02000400))				
				AHC_Step ++;


		break;

		case 53:
			
			if(((TML_MTR_INST[TML_AX_AHC].PosFBK < AHC_Move_Target) &&   !TML_MTR_INST[TML_AX_AHC].DigInp[0])
				&&   ((TML_MTR_INST[TML_AX_AHC].PosFBK > AHC_Move_Target) &&   !TML_MTR_INST[TML_AX_AHC].DigInp[1]))				
				AHC_Step = 100;
			
			else if((VectorStateMach[VECT_AX_AHC].StatusReg &0x02000400))				
				AHC_Step = 2;


		break;

		case 100:

			VectorStateMach[VECT_AX_AHC].cmdWord = 1;	
			VectorStateMach[VECT_AX_AHC].xCMD = VECT_Set_CmdWord;
			AHC_Step ++;

		break;

		case 101:

			if(VectorStateMach[VECT_AX_AHC].StatusReg &0x02000400)				
				AHC_Step = 2;			
			
		break;
		
		default:  
			//AHC_Step = 0;

		break;

	}

	#ifndef OLD_AHC
	// New Assembly - motor direction change
	VectorStateMach[VECT_AX_AHC].InVal = -VectorStateMach[VECT_AX_AHC].InVal;
	#endif

	TimerExecute(&AHC_Tmr);
	TimerExecute(&AHC_StatusReg_Tmr);
				

		
}
void WF_Func_Volt(){	

	switch(WF_Step){


		case 0: // IDLE		 

		break;
	
		case 1:

			WF_InVal = 0.0;

			if(!TimerIsTimeOut(&WF_Tmr))
				break;				

			TimerStart(&WF_DT_Tmr, 0.1);

			WF_Step++;

			

		break;

		case 2:

						
			VectorStateMach[VECT_AX_WF].Mode = vectSt_Mode_Volt;
			VectorStateMach[VECT_AX_WF].InVal = 0.0;
			WF_Step++;	


		break;

		case 3:

			if(VectorStateMach[VECT_AX_WF].FbkVect.Mode  == vectSt_Mode_Volt){					

					WF_InVal = 0.0;
					WF_Step = 10;
			}

		break;


		case 10:


			if(WF_Calibr_Speed != 0.0){						
			
				WF_Step = 20;

				break;

			}

			
			if(VectorStateMach[VECT_AX_WF].FbkVect.Mode != vectSt_Mode_Volt){

				WF_Step = 0;
				break;

			}
			
			if(SequenceRun_Out && MoveEnable && !SKIP_Weld_Cmd){
		
				switch(TravelSpeed.IN){

					default:
						VectorStateMach[VECT_AX_WF].InVal = 0.0;
					break;

					case  Forward:		
					case  Reverse:		
						WF_InVal = getFromCalib(permMem.Analog_Out[WireFeedSpeedAO_Indx].Value , &CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS) * KK_WF_Run_Scale ;
									
					break;
				}

			}
			else {
				
				switch(WFJog.IN){

					case Forward:						

						WF_InVal = permMem.Analog_Out[WF_Jog_Speed_Indx].Value/100.0 * 36.0 ;  

					break;
					
					case Reverse:
						
						WF_InVal = -permMem.Analog_Out[WF_Jog_Speed_Indx].Value/100.0 * 36.0 ;  

					break;

					default:

						WF_InVal = 0.0;

					break;
				}
					 

			}
	
		break;


		
		case 20:	

			WF_InVal = WF_Calibr_Speed * KK_WF_Run_Scale;

			if(WF_Calibr_Speed == 0.0)				
				WF_Step = 10;			

		break;


		default:

			//WF_Step = 0;

		break;

	}

	WF_Volt_Ramp.x = WF_InVal;
	AzzRamp(&WF_Volt_Ramp);
	VectorStateMach[VECT_AX_WF].InVal = WF_Volt_Ramp.y;

	TimerExecute(&WF_Tmr);
	TimerExecute(&WF_DT_Tmr);

}



void WF_Func(){


	switch(WF_Step){


		case 0: // IDLE		 

		break;
	
		case 1:

			if(!CheckAxisOn(&AxisCh[TML_AX_WF]))
				break;

			TimerStart(&WF_DT_Tmr, 0.1);

			WF_Step++;

			

		break;

		case 2:

			if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
				
				VectorStateMach[VECT_AX_WF].Mode = vectSt_Mode_Vel;
				VectorStateMach[VECT_AX_WF].InVal = 0.0;
				WF_Step++;
				
			}
			


		break;


		case 3:

		 	if(VectorStateMach[VECT_AX_WF].FbkVect.Mode == vectSt_Mode_Vel) {				

				//if(!VectorStateMach[VECT_AX_TVL].xCMD 
					//&& !VectorStateMach[VECT_AX_TVL].xCmd_busy){

				if(!TML_MTR_INST[TML_AX_WF].xCMD){
					
					TML_MTR_INST[TML_AX_WF].xCMD = TML_READ_MCR; //VECT_Read_MCR	
				
					//VectorStateMach[VECT_AX_TVL].xCMD = VECT_Read_MCR;
					TimerStart(&WF_StatusReg_Tmr, StatusReg_Tm * 2.0);
					WF_Step++;

				}

			}			

			else if(VectorStateMach[VECT_AX_WF].FbkVect.Mode != vectSt_Mode_Vel)
				break;			
			

		break;	

		
		case 4:

			if((VectorStateMach[VECT_AX_WF].MCRReg & 0x0700) == 0x0300)			
					WF_Step ++;

			else if(WF_StatusReg_Tmr.Q)
				TVL_Start_Err = 15;				
			

		break;	

		case 5:

			if(VectorStateMach[VECT_AX_WF].FbkVect.Mode  == vectSt_Mode_Vel){

				if(!VectorStateMach[VECT_AX_WF].xCmd_busy){

					//
					//VectorStateMach[VECT_AX_WF].fParam[V_fParam_Cmd_CACC] = getFromCalib(Calculate_Acc(permMem.Analog_Out[WireFeedSpeedAO_Indx].Value/60.0 , 0.2) , &CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);

					VectorStateMach[VECT_AX_WF].fParam[V_fParam_Cmd_CACC] = getFromCalib(Calculate_Acc(750/60.0 , permMem.Analog_Out[WF_Ramp].Value) , &CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);

					 _WF_Ramp = permMem.Analog_Out[WF_Ramp].Value;						
					 _WF_Vel = permMem.Analog_Out[WireFeedSpeedAO_Indx].Value;

					
					
					VectorStateMach[VECT_AX_WF].xCMD = VECT_Set_CACC;

					WF_Calibr_Speed = 0.0;

					KK_StartWF = 0;

					//_sequenceRun_WF = ((SequenceRun_WF && MoveEnable && !SKIP_Weld_Cmd) || KK_StartWF);
					_sequenceRun_WF = 0;
					WF_Step = 10;

				}
			}

		break;


		case 10:
			
			if(VectorStateMach[VECT_AX_WF].FbkVect.Mode != vectSt_Mode_Vel){

				WF_Step = 0;
				break;

			}
			

			if(VectorStateMach[VECT_AX_WF].xCmd_busy)
				break;


			if(_WF_Ramp != permMem.Analog_Out[WF_Ramp].Value){

				WF_Step = 3;
				break;

			}

			
			if(WF_Calibr_Speed != 0){				

				VectorStateMach[VECT_AX_WF].InVal = WF_Calibr_Speed;
			
				WF_Step = 20;

				break;

			}			
			
			if((SequenceRun_WF && MoveEnable && !SKIP_Weld_Cmd)
				|| KK_StartWF){				
		
				switch(	Seq.State){

					default:						
						WF_Run_Ramp.x = 0.0;
						//VectorStateMach[VECT_AX_WF].InVal = WF_InVal * WF_Run_Ramp.y;
						VectorStateMach[VECT_AX_WF].InVal = 0.0;
					break;

					case 2:
					case 3:

						WF_Run_Ramp.x = WF_Run_Ramp.y_max;
								
						VectorStateMach[VECT_AX_WF].InVal = getFromCalib(permMem.Analog_Out[WF_SlowSpeed].Value /60.0 , \
							&CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);					
						WF_InVal = 0;

						/*
						if(MachCfg.Weld  && ArcLink.robot.Cntrl.Cmd.RUN){

							if(ArcLink.robot.Cntrl.Fbk.Status & (Arc_Short ))
								VectorStateMach[VECT_AX_WF].InVal = 0;

						}
						*/

					break;

					
					case 11:	
						
						WF_Run_Ramp.x = WF_Run_Ramp.y_max;
						
						VectorStateMach[VECT_AX_WF].InVal = getFromCalib(permMem.Analog_Out[WireFeedSpeedAO_Indx].Value /60.0 , \
							&CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);					
						WF_InVal = 0;

						/*
						if(MachCfg.Weld  && ArcLink.robot.Cntrl.Cmd.RUN){

							if(ArcLink.robot.Cntrl.Fbk.Status & (Arc_Short ))
								VectorStateMach[VECT_AX_WF].InVal = 0;

						}
						*/
						
					break;					
						
				}				

				if(!_sequenceRun_WF){						
					TimerStart(&WF_DT_Tmr, permMem.Analog_Out[WF_Ramp].Value * 1.1);
					_sequenceRun_WF = 1;
					WF_Step = 11;

				}

			}
			else {

				if(_sequenceRun_WF){
					WF_Step = 3;
					break;

				}	
			
				switch(WFJog.IN){

					case Forward:

						if(permMem.Analog_Out[WF_Ramp].Value > 0.0 )
							WF_Run_Ramp.dy_up = 0.5 /permMem.Analog_Out[WF_Ramp].Value;
						else
							WF_Run_Ramp.dy_up = 1.0;

						WF_Run_Ramp.x = WF_Run_Ramp.y_max;
						VectorStateMach[VECT_AX_WF].InVal = permMem.Analog_Out[WF_Jog_Speed_Indx].Value/100.0 * WF_JOG_MAX / 60.0 * WF_Run_Ramp.y;  
						WF_InVal = VectorStateMach[VECT_AX_WF].InVal;
						
					break;
					
					case Reverse:	

						if(permMem.Analog_Out[WF_Ramp].Value > 0.0 )
							WF_Run_Ramp.dy_up = 0.5 /permMem.Analog_Out[WF_Ramp].Value;
						else
							WF_Run_Ramp.dy_up = 1.0;
						
						WF_Run_Ramp.x = WF_Run_Ramp.y_max;
						VectorStateMach[VECT_AX_WF].InVal = -permMem.Analog_Out[WF_Jog_Speed_Indx].Value/100.0 * WF_JOG_MAX / 60.0 * WF_Run_Ramp.y;  
						WF_InVal = VectorStateMach[VECT_AX_WF].InVal;
					break;

					default:

						WF_Run_Ramp.x = 0.0;
						VectorStateMach[VECT_AX_WF].InVal = 0.0;

					break;
				}
					 

			}
	
		break;


		case 11:
			
			switch( Seq.State){

				default:						
					WF_Run_Ramp.x = 0.0;
					//VectorStateMach[VECT_AX_WF].InVal = WF_InVal * WF_Run_Ramp.y;
					VectorStateMach[VECT_AX_WF].InVal = 0.0;
				break;

				case 2:
				case 3:

					WF_Run_Ramp.x = WF_Run_Ramp.y_max;
							
					VectorStateMach[VECT_AX_WF].InVal = getFromCalib(permMem.Analog_Out[WF_SlowSpeed].Value /60.0 , \
						&CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);
					WF_InVal = 0;

				break;

				
				case 11:	
					
					WF_Run_Ramp.x = WF_Run_Ramp.y_max;
					
					VectorStateMach[VECT_AX_WF].InVal = getFromCalib(permMem.Analog_Out[WireFeedSpeedAO_Indx].Value /60.0 , \
						&CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);
					WF_InVal = 0;
					
				break;					
					
			}

			if(KK_StartWF){

				VectorStateMach[VECT_AX_WF].InVal = getFromCalib(permMem.Analog_Out[WireFeedSpeedAO_Indx].Value /60.0 , \
					&CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);
				WF_InVal = 0;

			}

			if(TimerIsTimeOut(&WF_DT_Tmr)){

				WF_DT_Tmr.IN = 0;
				
				VectorStateMach[VECT_AX_WF].fParam[V_fParam_Cmd_CACC] = getFromCalib(Calculate_Acc(750/60.0 , WF_Stop_Ramp) , &CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);
				VectorStateMach[VECT_AX_WF].xCMD = VECT_Set_CACC;			

			}

			else if(!((SequenceRun_WF && MoveEnable && !SKIP_Weld_Cmd)
				|| KK_StartWF)){

				TimerStart(&WF_DT_Tmr, WF_Stop_Ramp * 1.1);
				WF_Step ++;

				VectorStateMach[VECT_AX_WF].InVal = 0;
				WF_InVal = 0;

			}	

		break;

		case 12:

			if(TimerIsTimeOut(&WF_DT_Tmr))
				WF_Step = 3;	


		break;
		
		case 20:

			VectorStateMach[VECT_AX_WF].InVal = WF_Calibr_Speed/60.0;
			if(WF_Calibr_Speed == 0.0){		
				VectorStateMach[VECT_AX_WF].InVal = 0.0;
				WF_InVal = 0.0;
				WF_Step = 10;			

			}

		break;

		case 21:
			
			WF_Run_Ramp.x = 0.0;
			VectorStateMach[VECT_AX_WF].InVal = WF_InVal * WF_Run_Ramp.y;
			
			if(WF_Run_Ramp.y == 0.0)
				WF_Step = 10;
		break;

		default:

			//WF_Step = 0;

		break;

	}

	TimerExecute(&WF_Tmr);
	TimerExecute(&WF_DT_Tmr);
	TimerExecute(&WF_StatusReg_Tmr);

	AzzRamp(&WF_Run_Ramp);
	WF_Run_Ramp.set = 0;

}


void Calculate_MIG_OSC_Param(float S){
float a , v , T , tv , ta;

	
	//S = fabsf(S) * in_2_mm;
	S = fabsf(S) ;

	KK_OSC_Amplit = S;

		
	KK_OSC_Time = permMem.Analog_Out[OSCSpeedAO_Indx].Value;
	

	T = KK_OSC_Time;
	
	tv= T * KK_ACC_ratio;
	ta = (T-tv)/2.0;
	//ta = (T-tv);

	v = S/(tv+ta);
	a = v/ta;
	
	VectorStateMach[VECT_AX_OSC].fParam[V_fParam_MIG_OSC_Speed] = v;
	VectorStateMach[VECT_AX_OSC].fParam[V_fParam_MIG_OSC_Acc] = a;

	return;

}



float Calculate_Acc(   float v , float t){
float ret;

	if(t > 0)
		//ret = v/t * 2.0;	
		ret = v/t;
		
	else
		ret = 0.0;

	
	return ret;
}

void MotorCfg(){
int i;

	/////////////////////////////////
	
		i = TML_AX_OSC; 
	
		strcpy(TML_MTR_INST[i].Name , "OSC");
		
		TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
					
		TML_MTR_INST[i]._cfg.units = 1024 * 4;		
		
		TML_MTR_INST[i]._cfg.rev_motor = 110.0;
		TML_MTR_INST[i]._cfg.rev_scale = 1.0/(360.0) ;
	
		TML_MTR_INST[i]._cfg.maxVel = 4000.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale;
		TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
		TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
		//TML_MTR_INST[i]._cfg.absFbk = 2;
		TML_MTR_INST[i]._cfg.absFbk = 0;
	
		TML_MTR_INST[i]._cfg.HeartBeat = 0.8;
		TML_MTR_INST[i]._cfg.PVTPeriod = 0.08;
						
		
		strcpy(TML_MTR_INST[i]._cfg.programName , "OSC_0021_100W_36V.sw");
		TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0021");
							
	
		TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("F508L");  //0x674B
		TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
		
	
		////////////////////////////////////////
		i = TML_AX_TVL; 
	
		strcpy(TML_MTR_INST[i].Name , "TVL");
		// Back EMF
		//TML_MTR_INST[i]._cfg.loopPeriod = 0.0005; // Slow loop
		TML_MTR_INST[i]._cfg.loopPeriod = 0.0005 * 2.0; // Slow loop
		TML_MTR_INST[i]._cfg.units = 1024 * 4; //
		
		TML_MTR_INST[i]._cfg.rev_motor = -81.37037037037037037037037037037; 
		TML_MTR_INST[i]._cfg.rev_scale = -(9.0/10.0) /(1.875 * M_PI); //ma byc w inchach
							
		TML_MTR_INST[i]._cfg.maxVel = fabsf(3600.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
		TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
		TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
		TML_MTR_INST[i]._cfg.pAbsLastPos = &OLD_MTR_POS[i];
		TML_MTR_INST[i]._cfg.absFbk = 3; //eksperyment
	
		TML_MTR_INST[i]._cfg.HeartBeat = 0.4;
								
		strcpy(TML_MTR_INST[i]._cfg.programName , "TVL_0025_200W_48V.sw");											
		TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0025");
		
		TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("514I");  //0x674B
		TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
		TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
		/////////////////////////////////////////////////
		///////////////////////////////////////////////////
		i = TML_AX_INDX; 
	
		strcpy(TML_MTR_INST[i].Name , "INDX"); 
	
		// Analog Feedback from REF
		TML_MTR_INST[i]._cfg.loopPeriod = 0.0005 * 2.0; // Slow loop
					
		TML_MTR_INST[i]._cfg.units = 1024 * 4;
		//TML_MTR_INST[i]._cfg.rev_motor = 75.0;
		TML_MTR_INST[i]._cfg.rev_motor = 389376.0/1225.0;
		TML_MTR_INST[i]._cfg.rev_scale = 1.0/(M_PI * 1.6);
		TML_MTR_INST[i]._cfg.absFbk = 0;	

		TML_MTR_INST[i]._cfg.maxVel = fabsf(8740.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
		TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	
		TML_MTR_INST[i]._cfg.HeartBeat = 0.8;
	
		strcpy(TML_MTR_INST[i]._cfg.programName , "INDX_0023_70W_36V.sw");
		TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0023");
							
	
		TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("508L");  
		TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
		TML_MTR_INST[i]._cfg.NotAutoDownload = 1;

		/////////////////////////////////////////////////
		///////////////////////////////////////////////////
		i = TML_AX_INDX_EXT; 
	
		strcpy(TML_MTR_INST[i].Name , "INDX_EXT"); 
	
		// Analog Feedback from REF
		TML_MTR_INST[i]._cfg.loopPeriod = 0.0005 * 2.0; // Slow loop
					
		TML_MTR_INST[i]._cfg.units = 1024 * 4;
		//TML_MTR_INST[i]._cfg.rev_motor = 75.0;
		TML_MTR_INST[i]._cfg.rev_motor = 389376.0/1225.0;
		TML_MTR_INST[i]._cfg.rev_scale = 1.0/(M_PI * 1.6);
		TML_MTR_INST[i]._cfg.absFbk = 0;	

		TML_MTR_INST[i]._cfg.maxVel = fabsf(8740.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
		TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	
		TML_MTR_INST[i]._cfg.HeartBeat = 0.8;
		
		strcpy(TML_MTR_INST[i]._cfg.programName , "INDX_0024_70W_36V.sw");
		TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0024");
							
	
		TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("508L");  
		TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
		TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
		
		////////////////////////////////////////////
		i = TML_AX_AHC; 
		
		strcpy(TML_MTR_INST[i].Name , "AHC");
	
		TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
		TML_MTR_INST[i]._cfg.units = 4096;
		TML_MTR_INST[i]._cfg.rev_motor = 1;
		TML_MTR_INST[i]._cfg.rev_scale = 1.0/5.0;	
		
		TML_MTR_INST[i]._cfg.maxVel = 6700.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale;
		TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 2.0;	
		TML_MTR_INST[i]._cfg.satpMax = 32000; // KPP bylo Stauration of proportional gain
		TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
		TML_MTR_INST[i]._cfg.pAbsLastPos = &OLD_MTR_POS[i];
		TML_MTR_INST[i]._cfg.absFbk = 3; 

		TML_MTR_INST[i]._cfg.kppMax = 20000;
		TML_MTR_INST[i]._cfg.kipMax = 1000;
		TML_MTR_INST[i]._cfg.kpp = 10000/2;
		TML_MTR_INST[i]._cfg.kip = 0;
			
		TML_MTR_INST[i]._cfg.HeartBeat = 0.3;	

		//strcpy(TML_MTR_INST[i]._cfg.programName , "AHC_0026_50W_24V.sw");		
		//TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0026");			
		//TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("514I");  //0x674C , 514D		
		//TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;

		strcpy(TML_MTR_INST[i]._cfg.programName , "AHC_0022_50W_24V.sw");		
		TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0022");		
		TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("508L");  
		TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	
		TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
		TML_MTR_INST[i]._cfg.AnalLimitPos = 0.0;
		TML_MTR_INST[i]._cfg.AnalLimitNeg = 0.0;	
	
		
		TML_MTR_INST[i]._cfg.MtrType = 1 ; // 3phase brusshless
		
		
		////////////////////////////////////////////
		i = TML_AX_WF; 
		
		strcpy(TML_MTR_INST[i].Name , "WF");
		

		// Lincoln Motor with David encoder
		TML_MTR_INST[i]._cfg.rev_scale = -1.0 /( 1.75 * M_PI);
		TML_MTR_INST[i]._cfg.rev_motor = -23.66; 
		TML_MTR_INST[i]._cfg.loopPeriod = 0.0005 * 2.0; // Slow loop
		TML_MTR_INST[i]._cfg.units = 1000 * 4; //	

		TML_MTR_INST[i]._cfg.HeartBeat = 0.8;

		/////////////////
	
		TML_MTR_INST[i]._cfg.maxVel = 3600.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale;
		TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0;
		TML_MTR_INST[i]._cfg.absFbk = 0;			
	
		//strcpy(TML_MTR_INST[i]._cfg.programName , "WF_0001_LincolnSepTML.sw");
		//TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0001");	
		//TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("508L");  
		//TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;

		strcpy(TML_MTR_INST[i]._cfg.programName , "WF_0026_Lincoln.sw");
		TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0026");
		TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("514I");  //0x674C , 514D		
		TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;

		
		TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
		
		
		/***********************************************************/
		/***************************************************************/
		/////////////////////////////////////////////////////////////////
	
		TML_MTR_INST[TML_AX_TVL].pState = &VectorStateMach[VECT_AX_TVL];
		TML_MTR_INST[TML_AX_OSC].pState = &VectorStateMach[VECT_AX_OSC];		
		TML_MTR_INST[TML_AX_AHC].pState = &VectorStateMach[VECT_AX_AHC];
		TML_MTR_INST[TML_AX_WF].pState = &VectorStateMach[VECT_AX_WF];
		TML_MTR_INST[TML_AX_INDX].pState = &VectorStateMach[VECT_AX_INDX];
		TML_MTR_INST[TML_AX_INDX_EXT].pState = &VectorStateMach[VECT_AX_INDX_EXT];
	
		for(i=0 ; i < VECTORS_NUM; i++)
			VectorStateMach[i].Indx = i;
		
	
		strcpy(VectorStateMach[VECT_AX_TVL].Name , TML_MTR_INST[TML_AX_TVL].Name);
		strcpy(VectorStateMach[VECT_AX_OSC].Name , TML_MTR_INST[TML_AX_OSC].Name);		
		strcpy(VectorStateMach[VECT_AX_AHC].Name , TML_MTR_INST[TML_AX_AHC].Name);
		strcpy(VectorStateMach[VECT_AX_WF].Name , TML_MTR_INST[TML_AX_WF].Name);
		strcpy(VectorStateMach[VECT_AX_INDX].Name , TML_MTR_INST[TML_AX_INDX].Name);
		strcpy(VectorStateMach[VECT_AX_INDX_EXT].Name , TML_MTR_INST[TML_AX_INDX_EXT].Name);
		
		TML_mgr[0].hrdw[0].AxisNum = 0; 
		TML_mgr[0].hrdw[1].AxisNum = 6;
	
		TML_mgr[0].pAxis[0] = 0;
		TML_mgr[0].pAxis[1] = &AxisCh[0];
	
		TML_mgr[0].pTML[0] = 0;
		TML_mgr[0].pTML[1] = &TML_MTR_INST[0];
	
		
		AxisCh[TML_AX_TVL].AxisID	= 25;	
		AxisCh[TML_AX_OSC].AxisID	= 21;				
		AxisCh[TML_AX_AHC].AxisID	= 22; 
		AxisCh[TML_AX_WF].AxisID	= 26; 
		AxisCh[TML_AX_INDX].AxisID	= 23; 
		AxisCh[TML_AX_INDX_EXT].AxisID	= 24;
			
		//////////////////////////////////////////////////////
	
		TML_MTR_INST[TML_AX_TVL].Disable = 0;
		TML_MTR_INST[TML_AX_OSC].Disable = 0;		
		TML_MTR_INST[TML_AX_AHC].Disable = 0;
		TML_MTR_INST[TML_AX_WF].Disable = 0;
		TML_MTR_INST[TML_AX_INDX].Disable = 0;
		TML_MTR_INST[TML_AX_INDX_EXT].Disable = 0;
	
		VectorStateMach[VECT_AX_TVL].simul = 0;
		VectorStateMach[VECT_AX_OSC].simul = 0;		
		VectorStateMach[VECT_AX_AHC].simul = 0;
		VectorStateMach[VECT_AX_WF].simul = 0;
		VectorStateMach[VECT_AX_INDX].simul = 0;
		VectorStateMach[VECT_AX_INDX_EXT].simul = 0; 
	
		VectorStateMach[VECT_AX_TVL].testMode = 0;
		VectorStateMach[VECT_AX_OSC].testMode = 0;	
		VectorStateMach[VECT_AX_AHC].testMode = 0;
		VectorStateMach[VECT_AX_WF].testMode = 0;
		VectorStateMach[VECT_AX_INDX].testMode = 0;
		VectorStateMach[VECT_AX_INDX_EXT].testMode = 0;

		

		//////////////////////////////////////////////////////
		


}


float AHC_Cntrl(USINT mode){
float out;

	
	switch(mode){

		case 0: // Leggacy

				if(ActCurrent_Filter.y > permMem.Analog_Out[AHCCurrentSPAO_Indx].Value + permMem.Analog_Out[AHCToleranceAO_Indx].Value){

					out = -permMem.Analog_Out[AHC_Response_Indx].Value / 100.0  * AHC_Respond_Legacy_MAX ;

				}
				else if(ActCurrent_Filter.y < permMem.Analog_Out[AHCCurrentSPAO_Indx].Value -  permMem.Analog_Out[AHCToleranceAO_Indx].Value){

					out = permMem.Analog_Out[AHC_Response_Indx].Value / 100.0 * AHC_Respond_Legacy_MAX ;
				}
				else{

					out = 0.0;

				}

		break;

		case 1: // PID with Window						
				

				if(AHC_Error > AHC_Margin)					
					out = (AHC_Error - AHC_Margin) * AHC_Gain_MAX * permMem.Analog_Out[AHC_Response_Indx].Value/100.0 * (-1.0);

				else if(AHC_Error  <  (AHC_Margin *(-1.0)))					
					out = (-AHC_Error - AHC_Margin)* AHC_Gain_MAX * permMem.Analog_Out[AHC_Response_Indx].Value/100.0 ;
										
				else
					out = 0;

				if(fabsf(AHC_Error) > AHC_Margin ){

					//if(fabsf(AHC_Integr)< (AHC_VAL_MAX/2.0))
					if(fabsf(AHC_Integr)< AHC_VAL_MAX)
						AHC_Integr +=  AHC_Error * AHC_Integr_MAX * permMem.Analog_Out[AHC_Integrator_Indx].Value/100.0;					
					
				}
				else
					AHC_Integr = 0.0;

				if(out > AHC_VAL_MAX)
					out = AHC_VAL_MAX;
				
				else if(out < -AHC_VAL_MAX)
					out = -AHC_VAL_MAX;

				if(fabsf(out - AHC_Integr)< AHC_VAL_MAX)				
					out -= AHC_Integr;		


		break;

		case 2: // Classic PID on PLC	

			AHC_PID.enable = 1;		
			out = AHC_PID.Y;


		break;

		case 3: // Classic PID in Drive
				

		break;
		

		default:

			out = 0.0;

		break;

	}

	return out;

}


plcbit WatchTransError(USINT mode){
plcbit ret = 0;

	
	if(TML_mgr[0].hrdw[1].TXCountReadBack == TML_mgr[0].hrdw[1].TXCount){
				
		WatchEthernet_Tmr.IN =0;		
		
	}
	else{

		if(!WatchEthernet_Tmr.IN)
			TimerStart(&WatchEthernet_Tmr, 1.0);
		
		else if(WatchEthernet_Tmr.Q){

			ret = 1;
			xsprintf(HMI_Error_Txt,"Ethernet Error " );

		}
	}	

	return ret;
}

plcbit CheckAxisOn(TML_Axis_typ * ax){
plcbit ret;

		ret = (ax->DriveStatusRegisterL & 0x8000)? 1:0;

return ret;

}

void CalculateParamForABSCmove(){
float f;

	if(permMem.Analog_Out[TVL_Ramp].Value > 0.0)
						//VectorStateMach[VECT_AX_TVL].fParam[0] =  (permMem.Analog_Out[TravelSpeedAO_Indx].Value/60.0) /permMem.Analog_Out[TVL_Ramp].Value; // ACC
						VectorStateMach[VECT_AX_TVL].fParam[0] =  (3600.0/60.0) /permMem.Analog_Out[TVL_Ramp].Value; // ACC
	else						
		VectorStateMach[VECT_AX_TVL].fParam[0] = 10e+23; // Max ACC from Config	
		

	if(SKIP_Weld_Cmd)
		f = permMem.Analog_Out[TVL_Skip_Speed].Value;
	else if(!MoveEnable)
		f = permMem.Analog_Out[TVL_Return_Speed].Value;
	else	
		f = permMem.Analog_Out[TravelSpeedAO_Indx].Value;

	
											
	switch(TravelSpeed.IN){

		default:
			VectorStateMach[VECT_AX_TVL].InVal = 0.0;
		break;

		case  Forward:	


			VectorStateMach[VECT_AX_TVL].Param[0] = (DINT)(CurCalib.TVL_Hi_Limit * TML_MTR_INST[TML_AX_TVL]._int.kPos);
			VectorStateMach[VECT_AX_TVL].fParam[1] = getFromCalib(f  , &CurCalib.Points[TVL_Calibr_Indx], CALIB_2POINTS)/60.0;

		break;

		case  Reverse:								

			VectorStateMach[VECT_AX_TVL].Param[0] = (DINT)(CurCalib.TVL_Lo_Limit * TML_MTR_INST[TML_AX_TVL]._int.kPos);
			VectorStateMach[VECT_AX_TVL].fParam[1] = getFromCalib(f  , &CurCalib.Points[TVL_Calibr_Indx], CALIB_2POINTS_4OFF)/60.0;

		break;
	}
					

}



////////////////////

void Gaug_Max_Amp_Protection(){

	if(MachCfg.GougingON && MachCfg.Weld){
	//if(1){

				
		/* Moving average filter */
		filterLCRMovAvgFlt.x = ESAB_PS.Fbk.actAmps;
		LCRMovAvgFlt(&filterLCRMovAvgFlt);		/* LCRMovAvgFlt function block call */
		filterLCRMovAvgFlt.y;
		/////////

		 
		JOG_AHC_OUT_prot = 0;
				
		switch(AmpsLimit_Step){


			case 1:

				TimerStart(&AmpsLimit_Tmr, permMem.Analog_Out[GOUG_Amps_Ramp].Value);
				AmpsLimit_Step ++;

			break;

			case 2:

				if(AmpsLimit_Tmr.Q)
					AmpsLimit_Step = 10;


			break;

			////////////////////////////////
			case 10:

				if(!ESAB_PS.Cmd.WeldON)
					AmpsLimit_Step = 0;

				JOG_AHC_OUT_prot = ((ESAB_PS.Fbk.actWF == 0.0) && MachCfg.GougAutAHC)?  1:0;

				if(filterLCRMovAvgFlt.y >= permMem.Analog_Out[GOUG_Amps_Limit].Value){

					TimerStart(&AmpsLimit_Tmr, permMem.Analog_Out[GOUG_Amps_Limit_Tm].Value);
					AmpsLimit_Step = 11;

					
					//xsprintf(HMI_Error_Txt,"Amp Max Limit Err. %4f" , ESAB_PS.Fbk.actAmps);
					//AmpsLimit_Step = 20; // Alarm

				}
				else if(!ESAB_PS.Cmd.WeldON)
					AmpsLimit_Step = 0;

			break;

			case 11:

				JOG_AHC_OUT_prot = ((ESAB_PS.Fbk.actWF == 0.0) && MachCfg.GougAutAHC)?  1:0;
				
				if(AmpsLimit_Tmr.Q){

				
					if(filterLCRMovAvgFlt.y < permMem.Analog_Out[GOUG_Amps_Limit].Value){
						
						AmpsLimit_Tmr.IN = 0;
						AmpsLimit_Step = 10;
					}
					else{						

						xsprintf(HMI_Error_Txt,"Amp Max Limit Err. %4f" , ESAB_PS.Fbk.actAmps);
						AmpsLimit_Step = 20; // Alarm
					}

				}


			break;

			////////////////////////////////

			case 20:

				ESAB_PS.Cmd.QuickStop = 1;
				ESAB_PS.Cmd.WeldON = 0;

				
				TimerStart(&AmpsLimit_Tmr, permMem.Analog_Out[GOUG_Amps_Limit_Tm].Value);
				AmpsLimit_Step ++;

			break;

			case 21: // Retract AHC

				
				JOG_AHC_OUT_prot = 1;
				
				if(AmpsLimit_Tmr.Q){
					
					AmpsLimit_Step = 100;

				}


			break;
			
			/////////////////////////////////
			case 100:

				LogTxt(&LogMgr, HMI_Error_Txt);

				INDX_Step = 100;
				TVL_Step = 100;

				VectorStateMach[VECT_AX_WF].Mode = vectSt_Mode_Idle;
				WF_Step = 0;

				VectorStateMach[VECT_AX_AHC].Mode = vectSt_Mode_Idle;				
				AHC_Step = 0;			
				///

				SequenceRun_Out = 0;
				Robot_Cmd_RUN = 0;
				SequenceRun_WF = 0;	
				
				EndOfBead_Stop = 0;
				Sequence_Halt_Cmd = 0;

				INDX_Step_Cmd = 0;

				Seq.State = 0;

				/////////////////////////////
				
				
				GAS_OFF_Tmr.PT = (int) (permMem.Analog_Out[GAS_PostPurge].Value * 1000.0 +0.5);					
				GAS_OFF_Tmr.IN = 1;
				GAS_ON_Tmr.IN = 0;

				EndOfBead_Stop = 0;
				Sequence_Halt_Cmd = 0;	

				SKIP_Weld_Cmd = 0;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;	

				ESAB_PS.Cmd.QuickStop = 1;
				ESAB_PS.Cmd.WeldON = 0;

				AmpsLimit_Step = 0;


			break;
			
			


			default:

				if(permMem.Analog_Out[GOUG_Amps_Limit].Value == 0.0)
					AmpsLimit_Step = 0;
		

				else if(ESAB_PS.Cmd.WeldON && (!_ESAB_PS_Cmd_WeldON))
					AmpsLimit_Step = 1;

			break;

		}
	}
	else		
		JOG_AHC_OUT_prot = 0;

	////////////////////////////////////////
	
	////////////////////////////////////////
	_ESAB_PS_Cmd_WeldON = ESAB_PS.Cmd.WeldON;
	TimerExecute(&AmpsLimit_Tmr);

return;
}


