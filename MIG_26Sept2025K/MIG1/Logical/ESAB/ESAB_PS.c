

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>
#include <..\Lib\LoggerLib.c>


#include <..\Lib\libUDP\libUDP.h>
#include <..\Lib\libUDP\libUDP.c>


#define REC_BUF_LEN 160


_LOCAL plcstring send_data[32];
_LOCAL USINT receive_data[REC_BUF_LEN];
//_LOCAL USINT temp_data[REC_BUF_LEN * 3];


_LOCAL UDP_typ2 ESAB_UDP;

_LOCAL USINT ESAB_Com_Step;

_LOCAL char ESAB_IP_Address[16];
_LOCAL char PLC_IP_Address[16];

////////////////////////////////////////////

//UDP Receive Callback function to push data on ring buffer 
static void udpReceivedNewData(UDINT pRxBuffer, UDINT RxDataLen , UDINT ringBuf){

	//DBG("New Data Received  of Length %d \r\n" , RxDataLen);
	
	//ckConvertByteStreamToHex(pRxBuffer, temp_data, RxDataLen, sizeof(temp_data));
	//LogTxt(&LogMgr, "<< %s" , temp_data);
	//DBG("(Hex): %s  \r\n" , temp_data);
	
}
//UDP Transmit Callback function for debugging purpose
static void udpTransmitedNewData(UDINT pTxBuffer, UDINT pTxDataLen){

	
	//DBG("New Data Trasmitted of Length %d  \r\n " , pTxDataLen );
	//ckConvertByteStreamToHex(pTxBuffer, temp_data, pTxDataLen, sizeof(temp_data));
	//LogTxt(&LogMgr, ">> %s" , temp_data);
	//DBG("(Hex): %s  \r\n" , temp_data);	
	
}

void Convert_UINT2HEX( char* txt, UINT inp);
void Convert_USINT2HEX( char* txt, USINT inp);
USINT  Convert_HEX2USINT(char* txt);
UINT  Convert_HEX2UINT(char* txt);

//_LOCAL TON_typ ESAB_Tmr;


void _INIT ProgramInit(void){	
UDP_typ2 *udp;


	strcpy(ESAB_IP_Address , "192.168.10.205" );
	
	strcpy(PLC_IP_Address , "192.168.10.123" ); // My PLC  Address
	
	udp = &ESAB_UDP;

	udp->listener_config.port = 10001;
	udp->listener_config.buffer = receive_data;
	udp->listener_config.bufferSize = REC_BUF_LEN;
	udp->listener.iRxCallback = (UDINT)udpReceivedNewData;
	//udp->listener.iRxRingBuf = (UDINT)&RingBufIn;

	
	udp->send.iPort = 10001;
	udp->send.iBuffer = send_data;
	udp->send.iBufferSize = REC_BUF_LEN;	
	udp->send.iTxCallback = (UDINT)udpTransmitedNewData;

	strcpy(udp->send.iIPaddress , ESAB_IP_Address);
	udp->internal.UdpOpen_0.pIfAddr = (UDINT)&PLC_IP_Address[0];


	ESAB_PS.step = 0;

	ESAB_PS.Msc.Main_Tmr.PT = 1000;
	ESAB_PS.Msc.Cmd_Hrbt_Tmr.PT = 400;

	//ESAB_Tmr.PT = 1000;

	ESAB_PS.step = 1;
		
}

