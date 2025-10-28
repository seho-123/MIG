
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "bgsprintf.h"

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>


USINT CameraChecksum(char* data, int index) {
	USINT sum=0;
	int i;

	for(i=0; i<index; i++) {
		sum += data[index];
	}

	return sum;
}

void _INIT TcpCamInit(void)
{
	int i;

	strcpy(cameraIP[0], "10.0.2.3");
	strcpy(cameraIP[1], "10.0.2.4");
	
	for (i=0; i<2; i++) {
		open[i].enable = 1;
		open[i].pIfAddr = 0;
		open[i].options = 0;
		open[i].port = 0;
		open[i].ident = 0;
		
		client[i].enable = 1;
		client[i].portserv = 16000;
		
		receive[i].enable = 1;
		receive[i].pData = (UDINT)response;
		receive[i].datamax = sizeof(response);
		receive[i].flags = 0;
		
		close[i].enable = 1;
		close[i].how = 0;
	}

	client[0].pServer = (UDINT)cameraIP[0];
	client[1].pServer = (UDINT)cameraIP[1];
	
	// Initialize message
	message[0] = 0x01;
}

void _CYCLIC TcpCamCyclic(void)
{
	int i;
	int index;
	
	for (i=0; i<2; i++) {
		// Run open socket until it returns successful with an ident
		if (!open[i].ident || open[i].status) {
			TcpOpen(&open[i]);
			continue;
		}
		
		if (connect[i]) {
			// Run client to connect to server
			if (!client[i].ident || client[i].status) {
				client[i].ident = open[i].ident;
				TcpClient(&client[i]);
	
				receive[i].ident = open[i].ident;
				send[i].ident = open[i].ident;
				close[i].ident = open[i].ident;
	
				continue;
			}
			
			// Check for received data
			TcpRecv(&receive[i]);
			
			if (!receive[i].status && receive[i].recvlen > 0) {
				
			}
			
			// Only process command if camera is currently picked
			if(cameraPicked == i) {
				// Process camera commands
				switch (cameraCommand) {
					case 1: // Focus in
						index = bgsprintf(&message[1], "%c%s,%s", 'F', cameraIP[i], "-100");
						break;
				
					case 2: // Focus out
						index = bgsprintf(&message[1], "%c%s,%s", 'F', cameraIP[i], "100");
						break;
				
					case 3: // LED on
						index = bgsprintf(&message[1], "%c%s,%s", 'L', cameraIP[i], "on");
						break;
				
					case 4: // LED off
						index = bgsprintf(&message[1], "%c%s,%s", 'L', cameraIP[i], "off");
						break;
				
					case 5: // LED power level
						index = bgsprintf(&message[1], "%c%s,", 'F', cameraIP[i]);
						message[index + 1] = ledLevel;
						index++;
						break;
	
					case 6: // Audo Weld Mode on
						index = bgsprintf(&message[1], "%c%s", 'A', "ON");
						break;
				
					case 7: // Auto Weld Mode off
						index = bgsprintf(&message[1], "%c%s", 'A', "OFF");
						break;
				
					case 8: // Pixel Depth 8
						index = bgsprintf(&message[1], "%c%s,%s", 'D', cameraIP[i], "8");
						break;
				
					case 9: // Pixel Depth 12
						index = bgsprintf(&message[1], "%c%s,%s", 'D', cameraIP[i], "12");
						break;
				
					case 10: // Weld Mode
						index = bgsprintf(&message[1], "%c%s,%s", 'E', cameraIP[i], "WELD");
						break;
				
					case 11: // Setup Mode
						index = bgsprintf(&message[1], "%c%s,%s", 'E', cameraIP[i], "SETUP");
						break;
				
					case 12: // Floating window Minimize Mode On
						index = bgsprintf(&message[1], "%c%s", 'G', "ON");
						break;
				
					case 13: // Floating window Minimize Mode Off
						index = bgsprintf(&message[1], "%c%s", 'G', "OFF");
						break;
							
				
					case 14: // Shutter Mode Rolling
						index = bgsprintf(&message[1], "%c%s,%s", 'U', cameraIP[i], "R");
						break;
				
					case 15: // Shutter Mode Global
						index = bgsprintf(&message[1], "%c%s,%s", 'U', cameraIP[i], "G");
						break;
							
					case 16: // Close all Tool Windows
						index = bgsprintf(&message[1], "%c", 'Z');
						break;
							
					case 0: // Do nothing
					default:
					
						break;
				}
	
				// Compute checksum and reset camera command
				if (cameraCommand) {
					message[index] = CameraChecksum(message, index);
			
					cameraCommand = 0;
				
					cameraSend[i] = 1;
				}
			}			

			// Send command
			if (cameraSend) {
				TcpSend(&send[i]);
				
				if (!send[i].status)
					cameraSend[i] = 0;
			}
			
		}
	}
}

void _EXIT TcpCamExit(void)
{

}

