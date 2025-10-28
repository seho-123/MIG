
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

#include <..\Lib\Statelib.c>
#include <..\Lib\WSI_lib.c>
#include <..\Lib\LoggerLib.c>


void SetDefaultValues();
void ConfigParam();
void GetWeldParamSet();
void CheckWeldParamSet();
void CheckIndxDone();


_LOCAL TON_typ Master_Tmr;
_LOCAL TON_typ BurnBack_Tmr;
_LOCAL TON_typ GetWeldParamSet_Tmr;
_LOCAL TON_typ StandBy_Tmr;
_LOCAL TON_typ OSC_OFF_Tmr;
_LOCAL plcbit _MoveEnable;
_LOCAL plcbit Gas_Arclink_ON;
_LOCAL UDINT HMI_StndBy_CntDown; 
_LOCAL UINT HMI_StndBy_CntDown_vis;


_LOCAL AzzScal_typ VoltCmd_Scale;

_LOCAL float WF_SlowSpeedTime;

//_LOCAL plcbit Robot_Cmd_RUN , Robot_Cmd_RUN1; 

_LOCAL plcbit HMI_Travel_Fwd , HMI_Travel_Rev ;
plcbit _HMI_Travel_Fwd , _HMI_Travel_Rev ;
plcbit _gas_Cmd;
plcbit _gas_Arclink_ON;


_LOCAL SINT Test_INDX_Step_Cmd;

_LOCAL SINT ArcLink_Gas_Valve_step;
///////////////////

_LOCAL TON_typ KK_TVL_Test_Tmr;
_LOCAL plcbit Virt_Cntrl_Cmd_RUN;

/////////////////
_LOCAL plcbit HMI_Graph_Trig , HMI_Graph_Enable; 
_LOCAL float HMI_Weld_Stat;


////////////////

_LOCAL plcbit KK_Test_Lincoln;

_LOCAL SINT KK_Test_Status;
UINT _Cntrl_Fbk_Error , _Cntrl_Fbk_Status;


_LOCAL plcbit FirstWeldBeat;

plcbit _sTANDBY_sign;
plcbit _sKIP_Weld_Cmd;


void _INIT ProgramInit(void)
{
	
	//ESTOP = 1;
	Robot_Cmd_RUN1 = 0;
	
	ESAB_Amp_Ramp.dy_up = 10.0;
	ESAB_Amp_Ramp.dy_down = 10.0;
	ESAB_Amp_Ramp.y_max = 1600.0;
	ESAB_Amp_Ramp.y_min = 0.0;
	ESAB_Amp_Ramp.enable = 1;

	KK_Test_Lincoln = 0;

	FirstWeldBeat = 1;
	ProcError_vis = INVISIBLE;
	
	StandBy_Tmr.PT = 10000;
	STANDBY_sign = 0;

	WF_SlowSpeedTime = 0.5;
	
	strcpy(SoftwareVersion , SOFTWARE_VERSION);

	Head.Remote_SW = 1;

	Test_INDX_Step_Cmd = 1;

	

	GAS_ON_Tmr.IN = 0;
	GAS_OFF_Tmr.IN = 1;

	GAS_ON_Tmr.PT = 10;
	GAS_OFF_Tmr.PT = 10;


	///////////////////////////////////


	ConfigParam();
	
	// Init permanent variables
	if (!permMemInit) {

		SetDefaultValues();

		SetDefault_TVL_Calibr = 1;
		SetDefault_WF_Calibr = 1;
		SetDefault_AMPS_Calibr = 1;
		
	}
	
	visChangePage = visChangePage;

	OSCJog = OSCJog;
	Purge = Purge;
	WFJog.IN = WFJog.IN;
	cameraPower = cameraPower;
	
	Seq.startTimer.PT = 250;
	Seq.stopTimer.PT = 250;
	
	// Configure Analog Ranges	

	
	// Initialize Computer Power
	computerPower[0] = 0;
	computerPower[1] = 0;

	/////////////////////////////////////

	ArcLink.robot.Cntrl.Fbk.Status = 0;

	
	///
	LocalJogMode = LocalJogMode;
	Head.LocalJog_FWD = Head.LocalJog_FWD;
	Head.LocalJog_REV = Head.LocalJog_REV;


	VoltCmd_Scale.x1 = 0.0;
	VoltCmd_Scale.y1 = 0.0;

	VoltCmd_Scale.x2 = 60.0;
	VoltCmd_Scale.y2 = 32767.0;


	TimerStart(&BurnBack_Tmr,0.1);

	MachActive = 0;


	

}