void _CYCLIC ProgramCyclic(void){	
ESAB_typ *esab;
UDP_typ2 *udp;

USINT  status;
INT myINT;


	esab = &ESAB_PS;
	udp = &ESAB_UDP;
	
	Udp_cyclic(udp);


	switch(esab->step){



		case 0: // Wait


		break;

		case 1:

			if(udp->internal.step == 5){


				esab->Msc._voltage = esab->Param.Voltage; 
				esab->Msc._amps = esab->Param.Amps;

				esab->Cmd.QuickStop = 0;
				esab->Cmd.WeldON = 0;
				
				esab->Msc._quickStop = esab->Cmd.QuickStop;
				esab->Msc._weldON = esab->Cmd.WeldON;
				
				strcpy (send_data , "R.001");
				udp->send.iBufferSize = 5;
				udp->send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->Msc.Cmd_Hrbt_Tmr.IN = 0;
				TON(&esab->Msc.Cmd_Hrbt_Tmr);
				esab->Msc.Cmd_Hrbt_Tmr.IN = 1;
				
				esab->Msc._recev_counts  =	udp->listener.oStatus.counts;
				esab->step ++;

			}


		break;	


		case 2:


			if(esab->Msc._recev_counts  !=  udp->listener.oStatus.counts){

				esab->Cmd.UpdateParam = 1;
				esab->step++;

			}
			else if(esab->Msc.Main_Tmr.Q)
				esab->step = 1;			


		break;

		
		case 3:


			if(esab->Cmd.UpdateParam){
				esab->Cmd.UpdateParam = 0;

				esab->Msc._quickStop = !esab->Cmd.QuickStop;
				esab->Msc._weldON = !esab->Cmd.WeldON;
				esab->Msc._wF_Jog_FWD = !esab->Cmd.WF_Jog_FWD;
				esab->Msc._wF_Jog_REV = !esab->Cmd.WF_Jog_REV;
				esab->Msc._inputArea = !esab->Cmd.InputArea;
				esab->Msc._jogHighSpeed = !esab->Cmd.JogHighSpeed;

				esab->Msc._regulType = esab->Param.RegulType + 1;				
				esab->Msc._voltage = esab->Param.Voltage + 1.0;				
				esab->Msc._amps = esab->Param.Amps + 1.0;

				

			}
			
			if(esab->Msc.Cmd_Hrbt_Tmr.Q){
				//&& (!esab->Cmd.QuickStop)){

				strcpy(send_data , "W.001.000.008=0000000000000000");
				//strcpy(send_data , "W.001.000.008=000000000000000000000");
				
				//////  Byte 0 - Command LSB
				status = 0;
				if(esab->Cmd.WeldON)
					status += 0x1;
				
				//if(esab->Cmd.QuickStop)
					//status += 0x2;

				if(esab->Cmd.WF_Jog_FWD)
					status += 0x10;

				if(esab->Cmd.WF_Jog_REV)
					status += 0x20;			

				Convert_USINT2HEX(&send_data[14], status);

				//strcpy(send_data , "W.001.000.001=00");
				//Convert_USINT2HEX(&send_data[14], status);

				//// Byte 1 - Command MSB

				status = (esab->Cmd.JogHighSpeed)? 0x4:0;
				Convert_USINT2HEX(&send_data[16], status);

				//status = (esab->Cmd.JogHighSpeed)? 0x4:0;				
				//strcpy(send_data , "W.001.001.001=00");
				//Convert_USINT2HEX(&send_data[14], status);

				//// Byte 7 - HeartBeat to ESAB
				Convert_USINT2HEX(&send_data[28], esab->Msc._heartBeat++);

				//strcpy(send_data , "W.001.007.001=00");
				//Convert_USINT2HEX(&send_data[14], esab->Msc._heartBeat++);

				///////////////////////////////////////////////////////
				
				ESAB_UDP.send.iBufferSize = 30;
				ESAB_UDP.send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->step = 11;

				////////////////////////////////////////
				///////////////////////////////////////

				/*
				strcpy(send_data , "W.001.007.001=00");

				Convert_USINT2HEX(&send_data[14], esab->Msc._heartBeat++);
				
				ESAB_UDP.send.iBufferSize = 16;
				ESAB_UDP.send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->step = 11;
				*/

			}
			
			else if((esab->Cmd.QuickStop != esab->Msc._quickStop)
				|| (esab->Cmd.WeldON != esab->Msc._weldON)
				 || (esab->Cmd.WF_Jog_FWD != esab->Msc._wF_Jog_FWD)
				 || (esab->Cmd.WF_Jog_REV != esab->Msc._wF_Jog_REV)
				 ){		
			//else if(0){	 

				/*
				esab->Msc._quickStop = esab->Cmd.QuickStop;
				esab->Msc._weldON = esab->Cmd.WeldON;
				esab->Msc._wF_Jog_FWD = esab->Cmd.WF_Jog_FWD;
				esab->Msc._wF_Jog_REV = esab->Cmd.WF_Jog_REV;
				*/

				status = 0;
				if(esab->Cmd.WeldON)
					status += 0x1;
				
				//if(esab->Cmd.QuickStop)
					//status += 0x2;

				if(esab->Cmd.WF_Jog_FWD)
					status += 0x10;

				if(esab->Cmd.WF_Jog_REV)
					status += 0x20;
				


				strcpy(send_data , "W.001.000.001=00");

				Convert_USINT2HEX(&send_data[14], status);
				
				ESAB_UDP.send.iBufferSize = 16;
				ESAB_UDP.send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->step = 10;

			}
			/*
			else if(esab->Cmd.InputArea != esab->Msc._inputArea){
				esab->Msc._inputArea = esab->Cmd.InputArea;				
			
				strcpy(send_data , "W.001.003.001=00");

				status = 0;
				if(esab->Cmd.InputArea)
					status += 0x4;
				
								
				Convert_USINT2HEX(&send_data[14], status);
				
				ESAB_UDP.send.iBufferSize = 16;
				ESAB_UDP.send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;
				
				esab->step = 10;
			
			}				
			else if(esab->Param.RegulType != esab->Msc._regulType){
				esab->Msc._regulType = esab->Param.RegulType;				
			
				strcpy(send_data , "W.001.009.001=00");
				
				Convert_USINT2HEX(&send_data[14], esab->Param.RegulType);
				
				ESAB_UDP.send.iBufferSize = 16;
				ESAB_UDP.send.iSendCMD = 1;


				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;
				
				esab->step = 10;
			
			}	
			*/
			else if(esab->Param.Voltage != esab->Msc._voltage){
				esab->Msc._voltage = esab->Param.Voltage;

				strcpy (send_data , "W.001.010.002=0000");

				Convert_UINT2HEX(&send_data[14], (esab->Param.Voltage * 10.0));
				
				ESAB_UDP.send.iBufferSize = 18;
				ESAB_UDP.send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->step = 10;
			
			}	
			else if(esab->Param.Amps != esab->Msc._amps){
				esab->Msc._amps = esab->Param.Amps;

				strcpy (send_data , "W.001.014.002=0000");

				Convert_UINT2HEX(&send_data[14], esab->Param.Amps);
				
				ESAB_UDP.send.iBufferSize = 18;
				ESAB_UDP.send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->step = 10;	

			}
			else if(esab->Cmd.JogHighSpeed != esab->Msc._jogHighSpeed){
				esab->Msc._jogHighSpeed = esab->Cmd.JogHighSpeed;

				status = (esab->Cmd.JogHighSpeed)? 0x4:0;
				
				strcpy(send_data , "W.001.001.001=00");

				Convert_USINT2HEX(&send_data[14], status);
				
				ESAB_UDP.send.iBufferSize = 16;
				ESAB_UDP.send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->step = 10;	

			}
			else{

				strcpy (send_data , "R.001");
				udp->send.iBufferSize = 5;
				udp->send.iSendCMD = 1;

				esab->Msc.Main_Tmr.IN = 0;
				TON(&esab->Msc.Main_Tmr);
				esab->Msc.Main_Tmr.IN = 1;

				esab->step = 50;
			}


		break;


		case 10: // Wait and Parse for Responde

			if(esab->Msc._recev_counts  !=  udp->listener.oStatus.counts){
				esab->Msc._recev_counts  ==  udp->listener.oStatus.counts;


				esab->Msc._quickStop = esab->Cmd.QuickStop;
				esab->Msc._weldON = esab->Cmd.WeldON;
				esab->Msc._wF_Jog_FWD = esab->Cmd.WF_Jog_FWD;
				esab->Msc._wF_Jog_REV = esab->Cmd.WF_Jog_REV;
				
				//esab->step = 0;
				esab->step = 3;
			
			}else if(esab->Msc.Main_Tmr.Q){
				ESAB_Tmo_Err = 1;
				esab->step = 1;	

			}

		break;	

		case 11: // Wait and Parse for Responde -  Command HeartBeat

			if(esab->Msc._recev_counts  !=  udp->listener.oStatus.counts){
				esab->Msc._recev_counts  ==  udp->listener.oStatus.counts;

				esab->Msc.Cmd_Hrbt_Tmr.IN = 0;
				TON(&esab->Msc.Cmd_Hrbt_Tmr);
				esab->Msc.Cmd_Hrbt_Tmr.IN = 1;
				
				//esab->step = 0;
				esab->step = 3;
			
			}else if(esab->Msc.Main_Tmr.Q){
				ESAB_Tmo_Err = 2;
				esab->step = 1;
			}

		break;	


		case 50: // Parse Fbk

			if(esab->Msc._recev_counts  !=  udp->listener.oStatus.counts){

				//// Nowe

				// Sprawdzone
				status = Convert_HEX2USINT(&receive_data[2]);
				//esab->Fbk.Status = status & 0x07;
				esab->Fbk.Status = status;
				esab->Fbk.Error = (status & 0x8)? 1:0;
					
				status = Convert_HEX2USINT(&receive_data[4]);
				esab->Fbk.StatusMSB = status;
				esab->Fbk.ReadyToStart = (status & 0x8)? 1:0;

				status = Convert_HEX2USINT(&receive_data[8]);
				esab->Fbk.InputAreaActive = (status & 0x4)? 1:0;

				esab->Fbk.ErrorNum = Convert_HEX2USINT(&receive_data[10]);				
				
				esab->Fbk.actVolts = ((float)Convert_HEX2UINT(&receive_data[18]))/10.0;
				esab->Fbk.actAmps = ((float)Convert_HEX2UINT(&receive_data[22]));
				esab->Fbk.actHeat = ((float)Convert_HEX2UINT(&receive_data[26]));
				
				myINT = (INT)Convert_HEX2UINT(&receive_data[30]);
				esab->Fbk.actWF = (float)myINT;

				esab->Fbk.SetRegulType = Convert_HEX2USINT(&receive_data[40]);
				
				esab->Fbk.SetVoltage = ((float)Convert_HEX2UINT(&receive_data[50]))/10.0;
				esab->Fbk.SetAmps = ((float)Convert_HEX2UINT(&receive_data[54]));
				esab->Fbk.SetWF = ((float)Convert_HEX2UINT(&receive_data[58]));	
				esab->Fbk.SetTVL = ((float)Convert_HEX2UINT(&receive_data[62]));	

				myINT = (INT)Convert_HEX2UINT(&receive_data[14]);
				esab->Fbk.Req_TVL_Speed = (float)myINT;
				
				myINT = (INT)Convert_HEX2UINT(&receive_data[78]);
				esab->Fbk.Req_WF_Speed = (float)myINT;
				
				esab->Fbk.ErrorCode = Convert_HEX2UINT(&receive_data[82]);
				esab->Fbk.ErrorSubCode = Convert_HEX2USINT(&receive_data[86]);
				esab->Fbk.ErrorNodeID = Convert_HEX2USINT(&receive_data[88]);
	
				esab->Fbk.HeartBeat = Convert_HEX2UINT(&receive_data[94]);	

				if(esab->Fbk.ErrorNum != 0){
					if(esab->Fbk.ErrorNum != esab->Msc._errorNum){
						esab->Msc._errorNum = esab->Fbk.ErrorNum;
						esab->Fbk.NewErrorCode = esab->Fbk.ErrorCode;
					}
				}

				esab->Msc._recev_counts  =  udp->listener.oStatus.counts;

				//esab->step = 0;
				esab->step = 3;
			}
			else if(esab->Msc.Main_Tmr.Q){
				ESAB_Tmo_Err = 3;
				esab->step = 1;	
			}

		break;


	}

	TON(&esab->Msc.Main_Tmr);
	TON(&esab->Msc.Cmd_Hrbt_Tmr);

}


