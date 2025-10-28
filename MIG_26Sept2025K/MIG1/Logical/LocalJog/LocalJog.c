
#include <bur/plctypes.h>


#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

#include <..\Lib\Statelib.c>
#include <..\Lib\LoggerLib.c>
#include <..\Lib\WSI_lib.c>



_LOCAL TON_typ JogMode_Tmr;
_LOCAL float ModeTmo;

_LOCAL plcbit SetLED;

_LOCAL USINT _LocalJogMode;
USINT JogFwdRev(plcbit fwd , plcbit rev);
void LED_Step_Seq();


_LOCAL BOOL blink , _blink;
BOOL _blink;
_LOCAL UINT blinkOnTmr , blinkOffTmr;

_LOCAL TON_typ LED_Tmr;
_LOCAL UINT LED_Indx , LED_Step ;
UINT _LED_Indx;

_LOCAL plcbit LED_Static_Style;

plcbit _Remote_SW;
plcbit _SR_Done;
plcbit _LocalJog_FWD , _LocalJog_REV;

USINT _indx_Limit_Bypass_Btn;

_LOCAL plcbit HMI_INDX_Inp2 , HMI_INDX_Inp3 , HMI_INDX_Out2;

_LOCAL plcbit KK_Block_LED , KK_AllowAll_LED;
_LOCAL float KK_Blink;

void _INIT ProgramInit(void)
{

	KK_Block_LED = 0;
	KK_AllowAll_LED = 0;
	KK_Blink = 0.5;
	
	Head.Remote_SW = 1;	
	ModeTmo = 15.0;

	HardwInit_Done = 0;
	LED_Static_Style = 1;

	Glbl_TML_CMD_Delay = Glbl_TML_CMD_Delay;
}

