
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

#include <..\Lib\Statelib.c>
#include <..\Lib\LoggerLib.c>
#include <..\Lib\WSI_lib.c>



/****************************/
/*** File and XML operations */
/* Variables - can be set as non PVI */
#define MAX_XML_FILE_SIZE 1500000
_LOCAL plcstring sPV[80];
_LOCAL fiDIR_READ_DATA FileNames[50];
_LOCAL signed long status_xml;

_LOCAL double HMI_PowerRatio , HMI_PowerRatio_w1 , HMI_PowerRatio_w2 , HMI_PowerRatio_w3;
//////////////////////////////


UINT _PageBtnPress[6];
_LOCAL UINT LastPageIndx;


// For HMI

_LOCAL Analog_Out_Mem_typ *HMI_Analog_Mem;
_LOCAL Analog_Out_Module_typ *HMI_Analog_AO;
_LOCAL UINT HMI_LockedFWD_REV;
_LOCAL UINT HMI_TVL_Jog_Cont;

_LOCAL UINT Status_Color_Indx;

_LOCAL UINT blinkCnt, blinkCntMax;
_LOCAL BOOL blink;

_LOCAL plcbit HMI_Reset_Errors;
_LOCAL UINT HMI_Reset_Errors_vis;
_LOCAL UINT Misc_Reset_Btn_vis;

_LOCAL UINT HMI_DiagPg_ProgrVer_vis;

_LOCAL plcbit HardwareFaulted;

_LOCAL UINT NewEditBox, OldEdittBox;

_LOCAL plcbit HMI_GEN_Btn[8] , HMI_GEN_accomp[8];
_LOCAL float HMI_Low_Limit_Inp , HMI_High_Limit_Inp , HMI_High_Low_Dist_Inp;
_LOCAL UINT HMI_GEN_vis[8];
_LOCAL plcstring HMI_GEN_txt[64];
_LOCAL UINT HMI_Pend_Stnd_vis;
_LOCAL UINT HMI_Pend_Gaug_vis , HMI_Pend_NOT_Gaug_vis;

_LOCAL UINT HMI_Lock_Seq_Stat_vis;



_LOCAL UDINT HMI_MainTimerDT;
_LOCAL USINT HMI_MainTimerSec;
_LOCAL TON_typ MainTmr;
_LOCAL plcbit HMI_MainTmer_Start_btn;
_LOCAL UINT  HMI_MainTimer_Start_btn_vis;
_LOCAL plcstring HMI_CalibrList[8][64];

_LOCAL UINT HMI_Left_Arrow_vis, HMI_Right_Arrow_vis;
_LOCAL UINT HMI_Down_Arrow_vis, HMI_Up_Arrow_vis;

_LOCAL UINT HMI_Jog_Btns_locked , HMI_AHC_Jog_Btns_locked;
_LOCAL UINT HMI_WeldMode_Setup_vis_lock;
_LOCAL UINT HMI_MachCfg_TestMode_vis;
_LOCAL UINT HMI_TVL_Btns_vis;


_LOCAL UINT HMI_Supervisor_vis , HMI_Test_Icon_vis;
_LOCAL UINT HMI_WMode_164 , HMI_WMode_170 , HMI_WMode_5 , HMI_WMode_21 , HMI_WMode_22 , HMI_WMode_Goug ;

_LOCAL UINT HMI_AHC_FWD , HMI_AHC_REV;

_LOCAL UINT HMI_PS_Ready_vis , HMI_PS_Arc_OFF_vis , HMI_PS_Arc_OCV_vis , HMI_PS_Arc_Short_vis , HMI_PS_Arc_Active_vis;


_LOCAL plcbit HMI_Change_TVL_Dir_Btn;
_LOCAL plcstring HMI_JogMode_Txt[32];

_LOCAL float MAX_ERRROR;

UINT _hmi_Calibr_Indx;

SINT _IndxExt;

_LOCAL UINT 	ready;
_LOCAL UDINT   VC_HANDLE;


// For Graph
_LOCAL UINT HMI_Graph_Vis , HMI_Report_Vis;

void P_Pendant();
void P_Jog();
void P_Setup();
void P_Camera();
void P_Maint();
void P_Diagnostic();
void P_Param1();
void P_Param2();
void P_Calib_0();

void SCR40_Recipe_1();
int  CheckFileName( char *str , char *msg);


void Main_Calib();
void AMPS_Calib();

void P_SetLimits();


void PageNav();
void MtrErrorText(char* txt , UDINT i , UDINT val );
void Calcul_Slop_Bias(Calib_Param_typ *cal , UINT type);

////////////////



/////////////////
_LOCAL float KK_GetFrom_Low , KK_GetFrom_High , KK_GetFrom_Low2 , KK_GetFrom_High2;
_LOCAL plcbit KK_Pseudo_Calibr;

void _INIT ProgramInit(void)
{UINT i;

	blinkCntMax = 5;

	MAX_ERRROR = 5.0;
	KK_Pseudo_Calibr = 0;

	HMI_Calibr_Indx = 0;
	_hmi_Calibr_Indx = HMI_Calibr_Indx;
	HMI_Calibr_Resume_Btn_vis = INVISIBLE;

	visESTOP = INVISIBLE;
	
	

	HMI_MainTimerDT = 0;
	HMI_MainTimerSec = 0;

	MainTmr.PT = 10000000;

	AccessLevel = 0;

	HMI_Graph_Vis = INVISIBLE;
	HMI_Report_Vis = INVISIBLE;
	HMI_DiagPg_ProgrVer_vis = INVISIBLE;


	////////////////////////////////////////////////
	//  Files Operation
	/* Establish Variable File Name Extensions */
	strcpy(File_out.Extension[0].Name,	".NON");
	strcpy(File_out.Extension[1].Name,	".CSV");
	strcpy(File_out.Extension[2].Name,	".XML");	

	/* Default Extension Type */
	File_out.ExtensionIndex		= 3;


	/* Default Extension Type */
	File_out.ExtensionIndex		= 3;	

	/* XML File Init: */
	//XMLp_Init( &XMLFile, MAX_XML_FILE_SIZE );
	XMLFile.ParseInfo.IgnoreNotFound	= 1;
	XMLFile.DiagInfo.Logger			= 1;

	/* File Manager Initialization */
	FU_init( (UDINT) &FileMgr, 			"XML", 0 , 0 , 0, 	fuDRIVE_FILEIO, 	
			 (UDINT) &File_in,    		(UDINT) 	&File_out, 	"USB1", 	
			 (UDINT) FileNames,   		0, 			50);

	//strcpy(FileMgr.Device ,  "XML");
	strcpy(FileMgr.Device ,  "USB1");

	//ROM_USB = 1;

	sm_init(&Screen_sm);

	Screen.AccessLevel = 5;

	for(i = 0 ; i < 6 ; i++){
		HMI_AXIS_Disable[i] = 0;
		TML_MTR_INST[i].Disable = 0;
		VectorStateMach[i].simul = 0; 
	}


	///////////////////////////////

	

	ArcLink.robot.Cntrl.Cmd.AmpSP = ArcLink.robot.Cntrl.Cmd.AmpSP;
	ActCurrrentFbk = ActCurrrentFbk;

	


}

void _CYCLIC ProgramCyclic(void)
{
int  k , n;
UINT i;
USINT status[6];

	//////////////////////////////////////////////////////
	if (!ready)
	{
		VC_HANDLE = VA_Setup(1 , "Visu");
	   if (VC_HANDLE)
			ready = 1;
	}

	if (ready)
	{
	   if (!VA_Saccess(1,VC_HANDLE))
		{			
			VA_GetTouchAction (1,VC_HANDLE, 0, (UDINT)&Screen_touch);
			VA_Srelease(1,VC_HANDLE);	
		}
	}
	//////////////////////////////////////////////////////////////

	
	HMI_MachCfg_TestMode_vis = (AccessLevel > 0)? VISIBLE:INVISIBLE;
	
	HMI_AHC_Jog_Btns_locked = (Head.Remote_SW)? 0:1; 

	HMI_Lock_Seq_Stat_vis = ((Seq.State > 0) && (Seq.State <= 25) && (AccessLevel == 0))? VISIBLE:INVISIBLE;
	
	if(AccessLevel == 0)
		MachCfg.TestMode = 0;

	HMI_JogMode_Txt[0] = 0;
	
	switch(LocalJogMode){
		
			case MODE_TVL:				

				if(TravelJog.IN == 0)
					strcpy(HMI_JogMode_Txt , " TVL Selected"); 
				
			break;

			case MODE_OSC:
				
				if(OSCJog_Cmd == 0)
					strcpy(HMI_JogMode_Txt , " OSC Selected");

			break;

			case MODE_AHC:

				if(AHCJog.IN == 0)
					strcpy(HMI_JogMode_Txt , " AHC Selected");

			break;

			case MODE_WF:

				if(WFJog.IN == 0)
					strcpy(HMI_JogMode_Txt , " WF Selected");

			break;

			case MODE_INDX:

				if(IndexJog.IN == 0)
					strcpy(HMI_JogMode_Txt , " Indx Selected");

			break;

			case MODE_INDX_EXT:

				if(IndexExtJog.IN == 0)
					strcpy(HMI_JogMode_Txt , " Indx EXt Selected");

			break;


	}

	
	if(INDX_Step == 20)
		strcpy(HMI_JogMode_Txt , "INDX Limits !!!!");
	
	else if(TravelJog.IN != 0)
		strcpy(HMI_JogMode_Txt , "TVL Jog. ");
	else if(OSCJog_Cmd != 0)
		strcpy(HMI_JogMode_Txt , "OSC Jog. ");
	else if(AHCJog.IN != 0)
		strcpy(HMI_JogMode_Txt , "AHC Jog. ");
	else if(WFJog.IN != 0)
		strcpy(HMI_JogMode_Txt , "WF Jog.");
	else if(IndexJog.IN != 0){

		if(MachCfg.IndxExt)
			strcpy(HMI_JogMode_Txt , "Indx Ext. Jog.");
		else
			strcpy(HMI_JogMode_Txt , "Indx Jog.");

	}
	

	////////////////////////////////////
	
	HMI_Supervisor_vis = (AccessLevel > 0)?  VISIBLE:INVISIBLE;
	HMI_Test_Icon_vis = (MachCfg.Weld > 0)?  INVISIBLE:VISIBLE;
	
	////////////////	

	if(HMI_Reset_Errors){
		HMI_Reset_Errors = 0;		
		
	
		if(!HMI_Reset_Errors_vis){ // VISIBLE

			TVL_Start_Err = 0;
			ESAB_Tmo_Err = 0;

			AckAllErrors(&ErrorMgr);

			memset(ErrorMgr_Txt[0] , 0 , sizeof(ErrorMgr_Txt));
			
			HardInit_Step = 1;			
		
		}
	}
	//////////////////////////////////

	
	//////////////////////////////////

	// Fast Blink

	blinkCnt++;
	if(blinkCnt > blinkCntMax){
		blinkCnt = 0;
		blink = !blink;
	}
	


	/////////////////

	switch(Seq.State){

		case 200: // Local Mode

			Status_Color_Indx = 4; // Local Mode

		break;

		case 300: // Harware Reset

			Status_Color_Indx = 0;

		break;

		case 304: // Torch Brake
		case 301: // Hardware Error

			Status_Color_Indx = 1;	

		break;	

		case 302: // Estop

			Status_Color_Indx = 2;

		break;

		case 303: // Hardware Init

			Status_Color_Indx = 5;

		break;

		

		default:			
			
			if(Head.Remote_SW )
				Status_Color_Indx = 3; // Ready
			else
				Status_Color_Indx = 4; // Local Mode

		break;
		
	}

	///  TVL Direction

	if(HMI_Change_TVL_Dir_Btn){
		HMI_Change_TVL_Dir_Btn = 0;

		if(Seq.State == 0)
			TravelSpeed.IN = (TravelSpeed.IN == Forward)? Reverse:Forward;
	}
	
	if(MachCfg.GougingON){


		HMI_Left_Arrow_vis = (TravelSpeed.IN == Forward)? INVISIBLE:VISIBLE;
		HMI_Right_Arrow_vis = (TravelSpeed.IN == Forward)? VISIBLE:INVISIBLE;	

		HMI_Jog_Btns_locked = (Head.Remote_SW)? 0:1;
		HMI_TVL_Btns_vis = (Head.Remote_SW && (Seq.State == 0))? 0:1;


	}else{	

		switch(MachCfg.WeldMode){

			case 0: // Vessel

				HMI_Down_Arrow_vis = INVISIBLE;
				HMI_Up_Arrow_vis = INVISIBLE;
				
				HMI_Left_Arrow_vis = (TravelSpeed.IN == Forward)? INVISIBLE:VISIBLE;
				HMI_Right_Arrow_vis = (TravelSpeed.IN == Forward)? VISIBLE:INVISIBLE;	

				HMI_Jog_Btns_locked = (Head.Remote_SW && (Seq.State == 0))? 0:1;


			break;

			case 1: // Boiler
			case 2:

				HMI_Down_Arrow_vis = (TravelSpeed.IN == Forward)? INVISIBLE:VISIBLE;
				HMI_Up_Arrow_vis = (TravelSpeed.IN == Forward)? VISIBLE:INVISIBLE;

				HMI_Left_Arrow_vis = INVISIBLE;
				HMI_Right_Arrow_vis = INVISIBLE;

				HMI_Jog_Btns_locked = (Head.Remote_SW && (Seq.State == 0))? 0:1;


			break;

		}

	}

	
	
	if(MachCfg.GougingON){ // Gouging

		HMI_Pend_Stnd_vis = INVISIBLE;
		HMI_Pend_Gaug_vis = VISIBLE;
		HMI_Pend_NOT_Gaug_vis = INVISIBLE;
		

		HMI_WMode_5 = INVISIBLE;
		HMI_WMode_21 = INVISIBLE;
		HMI_WMode_22 = INVISIBLE;
		HMI_WMode_164 = INVISIBLE;
		HMI_WMode_170 = INVISIBLE;

		

	}else{
		
		HMI_Pend_Stnd_vis = VISIBLE;
		HMI_Pend_Gaug_vis = INVISIBLE;
		HMI_Pend_NOT_Gaug_vis = VISIBLE;
		
		switch(MachCfg.Weld_mode)
		{

			case 5:			

				HMI_WMode_5 = VISIBLE;
				HMI_WMode_21 = INVISIBLE;
				HMI_WMode_22 = INVISIBLE;
				HMI_WMode_164 = INVISIBLE;
				HMI_WMode_170 = INVISIBLE;
				
			break;

			case 21:			

				HMI_WMode_5 = INVISIBLE;
				HMI_WMode_21 = VISIBLE;
				HMI_WMode_22 = INVISIBLE;
				HMI_WMode_164 = INVISIBLE;
				HMI_WMode_170 = INVISIBLE;

			break;

			case 22:		

				HMI_WMode_5 = INVISIBLE;
				HMI_WMode_21 = INVISIBLE;
				HMI_WMode_22 = VISIBLE;
				HMI_WMode_164 = INVISIBLE;
				HMI_WMode_170 = INVISIBLE;

			break;

			case 160:
			case 161:
			case 162:
			case 163:
			case 164:
			case 165:
			case 166:
			case 167:		

				HMI_WMode_5 = INVISIBLE;
				HMI_WMode_21 = INVISIBLE;
				HMI_WMode_22 = INVISIBLE;
				HMI_WMode_164 = VISIBLE;
				HMI_WMode_170 = INVISIBLE;

			break;

			case 169:
			case 170:		
			case 171:
				
				HMI_WMode_5 = INVISIBLE;
				HMI_WMode_21 = INVISIBLE;
				HMI_WMode_22 = INVISIBLE;
				HMI_WMode_164 = INVISIBLE;
				HMI_WMode_170 = VISIBLE;

			break;


			default:
				
				HMI_WMode_5 = INVISIBLE;
				HMI_WMode_21 = INVISIBLE;
				HMI_WMode_22 = INVISIBLE;
				

			break;
		}

	}

	
	////////////////
	// Page logic

	////
	if (visPage != 5) {
		visMaint = VISIBLE;
	}
	visMaint = INVISIBLE;

	//HMI_LockedFWD_REV = (Indexer.Limit_Reverse || Indexer.Limit_Forward || (Seq.State == 1))? 1:0;
	HMI_LockedFWD_REV = ((Seq.State == 1))? 1:0;

	OldEdittBox = (NewEditBox == VISIBLE)? INVISIBLE: VISIBLE;

	DisplayInfo = (ModulesOK > 0)? 0:1;
	
	if(EditBox_Indx > (LastPermMem_Indx+1))
		EditBox_Indx = LastPermMem_Indx+1;

	

	EditBox_Vis = (EditBox_Indx > 0)? VISIBLE:INVISIBLE;

	if(EditBox_Indx > 0){
		HMI_Analog_Mem = &permMem.Analog_Out[EditBox_Indx-1];
		HMI_Analog_AO = &Analog_Out_Mod[EditBox_Indx-1];
	
	}else{

		if(!HMI_Analog_Mem || !HMI_Analog_AO ){
			HMI_Analog_Mem = &permMem.Analog_Out[0];
			HMI_Analog_AO = &Analog_Out_Mod[0];
		}

	}
	

	switch(EditBox.Cmd){

  
		case 1: // Minus High

			HMI_Analog_Mem->Value -= HMI_Analog_Mem->H_Incr;
			if(HMI_Analog_Mem->Value <= HMI_Analog_AO->Min_Val)
				HMI_Analog_Mem->Value = HMI_Analog_AO->Min_Val;

		break;

		case 2: // Minus Low

			HMI_Analog_Mem->Value -= HMI_Analog_Mem->L_Incr;
			if(HMI_Analog_Mem->Value <= HMI_Analog_AO->Min_Val)
				HMI_Analog_Mem->Value = HMI_Analog_AO->Min_Val;

		break;

		case 3: // Plus Low

			HMI_Analog_Mem->Value += HMI_Analog_Mem->L_Incr;
			if(HMI_Analog_Mem->Value > HMI_Analog_AO->Max_Val)
				HMI_Analog_Mem->Value = HMI_Analog_AO->Max_Val;

		break;

		case 4: // Plus High

			HMI_Analog_Mem->Value += HMI_Analog_Mem->H_Incr;
			if(HMI_Analog_Mem->Value > HMI_Analog_AO->Max_Val)
				HMI_Analog_Mem->Value = HMI_Analog_AO->Max_Val;

		break;

		case 5: // User Input

			if(HMI_Analog_Mem->Value > HMI_Analog_AO->Max_Val)
				HMI_Analog_Mem->Value = HMI_Analog_AO->Max_Val;
			
			else if(HMI_Analog_Mem->Value < HMI_Analog_AO->Min_Val)
				HMI_Analog_Mem->Value = HMI_Analog_AO->Min_Val;

		break;

	

	}
	EditBox.Cmd = 0;

	if(HMI_Analog_Mem && HMI_Analog_AO ){
		Inc_Minus_Btn_Vis = (HMI_Analog_Mem->Value > HMI_Analog_AO->Min_Val)? VISIBLE:INVISIBLE; 
		Inc_Plus_Btn_Vis = (HMI_Analog_Mem->Value < HMI_Analog_AO->Max_Val)? VISIBLE:INVISIBLE; 
	}

	WeldCalibr_Mode = 0;
	
	switch(visPage){

		case 1: // Pendant

			P_Pendant();
		
		break;

		case 2: // Jog
			P_Jog();
	
		break;

		case 3: // Setup

			 P_Setup();
			 
		break;

		
		case 4: // Camera

			P_Camera();
		
		break;

		case 5: // Maint.

			P_Maint();
		
		break;

		case 7: // spare

			P_Param1();


		break;

		case 8:
			

		break;

		case 9:

			P_Param2();

		break;

		case 10:

			P_Calib_0();

		break;

		case 15:
			
			P_Diagnostic();

		break;

		case 16: // Set Low Limit
			
			P_SetLimits();

		break;

		case 20:

			Main_Calib();

		break;
		
		case 21:

			WeldCalibr_Mode = 1;
			AMPS_Calib();

		break;


		case 40:

			SCR40_Recipe_1();


		break;


		case 200:// STANDBY		

			

		break;

	}

	if(visPage != 20){ //Main_Calib();

		//TVL_Calibr_Speed = 0.0;
		//WF_Calibr_Speed = 0.0;


	}

	PageNav();

	/*********RESET BUTTONS *********/	
	for(i = 0 ; i < 32 ; i++){ 
		Screen.btn[i] = 0;
	}	
	
	for(i = 0 ; i < 32 ; i++){ 
		Screen.btnBar[i] = 0;
	}
	
	Screen.btn_NO = 0;
	Screen.btn_YES= 0;
	Screen.Update = 0;	

	if(HMI_MainTmer_Start_btn){
		HMI_MainTmer_Start_btn = 0;
		MainTmr.IN = 1;
	}
	TimerExecute(&MainTmr);	

	sm_serv(&Screen_sm);
	FU_serv((UDINT) &FileMgr);


	///////////////////////////
	

}