void Convert_UINT2HEX( char* txt, UINT inp){
USINT val[4];
char *str;
str = "0123456789ABCDEF";

		val[0] = (USINT)(inp/(256*16));
		val[1] = (USINT)(inp/256%16);
		val[2] = (USINT)(inp%256/16);
		val[3] = (USINT)(inp%256%16);

		txt[2] = str[val[0]];
		txt[3] = str[val[1]];
		txt[0] = str[val[2]];
		txt[1] = str[val[3]];

	

}
void Convert_USINT2HEX( char* txt, USINT inp){
USINT val[2];
char *str;
str = "0123456789ABCDEF";

		val[0] = (USINT)(inp/16);
		val[1] = (USINT)(inp%16);		
		
		txt[0] = str[val[0]];
		txt[1] = str[val[1]];


}

USINT  Convert_HEX2USINT(char* txt){
USINT ret = 0;

	
		if(txt[0] >= 'a')
			ret =((USINT)txt[0] - 'a' + 10)  * 16;
		else if(txt[0] >= 'A')
			ret = ((USINT)txt[0] - 'A' + 10) * 16;
		else if(txt[0] >= '0')
			ret =((USINT)txt[0] - '0') * 16;
	
		
		if(txt[1] >= 'a')
			ret +=(USINT)txt[1] - 'a' + 10;
		else if(txt[1] >= 'A')
			ret +=(USINT)txt[1] - 'A' +10;
		else if(txt[1] >= '0')
			ret +=((USINT)txt[1] - '0');

		
	return ret;	


}