void _CYCLIC ProgramCyclic(void)
{

USINT tml_indx ; 

/*
#define MODE_NONE 	0
#define MODE_TVL 	1
#define MODE_OSC 	2
#define MODE_AHC 	3
#define MODE_WF 	4
#define MODE_INDX 	5

TravelJog.IN
OSCJog_Cmd
AHCJog.IN
WFJog.IN
IndexJog.IN

*/


/** Blink **/
if(blinkOnTmr++ < 150)		blink = 1;	
else if(blinkOffTmr++ < 300)	blink = 0;
else{ blinkOnTmr = 0; blinkOffTmr = 0;}

////////////////////////////////////////////////////////
if(HardwReady){

	if(!HardwInit_Done){
		LED_Step = 0;
		LED_CMD = 20;
		_Remote_SW = Head.Remote_SW;
	}	
	
	HardwInit_Done = 1;

}


if(HardwInit_Done && HardwReady){


	Head.Remote_SW = !TML_MTR_INST[TML_AX_WF].DigInp[1];
	//Head.Remote_SW = 1;

	Head.LocalJog_FWD = !TML_MTR_INST[TML_AX_OSC].DigInp[1];
	Head.LocalJog_REV = !TML_MTR_INST[TML_AX_OSC].DigInp[0];


	
	if(LocalJogMode != MODE_NONE){

		if(Head.LocalJog_FWD || Head.LocalJog_REV)
			TimerStart(&JogMode_Tmr, ModeTmo);

	}

	
	if((Seq.State > 0) && (Seq.State < 25))
			TimerStart(&JogMode_Tmr, ModeTmo);
	
	
	if(!TML_MTR_INST[TML_AX_TVL].DigInp[0]){
		LocalJogMode = MODE_TVL;
		TimerStart(&JogMode_Tmr, ModeTmo);
	}

	else if(!TML_MTR_INST[TML_AX_INDX_EXT].DigInp[1]){
		LocalJogMode = MODE_OSC;
		TimerStart(&JogMode_Tmr, ModeTmo);
	}

	else if(!TML_MTR_INST[TML_AX_TVL].DigInp[1]){
		LocalJogMode = MODE_AHC;
		TimerStart(&JogMode_Tmr, ModeTmo);
	}

	else if(!TML_MTR_INST[TML_AX_INDX].DigInp[1]){
		LocalJogMode = MODE_WF;
		TimerStart(&JogMode_Tmr, ModeTmo);
	}

	else if(!TML_MTR_INST[TML_AX_INDX].DigInp[0] && (!MachCfg.IndxExt)){
		LocalJogMode = MODE_INDX;
		TimerStart(&JogMode_Tmr, ModeTmo);
	}

	else if(!TML_MTR_INST[TML_AX_INDX_EXT].DigInp[0] && (MachCfg.IndxExt)){
		LocalJogMode = MODE_INDX_EXT;
		TimerStart(&JogMode_Tmr, ModeTmo);
	}

	else if(TimerIsTimeOut(&JogMode_Tmr) && (OSC_Step != 10))
		LocalJogMode = MODE_NONE;
		


}
else {

	Head.Remote_SW = 0;	
	Head.LocalJog_FWD = 0;
	Head.LocalJog_REV = 0;
	
	return;

}



	
	if(Head.Remote_SW)
		LocalJogMode = MODE_NONE;

	if(Head.Remote_SW && !_Remote_SW)
		//LED_CMD = 40;
		LED_CMD = 10;
	else if(!Head.Remote_SW && _Remote_SW)
		//LED_CMD = 30;
		LED_CMD = 20;

/*
	if(Head.Remote_SW != _Remote_SW){

		OSCJog = Stop;

		OSCJog_Cmd = Stop;

		TravelJog.IN = Stop;

		AHCJog.IN = Stop;

		WFJog.IN = Stop;

		IndexJog.IN = Stop;

	}
	*/

	_Remote_SW = Head.Remote_SW;

//////////  Override POT & NOT on INDX motor

	/*
	if(Head.Remote_SW || !HardwInit_Done || (AccessLevel < 0)){

		INDX_Limit_Bypass_Btn = 0;
		_indx_Limit_Bypass_Btn = INDX_Limit_Bypass_Btn;


	}		
	*/
	
	if(MachCfg.IndxExt)	
		tml_indx = TML_AX_INDX_EXT;
	else
		tml_indx = TML_AX_INDX;

	HMI_INDX_Inp2 = TML_MTR_INST[tml_indx].DigInp[2];
	HMI_INDX_Inp3 = TML_MTR_INST[tml_indx].DigInp[3];

	HMI_INDX_Out2 = TML_MTR_INST[tml_indx].DigOut_stat[2];

	if(INDX_Limit_Bypass_Btn && !_indx_Limit_Bypass_Btn){
		

		if(TML_MTR_INST[tml_indx].xCMD == TML_IDLE){
				TML_MTR_INST[tml_indx].xCMD 	= TML_RST_OUT3_CMD; 	

			_indx_Limit_Bypass_Btn = INDX_Limit_Bypass_Btn;

		}

	}
	else if(!INDX_Limit_Bypass_Btn && _indx_Limit_Bypass_Btn){

		if(TML_MTR_INST[tml_indx].xCMD == TML_IDLE){
				TML_MTR_INST[tml_indx].xCMD 	= TML_SET_OUT3_CMD; 	

			_indx_Limit_Bypass_Btn = INDX_Limit_Bypass_Btn;

		}

	}
	
/////////////////////////////////////////////////////////

	
	switch(LocalJogMode){

		case MODE_TVL:

			TravelJog.IN = JogFwdRev(Head.LocalJog_FWD , Head.LocalJog_REV); 	
				
			if(_LocalJogMode != LocalJogMode)
				LED_CMD = 11;

			
		break;

		case MODE_OSC:
			 // (0, 1 ,-1)

			if(TML_MTR_INST[TML_AX_INDX_EXT].DigInp[1]){


				if(HardwReady && !Head.Remote_SW){

					if(Head.LocalJog_REV  && !_LocalJog_REV )
						//OSCToggle = 1;
						OSCToggle = !OSCToggle;

					
					else if(Head.LocalJog_FWD  && !_LocalJog_FWD ){
						if(OSC_Step == 5)
							OSCJog = 1;
						else if(OSC_Step == 10)
							OSCJog = 0;
					}
					
					

				}			

			}
			else{
				
				if(Head.LocalJog_FWD == Head.LocalJog_REV)
					OSCJog_Cmd = 0;

				else if(Head.LocalJog_FWD){

					/*
					if(OSCJog)					
						OSCJog = 0;
					else */
						OSCJog_Cmd = 1;
				}	
				else {				
					/*
					if(OSCJog)
						OSCJog = 0;
					else */
						OSCJog_Cmd = -1;
				}	
			}

			if(_LocalJogMode != LocalJogMode)
				LED_CMD = 14;

			
		break;

		case MODE_AHC:

			AHCJog.IN = JogFwdRev(Head.LocalJog_FWD , Head.LocalJog_REV); 
			if(_LocalJogMode != LocalJogMode)
				LED_CMD = 13;

		break;

		case MODE_WF:

			WFJog.IN = JogFwdRev(Head.LocalJog_FWD , Head.LocalJog_REV); 

			if(_LocalJogMode != LocalJogMode)
				LED_CMD = 15;

		break;

		case MODE_INDX:

			IndexJog.IN = JogFwdRev(Head.LocalJog_FWD , Head.LocalJog_REV); 
			if(_LocalJogMode != LocalJogMode)
				LED_CMD = 12;

		break;

		case MODE_INDX_EXT:

			//IndexExtJog.IN = JogFwdRev(Head.LocalJog_FWD , Head.LocalJog_REV); 
			IndexJog.IN = JogFwdRev(Head.LocalJog_FWD , Head.LocalJog_REV); 
			if(_LocalJogMode != LocalJogMode)
				LED_CMD = 16;

		break;

		default:

			
			 if (_LocalJogMode != LocalJogMode){

				if(LED_Static_Style)
					LED_CMD = 20;
				else	
					LED_CMD = 30;

			 	OSCJog = Stop;

				OSCJog_Cmd = Stop;

				TravelJog.IN = Stop;

				AHCJog.IN = Stop;

				WFJog.IN = Stop;

				IndexJog.IN = Stop;
			

			}
			 /*
			 else if(HardwReady && !Head.Remote_SW){

				if(Head.LocalJog_FWD && Head.LocalJog_REV && !_LocalJog_FWD_REV )
					OSCToggle = !OSCToggle;

			}
			*/

		break;

	}

	if(_LocalJogMode != LocalJogMode)
		OSCJog = 0;

	_LocalJog_FWD = (Head.LocalJog_FWD);
	_LocalJog_REV = (Head.LocalJog_REV);

	_LocalJogMode = LocalJogMode;
	_blink = blink;
	TimerExecute(&JogMode_Tmr);



	if(LED_CMD){

		
		TimerStart(&LED_Tmr,0.02);

		if(LED_CMD == 255){ // Reset 

			LED_Step = 0;
			LED_Indx = 1;
			LED_CMD = 0;
			
		}
		else if(LED_CMD <= 10){ 

			if((LED_Indx > 6)
				|| (LED_Indx == 0))
				LED_Indx = 1;
			
			LED_Step = LED_CMD;
			
		}			
		else if(LED_CMD < 20){	// Imprtant - Clear & Set	
		
			if((LED_Indx > 6)
			|| (LED_Indx == 0))			
				LED_Indx = 1;
			
			LED_Step = 10; 
			
			
		}
		else if(LED_CMD == 20){
			LED_Step = 20;
			LED_Indx = 1;
			LED_CMD = 0;

		}		
		else if(LED_CMD == 30){
			LED_Step = 30;
			LED_Indx = 1;
			LED_CMD = 0;

		}
		else if(LED_CMD == 40){
			LED_Step = 40;
			LED_Indx = 1;
			LED_CMD = 0;
		}		
		else if(LED_CMD == 100){

			if(LED_Step < 100)
				LED_Step = 100;

			LED_CMD = 0;

		}

		else {

			LED_Step = 20;
			LED_CMD = 0;
		}

		

	}

	
	if(LED_One_Blink_Req || (Seq.State == 0) || (Seq.State > 25) || KK_AllowAll_LED){

		if(!KK_Block_LED)
			LED_Step_Seq();

	}	
	
	TimerExecute(&LED_Tmr);

}