void P_Pendant(){
double w1 , w2 , w3;

#define TravelSpeedAO_Indx 				0
#define ArcVoltsAO_Indx  				1
#define	WireFeedSpeedAO_Indx  			2
#define	AHCCurrentSPAO_Indx  			3  
#define	AHCToleranceAO_Indx  			4
#define	OSCSpeedAO_Indx  				5

	// Calculate Heat Input

	if(HMI_Report_Vis == VISIBLE){

		if(permMem.Analog_Out[TravelSpeedAO_Indx].Value > 0.0){


			/*
			w1 = permMem.Analog_Out[AHCCurrentSPAO_Indx].Value * permMem.Analog_Out[ArcVoltsAO_Indx].Value;			
			w2 = permMem.Analog_Out[WireFeedSpeedAO_Indx].Value / permMem.Analog_Out[TravelSpeedAO_Indx].Value;
			w3 = permMem.Analog_Out[Wire_Dim].Value / 2;
			w3 = w3 * w3;
			w3 = w3 * M_PI;

			HMI_PowerRatio_w1 = w1;
			HMI_PowerRatio_w2 = w2;
			HMI_PowerRatio_w3 = w3;
			
			HMI_PowerRatio = w1/(w2*w3) * 0.001;

			*/

			w1 = permMem.Analog_Out[AHCCurrentSPAO_Indx].Value * permMem.Analog_Out[ArcVoltsAO_Indx].Value;	
			w2 = permMem.Analog_Out[TravelSpeedAO_Indx].Value;

			HMI_PowerRatio_w1 = w1;
			HMI_PowerRatio_w2 = w2;
			
			HMI_PowerRatio = w1 * 60.0/w2; 

			

		}else{

			HMI_PowerRatio_w1 = 0.0;
			HMI_PowerRatio_w2 = 0.0;
			HMI_PowerRatio_w3 = 0.0;
			
			HMI_PowerRatio = 0.0;

		}

		
	}
	

	HMI_TVL_Jog_Cont = (TVL_Step == 13)? VISIBLE:INVISIBLE;
	
	Screen.Index = (MoveEnable)? 0:1;
	
	if((Seq.State > 0)){
		//&& (Seq.State < 30)){

		HMI_GEN_vis[0] = VISIBLE;    // Stop Btn
		HMI_GEN_vis[1] = INVISIBLE;  // Start Btn	
		
		//HMI_GEN_vis[2] = (MoveEnable && (TVL_Step >=15))? VISIBLE:INVISIBLE;  // Skip Weld
		//HMI_GEN_vis[2] = (MoveEnable && ((Seq.State == 1)||(Seq.State == 11)) && (TVL_Step == 16))? VISIBLE:INVISIBLE;  // Skip Weld
		HMI_GEN_vis[2] = (MoveEnable && ((Seq.State >= 1)&&(Seq.State <= 11)))? VISIBLE:INVISIBLE;  // Skip Weld

		HMI_GEN_vis[3] = (Seq.State < 100)? INVISIBLE:VISIBLE;
		//HMI_GEN_vis[3] = INVISIBLE;  // Weld Test On/Off

		// Gouging
		HMI_GEN_vis[4] = INVISIBLE;  // TVL Buttons 
		HMI_GEN_vis[5] = VISIBLE;  // Big INDX Buttons
		HMI_GEN_vis[6] = INVISIBLE;  // Small INDX Buttons 
		HMI_GEN_vis[7] = VISIBLE;  // AHC Buttons

		if(SKIP_Weld_Cmd){
			
			Screen.vis[0] = VISIBLE; // JOG AHC
			Screen.vis[1] = INVISIBLE; // JOG INDX
			
		}else{

			Screen.vis[0] = INVISIBLE; // JOG AHC
			Screen.vis[1] = VISIBLE; // JOG INDX

		}
	}
	else if((Seq.State == 0))
	{

		HMI_GEN_vis[0] = INVISIBLE;    // Stop Btn
		HMI_GEN_vis[1] = VISIBLE;  	// Start Btn

		HMI_GEN_vis[2] = INVISIBLE;  // Skip Weld

		HMI_GEN_vis[3] = VISIBLE;  // Weld Test On/Off
		
		// Gouging
		HMI_GEN_vis[4] = VISIBLE;  // TVL Buttons 
		HMI_GEN_vis[5] = INVISIBLE;  // Big INDX Buttons
		HMI_GEN_vis[6] = VISIBLE;  // Small INDX Buttons 
		HMI_GEN_vis[7] = VISIBLE;  // AHC Buttons

		
		Screen.vis[0] = VISIBLE; // JOG AHC
		Screen.vis[1] = INVISIBLE; // JOG INDX
		
	}	
	else{

		HMI_GEN_vis[0] = INVISIBLE;    // Stop Btn
		HMI_GEN_vis[1] = INVISIBLE;  	// Start Btn

		HMI_GEN_vis[2] = INVISIBLE;  // Skip Weld

		HMI_GEN_vis[3] = VISIBLE;  // Weld Test On/Off

		// Gouging
		HMI_GEN_vis[4] = INVISIBLE;  // TVL Buttons
		HMI_GEN_vis[5] = INVISIBLE;  // Big INDX Buttons
		HMI_GEN_vis[6] = INVISIBLE;  // Small INDX Buttons 
		HMI_GEN_vis[7] = INVISIBLE;  // AHC Buttons

		Screen.vis[0] = INVISIBLE; // JOG AHC
		Screen.vis[1] = INVISIBLE; // JOG INDX
	}

		if(MachCfg.AHC_BackStep && MachCfg.WeldMode 
			&& !MachCfg.GougingON){
			Screen.vis[2] = (AHC_MaxPos > -3.40282e+038)?  VISIBLE:INVISIBLE; // JOG INDX			
			Screen.vis[3] = (AHC_StartPos > -3.40282e+038)?  VISIBLE:INVISIBLE; // JOG INDX
			Screen.vis[4] = VISIBLE;
		}else {

			Screen.vis[2] = INVISIBLE;			
			Screen.vis[3] = INVISIBLE;			
			Screen.vis[4] = INVISIBLE;

		}


	//if(Seq.State > 0){

		HMI_AHC_FWD = (VectorStateMach[VECT_AX_AHC].InVal > 0.0)? VISIBLE:INVISIBLE;
		HMI_AHC_REV = (VectorStateMach[VECT_AX_AHC].InVal < 0.0)? VISIBLE:INVISIBLE;			

	//}



	////////   Arclink Status

	HMI_PS_Ready_vis = (ArcLink.robot.Cntrl.Fbk.Status & PS_Ready)? VISIBLE:INVISIBLE; 
	HMI_PS_Arc_OFF_vis = (ArcLink.robot.Cntrl.Fbk.Status & Arc_OFF)? VISIBLE:INVISIBLE;  
	HMI_PS_Arc_OCV_vis = (ArcLink.robot.Cntrl.Fbk.Status & Arc_OCV)? VISIBLE:INVISIBLE;  
	HMI_PS_Arc_Short_vis = (ArcLink.robot.Cntrl.Fbk.Status & Arc_Short)? VISIBLE:INVISIBLE;  
	HMI_PS_Arc_Active_vis = (ArcLink.robot.Cntrl.Fbk.Status & Arc_Active)? VISIBLE:INVISIBLE;  

	

}
void P_Jog(){
}
void P_Setup(){

	HMI_WeldMode_Setup_vis_lock = (SequenceRun_Out && (AccessLevel == 0))? VISIBLE:INVISIBLE;

	//HMI_MachCfg_TestMode_vis = (AccessLevel > 0)? VISIBLE:INVISIBLE;


	
	/////////////////////////////////////////////////////
	
}
void P_Camera(){
}

