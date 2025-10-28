
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>


void ReverseRelay(ReversingRelay* r);
void LEDdebounce(LED_typ* l, REAL value, REAL threshold);
void SetDefaultValues();
void ConfigAnalogIO();

_LOCAL plcbit SetDefault;
void _INIT ProgramInit(void)
{

	ESTOP = 1;

	ConfigAnalogIO();
	
	// Init permanent variables
	if (!permMemInit) {

		SetDefaultValues();
		
	}
	
	visChangePage = visChangePage;

	OSCJog = OSCJog;
	Purge = Purge;
	Weld = Weld;
	WireJog = WireJog;
	AHCauto = AHCauto;
	cameraPower = cameraPower;
	
	Seq.startTimer.PT = 250;
	Seq.stopTimer.PT = 250;
	
	// Configure Analog Ranges	

	
	// Initialize Computer Power
	computerPower[0] = 0;
	computerPower[1] = 0;

	Indexer.state = Indexer.state;

	

}

void _CYCLIC ProgramCyclic(void)
{
	int i;

	if(SetDefault){
		SetDefault = 0;
		SetDefaultValues();
	}
	
	// Check for module error
	ModulesOK = 1;
	// X20 Modules
	/*
	for(i=1; i<=13; i++) {

		ModulesOK &= X20ModuleOK[i];
		
	}
	*/
	// X67 Modules
	/*
	for(i=1; i<=2; i++) {
		
		if(i != 2)
			ModulesOK &= X67ModuleOK[i];
	}
	*/
	// Estop display
	//visESTOP = ESTOP;
	
	
	// AHC Response - Separate top and bottom 4 bits into two different bytes
	
	ioAHCResponse[0] = (((USINT)Analog_Out_Mod[AHCResponse_Indx].LimScale.y) >> 4) & 0x0F;
	ioAHCResponse[1] = (((USINT)Analog_Out_Mod[AHCResponse_Indx].LimScale.y)) & 0x0F;


	//////
	
	// Run Reversing Relays
	ReverseRelay(&AHCJog);
	ReverseRelay(&IndexJog);
	ReverseRelay(&TravelSpeed);
	ReverseRelay(&TravelJog);

	///////////////////////////////

	
	// Sequence pulse pushbutton

	// Stop Pushbutton
	// Make pulse if pushbutton pressed
	if (Seq.Stop_in && !Seq._stop)
		Seq.stopTimer.IN = 1;
	else
		Seq.stopTimer.IN = 0;
	
	// Keep track of last state (to make the pulse)
	Seq._stop = Seq.Stop_in;
	
	// Run Timer
	TOF(&Seq.stopTimer);
	
	// Set output
	
	Seq.Stop_out = Seq.stopTimer.Q || !GlobalPermission_Switch;

	// Start Pushbutton
	// Make pulse if pushbutton pressed, unless stop active
	if (Seq.Start_in && !Seq._start && !Seq.Stop_out)
		Seq.startTimer.IN = 1;
	else
		Seq.startTimer.IN = 0;
	
	// Keep track of last state (to make the pulse)
	Seq._start = Seq.Start_in;
	
	// Run Timer
	TOF(&Seq.startTimer);
	
	// Set output, but only if no stop command
	if (!Seq.stopTimer.Q)
		Seq.Start_out = Seq.startTimer.Q;
	else
		Seq.Start_out = 0;
	
	/* I Do NOT Know for what purpose was that code
	// Analogs
	// Analog Out's
	if (TravelSpeed.Stop) {
		_TravelSpeed = 0;
	} else if (TravelSpeed.Reverse) {
		// Make negative
		_TravelSpeed = permMem.Travel_Speed * (-1);
	} else {
		_TravelSpeed = permMem.Travel_Speed;
	}
	*/

	/// New ///

	for(i = TravelSpeedAO_Indx; i <= Index_PostDlyTime_Indx ; i++){
		Analog_Out_Mod[i].LimScale.x = permMem.Analog_Out[i].Value;
		LCRLimScal(&Analog_Out_Mod[i].LimScale);
	}

	
	ArcVoltsAO.OUT = (INT)Analog_Out_Mod[ArcVoltsAO_Indx].LimScale.y;
	WireFeedSpeedAO.OUT = (INT)Analog_Out_Mod[WireFeedSpeedAO_Indx].LimScale.y;
	AHCHighCurrentLimitAO.OUT = (INT)Analog_Out_Mod[AHCHighCurrentLimitAO_Indx].LimScale.y;
	AHCLowCurrentLimitAO.OUT = (INT)Analog_Out_Mod[AHCLowCurrentLimitAO_Indx].LimScale.y;
	OSCSpeedAO.OUT = (INT)Analog_Out_Mod[OSCSpeedAO_Indx].LimScale.y;

	switch(TravelSpeed.IN){

		case Stop:
			TravelSpeedAO.OUT = 0;
		break;

		case  Forward:		
			TravelSpeedAO.OUT = (INT)(Analog_Out_Mod[TravelSpeedAO_Indx].LimScale.y *(-1.0));
		break;

		case  Reverse:		
			TravelSpeedAO.OUT = (INT)(Analog_Out_Mod[TravelSpeedAO_Indx].LimScale.y);
		break;
	}
	if(!SequenceRun_Out) // Not Run State or Index Moving
		TravelSpeedAO.OUT = 0;

	/////////////
	
	// Force OSC Speed output to 0 if OSC is off
	if (!OSCon)
		OSCSpeedAO.OUT = 0;
	
	// Process LEDs
	LEDdebounce(&AHC_High_LED, AHC_High.OUT, 4.0);
	LEDdebounce(&AHC_In_Band_LED, AHC_In_Band.OUT, 4.0);
	LEDdebounce(&AHC_Low_LED, AHC_Low.OUT, 4.0);
	LEDdebounce(&Arc_Detect_LED, Arc_Detect.OUT, 4.0);


	
}