void _CYCLIC ProgramCyclic(void)
{
int i;

	

	switch(KK_TVL_Test_step){


		case 0:

			TravelJog.IN = Stop;
			KK_TVL_Test_step = 100;

		break;


		case 1:

			TravelJog.IN = Forward;
			TimerStart(&KK_TVL_Test_Tmr, 3.0);
			KK_TVL_Test_step ++;
			
		break;

		case 2:

			if(TimerIsTimeOut(&KK_TVL_Test_Tmr)){
				TravelJog.IN = Stop;
				TimerStart(&KK_TVL_Test_Tmr, 2.0);
				KK_TVL_Test_step ++;
			}

		break;


		case 3:

			if(TimerIsTimeOut(&KK_TVL_Test_Tmr)){

				TravelJog.IN = Reverse;
				TimerStart(&KK_TVL_Test_Tmr, 3.0);
				KK_TVL_Test_step ++;
			}

		break;


		case 4:

			if(TimerIsTimeOut(&KK_TVL_Test_Tmr)){
				TravelJog.IN = Stop;
				TimerStart(&KK_TVL_Test_Tmr, 2.0);
				KK_TVL_Test_step ++;
			}

		break;

		case 5:

			if(TimerIsTimeOut(&KK_TVL_Test_Tmr)){
				KK_TVL_Test_step = 1;
			}

		break;	


		default:

		break;

	}
	
	TimerExecute(&KK_TVL_Test_Tmr);

	/////////////////////////////////////////////////

	if(SetDefault){
		SetDefault = 0;
		SetDefaultValues();
	}
	if(SetDefault_TVL_Calibr){
		SetDefault_TVL_Calibr = 0;
		Page_Step = 0;

		CurCalib.Points[TVL_Calibr_Indx].num = 8;

		CurCalib.Points[TVL_Calibr_Indx].val[0] = 1.0;
		CurCalib.Points[TVL_Calibr_Indx].val[1] = 1.0;
		CurCalib.Points[TVL_Calibr_Indx].val[2] = 10.0;
		CurCalib.Points[TVL_Calibr_Indx].val[3] = 10.0;

		CurCalib.Points[TVL_Calibr_Indx].val[4] = 1.0;
		CurCalib.Points[TVL_Calibr_Indx].val[5] = 1.0;
		CurCalib.Points[TVL_Calibr_Indx].val[6] = 10.0;
		CurCalib.Points[TVL_Calibr_Indx].val[7] = 10.0;

	}

	if(SetDefault_WF_Calibr){
		SetDefault_WF_Calibr = 0;
		Page_Step = 0;

		CurCalib.Points[WF_Calibr_Indx].num = 4;

		CurCalib.Points[WF_Calibr_Indx].val[0] = 1.0;
		CurCalib.Points[WF_Calibr_Indx].val[1] = 1.0;
		CurCalib.Points[WF_Calibr_Indx].val[2] = 10.0;
		CurCalib.Points[WF_Calibr_Indx].val[3] = 10.0;

	}

	if(SetDefault_AMPS_Calibr){
		SetDefault_AMPS_Calibr = 0;
		Page_Step = 0;

		CurCalib.Points[AMPS_Calibr_Indx].num = 4;

		CurCalib.Points[AMPS_Calibr_Indx].val[0] = 50.0;
		CurCalib.Points[AMPS_Calibr_Indx].val[1] = 50.0;
		CurCalib.Points[AMPS_Calibr_Indx].val[2] = 500.0;
		CurCalib.Points[AMPS_Calibr_Indx].val[3] = 500.0;

	}
	
	
	
	// Check for module error
	ModulesOK = 1;


	//////////////////////////////////////////////////////////////////////////	
	if(Seq.Stop_in || Sequence_Halt_Cmd 
			|| Main_IO.Inp[STOP_RST_BTN]
			|| Main_IO.Inp[STOP_RST_BTN_LOC]){


		//if(!Seq.Stop_in){ 		
			Robot_Cmd_RUN = 0;	
			SequenceRun_WF = 0;
		//}

		SequenceRun_Out = 0;
		
		ESAB_PS.Cmd.QuickStop = 1;
		ESAB_PS.Cmd.WeldON = 0;

		// NOWE W STOP przeniesione z State 0
		AHCJog.IN = Stop;

		
		
		EndOfBead_Stop = 0;
		Sequence_Halt_Cmd = 0;
		
		SKIP_Weld_Cmd = 0;
		

		//////////////////	

		if(ReadyToOperate && HardwReady)
			INDX_Step_Cmd = 0;
		
		if(Seq.State != 0){
			TimerStart(&GAS_OFF_Tmr, permMem.Analog_Out[GAS_PostPurge].Value);
			GAS_ON_Tmr.IN = 0;
			OSCJog = 0;
			LogTxt(&LogMgr, "Seq.Stop");

		}

		if(ESTOP)	
			Seq.State = 0;	

	}
			

	ObserveAHC_MaxPos = 0;
	WatchIndx = 0;		
	
	switch(Seq.State){

		case 0: // Stopped

			SequenceRun_Out = 0;
			Robot_Cmd_RUN = 0;
			SequenceRun_WF = 0;	

			EndOfBead_Stop = 0;
			Sequence_Halt_Cmd = 0;

			//INDX_Step_Cmd = 0;

			
			
			SKIP_Weld_Cmd = 0;
	
			if(Seq.Start_in && !Seq.Stop_in
				&& Head.Remote_SW){
				////////////////////////////////////////////////////

				 CheckIndxDone();
				

				if(ProcError_vis == INVISIBLE){

					if(MoveEnable){
						TimerStart(&GAS_ON_Tmr, permMem.Analog_Out[GAS_PrePurge].Value);
						TimerStart(&GAS_OFF_Tmr, 0.1);
					}

					Master_Tmr.IN = 0;

					LogTxt(&LogMgr , "Seq. Start");

					ErrorFromMaster_flag = 0;

					SKIP_Weld_Cmd = 0;	

					ESAB_PS.Cmd.QuickStop = 0;
			
					Seq.State = 1;		

				}

			}

		break;


		case 1: // Wait Prepurge


			/////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////
			if(MachCfg.GougingON){ // gouging

				LogTxt(&LogMgr , "Goug. Start");				
			
				if(MachCfg.Weld && MoveEnable
					&& (TVL_Step == 10)){
					Robot_Cmd_RUN = 1;
					TimerStart(&Master_Tmr, 0.5);
					Seq.State = 5;

				}
				else{

					Robot_Cmd_RUN = 0;
					SequenceRun_Out = 1;
					_MoveEnable = MoveEnable;
					TimerStart(&Master_Tmr, 0.5);
					Seq.State = 11;

				}
					
				break;

			}
			/////////////////////////////////////////////////////
			
			
			if(SKIP_Weld_Cmd){

				if(!SequenceRun_Out){
					SKIP_Weld_Cmd = 0;
					Seq.State = 11;
				}
				
				break;
				
			}else if(MachCfg.STOP_TVL_After_SKIP){

				if(_sKIP_Weld_Cmd){
					SequenceRun_Out = 0;
				}
			}
			
			
			_MoveEnable = MoveEnable;
			
			if(!MoveEnable){
				SequenceRun_Out = 1;
				Seq.State = 11;

			}
			else if(GAS_ON_Tmr.Q){

				//GAS_OFF_Tmr.IN = 0;

				SKIP_Weld_Cmd = 0;

				// Start Lincoln

				if(MachCfg.Weld ){
				
					if((TVL_Step == 10) || (!MachCfg.STOP_TVL_After_SKIP)){	

						Robot_Cmd_RUN = 1;
						TimerStart(&Master_Tmr, WF_SlowSpeedTime);		
						Seq.State = 2;					

					}
				}
				else{	
					
					Robot_Cmd_RUN = 0;

					TimerStart(&Master_Tmr, WF_SlowSpeedTime);		
					Seq.State = 2;
				}

				
				
			}

			
		break;

#if 0 
		// To dziala dobrze
		case 2: // Wait for Short			

			if((Master_Tmr.Q && (!MachCfg.Weld || KK_Test_Lincoln))
				|| (ArcLink.robot.Cntrl.Fbk.Status & (Arc_OCV + Arc_Short + Arc_Active))				
				){

				LogTxt(&LogMgr , "Arc2x: %x" , ArcLink.robot.Cntrl.Fbk.Status);
				//LogTxt(&LogMgr , "Arc2d: %d" , ArcLink.robot.Cntrl.Fbk.Status);

			
				if(MachCfg.TestMode){
					SequenceRun_WF = 1;					
				}
				
				if(ArcLink.robot.Cntrl.Fbk.Status & (Arc_Short + Arc_Active)){

					// Start Lincoln				
					if(MachCfg.Weld || MachCfg.TestMode){
						SequenceRun_WF = 1;
						
					}
					else {
						SequenceRun_WF = 0;	
					}
					
					TimerStart(&Master_Tmr, 0.005);

				}
				else					
					TimerStart(&Master_Tmr, WF_SlowSpeedTime);

				AHC_StartPos = TML_MTR_INST[TML_AX_AHC].PosFBK;
				Seq.State++;				
			}
			

		break;

	


		case 3: // Wait Arc		

			if(!Master_Tmr.Q)
				break;			

			// Start Lincoln				
			if(MachCfg.Weld || MachCfg.TestMode || KK_Test_Lincoln){
				SequenceRun_WF = 1;
				
			}
			else {
				SequenceRun_WF = 0;	
			}

			if((ArcLink.robot.Cntrl.Fbk.Status & (Arc_Active)) //(Arc_Short + Arc_Active))
				|| (!MachCfg.Weld) || KK_Test_Lincoln){ 
			
				LogTxt(&LogMgr , "Arc3x: %x" , ArcLink.robot.Cntrl.Fbk.Status);

				SequenceRun_Out = 1;
				Master_Tmr.IN = 0;

				TimerStart(&Master_Tmr, 0.5);
				
				Seq.State = 11;			

			} 			

		break;
#endif

//#if 0	
		/// NOWE
		case 2: // Wait for Short

			
			//////////////////////////

			AHC_StartPos = TML_MTR_INST[TML_AX_AHC].PosFBK;
			
			if((Master_Tmr.Q && (!MachCfg.Weld || KK_Test_Lincoln))
				|| (ArcLink.robot.Cntrl.Fbk.Status & (Arc_OCV + Arc_Short + Arc_Active))				
				){

				LogTxt(&LogMgr , "Arc2x: %x" , ArcLink.robot.Cntrl.Fbk.Status);
				
				
				if(ArcLink.robot.Cntrl.Fbk.Status & (Arc_Short + Arc_Active)){ 

					// Start Lincoln				
					if(MachCfg.Weld || MachCfg.TestMode){

							
						if(MachCfg.Weld || MachCfg.TestMode || KK_Test_Lincoln){
							SequenceRun_WF = 1;
							
						}
						else {
							SequenceRun_WF = 0; 
						}

						
						TimerStart(&Master_Tmr, 0.005);
						
					}
					else {
						
						TimerStart(&Master_Tmr, WF_SlowSpeedTime);
						
						SequenceRun_WF = 0;
					}
					
										

				}
				
				else if((!MachCfg.Weld || KK_Test_Lincoln)){

					TimerStart(&Master_Tmr, WF_SlowSpeedTime);
					
					if( MachCfg.TestMode)						
						SequenceRun_WF = 1;						
					else 
						SequenceRun_WF = 0;
					
				}

				Seq.State++;
							
			}
			else if(Master_Tmr.Q && (!(ArcLink.robot.Cntrl.Fbk.Status & (Arc_OCV)))){
					
					xsprintf(HMI_Error_Txt,"Err.: Touch TMO");
					LogTxt(&LogMgr , "Err.: Touch TMO");
					ErrorFromMaster_flag = 1;	
					SequenceRun_WF = 0;	
					Robot_Cmd_RUN = 0;
					Seq.State = 0;	

			}			
			

		break;


		case 3: // Wait Arc							

			if(!Master_Tmr.Q && !SequenceRun_WF)
				break;			

			// Start Lincoln				
			if(MachCfg.Weld || MachCfg.TestMode || KK_Test_Lincoln){

				if(!SequenceRun_WF){

					if(permMem.Analog_Out[ARC_Start_Tmo].Value > 0.0)
						TimerStart(&Master_Tmr, permMem.Analog_Out[ARC_Start_Tmo].Value);
					else
						TimerStart(&Master_Tmr, WF_SlowSpeedTime);


				}
				
				SequenceRun_WF = 1;
				
			}
			else {
				
				TimerStart(&Master_Tmr, WF_SlowSpeedTime);
				SequenceRun_WF = 0;	
			}
			
			
			//if((ArcLink.robot.Cntrl.Fbk.Status & (Arc_Active)) //(Arc_Short + Arc_Active))
			if((ArcLink.robot.Cntrl.Fbk.Status & (Arc_Short + Arc_Active)) // change 8 Aug 2023
				|| (!MachCfg.Weld) || KK_Test_Lincoln){ 
			
				LogTxt(&LogMgr , "Arc3x: %x" , ArcLink.robot.Cntrl.Fbk.Status);

				SequenceRun_Out = 1;
				Master_Tmr.IN = 0;

				//TimerStart(&Master_Tmr, 0.5); za krotki
				//TimerStart(&Master_Tmr, 1.5);

				if(permMem.Analog_Out[ARC_Start_Tmo].Value > 0.0)
					TimerStart(&Master_Tmr, permMem.Analog_Out[ARC_Start_Tmo].Value);
				else
					TimerStart(&Master_Tmr, 3.0);
		
				Seq.State = 11;			

			}
			else if(Master_Tmr.Q){
				
				xsprintf(HMI_Error_Txt,"Arc Start TMO");
				LogTxt(&LogMgr , HMI_Error_Txt);
				Robot_Cmd_RUN = 0;
				ErrorFromMaster_flag = 1;	
				SequenceRun_WF = 0;	
				Seq.State = 0;

			}
			

		break;

//#endif
		case 5: // Waiting for Gouging start

			if(((ESAB_PS.Fbk.Status & 0x3)== 0x3)){
				//&&(fabs(ESAB_PS.Fbk.Req_TVL_Speed) > 0.0)){ // Welding
				//&& Master_Tmr.Q){ // Welding

				SequenceRun_Out = 1;
				_MoveEnable = MoveEnable;

				TimerStart(&Master_Tmr, 0.5);
				Seq.State = 11;	

			}
			else if(ESAB_PS.Fbk.Status & 0x08){ // error

				LogTxt(&LogMgr , "G Stop by Err.");
				Robot_Cmd_RUN = 0;
				Seq.State = 0;

			}


		break;

		
		case 11: // Run Travel 	


			if(!SequenceRun_Out){	

				Robot_Cmd_RUN = 0;
				SequenceRun_WF = 0;				

				
				//if(GAS_ON_Tmr.IN){
				if(GAS_Cmd){	
					TimerStart(&GAS_OFF_Tmr, permMem.Analog_Out[GAS_PostPurge].Value);
					GAS_ON_Tmr.IN = 0;
				}
				
				
				if(MachCfg.GougingON){

					if(!_MoveEnable
						|| (MachCfg.WeldMode == 0)){ // Index

						if(MachCfg.ContinSingle)
							EndOfBead_Stop = 1;
				
						TimerStart(&Master_Tmr, permMem.Analog_Out[Index_PreDlyTime_Indx].Value);
						
						if(MachCfg.WeldMode == 0)
							AHCJog.IN = Reverse;
						
						Seq.State = 15;

					}
					else {  // Go return							

						TimerStart(&Master_Tmr, permMem.Analog_Out[Index_PostDlyTime_Indx].Value);
						AHCJog.IN = Reverse;
						Seq.State = 25;	

					}				

				}
				//if(MoveEnable){
				else if(!_MoveEnable  			// End of return
					|| (MachCfg.WeldMode == 0)){ // or weld Vessel
					if(MachCfg.ContinSingle)
						EndOfBead_Stop = 1;	

					TimerStart(&Master_Tmr, permMem.Analog_Out[Index_PreDlyTime_Indx].Value);

					KK_Test_Status = 15;
					Seq.State = 15;	

					if(MachCfg.WeldMode
						&& (AHC_StartPos > -3.40282e+038)//NOT just after set zero)
						&&  MachCfg.AHC_BackStep
						&& !MachCfg.GougingON){ 
						
						AHC_Move_Target = AHC_StartPos;
						AHC_Move_Cmd = 1;
					}
					
				}
				else{ // End of Welding

					TimerStart(&Master_Tmr, permMem.Analog_Out[Index_PostDlyTime_Indx].Value);
					
					KK_Test_Status = 25;

					if(MachCfg.WeldMode && (AHC_Scaled_Fact != 0.0)
						&& (AHC_MaxPos > -3.40282e+038) //NOT just after set zero)
						&& MachCfg.AHC_BackStep
						&& !MachCfg.GougingON){
						
						AHC_Move_Target = AHC_MaxPos - permMem.Analog_Out[AHC_Retrack_Dist].Value / AHC_Scaled_Fact ;
						AHC_Move_Cmd = 1;
					}
					Seq.State = 25;	
				}

			}
			else{				 
			

				///   OBESRVE Max AHC Position and current Indx  //
				//if(MoveEnable && Master_Tmr.Q){
				if(MoveEnable){
					
					FirstWeldBeat = 0;
					ObserveAHC_MaxPos = 1;
					WatchIndx = 1;

					TVL_Last_Dir = TravelSpeed.IN;

				}

			}

			//////////////////////////////////////////////
			if(MachCfg.GougingON) // Gouging
				break;
		

			/////////////////////////////////////////////////////////////////
			if(SKIP_Weld_Cmd || (!MoveEnable)){		
			
				Robot_Cmd_RUN = 0;
				SequenceRun_WF = 0;

				if(MoveEnable)
					Seq.State = 1;

			}
			else {

				
				if(MachCfg.Weld || MachCfg.TestMode){					

					/////////////////////////////////////////					
					//if(((ArcLink.robot.Cntrl.Fbk.curArcAmp <  (0.75 * permMem.Analog_Out[AHCCurrentSPAO_Indx].Value))
					if(((ArcLink.robot.Cntrl.Fbk.curArcAmp <  (permMem.Analog_Out[ARC_NotActive_Level].Value/100.0 * permMem.Analog_Out[AHCCurrentSPAO_Indx].Value))
						&& !MachCfg.TestMode && !KK_Test_Lincoln)
						|| ArcLink.robot.Cntrl.Fbk.Error){							

						if(Master_Tmr.Q || ArcLink.robot.Cntrl.Fbk.Error){
							LogTxt(&LogMgr , "Err.: Arc Not Active");
							xsprintf(HMI_Error_Txt,"Err.: Arc Not Active");
							ErrorFromMaster_flag = 1;	
							SequenceRun_WF = 0;	
							Seq.State = 0;	

						}
						
					}
					else{					
					
						//TimerStart(&Master_Tmr, 1.5);
						TimerStart(&Master_Tmr, 0.5);
					}

					
				}
				else {
					Robot_Cmd_RUN = 0;
					SequenceRun_WF = 0;


				}
			}				
			

		break;


		case 15: // Pre Dly
			
			if(Master_Tmr.Q && !AHC_Move_Cmd){
				Master_Tmr.IN = 0;

				AHCJog.IN = Stop;				
								
				if(MachCfg.Indxon > 0){

					if(MachCfg.Weld || (!MachCfg.TestMode) || KK_Test_Lincoln){
						
						if(MachCfg.IndxDir > 0)
							INDX_Step_Cmd = 1;
						else
							INDX_Step_Cmd = -1;
					}
					else{
						Test_INDX_Step_Cmd = -Test_INDX_Step_Cmd;
						INDX_Step_Cmd = Test_INDX_Step_Cmd;
						
					}
				}
				
				Seq.State = 20;
			}	
			

		break;

		case 20: // Indexing

			if(INDX_Step_Cmd)
				break;

			TimerStart(&Master_Tmr, permMem.Analog_Out[Index_PostDlyTime_Indx].Value);
			Seq.State = 25;
				
		
			
			
		break;


		case 25: // Post Dly	

			SKIP_Weld_Cmd = 0;
		
			if(Master_Tmr.Q && !AHC_Move_Cmd){

				AHCJog.IN = Stop;
				
				Master_Tmr.IN = 0;

									
				
				if(EndOfBead_Stop){
					AHCJog.IN = Stop;
					Seq.State = 0;
				}	
				else{


					CheckIndxDone();

					if(ProcError_vis == INVISIBLE){

						if(MoveEnable){
							
							if(GAS_OFF_Tmr.Q){ // Start Gas again
								
								TimerStart(&GAS_ON_Tmr, permMem.Analog_Out[GAS_PrePurge].Value);
								TimerStart(&GAS_OFF_Tmr, 0.1);
					
							}
							else{ // Continue Gas

							
								GAS_ON_Tmr.IN = 1;
															

							}

						}
						else{

							TimerStart(&GAS_OFF_Tmr, permMem.Analog_Out[GAS_PostPurge].Value);
							GAS_ON_Tmr.IN = 0;
						}

						
						Seq.State = 1;

					}
					else{

						TimerStart(&GAS_OFF_Tmr, permMem.Analog_Out[GAS_PostPurge].Value);
						GAS_ON_Tmr.IN = 0;

						AHCJog.IN = Stop;
						Seq.State = 0;
					}
						

				}
				
			}	
			

		break;


		//////////////////////////////////////////////////////

		

		case 200: // No Permision

			INDX_Step_Cmd = 0;
			SequenceRun_Out = 0; 


			if(Head.Remote_SW || !HardwReady)			
				Seq.State = 0;
			

		break;

		case 300:
		case 301:
		case 302:
		case 303:			
			// Hardware Init or Error	

			Robot_Cmd_RUN = 0;
			ESAB_PS.Cmd.WeldON = 0;
			ESAB_PS.Cmd.QuickStop = 1;
			
		break;

		
		case 304:  // Torch Brake	
		
			if(TML_MTR_INST[TML_AX_WF].DigInp[1])
				Seq.State = 0;

		break;	

		

	}

	////

	_sKIP_Weld_Cmd = SKIP_Weld_Cmd;

	//////////////////////////////////////

	if(WeldCalibr_Mode){

		ArcLink.robot.Cntrl.Cmd.Mode = 200;
		ArcLink.robot.Cntrl.Cmd.AmpSP = AMPS_Calibr_Val;
	
		Robot_Cmd_RUN = (AMPS_Calibr_Val > 0)? 1:0;
			

	}else {

		
		
			
		if(ArcLink.robot.isMapped){

			switch(MachCfg.Weld_mode)
			{

				case 5:
					ArcLink.robot.Cntrl.Cmd.Mode = 5;
					ArcLink.robot.Cntrl.Cmd.Volts = permMem.Analog_Out[ArcVoltsAO_Indx].Value;
					ArcLink.robot.Cntrl.Cmd.Pinch = permMem.Analog_Out[Weld_Pinch].Value;

					ArcLink.robot.Cntrl.Cmd.Development = 0;
			
				break;


				case 21: 
				
					ArcLink.robot.Cntrl.Cmd.Mode = 21;
					ArcLink.robot.Cntrl.Cmd.WFS = permMem.Analog_Out[WireFeedSpeedAO_Indx].Value;
					ArcLink.robot.Cntrl.Cmd.Volts = permMem.Analog_Out[ArcVoltsAO_Indx].Value;
					ArcLink.robot.Cntrl.Cmd.Pinch = permMem.Analog_Out[Weld_Pinch].Value;

					ArcLink.robot.Cntrl.Cmd.Development = 1;

					GetWeldParamSet();

					//if(ArcLink.robot.Comm.CntrlParamData.UnitsCode == 0)
						//MachCfg.Weld_mode = 22;
						

				break;

				case 22:
					
					ArcLink.robot.Cntrl.Cmd.Mode = 22;
					ArcLink.robot.Cntrl.Cmd.WFS = permMem.Analog_Out[WireFeedSpeedAO_Indx].Value;
					ArcLink.robot.Cntrl.Cmd.Trim = permMem.Analog_Out[Weld_Trim].Value;
					ArcLink.robot.Cntrl.Cmd.UltimArc = permMem.Analog_Out[Weld_UltimArc].Value;

					ArcLink.robot.Cntrl.Cmd.Development = 1;

					GetWeldParamSet();

										
				break;


				case 160:
				case 161:
				case 162:
				case 163:
				case 164:
				case 165:
				case 166:
				case 167:					
					
					ArcLink.robot.Cntrl.Cmd.Mode = MachCfg.Weld_mode;
					ArcLink.robot.Cntrl.Cmd.Volts = permMem.Analog_Out[ArcVoltsAO_Indx].Value;	

					ArcLink.robot.Cntrl.Cmd.Development = 0;

					if(AccessLevel > 0)
						CheckWeldParamSet();
					
				break;

				case 169:
				case 170:
				case 171:	
					
					ArcLink.robot.Cntrl.Cmd.Mode = MachCfg.Weld_mode;
					ArcLink.robot.Cntrl.Cmd.Volts = permMem.Analog_Out[ArcVoltsAO_Indx].Value;	
					ArcLink.robot.Cntrl.Cmd.WFS = permMem.Analog_Out[WireFeedSpeedAO_Indx].Value;

					ArcLink.robot.Cntrl.Cmd.Development = 0;

					if(AccessLevel > 0)
						CheckWeldParamSet();
					
				break;

			}

		}
		else{

			// This force sending new mode after PS is down
			ArcLink.robot.Cntrl.Int._Mode = 0;
			ArcLink.robot.Cntrl.Int._AmpSP = 0.0;
			ArcLink.robot.Cntrl.Int._BckAmpSP = 0.0;
		}
	}

	

	//////////////////////////////////////////////////////

	if((ArcLink.robot.Cntrl.Fbk.Error != _Cntrl_Fbk_Error)
		|| (ArcLink.robot.Cntrl.Fbk.Status != _Cntrl_Fbk_Status)){

		if(ArcLink.robot.Cntrl.Fbk.Error)
			LogTxt(&LogMgr, "Linc.Err: %X -> %X" ,ArcLink.robot.Cntrl.Fbk.Error , ArcLink.robot.Cntrl.Fbk.Status);
		else{

			if(ArcLink.robot.Cntrl.Fbk.Status & Arc_Active)//(Arc_OCV + Arc_Short + Arc_Active)
				LogTxt(&LogMgr, "Linc.Stat: Arc Active");
			else if(ArcLink.robot.Cntrl.Fbk.Status & Arc_Short)
				LogTxt(&LogMgr, "Linc.Stat: Arc Short");			
			else if(ArcLink.robot.Cntrl.Fbk.Status & Arc_OCV)
				LogTxt(&LogMgr, "Linc.Stat: OCV");
			else
				LogTxt(&LogMgr, "Linc.Stat: OFF");


		}

	}
	_Cntrl_Fbk_Error = ArcLink.robot.Cntrl.Fbk.Error;
	_Cntrl_Fbk_Status = ArcLink.robot.Cntrl.Fbk.Status;
	
	//////////////////////////////////////

	TON(&Master_Tmr);
	TON(&GAS_ON_Tmr);
	TON(&GAS_OFF_Tmr);
	
	if(MachCfg.GougingON) // Gouging
		GAS_Cmd = 0;
	else	
		GAS_Cmd = (GAS_ON_Tmr.IN || !GAS_OFF_Tmr.Q) && (MachCfg.Weld || MachCfg.TestMode);		

	//if(HardwInit_Done){
	if(HardInit_Step == 50){

		if((GAS_Cmd || Purge) && !_gas_Cmd){

			Gas_Arclink_ON = 1;

			//if(TML_MTR_INST[TML_AX_WF].xCMD == TML_IDLE){						
				//TML_MTR_INST[TML_AX_WF].xCMD 	= TML_SET_OUT0_CMD; 
			if(!VectorStateMach[VECT_AX_WF].xCmd_busy){						
				VectorStateMach[VECT_AX_WF].xCMD = VECT_Set_OUT0; //TML_SET_OUT0_CMD; 	
				_gas_Cmd = (GAS_Cmd || Purge);
			}			
		}
		else if(!(GAS_Cmd || Purge) && _gas_Cmd){

			Gas_Arclink_ON = 0;

			//if(TML_MTR_INST[TML_AX_WF].xCMD == TML_IDLE){						
				//TML_MTR_INST[TML_AX_WF].xCMD 	= TML_RST_OUT0_CMD; 
			if(!VectorStateMach[VECT_AX_WF].xCmd_busy){						
				VectorStateMach[VECT_AX_WF].xCMD = VECT_Rst_OUT0; //  TML_RST_OUT0_CMD; 	
				_gas_Cmd = (GAS_Cmd || Purge);
			}			
		}
		
	}
	else {

		Gas_Arclink_ON = 0;

		if(!TML_MTR_INST[TML_AX_WF].DigOut_stat[0]
			&& TML_MTR_INST[TML_AX_WF]._int.Init){

			if(!VectorStateMach[VECT_AX_WF].xCmd_busy){						
				VectorStateMach[VECT_AX_WF].xCMD = VECT_Rst_OUT0; //  TML_RST_OUT0_CMD; 
			
			//if(TML_MTR_INST[TML_AX_WF].xCMD == TML_IDLE){						
				//TML_MTR_INST[TML_AX_WF].xCMD = TML_RST_OUT0_CMD; 

			}
		}

		Purge = 0;
		_gas_Cmd = !(GAS_Cmd || Purge);
	}
	
	/////////  GAS Solenoid ////

	if(!ArcLink.robot.isMapped){	
		
		ArcLink_Gas_Valve_step = 1;
	
	}else{	

		switch(ArcLink_Gas_Valve_step){

			default:

			break;

			case 1:

				if(ArcLink.robot.isMapped && (ArcLink.robot.miscSetup.state == 9)
					&& ArcLink.robot.isReady){

					if(!ArcLink.robot.Intrf.xCMD){

						ArcLink.robot.miscSetup.GasSolAddr = 0;
						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_SEARCH_GAS_SOL_ADR;
						ArcLink_Gas_Valve_step ++;
					}

				}
				
			break;

			case 2:

				if(!ArcLink.robot.Intrf.xCMD 
					&& (ArcLink.robot.miscSetup.GasSolAddr > 0)){

					_gas_Arclink_ON = !Gas_Arclink_ON;
					ArcLink_Gas_Valve_step ++;

				}
				
			break;


			case 3:

				if(!ArcLink.robot.Intrf.xCMD){

					if(Gas_Arclink_ON !=_gas_Arclink_ON){

						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_GAS_SOL_ENABLE;
						ArcLink_Gas_Valve_step ++;
						
					}
				}

			break;

			case 4:

				if(!ArcLink.robot.Intrf.xCMD){

					if(Gas_Arclink_ON)
						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_GAS_SOL_ON;					

					else 
						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_GAS_SOL_OFF;
						
					
					ArcLink_Gas_Valve_step ++;

				}

			break;

			case 5:

				if(!ArcLink.robot.Intrf.xCMD){
					
					_gas_Arclink_ON = Gas_Arclink_ON;
					ArcLink_Gas_Valve_step = 3;

				}	

			break;

		}

	}
	/////////////////////
	VoltCmd_Scale.x = permMem.Analog_Out[ArcVoltsAO_Indx].Value;
	AzzScal(&VoltCmd_Scale);
	VoltCmd_Out = (INT)VoltCmd_Scale.y;

	Main_IO.Out[2] = Robot_Cmd_RUN;

	if(!ESTOP 
		|| TML_MTR_INST[TML_AX_WF].Error){

		if(Robot_Cmd_RUN && TML_MTR_INST[TML_AX_WF].Error){

			LogTxt(&LogMgr , "WF Err: %x" , TML_MTR_INST[TML_AX_WF].ErrorReg);	
			//LogTxt(&LogMgr , "Arc E: %x" , ArcLink.robot.Cntrl.Fbk.Status);
		}

		Robot_Cmd_RUN = 0;
	}

	if(Robot_Cmd_RUN){
		Virt_Cntrl_Cmd_RUN = 1;
		BurnBack_Tmr.IN = 0;

		HMI_Graph_Enable = 1;
		HMI_Graph_Trig = 1;
	}	
	else{	

		HMI_Graph_Enable = 0;
		HMI_Graph_Trig = 0;

		if(!BurnBack_Tmr.IN)
			TimerStart(&BurnBack_Tmr, permMem.Analog_Out[BURN_BACK_Tm].Value);

		else if(TimerIsTimeOut(&BurnBack_Tmr))
			Virt_Cntrl_Cmd_RUN = 0;

	}
	TimerExecute(&BurnBack_Tmr);

	///////////////////////////////////

	if(SequenceRun_Out){
		SequenceRun_OSC = 1;
		OSC_OFF_Tmr.IN = 0;
	}
	else if(permMem.Analog_Out[OSC_OFF_Tm].Value > 0){

		if(!OSC_OFF_Tmr.IN)
			TimerStart(&OSC_OFF_Tmr, permMem.Analog_Out[OSC_OFF_Tm].Value);

		else if(OSC_OFF_Tmr.Q)
			SequenceRun_OSC = 0;
	}
	else
		SequenceRun_OSC = 0;

	TimerExecute(&OSC_OFF_Tmr);

	//////////////////////////////////////

	
	if(KK_Test_Lincoln)
		ArcLink.robot.Cntrl.Cmd.RUN = 0;
	else		
		ArcLink.robot.Cntrl.Cmd.RUN = (Virt_Cntrl_Cmd_RUN && !MachCfg.GougingON);

	if(MachCfg.GougingON &&  MachCfg.Weld)
		ESAB_PS.Cmd.WeldON = Robot_Cmd_RUN || Robot_Cmd_RUN1;
	else
		ESAB_PS.Cmd.WeldON = 0;

	ESAB_PS.Cmd.JogHighSpeed = (MachCfg.GougHighJogSpeed > 0)? 1:0;

	ESAB_PS.Param.Voltage = permMem.Analog_Out[GOUG_Volts].Value;


	//ESAB_Amp_Ramp.dy_up = permMem.Analog_Out[GOUG_Amps_Ramp].Value;	
	if(permMem.Analog_Out[GOUG_Amps_Ramp].Value > 0.0)
		ESAB_Amp_Ramp.dy_up = fabsf((permMem.Analog_Out[GOUG_Amps].Value - permMem.Analog_Out[GOUG_Amps_Init].Value)/permMem.Analog_Out[GOUG_Amps_Ramp].Value);
	else
		permMem.Analog_Out[GOUG_Amps_Ramp].Value = 0.01;
	
	ESAB_Amp_Ramp.dy_down = ESAB_Amp_Ramp.dy_up;

	if(!ESAB_PS.Cmd.WeldON){
		ESAB_Amp_Ramp.y_set = permMem.Analog_Out[GOUG_Amps_Init].Value;
		ESAB_Amp_Ramp.set = 1;
	}else
		ESAB_Amp_Ramp.x = permMem.Analog_Out[GOUG_Amps].Value;
	
	AzzRamp(&ESAB_Amp_Ramp);
	ESAB_Amp_Ramp.set = 0;

	ESAB_PS.Param.Amps = ESAB_Amp_Ramp.y;
	//ESAB_PS.Param.Amps = permMem.Analog_Out[GOUG_Amps].Value;

	
	////
	/*
	#define PS_Ready 0x0001
	#define Arc_OFF 0x0002
	#define Arc_OCV 0x0004
	#define Arc_Short 0x0008
	#define Arc_Active 0x0010
	*/
	HMI_Weld_Stat = (ArcLink.robot.Cntrl.Fbk.Status & 0x001F) * 10.0;
	
	////////////////////////////////////////////////

	if(STANDBY_sign != _sTANDBY_sign){
		
		
		if(!STANDBY_sign && _sTANDBY_sign){

			AckAllErrors(&ErrorMgr);

			memset(ErrorMgr_Txt[0] , 0 , sizeof(ErrorMgr_Txt));
			
			HardInit_Step = 1;	
			
		}	
		else if(STANDBY_sign && !_sTANDBY_sign){
			
			if(visPage != 200)
				NewPageReq_Cmd = 200;// Stanby Screen or return from 		

		}
	}	

	if(!STANDBY_sign){

		if(visPage == 200)
			NewPageReq_Cmd = 1;// 
	}
	_sTANDBY_sign = STANDBY_sign;

	/////////	
	
	MachActive = 0;

	if(permMem.Analog_Out[StanBy_Time].Value == 0.0) // Stanby not activated
		MachActive = 1;
	
	else if(!Head.Remote_SW)
		MachActive = 1;

	else if(Main_IO.Inp[STOP_RST_BTN] || Main_IO.Inp[STOP_RST_BTN_LOC])
		MachActive = 1;

	/*
	else if(!HardwReady)
		MachActive = 1;
	*/
	
	else if(Screen_touch.status == 1)
		MachActive = 1;
	/*
	if(Seq.State != 0)
		MachActive = 1;
	
	else */
		if(TravelJog.IN != Stop)
		MachActive = 1;

	else if(OSCJog)
		MachActive = 1;

	else if(TVL_Step > 10)
		MachActive = 1;
	
	else if(StandBy_Tmr.Q && (permMem.Analog_Out[StanBy_Time].Value > 0.0)){
		
		STANDBY_sign = 1;		
	}
	else
		STANDBY_sign = 0;
	
	

	if(MachActive){
		TimerStart(&StandBy_Tmr, permMem.Analog_Out[StanBy_Time].Value * 60.0);
		HMI_StndBy_CntDown_vis = INVISIBLE;
	}
	else
		HMI_StndBy_CntDown_vis = VISIBLE;

	
	HMI_StndBy_CntDown = (UDINT)((StandBy_Tmr.PT - StandBy_Tmr.ET)/1000) + 1;

	
	TimerExecute(&StandBy_Tmr);	


	
	

}