UINT  Convert_HEX2UINT(char* txt){
UINT ret = 0;
UINT ret2 = 0;


		if(txt[0] >= 'a')
			ret =((USINT)txt[0] - 'a' + 10)  * 16;
		else if(txt[0] >= 'A')
			ret = ((USINT)txt[0] - 'A' + 10) * 16;
		else if(txt[0] >= '0')
			ret =((USINT)txt[0] - '0') * 16;
	
		
		if(txt[1] >= 'a')
			ret +=(USINT)txt[1] - 'a' + 10;
		else if(txt[1] >= 'A')
			ret +=(USINT)txt[1] - 'A' +10;
		else if(txt[1] >= '0')
			ret +=((USINT)txt[1] - '0');

		
		
		if(txt[2] >= 'a')
			ret2 =((USINT)txt[2] - 'a' + 10)  * 16;
		else if(txt[2] >= 'A')
			ret2 = ((USINT)txt[2] - 'A' + 10) * 16;
		else if(txt[2] >= '0')
			ret2 =((USINT)txt[2] - '0') * 16;
	
		
		if(txt[3] >= 'a')
			ret2 +=((USINT)txt[3] - 'a' + 10);
		else if(txt[3] >= 'A')
			ret2 +=((USINT)txt[3] - 'A' +10);
		else if(txt[3] >= '0')
			ret2 +=((USINT)txt[3] - '0');
		
		
	return ret  + ret2  * 256;	


}



void ckConvertByteStreamToHex(USINT* in , USINT* out , int lenIn , int lenOut ){
int i , ii , k;	
char *str;
USINT val;	

	str = "0123456789ABCDEF";
	

	for (i = 0 ; i < lenIn ; i++){
	
			val = in[i];
			
			for(k = 1 ; k >=0 ; k-- ){
	
				ii = k+ i*3;
	
	
				if(ii < lenOut){
					out[ii] = str[val % 16];
					val /= 16;
				}
				else {
					
					out[ii - 1] = 0;
					return;
	
				}
	
			}
	
			ii = 2 + i*3;
			
			if(ii < lenOut)
				out[ii] = ' '; 
			else{
	
				out[ii - 1] = 0;
				return;
	
			}
	
		}	
	
		ii++;
			
		if(ii < lenOut)
			out[ii] = 0; 
		else
			out[ii - 1] = 0;		
	

}