void LEDdebounce(LED_typ* l, REAL value, REAL threshold) {
	// Only run if timer is not running
	if (!l->timer.IN) {
		// Check if AI hits threshold
		if (value > threshold)
			l->IN = 1;
		else
			l->IN = 0;
		
		// Look for edge
		if (l->IN != l->_IN) {
			// Determine Direction
			if (l->IN)
				l->posEdge = 1;
			else
				l->posEdge = 0;
			
			// Start timer
			l->timer.IN = 1;
		}
		l->_IN = l->IN;
	}
	TON(&l->timer);
	
	// Once timer finishes, reset
	if (l->timer.Q) {
		l->timer.IN = 0;
		TON(&l->timer);
	}
}

void ReverseRelay(ReversingRelay* r) {
	r->Forward = r->Reverse = r->Stop = 0;
	
	switch (r->IN) {
		case Forward:
			r->Forward = 1;
			r->OUT[0] = r->OUT[2] = 1;
			r->OUT[1] = r->OUT[3] = 0;

		break;

		case Reverse:
			r->Reverse = 1;
			r->OUT[0] = r->OUT[2] = 0;
			r->OUT[1] = r->OUT[3] = 1;

		break;

		case Stop:
		default:
			r->Stop = 1;
			r->OUT[0] = r->OUT[2] = 0;
			r->OUT[1] = r->OUT[3] = 0;
			
		break;
	}
}






void SetDefaultValues(){
UINT i;
	permMemInit = 1;

	AHCResponse = 0;	

	//////////New////////
	i = TravelSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 30.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = ArcVoltsAO_Indx;
	permMem.Analog_Out[i].Value = 24.5;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.25;

	i = WireFeedSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 475.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = AHCHighCurrentLimitAO_Indx;
	permMem.Analog_Out[i].Value = 205.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = AHCLowCurrentLimitAO_Indx;
	permMem.Analog_Out[i].Value = 195.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = OSCSpeedAO_Indx;
	permMem.Analog_Out[i].Value = 2.5;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;


	////
	i = Forward_Speed_Indx;
	permMem.Analog_Out[i].Value = 25.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;

	i = Reverse_Speed_Indx;
	permMem.Analog_Out[i].Value = 25.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;


	i = Index_Jog_Speed_Indx;
	permMem.Analog_Out[i].Value = 50.0;
	permMem.Analog_Out[i].H_Incr = 10.0;
	permMem.Analog_Out[i].L_Incr = 5.0;
	

	i = Forward_Time_Indx;
	permMem.Analog_Out[i].Value = 2.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;

	i = Reverse_Time_Indx;
	permMem.Analog_Out[i].Value = 2.0;
	permMem.Analog_Out[i].H_Incr = 1.0;
	permMem.Analog_Out[i].L_Incr = 0.1;


	i = AHCResponse_Indx;
	permMem.Analog_Out[i].Value = 75;
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



}