void _EXIT ProgramExit(void)
{

}
void SetDefaultValues(){
UINT i;
	permMemInit = 1;


	i = TravelSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 30.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	///////////

	i = ArcVoltsAO_Indx;
	permMem.Analog_Out[i].Value = 24.5;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.25;

	
	i = GOUG_Volts;
	permMem.Analog_Out[i].Value = 24.5;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.25;
	
	
	i = GOUG_Amps;
	permMem.Analog_Out[i].Value = 500.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 1.0;

	i = GOUG_Amps_Init;
	permMem.Analog_Out[i].Value = 500.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 1.0;

	
	i = GOUG_Amps_Ramp;
	permMem.Analog_Out[i].Value = 2.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	/////////////////

	i = WireFeedSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 475.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	///////////

	i = OSCSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 0.125;
	permMem.Analog_Out[i].H_Incr = 0.01;
	permMem.Analog_Out[i].L_Incr = 0.005;

	i = OSC_AmplitAO_Indx;
	permMem.Analog_Out[i].Value = 0.5;
	permMem.Analog_Out[i].H_Incr = 0.1;
	permMem.Analog_Out[i].L_Incr = 0.005;

	i = OSC_DwellInAO_Indx;
	permMem.Analog_Out[i].Value = 0.0;
	permMem.Analog_Out[i].H_Incr = 0.1;
	permMem.Analog_Out[i].L_Incr = 0.05;

	i = OSC_DwellOutAO_Indx;
	permMem.Analog_Out[i].Value = 0.0;
	permMem.Analog_Out[i].H_Incr = 0.1;
	permMem.Analog_Out[i].L_Incr = 0.05;

	i = OSC_JogSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 1.0;

	i = OSC_StepDistAO_Indx;
	permMem.Analog_Out[i].Value = 0.1;
	permMem.Analog_Out[i].H_Incr = 0.1;
	permMem.Analog_Out[i].L_Incr = 0.05;

	/////

	i = AHCCurrentSPAO_Indx;
	permMem.Analog_Out[i].Value = 205.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = AHCToleranceAO_Indx;
	permMem.Analog_Out[i].Value = 5.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;
	
	i = AHC_Response_Indx;
	permMem.Analog_Out[i].Value = 75;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	
	i = AHC_Integrator_Indx;
	permMem.Analog_Out[i].Value = 0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = AHC_JogSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 50;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;	


	i = AHC_JogSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;


	////	
	i = Index_Dist_H_Indx;
	permMem.Analog_Out[i].Value = 0.25;
	permMem.Analog_Out[i].H_Incr = 0.5;
	permMem.Analog_Out[i].L_Incr = 0.1;	

	i = Index_Dist_L_Indx;
	permMem.Analog_Out[i].Value = 0.25;
	permMem.Analog_Out[i].H_Incr = 0.5;
	permMem.Analog_Out[i].L_Incr = 0.1;	

	i = Index_Jog_Speed_Indx;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = Index_Step_Speed_Indx;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;	
	
	i = Index_PreDlyTime_Indx;
	permMem.Analog_Out[i].Value = 1.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = Index_PostDlyTime_Indx;
	permMem.Analog_Out[i].Value = 1.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = TVL_Jog_Speed_Indx;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;
	

	i = WF_Jog_Speed_Indx;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	////////////////////////////////////////////
	
	i = TVL_Calibr_Vel_Low;
	permMem.Analog_Out[i].Value = 5.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = TVL_Calibr_Vel_High;
	permMem.Analog_Out[i].Value = 10.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = TVL_Calibr_Dist;
	permMem.Analog_Out[i].Value = 10.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	//////////	

	i = WF_Calibr_Vel_Low;
	permMem.Analog_Out[i].Value = 300.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = WF_Calibr_Vel_High;
	permMem.Analog_Out[i].Value = 500.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = WF_Calibr_Time;
	permMem.Analog_Out[i].Value = 10.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = TVL_Ramp;
	permMem.Analog_Out[i].Value = 1.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = TVL_Return_Speed;
	permMem.Analog_Out[i].Value = 150.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = WF_Ramp;
	permMem.Analog_Out[i].Value = 0.4;
	permMem.Analog_Out[i].H_Incr = 0.1;
	permMem.Analog_Out[i].L_Incr = 0.01;

	
	i = GAS_PrePurge;
	permMem.Analog_Out[i].Value = 2.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = GAS_PostPurge;
	permMem.Analog_Out[i].Value = 2.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;
	
	 
	i = Weld_Pinch;
	permMem.Analog_Out[i].Value = 0.0;
	permMem.Analog_Out[i].H_Incr = 10;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = Weld_Trim;
	permMem.Analog_Out[i].Value = 0.5;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	
	i = Weld_UltimArc;
	permMem.Analog_Out[i].Value = 0.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;


	//////////	

	i = AMPS_Calibr_Low;
	permMem.Analog_Out[i].Value = 100.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = AMPS_Calibr_High;
	permMem.Analog_Out[i].Value = 200.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;


	i = BURN_BACK_Tm;
	permMem.Analog_Out[i].Value = 0.150;
	permMem.Analog_Out[i].H_Incr = 0.1;
	permMem.Analog_Out[i].L_Incr = 0.01;


	i = WF_SlowSpeed;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 1.0;

	i = StanBy_Time;
	permMem.Analog_Out[i].Value = 15.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 1.0;

	i = TVL_Skip_Speed;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	
	i = AHC_Retrack_Dist;
	permMem.Analog_Out[i].Value = 1.0;
	permMem.Analog_Out[i].H_Incr = 0.5;
	permMem.Analog_Out[i].L_Incr = 0.125;

	/////////////////
	
	i = ARC_Start_Tmo;
	permMem.Analog_Out[i].Value = 1.0;
	permMem.Analog_Out[i].H_Incr = 0.1;
	permMem.Analog_Out[i].L_Incr = 0.01;

	
	i = ARC_NotActive_Level;
	permMem.Analog_Out[i].Value = 75.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 1.0;

	
	i = OSC_OFF_Tm;
	permMem.Analog_Out[i].Value = 15.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 1.0;
	


}
void ConfigParam(){
USINT i;

	
	////////////////////////////////////////////////

	for(i = 0 ; i <= LastPermMem_Indx ; i++){

		/*
		Analog_Out_Mod[i].LimScale.x1 = 0.0;
		Analog_Out_Mod[i].LimScale.y1 = 0.0;
		Analog_Out_Mod[i].LimScale.x2 = 100.0;
		Analog_Out_Mod[i].LimScale.y2 = 32767.0;
		*/

		Analog_Out_Mod[i].Min_Val = 0.0;
		Analog_Out_Mod[i].Max_Val = 100.0;
		
		
	}

	//  New 
	strcpy(Analog_Out_Mod[TravelSpeedAO_Indx].Name , "Travel Speed [inch/min]");
	//Analog_Out_Mod[TravelSpeedAO_Indx].Min_Val = 0.1;
	Analog_Out_Mod[TravelSpeedAO_Indx].Min_Val = 0.0;
	Analog_Out_Mod[TravelSpeedAO_Indx].Max_Val = 240.0;
	
	strcpy(Analog_Out_Mod[ArcVoltsAO_Indx].Name , "ArcVolts [V]");
	strcpy(Analog_Out_Mod[GOUG_Volts].Name , "Goug.Volts [V]");
	strcpy(Analog_Out_Mod[GOUG_Amps].Name , "Goug. Amps [A]");
	strcpy(Analog_Out_Mod[GOUG_Amps_Init].Name , "Goug. Start Amps [A]");
	strcpy(Analog_Out_Mod[GOUG_Amps_Ramp].Name , "Goug. Amps Ramp [s]");

	strcpy(Analog_Out_Mod[GOUG_Amps_Limit].Name , "Goug. Amps Limit [A]");
	strcpy(Analog_Out_Mod[GOUG_Amps_Limit_Tm].Name , "Goug.  HiAmps Tmr[s]");
	
	
	strcpy(Analog_Out_Mod[WireFeedSpeedAO_Indx].Name , "Wire Feed Speed [inch/min]");
	Analog_Out_Mod[WireFeedSpeedAO_Indx].Min_Val = 1.0;
	Analog_Out_Mod[WireFeedSpeedAO_Indx].Max_Val = 700.0;
	
	strcpy(Analog_Out_Mod[AHCCurrentSPAO_Indx].Name , "AHC Current SP [A]");
	strcpy(Analog_Out_Mod[AHCToleranceAO_Indx].Name , "AHC Tolerance [A]");
	strcpy(Analog_Out_Mod[AHC_Response_Indx].Name , "AHC Response [%]");	
	strcpy(Analog_Out_Mod[AHC_Integrator_Indx].Name , "AHC Integrator [%]");
	strcpy(Analog_Out_Mod[AHC_JogSpeedAO_Indx].Name , "AHC Jog Speed [%]");
	
	strcpy(Analog_Out_Mod[OSCSpeedAO_Indx].Name , "OSC Excur. [s]");
	strcpy(Analog_Out_Mod[OSC_AmplitAO_Indx].Name , "OSC Amplit. [inch]");
	strcpy(Analog_Out_Mod[OSC_DwellInAO_Indx].Name , "OSC Dwell In. [s]");
	strcpy(Analog_Out_Mod[OSC_DwellOutAO_Indx].Name , "OSC Dwel Out. [s]");
	strcpy(Analog_Out_Mod[OSC_JogSpeedAO_Indx].Name , "OSC Jog. [ % ]");
	strcpy(Analog_Out_Mod[OSC_StepDistAO_Indx].Name , "OSC Steer Step [inch]");

	strcpy(Analog_Out_Mod[TVL_Jog_Speed_Indx].Name , "TVL Jog Speed [%]");
	strcpy(Analog_Out_Mod[WF_Jog_Speed_Indx].Name , "WF Jog Speed [%]");

	strcpy(Analog_Out_Mod[AHC_Retrack_Dist].Name , "AHC Back Step [inch]");
	
	strcpy(Analog_Out_Mod[Wire_Dim].Name , "Wire Dim [inch]");

	//Analog_Out_Mod[ArcVoltsAO_Indx].LimScale.x2 = 45.0;
	Analog_Out_Mod[ArcVoltsAO_Indx].Max_Val = 45.0;		
	
	Analog_Out_Mod[WireFeedSpeedAO_Indx].Max_Val = 750.0;
	
	Analog_Out_Mod[AHCCurrentSPAO_Indx].Max_Val = 1000.0;

	Analog_Out_Mod[AHCToleranceAO_Indx].Max_Val = 20.0;
	
	Analog_Out_Mod[AHC_JogSpeedAO_Indx].Min_Val = 1.0;

	Analog_Out_Mod[TVL_Jog_Speed_Indx].Min_Val = 1.0;
	Analog_Out_Mod[WF_Jog_Speed_Indx].Min_Val = 1.0;
	

	Analog_Out_Mod[OSCSpeedAO_Indx].Min_Val = 0.05;
	Analog_Out_Mod[OSCSpeedAO_Indx].Max_Val = 2.0;

	Analog_Out_Mod[OSC_JogSpeedAO_Indx].Min_Val = 1.0;
	
	Analog_Out_Mod[OSC_StepDistAO_Indx].Min_Val = 0.0;
	Analog_Out_Mod[OSC_StepDistAO_Indx].Max_Val = 0.25;

	Analog_Out_Mod[OSC_AmplitAO_Indx].Min_Val = 0.125;
	Analog_Out_Mod[OSC_AmplitAO_Indx].Max_Val = 0.5;

	Analog_Out_Mod[OSC_DwellInAO_Indx].Min_Val = 0.0;
	Analog_Out_Mod[OSC_DwellInAO_Indx].Max_Val = 2.5;

	Analog_Out_Mod[OSC_DwellOutAO_Indx].Min_Val = 0.0;
	Analog_Out_Mod[OSC_DwellOutAO_Indx].Max_Val = 2.5;

	Analog_Out_Mod[AHC_Retrack_Dist].Min_Val = 0.0;
	Analog_Out_Mod[AHC_Retrack_Dist].Max_Val = 10.0;

	Analog_Out_Mod[GOUG_Volts].Max_Val = 50.0;	
	Analog_Out_Mod[GOUG_Volts].Min_Val = 12.0;	
	
	Analog_Out_Mod[GOUG_Amps].Max_Val = 1600.0;	
	Analog_Out_Mod[GOUG_Amps].Min_Val = 10.0;	

	Analog_Out_Mod[GOUG_Amps_Init].Max_Val = 1600.0;	
	Analog_Out_Mod[GOUG_Amps_Init].Min_Val = 10.0;	

	Analog_Out_Mod[GOUG_Amps_Ramp].Max_Val = 60.0;	
	Analog_Out_Mod[GOUG_Amps_Ramp].Min_Val = 0.01;	
	
	
	Analog_Out_Mod[GOUG_Amps_Limit].Max_Val = 10000.0;	
	Analog_Out_Mod[GOUG_Amps_Limit].Min_Val = 10.0;	

	Analog_Out_Mod[GOUG_Amps_Limit_Tm].Max_Val = 60.0;	
	Analog_Out_Mod[GOUG_Amps_Limit_Tm].Min_Val = 0.00;	

	Analog_Out_Mod[Wire_Dim].Min_Val = 0.01;
	Analog_Out_Mod[Wire_Dim].Max_Val = 1.0;
	

//////////////////////////			
	
	strcpy(Analog_Out_Mod[Index_Jog_Speed_Indx].Name , "Index Jog Speed [%]");
	strcpy(Analog_Out_Mod[Index_Step_Speed_Indx].Name , "Index Speed [%]");
	
	strcpy(Analog_Out_Mod[Index_Dist_H_Indx].Name , "Index Dist. H [inch]");
	Analog_Out_Mod[Index_Dist_H_Indx].Min_Val = 0.0;
	Analog_Out_Mod[Index_Dist_H_Indx].Max_Val = 100.0;	

	strcpy(Analog_Out_Mod[Index_Dist_L_Indx].Name , "Index Dist. L [inch]");
	Analog_Out_Mod[Index_Dist_L_Indx].Min_Val = 0.0;
	Analog_Out_Mod[Index_Dist_L_Indx].Max_Val = 100.0;

	strcpy(Analog_Out_Mod[Index_PreDlyTime_Indx].Name , "Index Pre-Delay Time [sec.]");
	strcpy(Analog_Out_Mod[Index_PostDlyTime_Indx].Name , "Index Post-Delay Time [sec.]");
	
	for(i = Index_PreDlyTime_Indx ; i <= Index_PostDlyTime_Indx ; i++){		

		Analog_Out_Mod[i].Min_Val = 0.0;
		Analog_Out_Mod[i].Max_Val = 10.0;
		
	}

	////////////////////////////////////////////

	strcpy(Analog_Out_Mod[TVL_Calibr_Vel_Low].Name , "TVL Calibr Speed Low [inch/min]");
	strcpy(Analog_Out_Mod[TVL_Calibr_Vel_High].Name , "TVL Calibr Speed High [inch/min]");
	strcpy(Analog_Out_Mod[TVL_Calibr_Dist].Name , "TVL Calibr Dist.[inch]");

	Analog_Out_Mod[TVL_Calibr_Vel_Low].Min_Val = 0.1;
	Analog_Out_Mod[TVL_Calibr_Vel_Low].Max_Val = 50.0;

	Analog_Out_Mod[TVL_Calibr_Vel_High].Min_Val = 0.1;
	Analog_Out_Mod[TVL_Calibr_Vel_High].Max_Val = 1000.0;
		
	Analog_Out_Mod[TVL_Calibr_Dist].Min_Val = 0.1;
	Analog_Out_Mod[TVL_Calibr_Dist].Max_Val = 1000.0;
	
	strcpy(Analog_Out_Mod[WF_Calibr_Vel_Low].Name , "WF Calibr Speed Low [inch/min]");
	strcpy(Analog_Out_Mod[WF_Calibr_Vel_High].Name , "WF Calibr Speed High [inch/min]");
	strcpy(Analog_Out_Mod[WF_Calibr_Time].Name , "WF Calibr. Time [sec.]");

	Analog_Out_Mod[WF_Calibr_Vel_Low].Min_Val = 1.0;
	Analog_Out_Mod[WF_Calibr_Vel_Low].Max_Val = 700.0;

	Analog_Out_Mod[WF_Calibr_Vel_High].Min_Val = 1.0;
	Analog_Out_Mod[WF_Calibr_Vel_High].Max_Val = 700.0;
		
	Analog_Out_Mod[WF_Calibr_Time].Min_Val = 0.1;
	Analog_Out_Mod[WF_Calibr_Time].Max_Val = 240.0;
	
	/// Ramps
	strcpy(Analog_Out_Mod[TVL_Ramp].Name , "TVL Ramp [sec]");
	strcpy(Analog_Out_Mod[TVL_Return_Speed].Name , "TVL Rewrap Speed [inch/min]");
	strcpy(Analog_Out_Mod[TVL_Skip_Speed].Name , "TVL Skip Speed [inch/min]");

	Analog_Out_Mod[TVL_Ramp].Min_Val = 0.0;
	Analog_Out_Mod[TVL_Ramp].Max_Val = 10.0;

	Analog_Out_Mod[TVL_Return_Speed].Min_Val = 1.0;
	Analog_Out_Mod[TVL_Return_Speed].Max_Val = 240.0;

	Analog_Out_Mod[TVL_Skip_Speed].Min_Val = 1.0;
	Analog_Out_Mod[TVL_Skip_Speed].Max_Val = 240.0;

	strcpy(Analog_Out_Mod[WF_Ramp].Name , "WF Ramp [sec]");
	Analog_Out_Mod[WF_Ramp].Min_Val = 0.1;
	Analog_Out_Mod[WF_Ramp].Max_Val = 2.0;

	////////////////////////////////
	strcpy(Analog_Out_Mod[GAS_PrePurge].Name , "GAS PrePurge [sec]");
	strcpy(Analog_Out_Mod[GAS_PostPurge].Name , "GAS PostPurge [sec]");

	Analog_Out_Mod[GAS_PrePurge].Min_Val = 0.5;
	Analog_Out_Mod[GAS_PrePurge].Max_Val = 600.0;

	Analog_Out_Mod[GAS_PostPurge].Min_Val = 0.5;
	Analog_Out_Mod[GAS_PostPurge].Max_Val = 600.0;

	
	strcpy(Analog_Out_Mod[Weld_Pinch].Name , "Weld Pinch");

	Analog_Out_Mod[Weld_Pinch].Min_Val = -10.0;
	Analog_Out_Mod[Weld_Pinch].Max_Val = 10.0;

	
	strcpy(Analog_Out_Mod[Weld_Trim].Name , "Weld Trim");

	Analog_Out_Mod[Weld_Trim].Min_Val = 0.5;
	Analog_Out_Mod[Weld_Trim].Max_Val = 1.5;

	strcpy(Analog_Out_Mod[Weld_UltimArc].Name , "Weld UltimArc");

	Analog_Out_Mod[Weld_UltimArc].Min_Val = -10.0;
	Analog_Out_Mod[Weld_UltimArc].Max_Val = 10.0;


	///////////////////////////
	strcpy(Analog_Out_Mod[AMPS_Calibr_Low].Name , "AMPS Calibr Low [A]");
	strcpy(Analog_Out_Mod[AMPS_Calibr_High].Name , "AMPS Calibr High [A]");
	
	Analog_Out_Mod[AMPS_Calibr_Low].Min_Val = 10.0;
	Analog_Out_Mod[AMPS_Calibr_Low].Max_Val = 300.0;

	Analog_Out_Mod[AMPS_Calibr_High].Min_Val = 10.0;
	Analog_Out_Mod[AMPS_Calibr_High].Max_Val = 500.0;

	
	strcpy(Analog_Out_Mod[BURN_BACK_Tm].Name , "BurnBack [sec]");

	Analog_Out_Mod[BURN_BACK_Tm].Min_Val = 0.01;
	Analog_Out_Mod[BURN_BACK_Tm].Max_Val = 2.0;


	strcpy(Analog_Out_Mod[WF_SlowSpeed].Name , "WF Slow Speed [inch/min]");

	Analog_Out_Mod[WF_SlowSpeed].Min_Val = 0.5;
	Analog_Out_Mod[WF_SlowSpeed].Max_Val = 300.0;

	
	strcpy(Analog_Out_Mod[StanBy_Time].Name , "Standby Time [min]");

	Analog_Out_Mod[StanBy_Time].Min_Val = 0.0;
	Analog_Out_Mod[StanBy_Time].Max_Val = 60.0;	

	//////////////

	strcpy(Analog_Out_Mod[ARC_Start_Tmo].Name , "Arc Start TMO [s]");

	Analog_Out_Mod[ARC_Start_Tmo].Min_Val = 0.0;
	Analog_Out_Mod[ARC_Start_Tmo].Max_Val = 10.0;
	

	strcpy(Analog_Out_Mod[ARC_NotActive_Level].Name , "Arc Not Active [%]");

	Analog_Out_Mod[ARC_NotActive_Level].Min_Val = 0.0;
	Analog_Out_Mod[ARC_NotActive_Level].Max_Val = 200.0;
	

	strcpy(Analog_Out_Mod[OSC_OFF_Tm].Name , "OSC OFF Delay [s]");

	Analog_Out_Mod[OSC_OFF_Tm].Min_Val = 0.0;
	Analog_Out_Mod[OSC_OFF_Tm].Max_Val = 240.0;



	
}

 void GetWeldParamSet(){

 	if(!GetWeldParamSet_Tmr.IN && !GetWeldParamSet_Tmr.Q){
		TimerStart(&GetWeldParamSet_Tmr, 1.5);

		ArcLink.robot.Comm.CntrlParamReq.Weld_Mode = 0;

		
		if(ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  == 517)
			ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 518;
		else
			ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 517;


		//ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 518;

		if(!ArcLink.robot.Intrf.xCMD)
			ArcLink.robot.Intrf.xCMD = ROBOT_READ_WC_PARAM_DATA;
		
 	}
	else if(GetWeldParamSet_Tmr.Q)
		GetWeldParamSet_Tmr.IN = 0;
		


	TimerExecute(&GetWeldParamSet_Tmr);
 }