void P_SetLimits(){
DINT d;


	HMI_TVL_Jog_Cont = (TVL_Step == 13)? VISIBLE:INVISIBLE;

	switch(Page_Step){

		case 0:

			HMI_Low_Limit_Inp = CurCalib.TVL_Lo_Limit;
			HMI_High_Limit_Inp = CurCalib.TVL_Hi_Limit;
			HMI_High_Low_Dist_Inp = HMI_High_Limit_Inp - HMI_Low_Limit_Inp;

			HMI_GEN_vis[0] = INVISIBLE;
			HMI_GEN_vis[1] = INVISIBLE;

			Page_Step++;

		break;

		case 1:

			// Set New Encoder Zero
			if(HMI_GEN_Btn[0]){


			}
			else if(HMI_GEN_Btn[1]){ // Set New Low Limit				

				
				if(TML_MTR_INST[TML_AX_TVL].PosFBK < CurCalib.TVL_Hi_Limit){

					HMI_GEN_vis[0] = VISIBLE;
					HMI_GEN_vis[1] = VISIBLE;
				
					strcpy(HMI_GEN_txt , " Set Low Limit");

					Page_Step = 11;

				}
				else
					Page_Step = 15;

				
			}
			else if(HMI_GEN_Btn[2]){ // Set New Low Limit and High limit as well

				HMI_GEN_vis[0] = VISIBLE;
				HMI_GEN_vis[1] = VISIBLE;
			
				strcpy(HMI_GEN_txt , " Set New Zero at Current Pos.");

				Page_Step = 12;


			}
			else if(HMI_GEN_Btn[3]){ // Set New High Limit


				if(TML_MTR_INST[TML_AX_TVL].PosFBK > CurCalib.TVL_Lo_Limit){

					HMI_GEN_vis[0] = VISIBLE;
					HMI_GEN_vis[1] = VISIBLE;

					strcpy(HMI_GEN_txt , " Set High Limit");

					Page_Step = 13;	

				}
				else
					Page_Step = 15;

			}
			else if(HMI_GEN_Btn[4]){ // Set New High Limit and Low limit as well
			

			}
			else if(HMI_GEN_accomp[1]){ // New Low Limit Value entered


				if(HMI_Low_Limit_Inp < CurCalib.TVL_Hi_Limit ){

					CurCalib.TVL_Lo_Limit = HMI_Low_Limit_Inp;

					Page_Step = 0;

				}
				else
					Page_Step = 15;
			

			}
			else if(HMI_GEN_accomp[2]){ // New High Limit Value entered

				if(HMI_High_Limit_Inp > CurCalib.TVL_Lo_Limit){

					CurCalib.TVL_Hi_Limit = HMI_High_Limit_Inp;

					Page_Step = 0;

				}
				else
					Page_Step = 15;

			}


		break;


		case 11:

			if(HMI_GEN_Btn[6]){  // Yes

				CurCalib.TVL_Lo_Limit = TML_MTR_INST[TML_AX_TVL].PosFBK;

				Page_Step = 0;	

			}
			else if(HMI_GEN_Btn[7]) // Cancel					
				Page_Step = 0;					

		break;


		case 12:

			if(HMI_GEN_Btn[6]){  // Yes

				CurCalib.TVL_Hi_Limit -= TML_MTR_INST[TML_AX_TVL].PosFBK;				
				CurCalib.TVL_Lo_Limit -= TML_MTR_INST[TML_AX_TVL].PosFBK;
				
				AxisCh[TML_AX_TVL]._lastPos -= AxisCh[TML_AX_TVL].curPos; // This make current zero

				Page_Step = 0;	

			}
			else if(HMI_GEN_Btn[7]) // Cancel					
				Page_Step = 0;					

		break;


		case 13:

			if(HMI_GEN_Btn[6]){  // Yes

				CurCalib.TVL_Hi_Limit = TML_MTR_INST[TML_AX_TVL].PosFBK;

				Page_Step = 0;	

			}
			else if(HMI_GEN_Btn[7]) // Cancel					
				Page_Step = 0;					

		break;


		case 15:	

			HMI_GEN_vis[0] = VISIBLE;
			HMI_GEN_vis[1] = INVISIBLE;
			strcpy(HMI_GEN_txt , "Hig Limit Lower\nthen\nLower Limit");

			Page_Step++;
			
		break;			

		case 16:

			if(HMI_GEN_Btn[6]){  // Yes

				Page_Step = 0;	

			}
			else if(HMI_GEN_Btn[7]) // Cancel					
				Page_Step = 0;					

		break;

		default:

		break;

	}	
			
}



void P_Maint(){
}
void P_Diagnostic(){
UINT i , n;

	
	HMI_Reset_Errors_vis = ((((HardInit_Step == 100) || (HardInit_Step == 101)) && ESTOP) 
		|| (HardInit_Step < 50) || (AccessLevel >= 9) || TVL_Start_Err || ESAB_Tmo_Err)? VISIBLE:INVISIBLE;
	
	
	Misc_Reset_Btn_vis = (AccessLevel >= 9)? VISIBLE:INVISIBLE;

	for(i = 0 ; i < 6 ; i++){

		TML_MTR_INST[i].Disable = (HMI_AXIS_Disable[i] > 0)? 1:0;
		VectorStateMach[i].simul = (HMI_AXIS_Disable[i] > 0)? 1:0; 
		
	}	

	if(Screen.btn[0]){
		Screen.btn[0] = 0;

		if(KK_TVL_Test_step == 100)
			KK_TVL_Test_step = 1;

		else
			KK_TVL_Test_step = 0;
	}

	

	///////  New errors evaluation  /////////////////////////

	for( i = 0 ; i < SERVO_NUM ; i++){
		
		ErrorMgr_Txt[i][0] = 0;	


		if(TML_MTR_INST[i].Disable){
			xsprintf(ErrorMgr_Txt[i], "Drive %s: Not Used" , TML_MTR_INST[i].Name);

		}				
		else if((TML_MTR_INST[i].ErrorReg > 0)){			

			for(n=0 ; n < 16 ; n++ ){			
				
				if(GetBit((UINT)TML_MTR_INST[i].ErrorReg , n)){					
					MtrErrorText(ErrorMgr_Txt[i] , i , n+1);
					break;
				}
				
			}
			
			
		}
		else if(TML_MTR_INST[i].EStop){
			xsprintf(ErrorMgr_Txt[i], "Drive %s: ESTOP" , TML_MTR_INST[i].Name);

		}			
		else if(TML_MTR_INST[i].xCMD){

			if(blink || 1)
				xsprintf(ErrorMgr_Txt[i], "Drive %s: xCmd Process" , TML_MTR_INST[i].Name);
			else
				xsprintf(ErrorMgr_Txt[i], "Drive %s: xCmd" , TML_MTR_INST[i].Name);
		}
		else if(!TML_MTR_INST[i]._int.Init){

			xsprintf(ErrorMgr_Txt[i], "Drive %s: Not Init" , TML_MTR_INST[i].Name);
		}
		else{

			xsprintf(ErrorMgr_Txt[i], "Drive %s: OK" , TML_MTR_INST[i].Name);			
		}

		xsprintf(DriveVer_Txt[i], "Drive: %s Ver.: %s" , TML_MTR_INST[i].Name , UDINT2ASC(AxisCh[i].ProgramVersion));
		
	}
	
	
}

void P_Param1(){

	
	
	
}

void P_Param2(){
	
	
	
}

void P_Calib_0(){
USINT i;

	
	if(HMI_Calibr_NewStart_Btn 
		|| HMI_Calibr_Verify_Btn
		|| HMI_Calibr_Resume_Btn){

		if(HMI_Calibr_Resume_Btn)
			HMI_Calibr_Type_Select = 2;

		if(HMI_Calibr_NewStart_Btn)
			HMI_Calibr_Type_Select = 1;
		
		else if(HMI_Calibr_Verify_Btn)
			HMI_Calibr_Type_Select = 0;

		HMI_Calibr_NewStart_Btn = 0;
		HMI_Calibr_Verify_Btn = 0;
		HMI_Calibr_Resume_Btn = 0;


		switch(HMI_Calibr_Indx){


			case CALIBR_TVL_Indx: // TVL
			case CALIBR_WF_Indx: // WF
			
				NewPageReq_Cmd = 20;

				if(HMI_Calibr_Type_Select == 2) // Resume
					HMI_Calibr_Step = HMI_Calibr_Step - HMI_Calibr_Step%10;
				else
					HMI_Calibr_Step = 0;
				
				for(i = 0 ; i < 8 ; i++)
					 HMI_Calibr_What_vis[i] = INVISIBLE;

				 HMI_Calibr_What_vis[HMI_Calibr_Indx] = VISIBLE;
		
			break;

			case CALIBR_AMPS_Indx: // AMPS

				
				NewPageReq_Cmd = 21;

				
				if(HMI_Calibr_Type_Select == 2) // Resume
					HMI_Calibr_Step = HMI_Calibr_Step - HMI_Calibr_Step%10;
				else
					HMI_Calibr_Step = 0;
				
				
				for(i = 0 ; i < 8 ; i++)
					 HMI_Calibr_What_vis[i] = INVISIBLE;

				 HMI_Calibr_What_vis[HMI_Calibr_Indx] = VISIBLE;

			break;

		}			

	}

	if(_hmi_Calibr_Indx != HMI_Calibr_Indx){
		_hmi_Calibr_Indx = HMI_Calibr_Indx;
		HMI_Calibr_Resume_Btn_vis = INVISIBLE;

	}	


	switch(Page_Step){

		case 0:		
			
			Calcul_Slop_Bias(&CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);
			xsprintf(HMI_CalibrList[0],"WF   Calib. FWD - Slop: %6.4f / Bias: %6.4f" , HMI_Calibr_Slop , HMI_Calibr_Bias); 
			Calcul_Slop_Bias(&CurCalib.Points[AMPS_Calibr_Indx], CALIB_2POINTS);
			xsprintf(HMI_CalibrList[1],"AMPS Fbk Calib. - Slop: %6.4f / Bias: %6.4f" , HMI_Calibr_Slop , HMI_Calibr_Bias); 
			Calcul_Slop_Bias(&CurCalib.Points[TVL_Calibr_Indx], CALC_SLOPE_BOTH);
			xsprintf(HMI_CalibrList[2],"TVL Calib. FWD - Slop: %6.4f / Bias: %6.4f" , HMI_Calibr_Slop , HMI_Calibr_Bias); 
			xsprintf(HMI_CalibrList[3],"TVL Calib.  REV - Slop: %6.4f / Bias: %6.4f" , HMI_Calibr_Slop2 , HMI_Calibr_Bias2);
			
			Page_Step ++;

		break;


		case 1:


		break;


	}
	
}