void ConfigAnalogIO(){
USINT i;
	

	//  New 
	strcpy(Analog_Out_Mod[TravelSpeedAO_Indx].Name , "Travel Speed [inch//min]");
	strcpy(Analog_Out_Mod[ArcVoltsAO_Indx].Name , "ArcVolts [V]");
	strcpy(Analog_Out_Mod[WireFeedSpeedAO_Indx].Name , "Wire Feed Speed [inch//min]");
	strcpy(Analog_Out_Mod[AHCHighCurrentLimitAO_Indx].Name , "AHC High CurrentLimit[A]");
	strcpy(Analog_Out_Mod[AHCLowCurrentLimitAO_Indx].Name , "AHC Low CurrentLimit [A]");
	strcpy(Analog_Out_Mod[OSCSpeedAO_Indx].Name , "OSC Freq. [Hz]");

	for(i = 0 ; i <= OSCSpeedAO_Indx ; i++){
		
		Analog_Out_Mod[i].LimScale.x1 = 0.0;
		Analog_Out_Mod[i].LimScale.y1 = 0.0;
		Analog_Out_Mod[i].LimScale.x2 = 100.0;
		Analog_Out_Mod[i].LimScale.y2 = 32767.0;
	}

	Analog_Out_Mod[ArcVoltsAO_Indx].LimScale.x2 = 45.0;
	
	Analog_Out_Mod[TravelSpeedAO_Indx].LimScale.y2 = 32767.0 * (1.427/10.0); // 
	
	Analog_Out_Mod[WireFeedSpeedAO_Indx].LimScale.x2 = 750.0;

	Analog_Out_Mod[AHCHighCurrentLimitAO_Indx].LimScale.x2 = 1000.0;

	Analog_Out_Mod[AHCLowCurrentLimitAO_Indx].LimScale.x2 = 1000.0;

	Analog_Out_Mod[OSCSpeedAO_Indx].LimScale.x1 = 0.01;
	Analog_Out_Mod[OSCSpeedAO_Indx].LimScale.x2 = 10.0;

//////////////////////////	
	strcpy(Analog_Out_Mod[Forward_Speed_Indx].Name , "Index Forward Speed [%]");	
	strcpy(Analog_Out_Mod[Reverse_Speed_Indx].Name , "Index Reverse Speed [%]");
	strcpy(Analog_Out_Mod[Index_Jog_Speed_Indx].Name , "Index Jog Speed [%]");
	
	for(i = Forward_Speed_Indx ; i <= Index_Jog_Speed_Indx ; i++){

		Analog_Out_Mod[i].LimScale.x1 = 0.0;
		Analog_Out_Mod[i].LimScale.y1 = 0.0;
		Analog_Out_Mod[i].LimScale.x2 = 100.0;
		Analog_Out_Mod[i].LimScale.y2 = 32767.0;
		
	}
	
	strcpy(Analog_Out_Mod[Forward_Time_Indx].Name , "Forward Time [sec.]");
	strcpy(Analog_Out_Mod[Reverse_Time_Indx].Name , "Reverse Time [sec.]");
	for(i = Forward_Time_Indx ; i <= Reverse_Time_Indx ; i++){

		Analog_Out_Mod[i].LimScale.x1 = 0.5;
		Analog_Out_Mod[i].LimScale.y1 = 0.5;
		Analog_Out_Mod[i].LimScale.x2 = 10.0;
		Analog_Out_Mod[i].LimScale.y2 = 10.0;
		
	}
	
	strcpy(Analog_Out_Mod[AHCResponse_Indx].Name , "AHCResponse [%]");
	Analog_Out_Mod[AHCResponse_Indx].LimScale.x1 = 0.0;
	Analog_Out_Mod[AHCResponse_Indx].LimScale.y1 = 0.0;
	Analog_Out_Mod[AHCResponse_Indx].LimScale.x2 = 100.0;
	Analog_Out_Mod[AHCResponse_Indx].LimScale.y2 = 255.0;

	strcpy(Analog_Out_Mod[Index_PreDlyTime_Indx].Name , "Index Pre-Delay Time [sec.]");
	strcpy(Analog_Out_Mod[Index_PostDlyTime_Indx].Name , "Index Post-Delay Time [sec.]");
	for(i = Index_PreDlyTime_Indx ; i <= Index_PostDlyTime_Indx ; i++){

		Analog_Out_Mod[i].LimScale.x1 = 0.5;
		Analog_Out_Mod[i].LimScale.y1 = 0.5;
		Analog_Out_Mod[i].LimScale.x2 = 10.0;
		Analog_Out_Mod[i].LimScale.y2 = 10.0;
		
	}


}



	