void CheckWeldParamSet(){

 	if(!GetWeldParamSet_Tmr.IN && !GetWeldParamSet_Tmr.Q){
		TimerStart(&GetWeldParamSet_Tmr, 2.0);

		ArcLink.robot.Comm.CntrlParamReq.Weld_Mode = ArcLink.robot.Cntrl.Cmd.Mode;

		
		if(ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  == 517)
			ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 518;
		else
			ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 517;


		//ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 518;

		if(!ArcLink.robot.Intrf.xCMD)
			ArcLink.robot.Intrf.xCMD = ROBOT_READ_WC_PARAM_DATA;
		
 	}
	else if(GetWeldParamSet_Tmr.Q)
		GetWeldParamSet_Tmr.IN = 0;
		


	TimerExecute(&GetWeldParamSet_Tmr);
 }


void CheckIndxDone(){

	//if(MoveEnable){
	if(0){

		/////////////////////////

		if(!FirstWeldBeat
			&& !MachCfg.GougingON){
			//&& (MachCfg.Indxon > 0)){

			if(fabsf(INDX_Last_Pos - INDX_Act_Pos) < 0.1){

				switch(TVL_Last_Dir){

					case Forward:				

						if((TVL_Act_Pos > (TVL_Last_Pos - 0.5))
							&& (TravelSpeed.IN == Forward)){}							
					
						else
							ProcError_Indx = 1;					

					break;

					case Reverse:				

						if((TVL_Act_Pos < (TVL_Last_Pos + 0.5))
							&& (TravelSpeed.IN == Reverse)){}							

						else 
							ProcError_Indx = 1;								

					break;

					default:					

					break;				

				}
			}
			
		}
		else
			ProcError_Indx = 0;	
				
		/////////////////////////
		// Super Important - prevent to not start ARC when TVL is not ready
		if(TVL_Step != 10){
				
			ProcError_Indx = 2;
			
		}		
		
		ProcError_vis = (ProcError_Indx>0)? VISIBLE:INVISIBLE;
		//////////////////////////
		
	}

}