USINT JogFwdRev(plcbit fwd , plcbit rev){
USINT ret;	
// Stop , Reverse , Forward

	if(fwd == rev)
		ret = Stop;
	else if(fwd)
		ret = Forward;
	else if(rev)
		ret = Reverse;
	else
		ret = Stop;

	return ret;
		

}

void LED_Step_Seq(){

switch(LED_Step){


		case 0:

	/*
	TML_MTR_INST[TML_AX_TVL].xCMD	= TML_SET_OUT1_CMD; // TVL	
	TML_MTR_INST[TML_AX_WF].xCMD	= TML_SET_OUT4_CMD; // OUT4 INDX
	TML_MTR_INST[TML_AX_AHC].xCMD	= TML_SET_OUT0_CMD; // AHC	
	TML_MTR_INST[TML_AX_OSC].xCMD	= TML_SET_OUT0_CMD; // OSC	
	TML_MTR_INST[TML_AX_WF].xCMD	= TML_SET_OUT1_CMD; // WF
	TML_MTR_INST[TML_AX_TVL].xCMD	= TML_SET_OUT4_CMD; // EXT INDX 

	TML_SET_OUT1_CMD  		VECT_Rst_OUT1
	*/

			

			
		break;

		//////////////////////////////////////////////////////////////////////////////////////


		case 1:			

			if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){							
				VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT1; // TVL	
				LED_CMD = 0;
				LED_Step = 0;
			
			}
		break;

		case 2:

			if(!VectorStateMach[VECT_AX_WF].xCmd_busy){						
				VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT4; // OUT4 INDX		
				LED_CMD = 0;
				LED_Step = 0;
			
			}
		break;

		case 3:

			if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){	
				VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Set_OUT0; // AHC		
				LED_CMD = 0;
				LED_Step = 0;
			
			}
		break;


		case 4:

			if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
				VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Set_OUT0; // OSC	
				LED_CMD = 0;
				LED_Step = 0;
			
			}
		break;

		case 5:

			if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
				VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT1; // WF	
				LED_CMD = 0;
				LED_Step = 0;
			
			}
		break;

		case 6:

			if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){
				VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT4; // EXT INDX	
				LED_CMD = 0;
				LED_Step = 0;
			
			}
		break;

		//****/
		
		case 10:

			switch(LED_Indx){


				case 1:

					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){	
						
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Rst_OUT1; // TVL					
						//
						LED_Indx++;
					}
					
				break;

				case 2:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Rst_OUT4; // OUT4 INDX				
						//
						LED_Indx++;
					}
					
				break;

				case 3:
					
					if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){	
						VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Rst_OUT0; // AHC					
						//
						LED_Indx++;
					}
					
				break;

				case 4:
					
					if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
						VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Rst_OUT0; // OSC			
						//
						LED_Indx++;
					}
					
				break;

				case 5:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Rst_OUT1; // WF					
						//
						LED_Indx++;
					}
					
				break;

				case 6:
					
					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Rst_OUT4; // EXT INDX				
						//
						LED_Indx++;
					}
					
				break;

				default:

					LED_Indx = 0;

				break;
			}

			if(LED_Indx > 6) {// Done				

				
				if(Head.Remote_SW)
					LED_Step = 40;
				else				
					LED_Step = LED_CMD%10;

				LED_CMD = 0;
			
			}
			
		break;

		///////////////////////////////////////////////////////////////////////////////	

		case 20:

			switch(LED_Indx){


				case 1:

					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){	
						
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT1; // TVL					
						//
						LED_Indx++;
					}
					
				break;

				case 2:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT4; // OUT4 INDX				
						//
						LED_Indx++;
					}
					
				break;

				case 3:
					
					if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){	
						VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Set_OUT0; // AHC					
						//
						LED_Indx++;
					}
					
				break;

				case 4:
					
					if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
						VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Set_OUT0; // OSC			
						//
						LED_Indx++;
					}
					
				break;

				case 5:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT1; // WF					
						//
						LED_Indx++;
					}
					
				break;

				case 6:
					
					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT4; // EXT INDX					
						//
						LED_Indx++;
					}
					
				break;

				default:

					LED_Indx = 0;

				break;
			}

			if(LED_Indx > 6){ // Done
				LED_CMD = 0;
				
				if(LED_Static_Style)
					LED_Step = 0;
				else
					LED_Step = 30;

			}
			
		break;


		///////////////////////////////////////////////////////////////////////////////	


		case 30:

				LED_CMD = 0;
				LED_Indx = 1;
				TimerStart(&LED_Tmr, 0.01);
				_SR_Done = 0;
				LED_Step++;

		break;

		case 31:

			if(LED_CMD)
				LED_Step = 0;

			if(!TimerIsTimeOut(&LED_Tmr))
				break;

			switch(LED_Indx){


				case 1:		
					
					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){	
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Rst_OUT1; // TVL					
						_SR_Done = 1;	
					}
					
				break;

				case 2:

					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){							
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Rst_OUT4; // OUT4 INDX				
						_SR_Done = 1;
					}
					
					
				break;

				case 3:
					if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){			
						VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Rst_OUT0; // AHC					
						_SR_Done = 1;
					}
					
					
				break;

				case 4:					

					if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
						VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Rst_OUT0; // OSC			
						_SR_Done = 1;
					}
					
					
				break;

				case 5:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Rst_OUT1; // WF					
						_SR_Done = 1;
					}
					
				break;

				case 6:
					
					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Rst_OUT4; // EXT INDX					
						_SR_Done = 1;
					}
					
					
				break;

				default:

					LED_Indx = 0;

				break;
			 }	
			if(_SR_Done){
				_SR_Done = 0;
				TimerStart(&LED_Tmr, KK_Blink * 0.5);
				LED_Indx++;
				if(LED_Indx > 6){	
					LED_Indx = 1;
					TimerStart(&LED_Tmr, KK_Blink *0.5);
					LED_Step++;
				}

			}
			
		break;


		case 32:

			if(!TimerIsTimeOut(&LED_Tmr))
				break;

			switch(LED_Indx){


				case 1:					
					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){	
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT1; // TVL					
						_SR_Done = 1;
					}
					
				break;

				case 2:

					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){							
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT4; // OUT4 INDX				
						_SR_Done = 1;
					}
					
					
				break;

				case 3:

					if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){
						VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Set_OUT0; // AHC					
						_SR_Done = 1;
					}
					
					
				break;

				case 4:	
					
					if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
						VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Set_OUT0; // OSC			
						_SR_Done = 1;
					}
					
					
				break;

				case 5:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT1; // WF					
						_SR_Done = 1;
					}
					
					
				break;

				case 6:
					
					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT4; // EXT INDX					
						_SR_Done = 1;
					}
					
					
				break;

				default:

					LED_Indx = 0;

				break;
			 }	
			
			if(_SR_Done){
				_SR_Done = 0;
				TimerStart(&LED_Tmr, KK_Blink *0.5);
				LED_Indx++;
				if(LED_Indx > 6){	
					LED_Indx = 1;
					TimerStart(&LED_Tmr, KK_Blink *0.5);
					LED_Step = 31;
				}

			}
			
		break;

		case 33:

			if(LED_CMD)
				LED_Step = 0;

			if(!TimerIsTimeOut(&LED_Tmr))
				break;

			switch(LED_Indx){

				case 1:

					if((!VectorStateMach[VECT_AX_TVL].xCmd_busy)){	
						
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT1; // TVL					
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 2:
					
					if((!VectorStateMach[VECT_AX_WF].xCmd_busy)){
						
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT4; // OUT4 INDX				
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 3:
					
					if((!VectorStateMach[VECT_AX_AHC].xCmd_busy)){	
						VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Set_OUT0; // AHC					
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 4:
					
					if((!VectorStateMach[VECT_AX_OSC].xCmd_busy)){
						VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Set_OUT0; // OSC			
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 5:
					
					if((!VectorStateMach[VECT_AX_WF].xCmd_busy)){
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT1; // WF					
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 6:
					
					if((!VectorStateMach[VECT_AX_TVL].xCmd_busy)){
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT4; // EXT INDX				
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				default:

					LED_Indx = 0;

				break;
			}

			if(LED_Indx > 6){ 
				
				LED_Indx = 1;
			}

			if(_SR_Done){
				_SR_Done = 0;
				TimerStart(&LED_Tmr, KK_Blink *0.5);
				LED_Step = 31;


			}
			
			
		break;


		//////////////////////////////////////////////////////////////////////

		case 40:

			
			LED_CMD = 0;
			LED_Indx = 1;
			TimerStart(&LED_Tmr, 0.1);
			_SR_Done = 0;
			LED_Step++;

			

		break;

		case 41:

			if(LED_CMD)
				LED_Step = 0;

			if(!TimerIsTimeOut(&LED_Tmr))
				break;
			

			switch(LED_Indx){


				case 1:

					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){	
						
						VectorStateMach[VECT_AX_TVL].xCMD	= VECT_Set_OUT1; // TVL					
						_SR_Done = 1;
					}
					
				break;

				case 2:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						
						VectorStateMach[VECT_AX_WF].xCMD	= VECT_Set_OUT4; // OUT4 INDX				
						_SR_Done = 1;
					}
					
				break;

				case 3:
					
					if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){	
						VectorStateMach[VECT_AX_AHC].xCMD	= VECT_Set_OUT0; // AHC					
						_SR_Done = 1;
					}
					
				break;

				case 4:
					
					if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
						VectorStateMach[VECT_AX_OSC].xCMD	= VECT_Set_OUT0; // OSC			
						_SR_Done = 1;
					}
					
				break;

				case 5:
					
					if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						VectorStateMach[VECT_AX_WF].xCMD	= VECT_Set_OUT1; // WF					
						_SR_Done = 1;
					}
					
				break;

				case 6:
					
					if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){
						VectorStateMach[VECT_AX_TVL].xCMD	= VECT_Set_OUT4; // EXT INDX					
						_SR_Done = 1;
					}
					
				break;

				default:

					LED_Indx = 0;

				break;
			 }	
			if(_SR_Done){
				_SR_Done = 0;
				TimerStart(&LED_Tmr, KK_Blink);
				LED_Step++;

			}
			
		break;

		case 42:

			if(LED_CMD)
				LED_Step = 0;

			if(!TimerIsTimeOut(&LED_Tmr))
				break;

			switch(LED_Indx){

				case 1:

					if(TML_MTR_INST[TML_AX_TVL].Error){

						VectorStateMach[VECT_AX_TVL].xCMD	= VECT_Rst_OUT1; // TVL	
						LED_Step = 0;

					}					
					else if(!VectorStateMach[VECT_AX_TVL].xCmd_busy){	
						
						VectorStateMach[VECT_AX_TVL].xCMD	= VECT_Rst_OUT1; // TVL					
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 2:

					if(TML_MTR_INST[TML_AX_INDX].Error){

						VectorStateMach[VECT_AX_WF].xCMD	= VECT_Rst_OUT4; // OUT4 INDX
						LED_Step = 0;

					}					
					else if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						
						VectorStateMach[VECT_AX_WF].xCMD	= VECT_Rst_OUT4; // OUT4 INDX				
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 3:


					if(TML_MTR_INST[TML_AX_AHC].Error){

						VectorStateMach[VECT_AX_AHC].xCMD	= VECT_Rst_OUT0; // AHC	
						LED_Step = 0;

					}					
					else if(!VectorStateMach[VECT_AX_AHC].xCmd_busy){	
						VectorStateMach[VECT_AX_AHC].xCMD	= VECT_Rst_OUT0; // AHC					
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 4:

					if(TML_MTR_INST[TML_AX_OSC].Error){

						VectorStateMach[VECT_AX_OSC].xCMD	= VECT_Rst_OUT0; // OSC	
						LED_Step = 0;

					}					
					else if(!VectorStateMach[VECT_AX_OSC].xCmd_busy){
						VectorStateMach[VECT_AX_OSC].xCMD	= VECT_Rst_OUT0; // OSC			
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 5:

					if(TML_MTR_INST[TML_AX_WF].Error){

						VectorStateMach[VECT_AX_WF].xCMD	= VECT_Rst_OUT1; // WF	
						LED_Step = 0;

					}					
					else if(!VectorStateMach[VECT_AX_WF].xCmd_busy){
						VectorStateMach[VECT_AX_WF].xCMD	= VECT_Rst_OUT1; // WF					
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				case 6:

					if(TML_MTR_INST[TML_AX_INDX_EXT].Error){

						VectorStateMach[VECT_AX_TVL].xCMD	= VECT_Rst_OUT4; // 	
						LED_Step = 0;

					}					
					else if(TML_MTR_INST[TML_AX_INDX_EXT].xCMD == TML_IDLE){
						VectorStateMach[VECT_AX_TVL].xCMD	= VECT_Rst_OUT4; // 					
						_SR_Done = 1;
						LED_Indx++;
					}
					
				break;

				default:

					LED_Indx = 0;

				break;
			}

			if(LED_Indx > 6){			
				
				LED_Indx = 1;
				
			}

			if(_SR_Done){
				_SR_Done = 0;
				TimerStart(&LED_Tmr, KK_Blink);
				LED_Step = 41;


			}
			
			
		break;


		/////////////////////////////////////


		case 100:

			if(LED_CMD != 100){
				LED_Step = 0;
				LED_CMD = 0;
			}	

			if(!TimerIsTimeOut(&LED_Tmr))
				break;

			if(LED_Indx == 0)
				LED_Indx = 1;
		
			switch(LED_Indx){

				case 1:

					if((!VectorStateMach[VECT_AX_TVL].xCmd_busy)
						|| (VectorStateMach[VECT_AX_TVL].xCMD >= VECT_Set_OUT0)){	
						
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT1; // TVL					
						LED_Indx++;
					}
					
				break;

				case 2:
					
					if((!VectorStateMach[VECT_AX_WF].xCmd_busy)
						|| (VectorStateMach[VECT_AX_WF].xCMD >= VECT_Set_OUT0)){	
						
						
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT4; // OUT4 INDX				
						LED_Indx++;
					}
					
				break;

				case 3:
					
					if((!VectorStateMach[VECT_AX_AHC].xCmd_busy)
						|| (VectorStateMach[VECT_AX_AHC].xCMD >= VECT_Set_OUT0)){	
						
						VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Set_OUT0; // AHC					
						LED_Indx++;
					}
					
				break;

				case 4:
					
					if((!VectorStateMach[VECT_AX_OSC].xCmd_busy)
						|| (VectorStateMach[VECT_AX_OSC].xCMD >= VECT_Set_OUT0)){	
						
						VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Set_OUT0; // OSC			
						LED_Indx++;
					}
					
				break;

				case 5:
					
					if((!VectorStateMach[VECT_AX_WF].xCmd_busy)
						|| (VectorStateMach[VECT_AX_WF].xCMD >= VECT_Set_OUT1)){	
						
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Set_OUT0; // WF					
						LED_Indx++;
					}
					
				break;

				case 6:
					
					if((!VectorStateMach[VECT_AX_TVL].xCmd_busy)
						|| (VectorStateMach[VECT_AX_TVL].xCMD >= VECT_Set_OUT4)){	
						
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Set_OUT4; // 					
						LED_Indx++;
					}
					
				break;

				default:

					LED_Indx = 1;

					TimerStart(&LED_Tmr, 0.5);
					LED_Step = 101;

				break;
			}

			
			
		break;


		case 101:

			if(LED_CMD != 100){
				LED_Step = 0;
				LED_CMD = 0;
			}	

			if(!TimerIsTimeOut(&LED_Tmr))
				break;

			if(LED_Indx == 0)
				LED_Indx = 1;
			
			switch(LED_Indx){

				case 1:

					if((!VectorStateMach[VECT_AX_TVL].xCmd_busy)
						|| (VectorStateMach[VECT_AX_TVL].xCMD >= VECT_Set_OUT0)){
						
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Rst_OUT1; // TVL					
						LED_Indx++;
					}
					
				break;

				case 2:
					
					if((!VectorStateMach[VECT_AX_WF].xCmd_busy)
						|| (VectorStateMach[VECT_AX_WF].xCMD >= VECT_Set_OUT0)){
						
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Rst_OUT4; // OUT4 INDX				
						LED_Indx++;
					}
					
				break;

				case 3:
					
					if((!VectorStateMach[VECT_AX_AHC].xCmd_busy)
						|| (VectorStateMach[VECT_AX_AHC].xCMD >= VECT_Set_OUT0)){
						
						VectorStateMach[VECT_AX_AHC].xCMD 	= VECT_Rst_OUT0; // AHC					
						LED_Indx++;
					}
					
				break;

				case 4:
					
					if((!VectorStateMach[VECT_AX_OSC].xCmd_busy)
						|| (VectorStateMach[VECT_AX_OSC].xCMD >= VECT_Set_OUT0)){
						
						VectorStateMach[VECT_AX_OSC].xCMD 	= VECT_Rst_OUT0; // OSC			
						LED_Indx++;
					}
					
				break;

				case 5:
					
					if((!VectorStateMach[VECT_AX_WF].xCmd_busy)
						|| (VectorStateMach[VECT_AX_WF].xCMD >= VECT_Set_OUT1)){
						
						VectorStateMach[VECT_AX_WF].xCMD 	= VECT_Rst_OUT0; // WF					
						LED_Indx++;
					}
					
				break;

				case 6:
					
					if((!VectorStateMach[VECT_AX_TVL].xCmd_busy)
						|| (VectorStateMach[VECT_AX_TVL].xCMD >= VECT_Set_OUT4)){
						
						VectorStateMach[VECT_AX_TVL].xCMD 	= VECT_Rst_OUT4; // EXT INDX					
						LED_Indx++;
					}
					
				break;

				default:

					LED_Indx = 1;

					TimerStart(&LED_Tmr, 1.0);
					LED_Step = 100;

				break;
			}
			
		break;

		
	
	}

if(LED_Indx != _LED_Indx)
	LED_One_Blink_Req = 0;

_LED_Indx = LED_Indx;


return ;
}