void AMPS_Calib(){
UINT i;

	

	if(!KK_Pseudo_Calibr){

		if(HardInit_Step != 50){ // Hardware Not Ready	

			HardwareFaulted = 1;

			if((HMI_Calibr_Step%10) == 2) // Active Calibr - lunch Stop Cmd
				HMI_Calibr_Start_Stop = 1;
			
			
		}else if(HardwareFaulted){
			HardwareFaulted = 0;

			// Restart Step
			if(HMI_Calibr_Step < 10)
				HMI_Calibr_Step = 0;
			else		 
				HMI_Calibr_Step = HMI_Calibr_Step - (HMI_Calibr_Step%10);

		}		
	}

	KK_GetFrom_Low = getFromCalib(permMem.Analog_Out[AMPS_Calibr_Low].Value, &TempCalibr, CALIB_2POINTS);
	KK_GetFrom_High = getFromCalib(permMem.Analog_Out[AMPS_Calibr_High].Value, &TempCalibr, CALIB_2POINTS);

	HMI_Act_Amps = getFromCalib(ArcLink.robot.Cntrl.Fbk.curArcAmp , &TempCalibr , CALIB_2POINTS_INV);
	
	
	KK_GetFrom_Low2 = 0.0;
	KK_GetFrom_High2 = 0.0;
	
	if(!HMI_Calibr_Step_Done[0]
		&& !HMI_Calibr_Step_Done[1]
		&& !HMI_Calibr_Step_Done[2]
		&& !HMI_Calibr_Step_Done[3]
		&& !HMI_Calibr_Step_Done[4]
		&& (HMI_Calibr_Step < 60))
		HMI_Step_Vis[5] = VISIBLE;	

	//HMI_Calibr_User_Input_vis = (HMI_Calibr_Step > 10 && HMI_Calibr_Step < 50)? VISIBLE:INVISIBLE;	
	//HMI_Calibr_UserInput_Lock = ((HMI_Calibr_Step >= 10) && HMI_Calibr_Type_Select); 
	
	HMI_Step_Vis[7] = (HMI_Step_Vis[1] && HMI_Step_Vis[2])?	INVISIBLE:VISIBLE; // // Description of Steps 1/2
	HMI_Step_Vis[8] = (HMI_Step_Vis[3] && HMI_Step_Vis[4])?	INVISIBLE:VISIBLE; //  Description of Steps 3/4	
	
	
	switch(HMI_Calibr_Step){

		// Prepare parameters 
		case 1: 

			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				HMI_Step_Vis[i] = INVISIBLE;
				
			}
			
			HMI_Step_Color[0] = GREEN;
			HMI_Step_Vis[0] = VISIBLE;	
						
			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;
			HMI_Calibr_User_Input_vis = INVISIBLE;

			

			strcpy(HMI_Calibr_Intruct_TXT , "Prepare Calibr. \n Parameters");	


			TempCalibr.num = 8;
								
			TempCalibr.val[0] = 1.0;
			TempCalibr.val[1] = 1.0;
			TempCalibr.val[2] = 1000.0;
			TempCalibr.val[3] = 1000.0;

			
			TempCalibr.val[4] = 1.0;
			TempCalibr.val[5] = 1.0;
			TempCalibr.val[6] = 1000.0;
			TempCalibr.val[7] = 1000.0;


			if(HMI_Calibr_Type_Select == 1){ // New Calibration
				for(i = 0 ; i < 8 ; i++){
					HMI_Calibr_Step_Done[i] = 0xFF;
					HMI_Calibr_User_Inputs[i] = 0.0;				
					Calibr_Error[i] = 0.0;

				}
			}	

			HMI_Calibr_Step++;
			
	

		break;


		case 2:			


			switch(HMI_Calibr_Indx){

				case CALIBR_AMPS_Indx:
				//////////////////////////////////////////////////////////////////////////

					Calcul_Slop_Bias(&CurCalib.Points[AMPS_Calibr_Indx], CALIB_2POINTS);
					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					
					//Parameters Validation
					if(((permMem.Analog_Out[AMPS_Calibr_Low].Value + 0.0) < permMem.Analog_Out[AMPS_Calibr_High].Value)
						&& (permMem.Analog_Out[AMPS_Calibr_High].Value > 0.0)
						&& (permMem.Analog_Out[AMPS_Calibr_Low].Value > 0.0)){
						
					
						HMI_Calibr_StartBtn_vis = VISIBLE;
						strcpy(HMI_Calibr_Intruct_TXT , "Press Start Btn \n For Next Step");

						
						switch(HMI_Calibr_Type_Select){

							///////////////////////////////////////
							case 0: // Only Verify

								memcpy(&TempCalibr , &CurCalib.Points[AMPS_Calibr_Indx] , sizeof(Calib_Param_typ));
								
								HMI_Step_Vis[0] = INVISIBLE;
								HMI_Step_Vis[1] = VISIBLE;
								HMI_Step_Vis[2] = VISIBLE;
								HMI_Step_Vis[3] = VISIBLE;
								HMI_Step_Vis[4] = VISIBLE;
						
								HMI_Calibr_Step_Done[0] = 0;
						
								HMI_MainTimerDT = 0;			
								HMI_MainTimerSec = 0;				
							
						
								HMI_Step_Vis[1] = INVISIBLE;
								HMI_Step_Vis[3] = INVISIBLE;
				
								HMI_Calibr_Step_Done[1] = 0;
								HMI_Calibr_Step_Done[3] = 0;
				
								HMI_Calibr_Step = 20;


							break;
							///////////////////////////////////////
							case 1: // New Calibration


								if(HMI_Calibr_Start_Stop){
					
									HMI_Calibr_Start_Stop = 0;	

									HMI_Calibr_Resume_Btn_vis = VISIBLE;
							
									//HMI_Calibr_Type_Select = 1; 				
							
									HMI_Step_Vis[0] = INVISIBLE;
									HMI_Step_Vis[1] = VISIBLE;
									HMI_Step_Vis[2] = VISIBLE;
									HMI_Step_Vis[3] = VISIBLE;
									HMI_Step_Vis[4] = VISIBLE;
							
									HMI_Calibr_Step_Done[0] = 0;					
							
											
									HMI_MainTimerDT = 0;			
									HMI_MainTimerSec = 0;
							
									TempCalibr.num = 8;
					
									
									TempCalibr.val[0] = permMem.Analog_Out[AMPS_Calibr_Low].Value;
									TempCalibr.val[1] = permMem.Analog_Out[AMPS_Calibr_Low].Value;
									TempCalibr.val[2] = permMem.Analog_Out[AMPS_Calibr_High].Value;
									TempCalibr.val[3] = permMem.Analog_Out[AMPS_Calibr_High].Value;
					
					
									TempCalibr.val[4] = permMem.Analog_Out[AMPS_Calibr_Low].Value;
									TempCalibr.val[5] = permMem.Analog_Out[AMPS_Calibr_Low].Value;
									TempCalibr.val[6] = permMem.Analog_Out[AMPS_Calibr_High].Value;
									TempCalibr.val[7] = permMem.Analog_Out[AMPS_Calibr_High].Value;

									/*
									TempCalibr.val[0] = 10;
									TempCalibr.val[1] = 10;
									TempCalibr.val[2] = 20;
									TempCalibr.val[3] = 20;
							
							
									TempCalibr.val[4] = 10;
									TempCalibr.val[5] = 10;
									TempCalibr.val[6] = 20;
									TempCalibr.val[7] = 20;
									*/
					
									HMI_Step_Vis[1] = INVISIBLE;
									HMI_Step_Vis[3] = INVISIBLE;
					
									HMI_Calibr_Step_Done[1] = 0;
									HMI_Calibr_Step_Done[3] = 0;

									HMI_Calibr_Step = 20;

								}

							break;

							///////////////////////////////////////
							case 2: // Resume Calibration

								HMI_Step_Vis[0] = INVISIBLE;
								HMI_Step_Vis[1] = VISIBLE;
								HMI_Step_Vis[2] = VISIBLE;
								HMI_Step_Vis[3] = VISIBLE;
								HMI_Step_Vis[4] = VISIBLE;
						
								HMI_Calibr_Step_Done[0] = 0;
						
								HMI_MainTimerDT = 0;			
								HMI_MainTimerSec = 0;				
							
						
								HMI_Step_Vis[1] = INVISIBLE;
								HMI_Step_Vis[3] = INVISIBLE;
				
								HMI_Calibr_Step_Done[1] = 0;
								HMI_Calibr_Step_Done[3] = 0;				
								//HMI_Calibr_Step = 20;
							
							break;

						}					
					
					}
					else{
					
						strcpy(HMI_Calibr_Intruct_TXT , "WRONG Calibr. \n Parameters !!!!");
						HMI_Calibr_StartBtn_vis = INVISIBLE;
						HMI_Calibr_Step_Done[0] = 0xFF;
					
					}

					/////////////////////////////////////////////////////////////////////////

				break;


			}				

		break;
	

		///  Low Amps
		case 20:


			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}
			
			HMI_Step_Color[2] = GREEN;
			
			
			HMI_Calibr_StartBtn_vis = VISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			switch(HMI_Calibr_Indx){
				
				case CALIBR_AMPS_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);	

					HMI_Calibr_Error = Calibr_Error[2] * 100.0;

					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					
					strcpy(HMI_Calibr_Intruct_TXT , "Meas. Low Amps [A]\n");

				break;

			}	

			MainTmr.IN = 0;

			HMI_Calibr_Meas_Value = HMI_Calibr_User_Inputs[2];			
			
			HMI_Calibr_User_Input_vis = VISIBLE;
			HMI_Calibr_UserInput_Lock = 1;
						
			HMI_Calibr_Step++;
			
			
		break;
		
		case 21: // Wait for Start

			
			
			if(HMI_Calibr_New_User_Input){
				HMI_Calibr_New_User_Input = 0;

				
				HMI_Calibr_User_Inputs[2] = HMI_Calibr_Meas_Value;

				if(HMI_Calibr_Meas_Value > 0){


					switch(HMI_Calibr_Indx){						

						case CALIBR_AMPS_Indx:
							
								
							TempCalibr.val[1] = getFromCalib(permMem.Analog_Out[AMPS_Calibr_Low].Value, &TempCalibr, CALIB_2POINTS); // y2 - rev
							TempCalibr.val[0] =  HMI_Calibr_Meas_Value; // x2 - rev

							Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);					
							
							Calibr_Error[2] =  (TempCalibr.val[0] - permMem.Analog_Out[AMPS_Calibr_Low].Value)/permMem.Analog_Out[AMPS_Calibr_Low].Value;
												
							HMI_Calibr_Error = Calibr_Error[2] * 100.0;							
							HMI_Calibr_Step = 40;							

						break;

					}						

					if(fabsf(HMI_Calibr_Error) <= MAX_ERRROR)
						HMI_Calibr_Step_Done[2] = 0;					
					else
						HMI_Calibr_Step_Done[2] = 0xFF;

					if(	   !HMI_Calibr_Step_Done[1]
						&& !HMI_Calibr_Step_Done[2]
						&& !HMI_Calibr_Step_Done[3]
						&& !HMI_Calibr_Step_Done[4])
						HMI_Calibr_Step = 50;


				}

			}

			if(HMI_Calibr_Start_Stop){

				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = INVISIBLE;
				HMI_Calibr_StopBtn_vis = VISIBLE;
				
				HMI_Calibr_Meas_Value = 0.0;

				HMI_Calibr_User_Input_vis = INVISIBLE;
				HMI_Calibr_UserInput_Lock = 0;

				switch(HMI_Calibr_Indx){					

					case CALIBR_AMPS_Indx:

						HMI_MainTimer_Start_btn_vis = INVISIBLE;

						AMPS_Calibr_Val = getFromCalib(permMem.Analog_Out[AMPS_Calibr_Low].Value, &TempCalibr, CALIB_2POINTS);

					break;

				}	
				
				HMI_Calibr_Step++;
			}

		break;

		case 22: // Wait for Stop			

			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = VISIBLE;
				HMI_Calibr_StopBtn_vis = INVISIBLE;
				HMI_MainTimer_Start_btn_vis = INVISIBLE;

				HMI_Calibr_User_Input_vis = VISIBLE;
				HMI_Calibr_UserInput_Lock = 0;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;
				AMPS_Calibr_Val = 0.0;

				MainTmr.IN = 0;

				switch(HMI_Calibr_Indx){					

					case CALIBR_AMPS_Indx:

						HMI_Calibr_Meas_Value = permMem.Analog_Out[AMPS_Calibr_Low].Value ;

					break;

				}	
				
				HMI_Calibr_Step = 21;
			}

		break;

				
		///  High Amps
		case 40:

			
			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}
			
			HMI_Step_Color[4] = GREEN;
			
			
			HMI_Calibr_StartBtn_vis = VISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			
			switch(HMI_Calibr_Indx){				

				case CALIBR_AMPS_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);	

					HMI_Calibr_Error = Calibr_Error[4] * 100.0;

					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					
					strcpy(HMI_Calibr_Intruct_TXT , "Meas. High Amps [A]\n");
					

				break;

			}	

			MainTmr.IN = 0;

			HMI_Calibr_Meas_Value = HMI_Calibr_User_Inputs[4];	

			HMI_Calibr_User_Input_vis = VISIBLE;
			HMI_Calibr_UserInput_Lock = 1;
			
			HMI_Calibr_Step++;


		break;


		case 41: // Wait for Start

			
			if(HMI_Calibr_New_User_Input){
				HMI_Calibr_New_User_Input = 0;

				
				HMI_Calibr_User_Inputs[4] = HMI_Calibr_Meas_Value;

				if(HMI_Calibr_Meas_Value > 0){	

					switch(HMI_Calibr_Indx){
						
						case CALIBR_AMPS_Indx:							
								
							TempCalibr.val[3] = getFromCalib(permMem.Analog_Out[AMPS_Calibr_High].Value, &TempCalibr, CALIB_2POINTS); // y2 - rev
							TempCalibr.val[2] =  HMI_Calibr_Meas_Value ; // x2 - rev

							Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);					
							
							Calibr_Error[4] =  (TempCalibr.val[2] - permMem.Analog_Out[AMPS_Calibr_High].Value)/permMem.Analog_Out[AMPS_Calibr_High].Value;
												
							HMI_Calibr_Error = Calibr_Error[4] * 100.0;
							
							HMI_Calibr_Step = 20;							

						break;

					}	

					

					if(fabsf(HMI_Calibr_Error) <= MAX_ERRROR)
						HMI_Calibr_Step_Done[4] = 0;
					else
						HMI_Calibr_Step_Done[4] = 0xFF;


					if(	   !HMI_Calibr_Step_Done[1]
						&& !HMI_Calibr_Step_Done[2]
						&& !HMI_Calibr_Step_Done[3]
						&& !HMI_Calibr_Step_Done[4])
						HMI_Calibr_Step = 50;


				}

			}

			if(HMI_Calibr_Start_Stop){

				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = INVISIBLE;
				HMI_Calibr_StopBtn_vis = VISIBLE;

				HMI_Calibr_User_Input_vis = INVISIBLE;
				HMI_Calibr_UserInput_Lock = 0;

				HMI_Calibr_Meas_Value = 0.0;

				switch(HMI_Calibr_Indx){					

					case CALIBR_AMPS_Indx:
						
						AMPS_Calibr_Val = getFromCalib(permMem.Analog_Out[AMPS_Calibr_High].Value, &TempCalibr, CALIB_2POINTS);
						HMI_MainTimer_Start_btn_vis = INVISIBLE;
						
					break;

				}	
				
				HMI_Calibr_Step++;
			}

		break;

		case 42: // Wait for Stop			

			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = VISIBLE;
				HMI_Calibr_StopBtn_vis = INVISIBLE;
				HMI_MainTimer_Start_btn_vis = INVISIBLE;
				
				HMI_Calibr_User_Input_vis = VISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
				
				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;
				AMPS_Calibr_Val = 0.0;
				
				

				MainTmr.IN = 0;

				switch(HMI_Calibr_Indx){

					case CALIBR_AMPS_Indx:

						HMI_Calibr_Meas_Value = permMem.Analog_Out[AMPS_Calibr_High].Value;

					break;

				}					
			
				HMI_Calibr_Step = 41;
			}

		break;

		
		///  Calculate
		case 50:
			
			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}

			HMI_Step_Color[5] = GREEN;
			//HMI_Step_Vis[5] = VISIBLE;
			
			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = VISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;
			HMI_Calibr_User_Input_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			strcpy(HMI_Calibr_Intruct_TXT , "Press Calculate to \n Calculate New Calibr.");

			switch(HMI_Calibr_Indx){

				
				case CALIBR_AMPS_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);
					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					

				break;

			}	
 
			HMI_Calibr_Step++;


		break;


		case 51:

			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;

				switch(HMI_Calibr_Indx){					

					case CALIBR_WF_Indx:

						

					break;

				}	
 
				
				HMI_Calibr_Step = 60;

			}
			

		break;


		case 52:



		break;
		
		///  Save New Values
		case 60:

			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				HMI_Step_Vis[i] = INVISIBLE;
				
			}

			HMI_Step_Color[6] = GREEN;
			HMI_Step_Vis[0] = VISIBLE;
			HMI_Step_Vis[6] = VISIBLE;
			
			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = VISIBLE;
			HMI_Calibr_User_Input_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;
			
			strcpy(HMI_Calibr_Intruct_TXT , "Press Save to \n Save New Calibr.");

			switch(HMI_Calibr_Indx){				

				case CALIBR_AMPS_Indx:

					

				break;

			}	

			HMI_Calibr_Step++;

		break;


		case 61:


			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;

				HMI_Calibr_Resume_Btn_vis = INVISIBLE;

				switch(HMI_Calibr_Indx){					

					case CALIBR_AMPS_Indx:

						memcpy(&CurCalib.Points[AMPS_Calibr_Indx] , &TempCalibr , sizeof(Calib_Param_typ));

					break;

				}	

				HMI_Calibr_Step = 0;
			}

		break;
		
		////////////////////////////

		default:			
	
			HMI_Calibr_Step = 1;
			
		break;

	}

	if(HMI_Calibr_Type_Select == 0){// Verification only
		
		HMI_Calibr_User_Input_vis = INVISIBLE;

	}
	

	if(!KK_Pseudo_Calibr){

		if(HardInit_Step != 50){ // Hardware Not Ready

			TVL_Calibr_Speed = 0.0;
			WF_Calibr_Speed = 0.0;
			AMPS_Calibr_Val = 0.0;

			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;
			HMI_Calibr_User_Input_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			
		}
	}
}

