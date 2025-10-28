
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

#include "AsMbTCP.h"

#define mb_WRITE_SINGLE_REGISTER 	1
#define mb_READ_COILS 				2


_LOCAL  plcstring mb_station[20]; /*path to modbus station*/
_LOCAL	UINT register_address; /*register address on modbus station*/
_LOCAL	UINT register_value; /*register value to write*/
_LOCAL	UINT fub_status; /*status of function blocks*/
_LOCAL	USINT mb_exception; /*modbus exception*/
_LOCAL	UDINT instruction; /*start instruction*/
_LOCAL	plcbit check_status; /*flag to check status*/
_LOCAL	UINT coil_address; /*coils address on modbus station*/

_LOCAL	plcbit coil_values[16]; /*read data*/
_LOCAL	plcbit instruction_ready; /*flag marks instruction "ready"*/

_LOCAL	mbWriteSingleRegister_typ mbWriteSingleRegister_0; /*function block structure*/
_LOCAL	mbReadCoils_typ mbReadCoils_0; /*function block structure*/
_LOCAL	mbGetLastException_typ mbGetLastException_0; /*function block structure*/

_LOCAL UINT HMI_Blink;
	
void _INIT ProgramInit(void)
{

	
	/* init values */
	strcpy(mb_station , "IF2.ST2"); /* path to modbus station */
	
	register_address = 0x2400; 		/* register address on modbus station */
	register_value = 0; /* register value to write */	
	
	coil_address	= 0x0;						/* coils address on modbus station */

	fub_status		= 0 ;							/* status of function blocks */
	
	instruction = mb_WRITE_SINGLE_REGISTER; /* start instruction */

	mb_exception = 0; /* modbus exception */

}

void _CYCLIC ProgramCyclic(void)
{

	switch(instruction){

		case mb_WRITE_SINGLE_REGISTER:
			
			mbWriteSingleRegister_0.enable 			= 1;
			mbWriteSingleRegister_0.pStation 		= mb_station;
			mbWriteSingleRegister_0.startAddress 	= register_address;
			mbWriteSingleRegister_0.value 			= register_value;
			
			mbWriteSingleRegister(&mbWriteSingleRegister_0) ;	/* call function block */
			
			if(mbWriteSingleRegister_0.status != ERR_FUB_BUSY){
				
				fub_status 			= mbWriteSingleRegister_0.status;
				check_status 		= 1;			/* flag to check status */
				instruction_ready 	= 1;			/* flag marks instruction "ready" */

				if(fub_status == 0)
					register_value++;

				
			}
			
		break;	

		case mb_READ_COILS:

			
			mbReadCoils_0.enable = 1;
			mbReadCoils_0.pStation = mb_station;
		
			mbReadCoils_0.startAddress = coil_address;			
			mbReadCoils_0.nrCoils = 16;
			mbReadCoils_0.pData = coil_values;			
   			mbReadCoils_0.dataSize = sizeof(coil_values);
			
			mbReadCoils(&mbReadCoils_0);/* call function block */
			
   			if(mbReadCoils_0.status != ERR_FUB_BUSY){
				
				fub_status 		= mbReadCoils_0.status;
				check_status 		= 1;			/* flag to check status */
				instruction_ready 	= 1;			/* flag marks instruction "ready" */
   			}		


		break;


	}

	/* ERROR HANDLING */
	if(check_status){
		check_status = 0;
 	
	  switch(fub_status){
  			
			case 0: //ERR_OK:
				/* no error */

	  		break;
						
			case 34205:	// mbERR_EXCEPTION:
			
				/* modbus station response sent a modbus exception */
				mbGetLastException_0.enable = 1;
				mbGetLastException_0.pStation = mb_station;

				mbGetLastException(&mbGetLastException_0);	/* call function block */

				mb_exception = mbGetLastException_0.mbExcp;			
							
			break;

			default:

			break;
				
	  	}		
	
		
		
	}


	/* STATE MACHINE CONTROL */
	if(instruction_ready && ( fub_status == ERR_OK)){
 	
		switch(instruction){
	
			case mb_WRITE_SINGLE_REGISTER:
				instruction 		= mb_READ_COILS;
				instruction_ready 	= 0;
			break;
				
			case mb_READ_COILS: 
				instruction 		= mb_WRITE_SINGLE_REGISTER;
				instruction_ready 	= 0;
			break;

			default:

			break;

		}
	}


	HMI_Blink = (coil_values[0])? 1:0;
}

void _EXIT ProgramExit(void)
{

}



