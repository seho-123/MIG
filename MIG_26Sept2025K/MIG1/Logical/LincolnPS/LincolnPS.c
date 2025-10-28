
///   ARC Link address: "192.168.10.2"

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif



#include <..\Lib\utilityLib\bgdebug.h>

#include <..\Lib\libUDP\libUDP.h>
#include <..\Lib\libUDP\libUDP.c>


#include <..\Lib\arclink\arclink.h>
#include <..\Lib\arclink\arclink.c>

#define REC_BUF_LEN 	512
#define REC_NUM 	8
#define ARCCOM_TIMEOUT 2000


_GLOBAL Arc_Link_typ ArcLink;

/*
#define REC_BUF_LEN 	4+1
#define REC_NUM 	4
*/

USINT send_data[REC_BUF_LEN];
USINT receive_data[REC_BUF_LEN];
USINT temp_data[REC_BUF_LEN * 3];


_LOCAL RingBuffer RingBufIn;
_LOCAL RingBuffer RingBufOut;
char RingBufInData[(REC_BUF_LEN +4) * REC_NUM];
char RingBufOutData[(REC_BUF_LEN +4) * REC_NUM];
char MsgInData[REC_BUF_LEN];
char MsgOutData[REC_BUF_LEN];


_LOCAL Arc_Link_typ *arcLink;
_LOCAL UDP_typ2 ArcLink_UDP;
_LOCAL plcbit Run_DispatchOutgoingMessages;


_LOCAL char ArcLinkPS_IP_Address[16];
_LOCAL char ArcLinkPLC_IP_Address[16];
_LOCAL plcbit ArcLink_Run;


_LOCAL plcbit KK_ReadParamData;

/*
_LOCAL plcbit RunOutgoingDispatcher; 

/// Test Ring Buffer

_LOCAL plcbit KK_Write , KK_Read;
_LOCAL plcstring KK_text_Write[32] ,  KK_text_Read[32];
_LOCAL UDINT KK_Write_Len , KK_Read_Len;
_LOCAL plcbit KK_isEmpty , KK_isFULL;


_LOCAL plcbit K_Send_Req_Action_28 , K_Send_Resp_Action_28;

const char Action_28_Request[29] = { 
	0x00, 0x02, 0x0a, 0x4a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13,
	0x00, 0x32, 0x08, 0x00, 0x00, 0x01, 0x02, 0x01, 0x00, 0x16,
	0x00, 0x00, 0x00, 0xff, 0x01, 0xfe, 0x01, 0x00, 0x01,
};

const char Action_28_Respond[16] = { 
	0x00, 0x03, 0x0A, 0x4A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
	0x01, 0x00, 0x00, 0x06, 0x00, 0x00,

};

*/

////////////////////////////////////////////


//UDP Receive Callback function to push data on ring buffer 
static void udpReceivedNewData(UDINT pRxBuffer, UDINT RxDataLen , UDINT ringBuf){

	DBG("New Data Received  of Length %d \r\n" , RxDataLen);
	
	ckConvertByteStreamToHex(pRxBuffer, temp_data, RxDataLen, sizeof(temp_data));
	DBG("(Hex): %s  \r\n" , temp_data);
    
	
	if(ringBuf){		
	
		if(!RingBuffer_isFull((RingBuffer*) ringBuf))		
			RingBuffer_writeRecord((RingBuffer*) ringBuf, pRxBuffer, RxDataLen);		
	
	}
	
}
//UDP Transmit Callback function for debugging purpose
static void udpTransmitedNewData(UDINT pTxBuffer, UDINT pTxDataLen){

	
	DBG("New Data Trasmitted of Length %d  \r\n " , pTxDataLen );
	ckConvertByteStreamToHex(pTxBuffer, temp_data, pTxDataLen, sizeof(temp_data));	
	DBG("(Hex): %s  \r\n" , temp_data);	
	
}

//handle network timeout
void ArcLink_Timer(Arc_Link_typ *arc);

//handle error/status interface
void error_status_handler(Arc_Link_typ *arc);