void Main_Calib(){
UINT i;

	if(!KK_Pseudo_Calibr){

		if(HardInit_Step != 50){ // Hardware Not Ready	

			HardwareFaulted = 1;

			if((HMI_Calibr_Step%10) == 2) // Active Calibr - lunch Stop Cmd
				HMI_Calibr_Start_Stop = 1;
			
			
		}else if(HardwareFaulted){
			HardwareFaulted = 0;

			// Restart Step
			if(HMI_Calibr_Step < 10)
				HMI_Calibr_Step = 0;
			else		 
				HMI_Calibr_Step = HMI_Calibr_Step - (HMI_Calibr_Step%10);

		}		
	}

	
	switch(HMI_Calibr_Indx){

		case CALIBR_TVL_Indx:
			KK_GetFrom_Low = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS);
			KK_GetFrom_High = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS);
			KK_GetFrom_Low2 = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS_4OFF);
			KK_GetFrom_High2 = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS_4OFF);
		break;

		case CALIBR_WF_Indx:
			KK_GetFrom_Low = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS_4OFF);
			KK_GetFrom_High = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS_4OFF);
			KK_GetFrom_Low2 = 0.0;
			KK_GetFrom_High2 = 0.0;
		break;

	}			
	
	if(!HMI_Calibr_Step_Done[0]
		&& !HMI_Calibr_Step_Done[1]
		&& !HMI_Calibr_Step_Done[2]
		&& !HMI_Calibr_Step_Done[3]
		&& !HMI_Calibr_Step_Done[4]
		&& (HMI_Calibr_Step < 60))
		HMI_Step_Vis[5] = VISIBLE;	

	//HMI_Calibr_User_Input_vis = (HMI_Calibr_Step > 10 && HMI_Calibr_Step < 50)? VISIBLE:INVISIBLE;	
	//HMI_Calibr_UserInput_Lock = ((HMI_Calibr_Step >= 10) && HMI_Calibr_Type_Select); 

	HMI_Step_Vis[7] = (HMI_Step_Vis[1] && HMI_Step_Vis[2])?	INVISIBLE:VISIBLE; // // Description of Steps 1/2
	HMI_Step_Vis[8] = (HMI_Step_Vis[3] && HMI_Step_Vis[4])?	INVISIBLE:VISIBLE; //  Description of Steps 3/4	

	
	switch(HMI_Calibr_Step){

		// Prepare parameters 
		case 1: 

			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				HMI_Step_Vis[i] = INVISIBLE;
				
			}
			
			HMI_Step_Color[0] = GREEN;
			HMI_Step_Vis[0] = VISIBLE;	
						
			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;
			HMI_Calibr_User_Input_vis = INVISIBLE;

			

			strcpy(HMI_Calibr_Intruct_TXT , "Prepare Calibr. \n Parameters");	


			TempCalibr.num = 8;
								
			TempCalibr.val[0] = 1.0;
			TempCalibr.val[1] = 1.0;
			TempCalibr.val[2] = 1000.0;
			TempCalibr.val[3] = 1000.0;

			
			TempCalibr.val[4] = 1.0;
			TempCalibr.val[5] = 1.0;
			TempCalibr.val[6] = 1000.0;
			TempCalibr.val[7] = 1000.0;


			
			if(HMI_Calibr_Type_Select == 1){ // New Calibration
			
				for(i = 0 ; i < 8 ; i++){
					HMI_Calibr_Step_Done[i] = 0xFF;
					HMI_Calibr_User_Inputs[i] = 0.0;				
					Calibr_Error[i] = 0.0;

				}
			}			

			HMI_Calibr_Step++;
			
	

		break;


		case 2:			


			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:
					/////////////////////////////////////////////////////////////////////////////////////
					Calcul_Slop_Bias(&CurCalib.Points[TVL_Calibr_Indx], CALC_SLOPE_BOTH);
					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = VISIBLE;
			
					//Parameters Validation
					if((permMem.Analog_Out[TVL_Calibr_Dist].Value > 0.0)
						&&((permMem.Analog_Out[TVL_Calibr_Vel_Low].Value + 0.0) < permMem.Analog_Out[TVL_Calibr_Vel_High].Value)
						&& (permMem.Analog_Out[TVL_Calibr_Vel_High].Value > 0.0)
						&& (permMem.Analog_Out[TVL_Calibr_Vel_Low].Value > 0.0)){						

						HMI_Calibr_StartBtn_vis = VISIBLE;
						strcpy(HMI_Calibr_Intruct_TXT , "Press Start Btn \n For Next Step");

						switch(HMI_Calibr_Type_Select){

							///////////////////////////////////////
							case 0: // Only Verify

								memcpy(&TempCalibr , &CurCalib.Points[TVL_Calibr_Indx] , sizeof(Calib_Param_typ));
							
								HMI_Step_Vis[0] = INVISIBLE;
								HMI_Step_Vis[1] = VISIBLE;
								HMI_Step_Vis[2] = VISIBLE;
								HMI_Step_Vis[3] = VISIBLE;
								HMI_Step_Vis[4] = VISIBLE;

								HMI_Calibr_Step_Done[0] = 0;

								HMI_MainTimerDT = 0;			
								HMI_MainTimerSec = 0;

								MainTmr.PT = 10000000;

								HMI_Calibr_Step = 20;	
								

							break;

							///////////////////////////////////////
							case 1: // New Calibration

								if(HMI_Calibr_Start_Stop){

									HMI_Calibr_Start_Stop = 0;			

									HMI_Calibr_Resume_Btn_vis = VISIBLE;

									HMI_Step_Vis[0] = INVISIBLE;
									HMI_Step_Vis[1] = VISIBLE;
									HMI_Step_Vis[2] = VISIBLE;
									HMI_Step_Vis[3] = VISIBLE;
									HMI_Step_Vis[4] = VISIBLE;

									HMI_Calibr_Step_Done[0] = 0;
									

									MainTmr.PT = 10000000;
									HMI_MainTimerDT = 0;			
									HMI_MainTimerSec = 0;
							
									TempCalibr.num = 8;

									
									TempCalibr.val[0] = permMem.Analog_Out[TVL_Calibr_Vel_Low].Value;
									TempCalibr.val[1] = permMem.Analog_Out[TVL_Calibr_Vel_Low].Value;
									TempCalibr.val[2] = permMem.Analog_Out[TVL_Calibr_Vel_High].Value;
									TempCalibr.val[3] = permMem.Analog_Out[TVL_Calibr_Vel_High].Value;


									TempCalibr.val[4] = permMem.Analog_Out[TVL_Calibr_Vel_Low].Value;
									TempCalibr.val[5] = permMem.Analog_Out[TVL_Calibr_Vel_Low].Value;
									TempCalibr.val[6] = permMem.Analog_Out[TVL_Calibr_Vel_High].Value;
									TempCalibr.val[7] = permMem.Analog_Out[TVL_Calibr_Vel_High].Value;

									HMI_Calibr_Step = 20;
									
									
									/*
									TempCalibr.val[0] = 10;
									TempCalibr.val[1] = 10;
									TempCalibr.val[2] = 20;
									TempCalibr.val[3] = 20;


									TempCalibr.val[4] = 10;
									TempCalibr.val[5] = 10;
									TempCalibr.val[6] = 20;
									TempCalibr.val[7] = 20;
									*/

								}

							break;


							///////////////////////////////////////
							case 2: // Resume Calibration

								HMI_Step_Vis[0] = INVISIBLE;
								HMI_Step_Vis[1] = VISIBLE;
								HMI_Step_Vis[2] = VISIBLE;
								HMI_Step_Vis[3] = VISIBLE;
								HMI_Step_Vis[4] = VISIBLE;

								HMI_Calibr_Step_Done[0] = 0;

								HMI_MainTimerDT = 0;			
								HMI_MainTimerSec = 0;

								MainTmr.PT = 10000000;

								//HMI_Calibr_Step = 20;	

							break;

							}																			

						}	
						else{

							strcpy(HMI_Calibr_Intruct_TXT , "WRONG Calibr. \n Parameters !!!!");
							HMI_Calibr_StartBtn_vis = INVISIBLE;
							HMI_Calibr_Step_Done[0] = 0xFF;

						}

					 
					////////////////////////////////////////////////////
				break;

				case CALIBR_WF_Indx:
				//////////////////////////////////////////////////////////////////////////

					Calcul_Slop_Bias(&CurCalib.Points[WF_Calibr_Indx], CALIB_2POINTS);
					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					
					//Parameters Validation
					if((permMem.Analog_Out[WF_Calibr_Time].Value > 0.0)
						&&((permMem.Analog_Out[WF_Calibr_Vel_Low].Value + 0.0) < permMem.Analog_Out[WF_Calibr_Vel_High].Value)
						&& (permMem.Analog_Out[WF_Calibr_Vel_High].Value > 0.0)
						&& (permMem.Analog_Out[WF_Calibr_Vel_Low].Value > 0.0)){
						
					
						HMI_Calibr_StartBtn_vis = VISIBLE;
						strcpy(HMI_Calibr_Intruct_TXT , "Press Start Btn \n For Next Step");
					

						switch(HMI_Calibr_Type_Select){

							///////////////////////////////////////
							case 0: // Only Verify

								memcpy(&TempCalibr , &CurCalib.Points[WF_Calibr_Indx] , sizeof(Calib_Param_typ));
							
								HMI_Step_Vis[0] = INVISIBLE;
								HMI_Step_Vis[1] = VISIBLE;
								HMI_Step_Vis[2] = VISIBLE;
								HMI_Step_Vis[3] = VISIBLE;
								HMI_Step_Vis[4] = VISIBLE;
						
								HMI_Calibr_Step_Done[0] = 0;
						
								HMI_MainTimerDT = 0;			
								HMI_MainTimerSec = 0;				
							
						
								HMI_Step_Vis[1] = INVISIBLE;
								HMI_Step_Vis[3] = INVISIBLE;
				
								HMI_Calibr_Step_Done[1] = 0;
								HMI_Calibr_Step_Done[3] = 0;
				
								HMI_Calibr_Step = 20;

							break;

							///////////////////////////////////////
							case 1: // New Calibration

								if(HMI_Calibr_Start_Stop){
					
									HMI_Calibr_Start_Stop = 0;			
							
									HMI_Calibr_Type_Select = 1; 				
							
									HMI_Step_Vis[0] = INVISIBLE;
									HMI_Step_Vis[1] = VISIBLE;
									HMI_Step_Vis[2] = VISIBLE;
									HMI_Step_Vis[3] = VISIBLE;
									HMI_Step_Vis[4] = VISIBLE;
							
									HMI_Calibr_Step_Done[0] = 0;					
							
											
									HMI_MainTimerDT = 0;			
									HMI_MainTimerSec = 0;
							
									TempCalibr.num = 8;
					
									
									TempCalibr.val[0] = permMem.Analog_Out[WF_Calibr_Vel_Low].Value;
									TempCalibr.val[1] = permMem.Analog_Out[WF_Calibr_Vel_Low].Value;
									TempCalibr.val[2] = permMem.Analog_Out[WF_Calibr_Vel_High].Value;
									TempCalibr.val[3] = permMem.Analog_Out[WF_Calibr_Vel_High].Value;
					
					
									TempCalibr.val[4] = permMem.Analog_Out[WF_Calibr_Vel_Low].Value;
									TempCalibr.val[5] = permMem.Analog_Out[WF_Calibr_Vel_Low].Value;
									TempCalibr.val[6] = permMem.Analog_Out[WF_Calibr_Vel_High].Value;
									TempCalibr.val[7] = permMem.Analog_Out[WF_Calibr_Vel_High].Value;
					
									HMI_Step_Vis[1] = INVISIBLE;
									HMI_Step_Vis[3] = INVISIBLE;
					
									HMI_Calibr_Step_Done[1] = 0;
									HMI_Calibr_Step_Done[3] = 0;
					
									HMI_Calibr_Step = 20;
							
										
									
									/*
									TempCalibr.val[0] = 10;
									TempCalibr.val[1] = 10;
									TempCalibr.val[2] = 20;
									TempCalibr.val[3] = 20;
							
							
									TempCalibr.val[4] = 10;
									TempCalibr.val[5] = 10;
									TempCalibr.val[6] = 20;
									TempCalibr.val[7] = 20;
									*/
							
									
							
								}

								break;

								///////////////////////////////////////
								case 2: // Resume Calibration


									HMI_Step_Vis[0] = INVISIBLE;
									HMI_Step_Vis[1] = VISIBLE;
									HMI_Step_Vis[2] = VISIBLE;
									HMI_Step_Vis[3] = VISIBLE;
									HMI_Step_Vis[4] = VISIBLE;
							
									HMI_Calibr_Step_Done[0] = 0;
							
									HMI_MainTimerDT = 0;			
									HMI_MainTimerSec = 0;				
								
							
									HMI_Step_Vis[1] = INVISIBLE;
									HMI_Step_Vis[3] = INVISIBLE;
					
									HMI_Calibr_Step_Done[1] = 0;
									HMI_Calibr_Step_Done[3] = 0;

								break;
							}

						}

						else{
					
							strcpy(HMI_Calibr_Intruct_TXT , "WRONG Calibr. \n Parameters !!!!");
							HMI_Calibr_StartBtn_vis = INVISIBLE;
							HMI_Calibr_Step_Done[0] = 0xFF;
						
						}			
						

					/////////////////////////////////////////////////////////////////////////

					break;


				}				

		break;


		
	
		///  Low Speed REV
		case 10:


			for(i = 0 ; i < 8 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}
			
			HMI_Step_Color[1] = GREEN;		
			

			HMI_Calibr_StartBtn_vis = VISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:
					
					//Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS_4OFF);
					HMI_Calibr_Slop0_vis = INVISIBLE;
					HMI_Calibr_Slop1_vis = VISIBLE;			

					HMI_Calibr_Error = Calibr_Error[1] * 100.0;

					strcpy(HMI_Calibr_Intruct_TXT , "Measure Time [s]\n @ Low Speed");

				break;

				case CALIBR_WF_Indx:

					
				break;


			}				

			MainTmr.IN = 0;

			HMI_Calibr_Meas_Value = HMI_Calibr_User_Inputs[1];	

			HMI_Calibr_User_Input_vis = VISIBLE;
			HMI_Calibr_UserInput_Lock = 1;
			
			HMI_Calibr_Step++;
			
		break;
		
		case 11: // Wait for Start

			
			
			if(HMI_Calibr_New_User_Input){
				HMI_Calibr_New_User_Input = 0;

				
				HMI_Calibr_User_Inputs[1] = HMI_Calibr_Meas_Value;

				if(HMI_Calibr_Meas_Value > 0){			

					
					switch(HMI_Calibr_Indx){

						case CALIBR_TVL_Indx:
										
							TempCalibr.val[5] =  getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS_4OFF); // y1 - rev
							TempCalibr.val[4] =  (permMem.Analog_Out[TVL_Calibr_Dist].Value / (HMI_Calibr_Meas_Value / 60.0)); //x1 - rev
							

							Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS_4OFF);					
							
							Calibr_Error[1] =  (TempCalibr.val[4] - permMem.Analog_Out[TVL_Calibr_Vel_Low].Value)/permMem.Analog_Out[TVL_Calibr_Vel_Low].Value;
												
							HMI_Calibr_Error = Calibr_Error[1] * 100.0;

							HMI_Calibr_Step = 40;
							

						break;

						case CALIBR_WF_Indx:

							

						break;

					}	
					
					if(fabsf(HMI_Calibr_Error) <= MAX_ERRROR)
						HMI_Calibr_Step_Done[1] = 0;
					else
						HMI_Calibr_Step_Done[1] = 0xFF;

					if(	   !HMI_Calibr_Step_Done[1]
						&& !HMI_Calibr_Step_Done[2]
						&& !HMI_Calibr_Step_Done[3]
						&& !HMI_Calibr_Step_Done[4])
						HMI_Calibr_Step = 50;


				}

			}

			if(HMI_Calibr_Start_Stop){

				HMI_Calibr_Start_Stop = 0;
				
				HMI_Calibr_StartBtn_vis = INVISIBLE;
				HMI_Calibr_StopBtn_vis = VISIBLE;

				HMI_Calibr_Meas_Value = 0.0;

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:

						HMI_MainTimer_Start_btn_vis = VISIBLE;

						TVL_Calibr_Speed = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS_4OFF);	

						TVL_Calibr_Distance = -permMem.Analog_Out[TVL_Calibr_Dist].Value;

						
						MainTmr.PT = 10000000;
						MainTmr.IN = 1;

						//HMI_Calibr_Meas_Value = permMem.Analog_Out[TVL_Calibr_Dist].Value / getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS_4OFF) * 60.0;
			
					break;

					case CALIBR_WF_Indx:

						HMI_MainTimer_Start_btn_vis = INVISIBLE;

					break;

				}				
				
				HMI_Calibr_User_Input_vis = INVISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
				
				HMI_Calibr_Step++;
			}

		break;

		case 12: // Wait for Stop


			HMI_MainTimerDT = MainTmr.ET/1000;
			HMI_MainTimerSec = (MainTmr.ET%1000)/10;


			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:

					if(TVL_Calibr_Distance == 0.0) // Distance finished
						HMI_Calibr_Start_Stop = 1;
					
				break;

				case CALIBR_WF_Indx:

									
				break;

			}	

			if(HMI_Calibr_Start_Stop){
				
				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = VISIBLE;
				HMI_Calibr_StopBtn_vis = INVISIBLE;
				HMI_MainTimer_Start_btn_vis = INVISIBLE;

				HMI_Calibr_User_Input_vis = VISIBLE;
				HMI_Calibr_UserInput_Lock = 0;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;
				AMPS_Calibr_Val = 0.0;
				

				switch(HMI_Calibr_Indx){


					case CALIBR_TVL_Indx:
						
						HMI_Calibr_Meas_Value = permMem.Analog_Out[TVL_Calibr_Dist].Value / permMem.Analog_Out[TVL_Calibr_Vel_Low].Value * 60.0;						

					break;

					case CALIBR_WF_Indx:

						HMI_Calibr_Meas_Value = permMem.Analog_Out[WF_Calibr_Vel_Low].Value * (permMem.Analog_Out[WF_Calibr_Time].Value / 60.0);

					break;

				}	

				MainTmr.IN = 0;
			
				HMI_Calibr_Step = 11;
			}

		break;

		///  Low Speed FWD
		case 20:


			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}
			
			HMI_Step_Color[2] = GREEN;
			
			
			HMI_Calibr_StartBtn_vis = VISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			//if(HMI_Calibr_Type_Select)
				//HMI_Calibr_New_User_Input_vis = VISIBLE;			

			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);	

					HMI_Calibr_Error = Calibr_Error[2] * 100.0;

					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;					
										
					strcpy(HMI_Calibr_Intruct_TXT , "Measure Time [s]\n @ Low Speed");

				break;

				case CALIBR_WF_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);	

					HMI_Calibr_Error = Calibr_Error[2] * 100.0;

					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					
					strcpy(HMI_Calibr_Intruct_TXT , "Meas. Length [inch]\n @ Low Speed");

				break;

			}	

			MainTmr.IN = 0;

			HMI_Calibr_Meas_Value = HMI_Calibr_User_Inputs[2];			

			HMI_Calibr_User_Input_vis = VISIBLE;
			HMI_Calibr_UserInput_Lock = 1;
			
			HMI_Calibr_Step++;
			
			
		break;
		
		case 21: // Wait for Start

			
			
			if(HMI_Calibr_New_User_Input){
				HMI_Calibr_New_User_Input = 0;

				
				HMI_Calibr_User_Inputs[2] = HMI_Calibr_Meas_Value;

				if(HMI_Calibr_Meas_Value > 0){


					switch(HMI_Calibr_Indx){

						case CALIBR_TVL_Indx:

							TempCalibr.val[1] = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS); // y2 - rev
							TempCalibr.val[0] =  (permMem.Analog_Out[TVL_Calibr_Dist].Value / (HMI_Calibr_Meas_Value / 60.0)); // x2 - rev

							Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);					
							
							Calibr_Error[2] =  (TempCalibr.val[0] - permMem.Analog_Out[TVL_Calibr_Vel_Low].Value)/permMem.Analog_Out[TVL_Calibr_Vel_Low].Value;
												
							HMI_Calibr_Error = Calibr_Error[2] * 100.0;

							
							HMI_Calibr_Step = 10;		

						break;

						case CALIBR_WF_Indx:


							if(permMem.Analog_Out[WF_Calibr_Time].Value > 0){
								
								TempCalibr.val[1] = getFromCalib(permMem.Analog_Out[WF_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS); // y2 - rev
								TempCalibr.val[0] =  (HMI_Calibr_Meas_Value /(permMem.Analog_Out[WF_Calibr_Time].Value / 60.0)); // x2 - rev

								Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);					
								
								Calibr_Error[2] =  (TempCalibr.val[0] - permMem.Analog_Out[WF_Calibr_Vel_Low].Value)/permMem.Analog_Out[WF_Calibr_Vel_Low].Value;
													
								HMI_Calibr_Error = Calibr_Error[2] * 100.0;

								
								HMI_Calibr_Step = 40;

							}

							

						break;

					}						

					if(fabsf(HMI_Calibr_Error) <= MAX_ERRROR)
						HMI_Calibr_Step_Done[2] = 0;					
					else
						HMI_Calibr_Step_Done[2] = 0xFF;

					if(	   !HMI_Calibr_Step_Done[1]
						&& !HMI_Calibr_Step_Done[2]
						&& !HMI_Calibr_Step_Done[3]
						&& !HMI_Calibr_Step_Done[4])
						HMI_Calibr_Step = 50;


				}

			}

			if(HMI_Calibr_Start_Stop){

				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = INVISIBLE;
				HMI_Calibr_StopBtn_vis = VISIBLE;

				HMI_Calibr_Meas_Value = 0.0;	

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:
						
						HMI_MainTimer_Start_btn_vis = VISIBLE;
						TVL_Calibr_Speed = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS);
						TVL_Calibr_Distance = permMem.Analog_Out[TVL_Calibr_Dist].Value;
						
						MainTmr.PT = 10000000;
						MainTmr.IN = 1;

					break;

					case CALIBR_WF_Indx:

						HMI_MainTimer_Start_btn_vis = INVISIBLE;

						TimerStart(&MainTmr,permMem.Analog_Out[WF_Calibr_Time].Value);

						WF_Calibr_Speed = getFromCalib(permMem.Analog_Out[WF_Calibr_Vel_Low].Value, &TempCalibr, CALIB_2POINTS);

					break;

				}	

				HMI_Calibr_User_Input_vis = INVISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
				
				HMI_Calibr_Step++;
			}

		break;

		case 22: // Wait for Stop


			HMI_MainTimerDT = (MainTmr.ET)/1000;			
			HMI_MainTimerSec = (MainTmr.ET%1000)/10;


			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:


					if(TVL_Calibr_Distance == 0.0) // Distance finished
						HMI_Calibr_Start_Stop = 1;
					
					
				break;

				case CALIBR_WF_Indx:

					if(MainTmr.Q)
						HMI_Calibr_Start_Stop = 1;
				
				break;

			}	

			

			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = VISIBLE;
				HMI_Calibr_StopBtn_vis = INVISIBLE;
				HMI_MainTimer_Start_btn_vis = INVISIBLE;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;

				MainTmr.IN = 0;

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:
						
						HMI_Calibr_Meas_Value = permMem.Analog_Out[TVL_Calibr_Dist].Value / permMem.Analog_Out[TVL_Calibr_Vel_Low].Value * 60.0;						

					break;

					case CALIBR_WF_Indx:

						HMI_Calibr_Meas_Value = permMem.Analog_Out[WF_Calibr_Vel_Low].Value * (permMem.Analog_Out[WF_Calibr_Time].Value / 60.0);

					break;

				}	

				HMI_Calibr_User_Input_vis = VISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
				
				HMI_Calibr_Step = 21;
			}

		break;

		///  High Speed REV
		case 30:

			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}		
			
			HMI_Step_Color[3] = GREEN;
						
			HMI_Calibr_StartBtn_vis = VISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;
			//HMI_Calibr_New_User_Input_vis = VISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			HMI_Calibr_Meas_Value = HMI_Calibr_User_Inputs[3];

			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:

					HMI_Calibr_Error = Calibr_Error[3] * 100.0;									
					
					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS_4OFF);	

					HMI_Calibr_Slop0_vis = INVISIBLE;
					HMI_Calibr_Slop1_vis = VISIBLE;

					strcpy(HMI_Calibr_Intruct_TXT , "Measure Time [s]\n @ High Speed");
							

				break;

				case CALIBR_WF_Indx:

					

				break;

			}

			HMI_Calibr_User_Input_vis = VISIBLE;
			HMI_Calibr_UserInput_Lock = 1;

			HMI_Calibr_Step++;


		break;

		
		case 31: // Wait for Start

			
			
			if(HMI_Calibr_New_User_Input){
				HMI_Calibr_New_User_Input = 0;

				
				HMI_Calibr_User_Inputs[3] = HMI_Calibr_Meas_Value;

				if(HMI_Calibr_Meas_Value > 0){	

					switch(HMI_Calibr_Indx){

						case CALIBR_TVL_Indx:

							TempCalibr.val[7] = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS_4OFF); // y2 - rev
							TempCalibr.val[6] =  (permMem.Analog_Out[TVL_Calibr_Dist].Value / (HMI_Calibr_Meas_Value / 60.0)); // x2 - rev

							Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS_4OFF);					
							
							Calibr_Error[3] =  (TempCalibr.val[6] - permMem.Analog_Out[TVL_Calibr_Vel_High].Value)/permMem.Analog_Out[TVL_Calibr_Vel_High].Value;
												
							HMI_Calibr_Error = Calibr_Error[3] * 100.0;
									
							HMI_Calibr_Step = 20;
							
						break;

						case CALIBR_WF_Indx:

							

						break;

					}	

					if(fabsf(HMI_Calibr_Error) <= MAX_ERRROR)
						HMI_Calibr_Step_Done[3] = 0;
					else
						HMI_Calibr_Step_Done[3] = 0xFF;


					if(	   !HMI_Calibr_Step_Done[1]
						&& !HMI_Calibr_Step_Done[2]
						&& !HMI_Calibr_Step_Done[3]
						&& !HMI_Calibr_Step_Done[4])
						HMI_Calibr_Step = 50;


				}

			}

			if(HMI_Calibr_Start_Stop){

				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = INVISIBLE;
				HMI_Calibr_StopBtn_vis = VISIBLE;

				HMI_Calibr_Meas_Value = 0.0;	

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:
						
						HMI_MainTimer_Start_btn_vis = VISIBLE;

						TVL_Calibr_Speed = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS_4OFF);
						TVL_Calibr_Distance = -permMem.Analog_Out[TVL_Calibr_Dist].Value;
						
						MainTmr.PT = 10000000;
						MainTmr.IN = 1;
				
					break;

					case CALIBR_WF_Indx:

						HMI_MainTimer_Start_btn_vis = INVISIBLE;

					break;

				}	
				HMI_Calibr_User_Input_vis = INVISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
				HMI_Calibr_Step++;
			}

		break;

		case 32: // Wait for Stop


			HMI_MainTimerDT = MainTmr.ET/1000;
			HMI_MainTimerSec = (MainTmr.ET%1000)/10;

			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:

					if(TVL_Calibr_Distance == 0.0) // Distance finished
						HMI_Calibr_Start_Stop = 1;
					
				break;

				case CALIBR_WF_Indx:

									
				break;

			}	

			if(HMI_Calibr_Start_Stop){
				
				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = VISIBLE;
				HMI_Calibr_StopBtn_vis = INVISIBLE;
				HMI_MainTimer_Start_btn_vis = INVISIBLE;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:

						//HMI_Calibr_Meas_Value = permMem.Analog_Out[TVL_Calibr_Dist].Value / permMem.Analog_Out[TVL_Calibr_Vel_Low].Value * 60.0;
						HMI_Calibr_Meas_Value = permMem.Analog_Out[TVL_Calibr_Dist].Value / getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS_4OFF) * 60.0;

						//HMI_Step_Vis[2] = VISIBLE;								

					break;

					case CALIBR_WF_Indx:

						

					break;

				}					

				MainTmr.IN = 0;

				HMI_Calibr_User_Input_vis = VISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
			
				HMI_Calibr_Step = 31;
			}

		break;


		
		///  High Speed FWD
		case 40:

			
			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}
			
			HMI_Step_Color[4] = GREEN;
			
			
			HMI_Calibr_StartBtn_vis = VISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			//if(HMI_Calibr_Type_Select)
				//HMI_Calibr_New_User_Input_vis = VISIBLE;

			

			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);

					HMI_Calibr_Error = Calibr_Error[4] * 100.0;

					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;

					
					strcpy(HMI_Calibr_Intruct_TXT , "Measure Time [s]\n @ High Speed");
					

				break;

				case CALIBR_WF_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);	

					HMI_Calibr_Error = Calibr_Error[4] * 100.0;

					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					
					strcpy(HMI_Calibr_Intruct_TXT , "Meas. Length [inch]\n @ High Speed");
					

				break;

			}	

			MainTmr.IN = 0;

			HMI_Calibr_Meas_Value = HMI_Calibr_User_Inputs[4];	

			HMI_Calibr_User_Input_vis = VISIBLE;
			HMI_Calibr_UserInput_Lock = 1;
			
			HMI_Calibr_Step++;


		break;


		case 41: // Wait for Start

			
			if(HMI_Calibr_New_User_Input){
				HMI_Calibr_New_User_Input = 0;

				
				HMI_Calibr_User_Inputs[4] = HMI_Calibr_Meas_Value;

				if(HMI_Calibr_Meas_Value > 0){	

					switch(HMI_Calibr_Indx){

						case CALIBR_TVL_Indx:

							TempCalibr.val[3] = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS); // y2 - rev
							TempCalibr.val[2] =  (permMem.Analog_Out[TVL_Calibr_Dist].Value / (HMI_Calibr_Meas_Value / 60.0)); // x2 - rev

							Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);					
							
							Calibr_Error[4] =  (TempCalibr.val[2] - permMem.Analog_Out[TVL_Calibr_Vel_High].Value)/permMem.Analog_Out[TVL_Calibr_Vel_High].Value;
												
							HMI_Calibr_Error = Calibr_Error[4] * 100.0;

							HMI_Calibr_Step = 30;
									

						break;

						case CALIBR_WF_Indx:


							if(permMem.Analog_Out[WF_Calibr_Time].Value > 0){
								
								TempCalibr.val[3] = getFromCalib(permMem.Analog_Out[WF_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS); // y2 - rev
								TempCalibr.val[2] =  (HMI_Calibr_Meas_Value /(permMem.Analog_Out[WF_Calibr_Time].Value / 60.0)); // x2 - rev

								Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);					
								
								Calibr_Error[4] =  (TempCalibr.val[2] - permMem.Analog_Out[WF_Calibr_Vel_High].Value)/permMem.Analog_Out[WF_Calibr_Vel_High].Value;
													
								HMI_Calibr_Error = Calibr_Error[4] * 100.0;

								
								HMI_Calibr_Step = 20;

							}
							

						break;

					}	

					

					if(fabsf(HMI_Calibr_Error) <= MAX_ERRROR)
						HMI_Calibr_Step_Done[4] = 0;
					else
						HMI_Calibr_Step_Done[4] = 0xFF;


					if(	   !HMI_Calibr_Step_Done[1]
						&& !HMI_Calibr_Step_Done[2]
						&& !HMI_Calibr_Step_Done[3]
						&& !HMI_Calibr_Step_Done[4])
						HMI_Calibr_Step = 50;


				}

			}

			if(HMI_Calibr_Start_Stop){

				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = INVISIBLE;
				HMI_Calibr_StopBtn_vis = VISIBLE;

				HMI_Calibr_Meas_Value = 0.0;

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:

						HMI_MainTimer_Start_btn_vis = VISIBLE;
						TVL_Calibr_Speed = getFromCalib(permMem.Analog_Out[TVL_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS);
						TVL_Calibr_Distance = permMem.Analog_Out[TVL_Calibr_Dist].Value;
						
						MainTmr.PT = 10000000;
						MainTmr.IN = 1;

					break;

					case CALIBR_WF_Indx:

						WF_Calibr_Speed = getFromCalib(permMem.Analog_Out[WF_Calibr_Vel_High].Value, &TempCalibr, CALIB_2POINTS);
						HMI_MainTimer_Start_btn_vis = INVISIBLE;
						TimerStart(&MainTmr,permMem.Analog_Out[WF_Calibr_Time].Value);

					break;

				}	
				HMI_Calibr_User_Input_vis = INVISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
				HMI_Calibr_Step++;
			}

		break;

		case 42: // Wait for Stop

			HMI_MainTimerDT = (MainTmr.ET)/1000;			
			HMI_MainTimerSec = (MainTmr.ET%1000)/10;


			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:

					if(TVL_Calibr_Distance == 0.0) // Distance finished
						HMI_Calibr_Start_Stop = 1;
					
				break;

				case CALIBR_WF_Indx:

					if(MainTmr.Q)
						HMI_Calibr_Start_Stop = 1;
				
				break;

			}	

			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;
				HMI_Calibr_StartBtn_vis = VISIBLE;
				HMI_Calibr_StopBtn_vis = INVISIBLE;
				HMI_MainTimer_Start_btn_vis = INVISIBLE;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;
				

				MainTmr.IN = 0;

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:

						HMI_Calibr_Meas_Value = permMem.Analog_Out[TVL_Calibr_Dist].Value / permMem.Analog_Out[TVL_Calibr_Vel_High].Value * 60.0;
						

					break;

					case CALIBR_WF_Indx:

						HMI_Calibr_Meas_Value = permMem.Analog_Out[WF_Calibr_Vel_High].Value * (permMem.Analog_Out[WF_Calibr_Time].Value / 60.0);

					break;

				}					
				HMI_Calibr_User_Input_vis = VISIBLE;
				HMI_Calibr_UserInput_Lock = 0;
				HMI_Calibr_Step = 41;
			}

		break;

		
		///  Calculate
		case 50:
			
			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				//HMI_Step_Vis[i] = INVISIBLE;
				
			}

			HMI_Step_Color[5] = GREEN;
			//HMI_Step_Vis[5] = VISIBLE;
			
			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = VISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;
			//HMI_Calibr_New_User_Input_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			strcpy(HMI_Calibr_Intruct_TXT , "Press Calculate to \n Calculate New Calibr.");

			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALC_SLOPE_BOTH);
					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = VISIBLE;

									

				break;

				case CALIBR_WF_Indx:

					Calcul_Slop_Bias(&TempCalibr, CALIB_2POINTS);
					HMI_Calibr_Slop0_vis = VISIBLE;
					HMI_Calibr_Slop1_vis = INVISIBLE;
					

				break;

			}	
 
			HMI_Calibr_Step++;


		break;


		case 51:

			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:
						

					break;

					case CALIBR_WF_Indx:

						

					break;

				}	
 
				
				HMI_Calibr_Step = 60;

			}
			

		break;


		case 52:



		break;
		
		///  Save New Values
		case 60:

			for(i = 0 ; i < 16 ; i++){
				
				HMI_Step_Color[i] = GRAY;
				HMI_Step_Vis[i] = INVISIBLE;
				
			}

			HMI_Step_Color[6] = GREEN;
			HMI_Step_Vis[0] = VISIBLE;
			HMI_Step_Vis[6] = VISIBLE;
			
			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = VISIBLE;
			//HMI_Calibr_New_User_Input_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;
			
			strcpy(HMI_Calibr_Intruct_TXT , "Press Save to \n Save New Calibr.");

			switch(HMI_Calibr_Indx){

				case CALIBR_TVL_Indx:
					

				break;

				case CALIBR_WF_Indx:

					

				break;

			}	

			HMI_Calibr_Step++;

		break;


		case 61:


			if(HMI_Calibr_Start_Stop){
				HMI_Calibr_Start_Stop = 0;

				HMI_Calibr_Resume_Btn_vis = INVISIBLE;

				switch(HMI_Calibr_Indx){

					case CALIBR_TVL_Indx:
						
						memcpy(&CurCalib.Points[TVL_Calibr_Indx] , &TempCalibr , sizeof(Calib_Param_typ));						

					break;

					case CALIBR_WF_Indx:

						memcpy(&CurCalib.Points[WF_Calibr_Indx] , &TempCalibr , sizeof(Calib_Param_typ));

					break;

				}	

				HMI_Calibr_Step = 0;
			}

		break;
		
		////////////////////////////

		default:			
	
			HMI_Calibr_Step = 1;
			
		break;

	}

	if(HMI_Calibr_Type_Select == 0){// Verification only
		
		HMI_Calibr_UserInput_Lock = 1;

	}

	HMI_MainTimer_Start_btn_vis = INVISIBLE;

	if(!KK_Pseudo_Calibr){

		if(HardInit_Step != 50){ // Hardware Not Ready

			TVL_Calibr_Speed = 0.0;
			WF_Calibr_Speed = 0.0;
			AMPS_Calibr_Val = 0.0;
			
			HMI_Calibr_StartBtn_vis = INVISIBLE;
			HMI_Calibr_StopBtn_vis = INVISIBLE;
			HMI_MainTimer_Start_btn_vis = INVISIBLE;
			HMI_Calibr_CalculateBtn_vis = INVISIBLE;
			HMI_Calibr_SaveBtn_vis = INVISIBLE;
			//HMI_Calibr_New_User_Input_vis = INVISIBLE;
			HMI_Calibr_User_Input_vis = INVISIBLE;

			HMI_MainTimerDT = 0;			
			HMI_MainTimerSec = 0;

			
		}
	}	
}



