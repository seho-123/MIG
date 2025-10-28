
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

int PelcoSetChecksum(USINT pelcoMsg[7]);

void _INIT ProgramInit(void)
{
	// Initialize UDP open function block
	open.enable = 1;
	open.options = 0;
	open.pIfAddr = 0;
	open.port = 0;
	open.ident = 0;
	
	// Initialize UDP close function block
	close.enable = 1;
	
	// Initialize UDP send function block
	send.enable = 1;
	send.flags = 0;
	send.pData = (UDINT)&pelcoMsg;
	send.datalen = sizeof(pelcoMsg);
	send.pHost = (UDINT)&pelcoSrvIP;
	send.port = pelcoSrvPort;
	
	// Initialize Pelco Server Timeout Timer
	pelcoSrvTimer.PT = pelcoSrvTimeout;
	
	// Initialize Pelco Message
	pelcoMsg[0] = 0xFF;
	
	// Initialize Pelco Address
	pelcoAddress = 1;
	
}

void _CYCLIC ProgramCyclic(void)
{
	UINT command;
	
	// Open UDP Port
	if( !open.ident || open.status ) {
		UdpOpen(&open);
		send.ident = open.ident;
		close.ident = open.ident;
		return;
	}
	
	// Check for new command
	if( _pelcoCommand != pelcoCommand ) {
		pelcoMsgReady = 1;
	}
	_pelcoCommand = pelcoCommand;
	
	if( pelcoMsgReady && !pelcoMsgSend && pelcoAddress != 0 ) {
		// Set address
		pelcoMsg[1] = pelcoAddress;
		
		// Process command
		switch (pelcoCommand) {
			default:
			case 0: // Stop
				pelcoMsg[2] = 0x00;
				pelcoMsg[3] = 0x00;
				pelcoMsg[4] = 0x00;
				pelcoMsg[5] = 0x00;

				command = 0;
				
				break;
			
			case 1: // Pan Right
			case 2: // Pan Left
			case 3: // Tilt Up
			case 4: // Tilt Down
			case 5: // Zoom In
			case 6: // Zoom Out
			case 7: // Focus Far
			case 8: // Focus Near
			case 9: // Iris Open
			case 10: // Iris Close
				command = 1<<pelcoCommand;

				// Set speeds for pan and tilt
				pelcoMsg[4] = 0x0F; // Speed (pan)
				pelcoMsg[5] = 0x0F; // Speed (tilt)

				break;
			
			case 11: // Preset
				command = 7;
			
				// Preset number
				pelcoMsg[4] = 0;
				pelcoMsg[5] = pelcoPreset;
			
				break;
			
		}

		pelcoMsg[2] = (USINT)(command>>8);
		pelcoMsg[3] = (USINT)command;
		
		// Calculate Checksum
		PelcoSetChecksum(pelcoMsg);
				
		// Set flag to send message
		pelcoMsgSend = 1;

	}
	
	// Send message
	if( pelcoMsgSend ) {
		
		UdpSend(&send);
		
		if( send.status == 0 ) {
			pelcoMsgSend = 0;
			pelcoMsgReady = 0;
		}
	}

}

int PelcoSetChecksum(USINT pelcoMsg[7]) {
	int i;
	USINT checksum = 0;
	
	// Calculate Checksum
	for( i=1; i<6; i++) {
		checksum += pelcoMsg[i];
	}
	
	// Set Checksum
	pelcoMsg[6] = checksum;

	return checksum;
}

