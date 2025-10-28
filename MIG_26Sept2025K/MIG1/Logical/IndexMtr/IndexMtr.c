
#include <bur/plctypes.h>

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>


#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

_LOCAL INT KK_Jog_Speed;

void _INIT ProgramInit(void)
{
	//Indexer.Speed_Forward 	= &permMem.Forward_Speed;
	//Indexer.Speed_Reverse 	= &permMem.Reverse_Speed;
	//Indexer.Speed_Jog 		= &permMem.Index_Jog_Speed;
	//Indexer.Forward_time 	= &permMem.Forward_Time;
	//Indexer.Reverse_time 	= &permMem.Reverse_Time;

	
	Indexer.Speed_Forward	= &Analog_Out_Mod[Forward_Speed_Indx].LimScale.y;
	Indexer.Speed_Reverse	= &Analog_Out_Mod[Reverse_Speed_Indx].LimScale.y;
	Indexer.Speed_Jog		= &Analog_Out_Mod[Index_Jog_Speed_Indx].LimScale.y;	
	
	Indexer.Forward_time	= &Analog_Out_Mod[Forward_Time_Indx].LimScale.y;
	Indexer.Reverse_time	= &Analog_Out_Mod[Reverse_Time_Indx].LimScale.y;

	
	Indexer.period = 1000;
}

void _CYCLIC ProgramCyclic(void)
{
	if(Indexer.Speed_Jog)
		KK_Jog_Speed = (INT)(*Indexer.Speed_Jog);
	
	// Go to idle state if Run switch is off

	//GlobalPermission_Switch  , SequenceRun_Out
	if(!Indexer.Run || !(Indexer.Start || (Indexer.ManualStep != 0))){
		
		if(Indexer.state != 0){
			Indexer.Start = 0;
			Indexer.timer.IN = 0;
			Indexer.state = 0;

		}
	}
	/////////////////////	
	
	switch (Indexer.state) {
		
		case 0: // Idle
			// Reset timer
			Indexer.timer.IN = 0;

			
			switch(IndexJog.IN){

				case Forward: 
					Indexer.Speed = (INT)(*Indexer.Speed_Jog);
				break;

				case Reverse: 
					Indexer.Speed = (INT)(*Indexer.Speed_Jog * (-1.0));
				break;

				default:
					
					// Step Motor
					Indexer.Speed = 0;

				break;


			}							
			
			// Disable welding
			//Indexer.Sequence = 1;
			//Indexer.Sequence = 0;
					
			//if(Indexer.Run && Seq.Start_in)
			if(Indexer.Run && GlobalPermission_Switch){
				
				if(Indexer.Start)	
					Indexer.state++;

				//else if(Indexer.ManualStep != 0)
					//Indexer.state = 10;
			}
			else
				Indexer.Start = 0;
				
		break;
		
		case 1: // Running - Check for Limit switches
			
			
			Indexer.Speed = 0;

			

			if(TravelSpeed.IN == Forward){

				Indexer.timer.IN = 1;
				Indexer.timer.PT = (int) (*Indexer.Forward_time * 1000.0 +0.5);		
				Indexer.state++;

			}
			else if(TravelSpeed.IN == Reverse){

				Indexer.timer.IN = 1;
				Indexer.timer.PT = (int) (*Indexer.Reverse_time * 1000.0 +0.5);		
				Indexer.state++;

			}
			else{

				Indexer.Start = 0;
				Indexer.timer.IN = 0;
				Indexer.state = 0;

			}
			
			
			
		break;

		case 2: // Index - Reset once timer expires
			//Indexer.Sequence = 1;
			//Indexer.Sequence = 0;
  
			if (Indexer.Direction) {
				Indexer.Speed = (INT)(*Indexer.Speed_Forward);
			} else {
				Indexer.Speed = (INT)(*Indexer.Speed_Reverse * (-1));
			}

	
			if (Indexer.timer.Q) {
				Indexer.Start = 0;
				Indexer.timer.IN = 0;
				Indexer.state = 0;
			}
			
		break;


		case 10: // Manual Step

			Indexer.timer.IN = 1;			
			Indexer.state++;
		
			if(Indexer.ManualStep > 0){	
				IndexJog.IN = Forward;
				Indexer.timer.PT = (int) (*Indexer.Forward_time * 1000.0 +0.5);	
			}
			
			else if(Indexer.ManualStep < 0){
				IndexJog.IN = Reverse;
				Indexer.timer.PT = (int) (*Indexer.Reverse_time * 1000.0 +0.5);	
			}			
			else{
				
				Indexer.ManualStep = 0;
				IndexJog.IN = Stop;
				Indexer.Speed = 0;
				Indexer.state = 0;
			}
	
		break;


		case 11: // Manual Step progress

			if (Indexer.ManualStep > 0)
				Indexer.Speed = (INT)(*Indexer.Speed_Forward);
			
			else if(Indexer.ManualStep < 0)
				Indexer.Speed = (INT)(*Indexer.Speed_Reverse * (-1));

			else{
				
				Indexer.ManualStep = 0;
				IndexJog.IN = Stop;
				Indexer.Speed = 0;
				Indexer.state = 0;
			}
			
			
			if (Indexer.timer.Q) {
				Indexer.Start = 0;
				Indexer.timer.IN = 0;
				IndexJog.IN = Stop;
				Indexer.Speed = 0;
				Indexer.ManualStep = 0;
				Indexer.state = 0;
			}

		break;
		
		default:
			
		break;
	}

	// Run timer;		
	TON(&Indexer.timer);
	
}