void P_WF_Calib(){
	
	
	
}

void SCR40_Recipe_1(){

	//Screen.vis[0]  	= VISIBLE; 		// scr_Main_vis
	//Screen.vis[1] 	= INVISIBLE; 	// scr_Confirm_vis
	//Screen.vis[2]	= INVISIBLE;	// scr_Wait_vis
	//Screen.vis[3]	= INVISIBLE;	// scr_Selected File, OK , Cancel
	//Screen.vis[4]	= INVISIBLE;	// scr_FileMgr_Message				
	//Screen.color[0] = 15; 			//scr_Confirm_indx	

	switch(Screen_sm.Step){ 
					
		case 0: sm_text("Hold" , &Screen_sm);

			Screen.vis[0]  	= INVISIBLE; 	// scr_Main_vis
			Screen.vis[1] 	= VISIBLE; 		// scr_Confirm_vis	
			Screen.vis[2]	= VISIBLE;		// scr_Wait_vis
			Screen.vis[3]	= INVISIBLE;	// scr_Selected File, OK , Cancel
			Screen.vis[4]	= VISIBLE;		// scr_FileMgr_Message

			if(ROM_USB)
				strcpy(FileMgr.Device ,  "XML");
			else				
				strcpy(FileMgr.Device ,  "USB1");
				
			
			//if ( Screen.ReadPage == 40 &&	Screen.SetPage == 0xFFFF )	
			if((visPage == 40) && (visChangePage == 255))
				Screen_sm.Step = 4;
			
			
		break;

		case 4: sm_text("Open Directory" , &Screen_sm);

			Screen.vis[0]  	= INVISIBLE; 	// scr_Main_vis
			Screen.vis[1] 	= VISIBLE; 		// scr_Confirm_vis
			Screen.vis[2]	= VISIBLE;		// scr_Wait_vis
			Screen.vis[3]	= INVISIBLE;	// scr_Selected File, OK , Cancel
			Screen.vis[4]	= VISIBLE;		// scr_FileMgr_Message
			Screen.color[0] = 3; 			//Directory Refresh: scr_Confirm_indx
									

			File_out.OpenDirectory = 1;
			Screen_sm.Step = 5;

		break;


		case 5: sm_text("Opening Directory" , &Screen_sm);
			
			Screen.Index = 1;

			Screen.vis[2]	= blink;		// scr_Wait_vis
				

			if ( !File_out.OpenDirectory ) {	
				
				if ( File_in.Status > 0 ){					
					Screen.Index = 0;					
					Screen_sm.Step = 10;
					}
				else {
					Screen.Index = 0;					
					Screen_sm.Step = 10;
				}
				Screen.vis[2]	= INVISIBLE;	// scr_Wait_vis
			}

		break;

		/************************/		
		/************************/
		case 10: sm_text("Wait:" , &Screen_sm);		


				Screen.vis[0]  	= VISIBLE; 		// scr_Main_vis
				Screen.vis[1] 	= INVISIBLE; 	// scr_Confirm_vis
				Screen.vis[2]	= INVISIBLE;	// scr_Wait_vis
				
				strcpy( File_out.SelectedFile, File_in.FileNames[Screen.Index] );
			

				/* C.Decrement File Num: Cursor moves up */
				if ( Screen.btn[0] ) {
					if (Screen.Index > 0) {
						Screen.Index--;
						//strcpy( File_out.SelectedFile, File_in.FileNames[Screen.Index] );
					}	
					/* Reached Top */
					else if ( File_in.PageNum > 1 ) {
						File_out.PageUp 	= 1;	
						Screen_sm.Step = 71;
					}		
				}
				
				/* C.Decrement File Num: Cursor moves up */
				if ( Screen.btn[1] ) {
					if (Screen.Index < (File_in.TotalPageFileNum-1)) {
						Screen.Index++;
						//strcpy( File_out.SelectedFile, File_in.FileNames[Screen.Index] );
					}	
					/* Reached Bottom of Full Page */
					else if ( File_in.TotalPageFileNum == 10) {//FU_MAX_PAGE_FILES
						File_out.PageDown		= 1;	
						Screen_sm.Step = 70;
					}		
				}

				/* Page Up: Cursor moves up */
				if ( Screen.btn[2] ) {
					if ( Screen.Index == 1 ) {
						Screen.Index = 0;
						//strcpy( File_out.SelectedFile, File_in.FileNames[Screen.Index] );
					}
					else {
						File_out.PageUp 		= 1;	
						Screen_sm.Step = 71;							
					}	
				}

				/* Page Down: Cursor moves down */
				if ( Screen.btn[3] ) {
					if ( File_in.TotalPageFileNum < 10 ) { //FU_MAX_PAGE_FILES
						Screen.Index	= File_in.TotalPageFileNum -1;
						//strcpy( File_out.SelectedFile, File_in.FileNames[Screen.Index] );
					}
					else {
						File_out.PageDown		= 1;	
						Screen_sm.Step = 70;		
					}
				}

				/* Load File */	
				if (Screen.btn[4]) {

					Screen.vis[0]  	= INVISIBLE; 	// scr_Main_vis
					Screen.vis[1] 	= VISIBLE; 		// scr_Confirm_vis
					//Screen.vis[2]	= INVISIBLE;	// scr_Wait_vis
					Screen.vis[3]	= VISIBLE;		// scr_Selected File, OK , Cancel
					Screen.vis[4]	= INVISIBLE;	// scr_FileMgr_Message	
					Screen.color[0] = 0; 			//scr_Confirm_indx						
					
					Screen_sm.Step = 40;
				}
				/* Save File */	
				if ( Screen.btn[5] ) {

					Screen.vis[0]  	= INVISIBLE; 	// scr_Main_vis
					Screen.vis[1] 	= VISIBLE; 		// scr_Confirm_vis
					//Screen.vis[2]	= INVISIBLE;	// scr_Wait_vis
					Screen.vis[3]	= VISIBLE;		// scr_Selected File, OK , Cancel
					Screen.vis[4]	= INVISIBLE;	// scr_FileMgr_Message	
					Screen.color[0] = 1; 			//scr_Confirm_indx					
					
					File_out.SelectedFile[0] = 0;
					
					Screen_sm.Step = 50;				
				}

				/* Delete File */	
				if (Screen.btn[6] ) {
				
					Screen.vis[0]  	= INVISIBLE; 	// scr_Main_vis
					Screen.vis[1] 	= VISIBLE; 		// scr_Confirm_vis
					//Screen.vis[2]	= INVISIBLE;	// scr_Wait_vis
					Screen.vis[3]	= VISIBLE;		// scr_Selected File, OK , Cancel
					Screen.vis[4]	= INVISIBLE;	// scr_FileMgr_Message	
					Screen.color[0] = 2; 			//scr_Confirm_indx		
					
					Screen_sm.Step = 60;				
				}

				/* Return */	
				if (Screen.btn[7] ) {
					//HMI.ChangePage = 500;
									
				}
				/* Refresh Directory */	
				if ( Screen.btn[8] ) {
					Screen_sm.Step = 4;									
				}	

				/* Change Source */	
				if ( Screen.btn[9] ) {

					ROM_USB = !ROM_USB;
					Screen_sm.Step = 0;									
				}	
			
			
			/*******************************************************/
			
		break;

		/*********************************************************************************/
		
		/*********************************************************************************/
		/** READ XML FILE: **/
		case 40: sm_text("Wait: Confirm Load" , &Screen_sm);
			
			if(Screen.btn_YES){

				if(XMLp_Init( &XMLFile, MAX_XML_FILE_SIZE )){
				
					FU_setBufPointer(&FileMgr, XMLFile.pMessage , XMLFile.BufferSize);
					
					if ( File_in.Status > 0 ) 	{
						File_out.OpenFile 	= 1;
						Screen.vis[2]	= VISIBLE;	// scr_Wait_vis
						Screen.vis[3]	= INVISIBLE;	// scr_Selected File, OK , Cancel
						Screen.vis[4]	= VISIBLE;		// scr_FileMgr_Message	
						Screen_sm.Step++;
					}	
					else {
						Screen_sm.Step = 10;
					}
				}	
			}
			else if (Screen.btn_NO){

				Screen_sm.Step = 10;
				}
			

			
		break;	
			
		/*****************************/
		case 41: sm_text("Wait: Load XMLCfg", &Screen_sm);
			
			/** WAIT FILE OPERATION COMPLETE **/

			if(!File_out.OpenFile){				
			
				if (File_in.Status == 4) {	
				
					Screen_sm.Step++;
								
				}
				else if(File_in.Status < 0) {						
						Screen_sm.Step = 10;
				}				
			}
			
		break;

		case 42: sm_text("Wait: Parse XML", &Screen_sm);
			
			status_xml	= XMLp_CallParse(&XMLFile);	

			Screen_sm.Step++;			

		break;

		case 43: sm_text("Wait: Destroy XML buf" , &Screen_sm);
			
			if(XMLp_Destroy(&XMLFile)){
			
				if ( status_xml > 0 ) {
							
					Screen_sm.Step++;
				}
				else  {

					//Alarms[BAD_FORMAT] = 1; // Wrong File Format
					Screen_sm.Step = 10;
				}
			}

		break;	

		case 44: sm_text("Wait: done", &Screen_sm);			
			
			strcpy(RecipeName ,File_out.SelectedFile);
			Screen_sm.Step = 10;

		break;

		
		/*********************************************************************************/
		/** WRITE XML FILE: **/
		case 50: sm_text("Wait: Prepare XMLCfg Write", &Screen_sm);
		
			if(Screen.btn_YES){

				if ( CheckFileName(File_out.SelectedFile , FileMgr.Message)> 0 ) 	{
					strcpy( File_out.NewFileNameBase, File_out.SelectedFile);
					Screen.vis[2]	= VISIBLE;	// scr_Wait_vis
					Screen.vis[3]	= INVISIBLE;	// scr_Selected File, OK , Cancel
					Screen.vis[4]	= VISIBLE;		// scr_FileMgr_Message	
					Screen_sm.Step = 51;
				}	
				
					
			}
			else if (Screen.btn_NO){

				Screen_sm.Step = 10;
				}

		break;
		/*****************************/
		case 51: sm_text("Wait: Prepare XMLCfg Write", &Screen_sm);

			
			FU_setBufPointer(&FileMgr, XMLFile.pMessage , XMLFile.BufferSize);		
								
			if(XMLp_Init( &XMLFile, MAX_XML_FILE_SIZE )){
		
				FU_setBufPointer(&FileMgr, XMLFile.pMessage , XMLFile.BufferSize);			
				
				strcpy(sPV , "permMem");
				status_xml	= XMLp_CallCreate( &XMLFile, sPV);
				status_xml = 1;
				
				if ( status_xml > 0 ) {			
					File_out.FileSize			= XMLFile.CreateInfo.LengthInBytes;
					File_out.ExtensionIndex	= 3;		// XML 
					File_out.SaveAs			= 1;
					Screen_sm.Step++;
				}
				else {
					
					Screen_sm.Step = 10;
				}
			}
			
			
		break;
		/*****************************/
		case 52: sm_text("Wait: Write XMLConfig"  , &Screen_sm);

			
			
			if (!File_out.SaveAs) {
				
				if(XMLp_Destroy(&XMLFile)){	
				
					if ( File_in.Status >= 0 ) {									
						Screen_sm.Step = 4;
					}
					else {
						Screen_sm.Step = 10;
					}
				}				
				
			}
			
		break;

		/*********************************************************************************/
		/** Delete XML FILE: **/
		case 60: sm_text("Wait: Confirm Delete" , &Screen_sm);
		
			if(Screen.btn_YES){

				if ( File_in.Status > 0 ) 	{
					File_out.DeleteFile = 1;
					Screen.vis[2]	= VISIBLE;	// scr_Wait_vis
					Screen.vis[3]	= INVISIBLE;	// scr_Selected File, OK , Cancel
					Screen.vis[4]	= VISIBLE;	// scr_FileMgr_Message
					Screen_sm.Step = 61;
				}	
				else {
					Screen_sm.Step = 10;
					}
					
			}
			else if (Screen.btn_NO){

				Screen_sm.Step = 10;
			}
			

			
		break;	
			
		/*****************************/
		case 61: sm_text("Wait: Delete File" , &Screen_sm);
	
			/** WAIT FILE OPERATION COMPLETE **/
			if ((File_in.Status >= 0) && !File_out.OpenFile) {				
				Screen_sm.Step = 4;
				
			}
			else if((File_in.Status <0 ) && !File_out.OpenFile ) {
				Screen_sm.Step = 10;	
			}
			
			
		break;

		/***********************************************************/
		/***********************************************************/
		
		/* PageDown */
		case 70: sm_text("Please Wait: Returning Recipes" , &Screen_sm);
							
			if ( !File_out.PageDown ) {	
			
				Screen.Index	= 0;
				strcpy( File_out.SelectedFile, File_in.FileNames[Screen.Index] );
				
				Screen_sm.Step = 10;	
			}			
		break;
		/* PageUp */
		case 71: sm_text("Please Wait: Returing Recipes" , &Screen_sm);
			
				
			if ( !File_out.PageUp ) {
			
				Screen.Index = 10 -1; // FU_MAX_PAGE_FILES
				strcpy( File_out.SelectedFile, File_in.FileNames[Screen.Index] );
				
				Screen_sm.Step = 10;	
			}			
		break;

		
		/***********************************************************/
		/***********************************************************/
		case 100:
		break;

	
	}
}



