
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

#include <..\Lib\Statelib.c>
#include <..\Lib\LoggerLib.c>
#include <..\Lib\WSI_lib.c>


_LOCAL char TML_PLC_IP_Address[16];

typedef struct UDP_int_typ{

	UdpSend_typ send;
	UdpRecv_typ receive;
	UdpClose_typ close;
	UdpOpen_typ	open;

}UDP_int_typ;

typedef struct UDP_typ{


    UINT Step;

	UDINT SentCntr;
	UDINT RecevCntr;
	
	UDP_int_typ Intr;
	RS_Channel_typ *pSendChan;
	RS_Channel_typ *pRecevChan;
	

}UDP_typ;


_LOCAL UDP_typ UDP_Config[2];
_LOCAL UDP_typ UDP_Msg[2];

_LOCAL plcstring SrvIP[20];

_LOCAL UINT status;
_LOCAL plcbit sendNewMsg;
_LOCAL UINT receive_cntr;
_LOCAL UINT push_Cntr;

_LOCAL UINT ReceivedCntr;


_LOCAL plcstring KK_String[128];
_LOCAL USINT KK_Char;

_LOCAL plcbit firstScan;


void _INIT ProgramInit(void)
{UINT i;


	
	strcpy(SrvIP , "192.168.10.11");  // Must be Axis ID 10 
	//strcpy(SrvIP , "192.168.19.11"); // This is default IP in new converter
	//strcpy(SrvIP , "255.255.255.255");

	//strcpy(TML_PLC_IP_Address , "192.168.11.123" ); // My PLC  Address - PowerLink Channel
	strcpy(TML_PLC_IP_Address , "192.168.10.123" ); // My PLC  Address - Ethernet Channel

	i = 0;
	// Initialize UDP open function block
	UDP_Config[i].Intr.open.enable = 0;
	UDP_Config[i].Intr.open.options = 0;
	UDP_Config[i].Intr.open.pIfAddr = (UDINT)&TML_PLC_IP_Address[0];
	UDP_Config[i].Intr.open.port = 30689;
	UDP_Config[i].Intr.open.ident = 0;	
	
	
	// Initialize UDP send function block
	UDP_Config[i].Intr.send.enable = 0;
	UDP_Config[i].Intr.send.flags = 0;
	UDP_Config[i].Intr.send.pHost = (UDINT)&SrvIP[0];
	UDP_Config[i].Intr.send.port = 30689;	

	UDP_Config[i].Intr.receive.ident = 0;
	UDP_Config[i].Intr.receive.enable = 1;
	UDP_Config[i].Intr.receive.pIpAddr = 0;
	UDP_Config[i].Intr.receive.flags = 0;


	// Initialize UDP open function block
	UDP_Msg[i].Intr.open.enable = 0;
	UDP_Msg[i].Intr.open.options = 0;
	UDP_Msg[i].Intr.open.pIfAddr = (UDINT)&TML_PLC_IP_Address[0];
	UDP_Msg[i].Intr.open.port = 1700;
	UDP_Msg[i].Intr.open.ident = 0;	
	
	
	// Initialize UDP send function block
	UDP_Msg[i].Intr.send.enable = 0;
	UDP_Msg[i].Intr.send.flags = 0;
	UDP_Msg[i].Intr.send.pHost = (UDINT)&SrvIP[0];
	UDP_Msg[i].Intr.send.port = 1700;	

	UDP_Msg[i].Intr.receive.ident = 0;
	UDP_Msg[i].Intr.receive.enable = 1;
	UDP_Msg[i].Intr.receive.flags = 0;
	UDP_Msg[i].Intr.receive.pIpAddr = 0;

	////////////////////////////////////////////////////
	UDP_Config[i].pRecevChan = &TML_Data_Chan[1];
	UDP_Config[i].pSendChan = &UDP_Connect_Send[i];

	UDP_Msg[i].pRecevChan = 0;
	UDP_Msg[i].pSendChan = &TML_Msg_Chan[1]; 

	////////////////////////
	//UDP_Config[i].Step = 1;
	//UDP_Msg[i].Step = 1;

	// Start Connection
	UDP_Config[i].pSendChan->Data[0] = 0x01;
	UDP_Config[i].pSendChan->Len = 1;

	
	UDP_Ready = 0;

	firstScan = 1;

	//Kick_TML_Cmd = 1;
	
}