void _INIT ProgramInit(void){	
Arc_Link_typ *arc;
UDP_typ2 *udp;

	//ArcLink.robot.Comm.CntrlParamReq.Attribute_ID = 517;
	ArcLink.robot.Comm.CntrlParamReq.Attribute_ID = 0;
	ArcLink.robot.Comm.CntrlParamReq.Weld_Mode = 200;

	/*
	Work-point	517 Primary control.
	Trim	518 Arc length control (typically).  
	Wave-Control 1	519 Process adjustment.
	Wave-Control 2	520 Process adjustment.
	Wave-Control 3	521 Process adjustment.
	Wave-Control 4	522 Process adjustment.
	*/

	ArcLink_Run = 1;

	strcpy(ArcLinkPS_IP_Address , "192.168.10.2" );
	//strcpy(ArcLinkPS_IP_Address , "192.168.1.2" ); // Przez PowerLink Module

	strcpy(ArcLinkPLC_IP_Address , "192.168.10.123" ); // My PLC  Address
	
	Run_DispatchOutgoingMessages = 1;

	udp = &ArcLink_UDP;

	//if(!arcLink)
		//arcLink = ArcLink_init(REC_NUM , REC_BUF_LEN );

	arcLink = &ArcLink;	

	if(arcLink){
		udp->listener_config.port = 4323;
		udp->listener_config.buffer = receive_data;
		udp->listener_config.bufferSize = REC_BUF_LEN;
		udp->listener.iRxCallback = (UDINT)udpReceivedNewData;
		//udp->listener.iRxRingBuf = arcLink->bufIN;
		udp->listener.iRxRingBuf = (UDINT)&RingBufIn;

		//strcpy(udp->send.iIPaddress , "192.168.10.2");
		//	strcpy(udp->send.iIPaddress , "192.168.36.88");
		//strcpy(udp->send.iIPaddress , "192.168.10.178");
		udp->send.iPort = 4323;
		udp->send.iBuffer = send_data;
		udp->send.iBufferSize = REC_BUF_LEN;	
		udp->send.iTxCallback = (UDINT)udpTransmitedNewData;

		udp->internal.UdpOpen_0.pIfAddr = (UDINT)&ArcLinkPLC_IP_Address[0];

	}	
}

void _CYCLIC ProgramCyclic(void){

UDP_typ2 *udp;

	udp = &ArcLink_UDP;

	if(!arcLink)
		return;

	if(!RingBufIn.buffer)
		RingBuffer_Init(&RingBufIn , &RingBufInData , REC_NUM , REC_BUF_LEN);

	if(!RingBufOut.buffer)
		RingBuffer_Init(&RingBufOut , &RingBufOutData , REC_NUM , REC_BUF_LEN);

	
	if(!arcLink->bufIN
		|| !arcLink->bufOUT
		|| !arcLink->internal.pInMsg
		|| !arcLink->internal.pOutMsg)
	ArcLink_init(arcLink , &RingBufIn, &RingBufOut, MsgInData  , MsgOutData, REC_BUF_LEN );

	//////////////////////////////////////
	if(Run_DispatchOutgoingMessages){

		if(!RingBuffer_isEmpty(arcLink->bufOUT) && !udp->send.iSendCMD){	

			udp->send.iBufferSize = RingBuffer_readRecord(arcLink->bufOUT, udp->send.iBuffer);	
			udp->send.iSendCMD = 1;
			
		}

	}	

	
	if((strlen(ArcLinkPS_IP_Address) > 0) && ArcLink_Run) {

		if(udp->internal.step == 0)
			strcpy(udp->send.iIPaddress , ArcLinkPS_IP_Address);

		
		
		Udp_cyclic(udp);
		ArcLink_cyclic(arcLink);	

	}

	 if(KK_ReadParamData){

	 	KK_ReadParamData = 0;

		ArcLink.robot.Intrf.xCMD = ROBOT_READ_WC_PARAM_DATA;
		ArcLink.robot.Comm.CntrlParamReq.Attribute_ID++;
		if(ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  < 517)
			ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 517;
		else if(ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  > 522)
			ArcLink.robot.Comm.CntrlParamReq.Attribute_ID  = 522;

	 }
}