void PageNav(){
USINT i;

	if(visChangePage == 255){

		//if(NewPageReq_Cmd)
			//HMI_Graph_Vis = INVISIBLE;

		// return from Diagnostic or Set Limits page
		if((NewPageReq_Cmd ==  15) // diagnostic
			|| ((NewPageReq_Cmd ==  16)) // Set TVL Limits
			|| ((NewPageReq_Cmd ==  200))){ // StanBy

			if(NewPageReq_Cmd == visPage){
				NewPageReq_Cmd = LastPageIndx; // return to last page

				if(NewPageReq_Cmd == 20){ // Calibration
					
					// Restart Step
					if(HMI_Calibr_Step < 10)
						HMI_Calibr_Step = 0;
					else		 
						HMI_Calibr_Step = HMI_Calibr_Step - (HMI_Calibr_Step%10);

				}

			}			

		}
			
		// Navigate from Splash Scree
		if(visPage == 0)
			NewPageReq_Cmd = 1;

		// Watch main menu pressed	
		for(i = 1; i < 11 ; i++){
			
			if(PageBtnPress[i] && !_PageBtnPress[i]){
				NewPageReq_Cmd = i;
				EditBox_Indx = 0;
			}
			else {

				if(i== visPage )
					PageBtnPress[i] = 1;
				else
					PageBtnPress[i] = 0;
			}		
			
			_PageBtnPress[i] = PageBtnPress[i]; 
		}				
		

		if(NewPageReq_Cmd > 0){	
			
			visChangePage = NewPageReq_Cmd;

			LastPageIndx = visPage;

			HMI_Graph_Vis = INVISIBLE;
			HMI_Report_Vis = INVISIBLE;
			HMI_DiagPg_ProgrVer_vis = INVISIBLE;


			///  Stop any motion

			if((NewPageReq_Cmd == 20)
				|| (LastPageIndx == 20)){ // Jump to or from  Calibration
				
				//SequenceRun_Out = 0;

				Sequence_Halt_Cmd = 1;
				
				TravelJog.IN = Stop;			
				IndexJog.IN = Stop;
				AHCJog.IN = Stop;

				INDX_Step_Cmd = 0;
				
				OSCJog_Cmd = 0;

				WFJog.IN = Stop;

				WF_Calibr_Speed = 0.0;
				TVL_Calibr_Speed = 0.0;
					
				///////////////

			}

			Page_Step = 0;

			for(i = 1 ; i < 11 ; i++)
				PageBtnPress[i] = 0;	

			if(NewPageReq_Cmd < 11)
				PageBtnPress[NewPageReq_Cmd] = 1;

			Screen_sm.Step = 0;
						
		}	

		NewPageReq_Cmd = 0;

		for(i = 1; i < 8 ; i++){
			HMI_GEN_Btn[i] = 0;			
			HMI_GEN_accomp[i] = 0;
		}

		
	}

	
} 