void _CYCLIC ProgramCyclic(void)
{
UINT i , k ;

	

	//if(ESTOP && firstScan){
	if(firstScan){
		Kick_TML_Cmd = 1;
		firstScan = 0;

	}


	switch(Kick_TML_Cmd){


		case 1:


			UDP_Ready = 0;			
			
			TML_mgr[0].hrdw[0].RXCount = 0;
			TML_mgr[0].hrdw[1].RXCount = 0;

			TML_mgr[1].hrdw[0].RXCount = 0;
			TML_mgr[1].hrdw[1].RXCount = 0; 	

			TML_mgr[0].hrdw[0].TXCount = 0;
			TML_mgr[0].hrdw[1].TXCount = 0;

			TML_mgr[1].hrdw[0].TXCount = 0;
			TML_mgr[1].hrdw[1].TXCount = 0;

			TML_mgr[0].hrdw[0].TXCountReadBack = TML_mgr[0].hrdw[0].TXCount;
			TML_mgr[0].hrdw[1].TXCountReadBack = TML_mgr[0].hrdw[1].TXCount;

			TML_mgr[1].hrdw[0].TXCountReadBack = TML_mgr[1].hrdw[0].TXCount;
			TML_mgr[1].hrdw[1].TXCountReadBack = TML_mgr[1].hrdw[1].TXCount;

			
			if(UDP_Config[0].Step == 10)
				UDP_Config[0].Step = 20;
			else
				UDP_Config[0].Step = 1;

			if(UDP_Msg[0].Step == 10)
				UDP_Msg[0].Step = 20;
			else 
				UDP_Msg[0].Step = 1;
			
			
			Kick_TML_Cmd++;

		break;

		case 2:

			if((UDP_Msg[0].Step == 10)
				&& (UDP_Config[0].Step == 10)){
			
				UDP_Connect_Send[0].Cntr = 0;
				UDP_Connect_Send[0].Len = 1;

				Kick_TML_Cmd++;

			}

			
		break;

		case 3:

			if(UDP_Connect_Send[0].Cntr > 0)
				Kick_TML_Cmd++;

			Kick_TML_Cmd++;

		break;

		
		case 4:

			Kick_TML_Cmd++;

			TML_Msg_Chan[0].Trash_Cntr = 0;
			TML_Data_Chan[0].Trash_Cntr = 0;

			UDP_Ready = 1;

		break;


	}


	
	
	for(i = 0 ; i < 1 ; i++){
	
		for(k = 0 ; k < 2 ; k++){

			switch(k){

				case 0: 	UDP_Socket(&UDP_Config[i]);   	break;

				case 1: 	UDP_Socket(&UDP_Msg[i]);  		break;				

			}

		}		

	}
	
}

void UDP_Socket(UDP_typ *udp){


	switch(udp->Step){


			case 0:

			break;

			case 1:

				
				if(udp->pRecevChan){
					udp->Intr.receive.pData = (UDINT)&udp->pRecevChan->Data[0];
					udp->Intr.receive.datamax = 32;
				
				}else{
					udp->Intr.receive.pData = 0;
					udp->Intr.receive.datamax = 0;
				}

				if(udp->pSendChan){
					udp->Intr.send.pData = (UDINT)&udp->pSendChan->Data[0];
					udp->Intr.send.datalen = 0;
				}
				else{
					udp->Intr.send.pData = 0;
					udp->Intr.send.datalen = 0;
				}
				
				
				udp->Intr.open.enable = 1;
				
				
				UdpOpen(&udp->Intr.open);

				if(udp->Intr.open.ident && (udp->Intr.open.status == 0)){

					udp->Intr.send.enable = 1;
					udp->Intr.receive.enable = 1;
					
					udp->Intr.send.ident = udp->Intr.open.ident;						
					udp->Intr.receive.ident = udp->Intr.open.ident;
					udp->Intr.close.ident = udp->Intr.open.ident;

					udp->Step = 10;

				}										
				else if(udp->Intr.open.status == 32502){ // Port already opened

					udp->Intr.close.ident = udp->Intr.open.ident;
					udp->Intr.close.enable = 1;
					udp->Step = 20;
				}
				else if(udp->Intr.open.status != ERR_FUB_BUSY)
					udp->Step = 102;

				
					
				
			break;


			case 10:

						
				//////////////////////////////

				if(udp->pSendChan){
					
					if(udp->pSendChan->Len > 0){

						udp->Intr.send.datalen = udp->pSendChan->Len;
						UdpSend(&udp->Intr.send);
			
						if(udp->Intr.send.status == 0){							
							
							udp->Intr.send.datalen = 0;

							udp->pSendChan->_len_prev = udp->pSendChan->Len;
							udp->pSendChan->Len = 0;
							udp->SentCntr++;
							udp->pSendChan->Cntr++;
						}							
						else if(udp->Intr.send.status != ERR_FUB_BUSY){

							udp->Intr.send.datalen = 0;

						 	udp->pSendChan->_len_prev = udp->pSendChan->Len;
							udp->pSendChan->Len = 0;
							udp->Step = 110;
						}
					}

				}

				//////////////////////////////////////////////////////////////////


				if(udp->pRecevChan){
					
					if((udp->pRecevChan->Len == 0)
						&& (udp->pSendChan->Len == 0)){
						
						UdpRecv(&udp->Intr.receive);

						if(udp->Intr.receive.status == ERR_FUB_BUSY)
						{  }// Process busy 

						else if(udp->Intr.receive.status == 0)/* Data received */
						{								
							udp->pRecevChan->Len = udp->Intr.receive.recvlen;
							udp->RecevCntr++;
							udp->pRecevChan->Cntr++;

						}
						else if(udp->Intr.receive.status != 32505) // Error ( 32505 is no datat received)
							udp->Step = 115;
							
						
					}
				}

			break;

			case 20:

				UdpClose(&udp->Intr.close);				

				if(udp->Intr.close.status == ERR_FUB_BUSY)
				{  }// Process busy 

				else if(udp->Intr.close.status) {// error
						//udp->Step = 120;
						udp->Step = 1;
				}
				else{

					udp->Intr.open.ident = 0;
					udp->Intr.send.ident = 0;
					udp->Intr.close.ident = 0;
					udp->Intr.receive.ident = 0;

					udp->Step = 1;
				}	


			break;


			case 101:
			case 102:
			case 110:
			case 115:
			case 120:

			break;


		}


}