void xstrcpy(UINT len , char* out , char* in){
UINT i;  

		if(!out || !in || (len == 0))
			return;

		for(i = 0 ; i < len ; i++){

			if(in[i] != 0)
				out[i] = in[i];
			
			else{
				out[i] = in[i];	
				break;
			}
		}

		if(i== len)
			out[i-1] = 0;
	
	return;

}

void MtrErrorText(char* txt , UDINT i , UDINT val ){

plcstring errortxt[64];



	xsprintf(txt, "Drive %s: " , TML_MTR_INST[i].Name);
	


	switch(val){


		case 1: strcat(txt , "CANbus Error"); 			break;

		case 2: strcat(txt , "Short-Circuit Error"); 		break;

		case 3: strcat(txt , "Invalid Setup Error"); 		break;

		case 4: strcat(txt , "Control (Position) Error "); 		break;

		case 5: strcat(txt , "Serial Comm Error"); 		break;

		case 6: strcat(txt , "Feedback Error"); 		break;

		case 7: strcat(txt , "POT Limit Trig."); 			break;

		case 8: strcat(txt , "NOT Limit Trig."); 			break;

		case 9: strcat(txt , "Instantaneous Over-Current Err."); 			break;

		case 10: strcat(txt , "I2t Over-Current Error"); 					break;

		case 11: strcat(txt , "Over-Temperature Error - Motor"); 	break;

		case 12: strcat(txt , "Over-Temperature Error - Drive"); 	break;

		case 13: strcat(txt , "Over-Voltage Error"); 		break;

		case 14: strcat(txt , "Under-Voltage Error"); 		break;

		case 15: strcat(txt , "Command Error"); 		break;

		case 16: strcat(txt , "Enable Inactive"); 	break;

		case 17: strcat(txt , "Init Error"); 	break;

		case 18: strcat(txt , "No Comunnication"); 	break;

		default: xsprintf(txt+strlen(txt), "Unknown Error %X " , val);

	}


}


void Calcul_Slop_Bias(Calib_Param_typ *cal , UINT type){
USINT offset;

	switch(type){

		case CALIB_2POINTS:

			offset = 0;
			
		break;
	

		case CALIB_2POINTS_4OFF:

			offset = 4;

		break;


		case CALC_SLOPE_BOTH:

			offset = 4;

			if((cal->val[2+offset] != cal->val[0+offset])){
				
				HMI_Calibr_Slop2 = (cal->val[3+offset] - cal->val[1+offset])/(cal->val[2+offset] - cal->val[0+offset]);
				HMI_Calibr_Bias2 = cal->val[3+offset] - HMI_Calibr_Slop2* cal->val[2+offset];				
				HMI_Calibr_Slop2 = HMI_Calibr_Slop2 *100;
				
			}else{

				HMI_Calibr_Slop2 = 0;
				HMI_Calibr_Bias2 = 0;
			}

			offset = 0;

			
		break;

		

		default:

			HMI_Calibr_Slop = 0;
			HMI_Calibr_Bias = 0;

			return;

		break;
	}


	if((cal->val[2+offset] != cal->val[0+offset])){
				
		HMI_Calibr_Slop = (cal->val[3+offset] - cal->val[1+offset])/(cal->val[2+offset] - cal->val[0+offset]);
		HMI_Calibr_Bias = cal->val[3+offset] - HMI_Calibr_Slop* cal->val[2+offset];

		HMI_Calibr_Slop = HMI_Calibr_Slop *100.0;

	}
	else{

		
		HMI_Calibr_Slop = 0;
		HMI_Calibr_Bias = 0;

	}

	return;

}

int  CheckFileName( char *str , char *msg) {
int i , ret;

	ret = 1;

	for(i=0; i < 20 ; i++){

		if(*(str+i) != 0){
			switch(*(str+i)){

				case ' ':					
					ret=-1;
					*(str+i) = '_';
				break;
				
				case '#':
					ret=-2;
					strcpy(msg , "Illegal char - #");
				break;	

				case '/':
					ret=-3;
					strcpy(msg , "Illegal char - /");
				break;	

				default:
				break;				
			}			
		}
		else
			break;
		
	}

	if(strlen(str)==0){
		ret = -4;
		strcpy(msg , "Empty file name !!!");
	}

	return ret;
	
}


