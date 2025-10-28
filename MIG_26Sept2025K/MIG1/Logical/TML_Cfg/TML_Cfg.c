
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobalDef.h>
#include <..\GlobalDecl.h>

#include <..\Lib\Statelib.c>
#include <..\Lib\LoggerLib.c>
#include <..\Lib\WSI_lib.c>
//#include <..\Lib\utilityLib\bgdebug.h>


// Only For ESTOP from Universla Controller
_GLOBAL plcbit ESTOP_out;


void AssignMtrErrorText(char* txt , UDINT i , UDINT val );

_LOCAL plcbit KK_ClearErrors;

////    Super Mini Head
void _INIT ProgramInit(void)
{int i;

	//hardware.Inputs[5] = 1; //ControlModule_Estop_Active
	//hardware.Outputs[7];  // Estop Output

	ESTOP_out = 1;

	TML_mgr[0].pLog = TML_mgr[0].pLog;
	VectorStateMach[0].fParam[0] = VectorStateMach[0].fParam[0];
	TML_MTR_INST[0].AnInp[0] = TML_MTR_INST[0].AnInp[0] ;
	AxisCh[0].AxisID = 0;
	LogMgr.Cmd.Refresh = LogMgr.Cmd.Refresh;

	/***********************************************************/	
	//LogTxt(&LogMgr , "Start TML Cfg %d " , 1);
	LoggerInit(&LogMgr, &LogID, "LogBook1", 2000);
	///////////////////////////////////////
	/***********************************************************/

	
	/////////////////////////////////

	i = TML_AX_OSC; // UnvCntrl_SMH_19-10_25.m.zip

	strcpy(TML_MTR_INST[i].Name , "OSC");
											
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
				
	TML_MTR_INST[i]._cfg.units = 1024 * 4;
	
	//TML_MTR_INST[i]._cfg.rev_motor = -62.0;
	//TML_MTR_INST[i]._cfg.rev_scale = -1.0/ (0.5 * 25.4) ;

	TML_MTR_INST[i]._cfg.rev_motor = 1.0;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/ (100.0) ;

	TML_MTR_INST[i]._cfg.maxVel = 6000.0/60.0    /TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale;

	// (tml->_cfg.units * fabsf(tml->_cfg.rev_motor) * tml->_cfg.rev_scale   * tml->_cfg.loopPeriod * V = 60000/60.0 * TML_MTR_INST[i]._cfg.loopPeriod * 	TML_MTR_INST[i]._cfg.units ;
		
	
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 100.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
	//TML_MTR_INST[i]._cfg.absFbk = 2; // absolute
	TML_MTR_INST[i]._cfg.absFbk = 0;

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;
	TML_MTR_INST[i]._cfg.PVTPeriod = 0.08;					
	
	strcpy(TML_MTR_INST[i]._cfg.programName , "1521_SMH_OSC.sw");
	//TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1521");	
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1525");

	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;

	//TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("674C");  //0x674B
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
	/*
	strcpy(TML_MTR_INST[i].Name , "OSC");
											
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
				
	TML_MTR_INST[i]._cfg.units = 1024 * 4;

	//This is from BPV
	TML_MTR_INST[i]._cfg.rev_motor = (-75.0/19.0) * 50.0;
	//TML_MTR_INST[i]._cfg.rev_scale = -1.0/ (0.625 * 25.4 *M_PI) ; // To jest dobrze
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/ (0.625 * 25.4 *M_PI) ; // A to tylko do eksperymentu

	
	TML_MTR_INST[i]._cfg.maxVel = 8000.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale;
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[TML_AX_OSC];
	TML_MTR_INST[i]._cfg.pAbsLastPos = &OLD_MTR_POS[i];
	
	TML_MTR_INST[i]._int.kVolt = 1365.0;
	
	TML_MTR_INST[i]._cfg.absFbk = 10;

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;
	//TML_MTR_INST[i]._cfg.PVTPeriod = 0.08;
	
	strcpy(TML_MTR_INST[i]._cfg.programName , "1821_BPV2_OSC.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1821");					
	
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("674C");  //0x674B
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	*/

	////////////////////////////////////////
	i = TML_AX_TVL; // /// UnvCntrl_SMH_19-10_25.m.zip

	strcpy(TML_MTR_INST[i].Name , "TVL");
	// Back EMF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.0005; // Slow loop
	TML_MTR_INST[i]._cfg.units = 1024 * 4; // 					
	TML_MTR_INST[i]._cfg.rev_motor = -231.0; 
	TML_MTR_INST[i]._cfg.rev_scale = (36.0/33.0) /( 1.25 * 25.4 * M_PI) ;
				
	TML_MTR_INST[i]._cfg.maxVel = fabsf(4500.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
	TML_MTR_INST[i]._cfg.absFbk = 2;

	//TML_MTR_INST[i]._cfg.HeartBeat = 0.08;
	TML_MTR_INST[i]._cfg.HeartBeat = 0.5;
					
	strcpy(TML_MTR_INST[i]._cfg.programName , "1525_SMH_TVL.sw");											
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1525");
	
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;

	

	
	/*
	strcpy(TML_MTR_INST[i].Name , "TVL");

	// Corrected 29May19
	//TML_MTR_INST[i]._cfg.loopPeriod = 0.0005; // Slow loop
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001; // Slow loop
	// It was in BPV
	TML_MTR_INST[i]._cfg.units = 1024 * 4; // 					
	TML_MTR_INST[i]._cfg.rev_motor = 156;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0 /( 1.25 * 25.4 * M_PI) ;

				
	TML_MTR_INST[i]._cfg.maxVel = fabsf(4500.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
	TML_MTR_INST[i]._cfg.absFbk = 2;

	TML_MTR_INST[i]._cfg.HeartBeat = 0.2;
					
	strcpy(TML_MTR_INST[i]._cfg.programName , "1625_BPV2_TVL.sw");											
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1625");

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	/////////////////////////////////////////////////////////////
	*/

					

	///////////////////////////////////////////////////
	i = TML_AX_INDX; // UnvCntrl_SMH_19-10_25.m.zip

	strcpy(TML_MTR_INST[i].Name , "INDX"); 
											
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
				
	TML_MTR_INST[i]._cfg.units = 1024 * 4;
	TML_MTR_INST[i]._cfg.rev_motor = 3.9 * 50 ;
	TML_MTR_INST[i]._cfg.rev_scale = -1.0/ (0.625 * 25.4 *M_PI) ;
	TML_MTR_INST[i]._cfg.maxVel = fabsf(8000.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
	TML_MTR_INST[i]._cfg.absFbk = 2;

	TML_MTR_INST[i]._cfg.HeartBeat = 0.5;
	TML_MTR_INST[i]._cfg.PVTPeriod = 0.08;
					

	strcpy(TML_MTR_INST[i]._cfg.programName , "1523_SMH_WRST.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1523");
						

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("674C");  //0x674B
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;


	///////////	
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1525");

	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;

	//TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("674C");  //0x674B
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
	/*
												
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
				
	TML_MTR_INST[i]._cfg.units = 500 * 4;
	TML_MTR_INST[i]._cfg.rev_motor = 24;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/5.0;
	TML_MTR_INST[i]._cfg.maxVel = 10000.0; //
	TML_MTR_INST[i]._cfg.maxAcc = 100.0;
	TML_MTR_INST[i]._cfg.absFbk = 0;	

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;

	strcpy(TML_MTR_INST[i]._cfg.programName , "0326_BPV_WP.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0326");
						

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	*/
	////////////////////////////////////////////
	i = TML_AX_AHC; // UnvCntrl_SMH_19-10_25.m.zip
	
	strcpy(TML_MTR_INST[i].Name , "AHC");

	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
	TML_MTR_INST[i]._cfg.units = 4096;
	TML_MTR_INST[i]._cfg.rev_motor = 1;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/5.0;					
	TML_MTR_INST[i]._cfg.maxVel = 1500.0;
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0;	
	//TML_MTR_INST[i]._cfg.satpMax = 27853; // KPP bylo Stauration of proportional gain
	TML_MTR_INST[i]._cfg.satpMax = 32000; // KPP bylo Stauration of proportional gain
	//TML_MTR_INST[i]._cfg.kppMax = 32000; // KPP bylo Stauration of proportional gain
	//TML_MTR_INST[i]._cfg.kipMax = 200; // KPP bylo Stauration of proportional gain
	TML_MTR_INST[i]._cfg.kppMax = 32000;
	TML_MTR_INST[i]._cfg.kipMax = 1000;
	TML_MTR_INST[i]._cfg.absFbk = 0; 

	TML_MTR_INST[i]._cfg.HeartBeat = 0.8;
													
	//TML_MTR_INST[i]._cfg.signFilter = curCfg->Param[parAVC_FbkFilter].val[0];

	//U_BEND_HP_PINS
	strcpy(TML_MTR_INST[i]._cfg.programName , "1526_SMH_AVC.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1526");							
	
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674C , 514D
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;

	///////////	
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1525");

	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;

	//TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("674C");  //0x674B
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	///////////////////

	//TML_MTR_INST[i]._cfg.sftLimitPos = 0.0;
	//TML_MTR_INST[i]._cfg.sftLimitNeg = 0.0;	

	//TML_MTR_INST[i]._cfg.AnalLimitPos = 3900;
	//TML_MTR_INST[i]._cfg.AnalLimitNeg = 100;

	TML_MTR_INST[i]._cfg.MtrType = 1 ; // 3phase brusshless

	/////

	TML_MTR_INST[i]._cfg.loopPeriod = 0.0005; // Slow loop
	TML_MTR_INST[i]._cfg.units = 1024 * 4; //					
	TML_MTR_INST[i]._cfg.rev_motor = -231.0; 
	TML_MTR_INST[i]._cfg.rev_scale = (36.0/33.0) /( 1.25 * 25.4 * M_PI) ;
				
	TML_MTR_INST[i]._cfg.maxVel = fabsf(4500.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
	TML_MTR_INST[i]._cfg.absFbk = 2;

	//TML_MTR_INST[i]._cfg.HeartBeat = 0.08;
	TML_MTR_INST[i]._cfg.HeartBeat = 0.5;
					
	strcpy(TML_MTR_INST[i]._cfg.programName , "1525_SMH_TVL.sw");											
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1525");

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
					

	/*
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
	TML_MTR_INST[i]._cfg.units = 4096;
	TML_MTR_INST[i]._cfg.rev_motor = 1;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/5.0;					
	TML_MTR_INST[i]._cfg.maxVel = 1500.0;
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0;	
	//TML_MTR_INST[i]._cfg.satpMax = 27853; // KPP bylo Stauration of proportional gain
	TML_MTR_INST[i]._cfg.satpMax = 32000; // KPP bylo Stauration of proportional gain
	//TML_MTR_INST[i]._cfg.kppMax = 32000; // KPP bylo Stauration of proportional gain
	//TML_MTR_INST[i]._cfg.kipMax = 200; // KPP bylo Stauration of proportional gain
	TML_MTR_INST[i]._cfg.kppMax = 32000;
	TML_MTR_INST[i]._cfg.kipMax = 1000;
	TML_MTR_INST[i]._cfg.absFbk = 0; 

	TML_MTR_INST[i]._cfg.HeartBeat = 0.8;
													
	//TML_MTR_INST[i]._cfg.signFilter = curCfg->Param[parAVC_FbkFilter].val[0];

	strcpy(TML_MTR_INST[i]._cfg.programName , "1526_SMH_AVC.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1526");							
	
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674C , 514D
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;

	//TML_MTR_INST[i]._cfg.sftLimitPos = 0.0;
	//TML_MTR_INST[i]._cfg.sftLimitNeg = 0.0;	

	TML_MTR_INST[i]._cfg.AnalLimitPos = 3900;
	TML_MTR_INST[i]._cfg.AnalLimitNeg = 100;

	TML_MTR_INST[i]._cfg.MtrType = 1 ; // 3phase brusshless
	*/
	
	////////////////////////////////////////////
	i = TML_AX_WF; // UnvCntrl_SMH_19-10_25.m.zip
	
	strcpy(TML_MTR_INST[i].Name , "WF");

	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
	TML_MTR_INST[i]._cfg.units = 1024 * 4; //					
	//TML_MTR_INST[i]._cfg.rev_motor = 35;					

	TML_MTR_INST[i]._cfg.maxVel = 400 * 25.4/ 60.0; //// 520 ipm
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0;
	TML_MTR_INST[i]._cfg.absFbk = 0;	

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;

	strcpy(TML_MTR_INST[i]._cfg.programName , "1522_SMH_WF.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1522");
						

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("674C");  
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
	////
	
	TML_MTR_INST[i]._cfg.loopPeriod = 0.0005; // Slow loop
	TML_MTR_INST[i]._cfg.units = 1024 * 4; // 					
	TML_MTR_INST[i]._cfg.rev_motor = -231.0; 
	TML_MTR_INST[i]._cfg.rev_scale = (36.0/33.0) /( 1.25 * 25.4 * M_PI) ;
				
	TML_MTR_INST[i]._cfg.maxVel = fabsf(4500.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
	TML_MTR_INST[i]._cfg.absFbk = 2;

	//TML_MTR_INST[i]._cfg.HeartBeat = 0.08;
	TML_MTR_INST[i]._cfg.HeartBeat = 0.5;
					
	strcpy(TML_MTR_INST[i]._cfg.programName , "1525_SMH_TVL.sw");											
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1525");
	
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	
	
	/*
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
	TML_MTR_INST[i]._cfg.units = 1024 * 4; //					

	TML_MTR_INST[i]._cfg.rev_scale = 1.0 /( 0.75 * 25.4 * M_PI) ;
	TML_MTR_INST[i]._cfg.rev_motor = 35.0;				

	TML_MTR_INST[i]._cfg.maxVel = 520 * 25.4/ 60.0; //// 520 ipm
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 4.0;
	TML_MTR_INST[i]._cfg.absFbk = 0;	

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;

	strcpy(TML_MTR_INST[i]._cfg.programName , "0325_BPV_WF.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0325");
						

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;
	*/
	/***********************************************************/
	/***************************************************************/
	/////////////////////////////////////////////////////////////////

	TML_MTR_INST[TML_AX_TVL].pState = &VectorStateMach[VECT_AX_TVL];
	TML_MTR_INST[TML_AX_OSC].pState = &VectorStateMach[VECT_AX_OSC];
	TML_MTR_INST[TML_AX_INDX].pState = &VectorStateMach[VECT_AX_INDX];
	TML_MTR_INST[TML_AX_AHC].pState = &VectorStateMach[VECT_AX_AHC];
	TML_MTR_INST[TML_AX_WF].pState = &VectorStateMach[VECT_AX_WF];

	for(i=0 ; i < VECTORS_NUM; i++)
		VectorStateMach[i].Indx = i;
	

	strcpy(VectorStateMach[VECT_AX_TVL].Name , TML_MTR_INST[TML_AX_TVL].Name);
	strcpy(VectorStateMach[VECT_AX_OSC].Name , TML_MTR_INST[TML_AX_OSC].Name);
	strcpy(VectorStateMach[VECT_AX_INDX].Name , TML_MTR_INST[TML_AX_INDX].Name);
	strcpy(VectorStateMach[VECT_AX_AHC].Name , TML_MTR_INST[TML_AX_AHC].Name);
	strcpy(VectorStateMach[VECT_AX_WF].Name , TML_MTR_INST[TML_AX_WF].Name);
	
	TML_mgr[0].hrdw[0].AxisNum = 0; 
	TML_mgr[0].hrdw[1].AxisNum = 5;

	TML_mgr[0].pAxis[0] = 0;
	TML_mgr[0].pAxis[1] = &AxisCh[0];

	TML_mgr[0].pTML[0] = 0;
	TML_mgr[0].pTML[1] = &TML_MTR_INST[0];

	
	//AxisCh[TML_AX_TVL].AxisID	= 25;	// BPV2 - TVL
	AxisCh[TML_AX_TVL].AxisID	= 17;	// tylko w biurze
	AxisCh[TML_AX_OSC].AxisID 	= 14;	// BPV2 - OSC	
	//AxisCh[TML_AX_OSC].AxisID 	= 21;	// BPV2 - OSC	
	AxisCh[TML_AX_INDX].AxisID 	= 12;	// BPV2 - WP
	//AxisCh[TML_AX_INDX].AxisID 	= 6;	// BPV2 - WP	
	AxisCh[TML_AX_AHC].AxisID 	= 16; 	// BPV2 - AVC	
	AxisCh[TML_AX_WF].AxisID 	= 15;	// BPV2 - WF
	//AxisCh[TML_AX_WF].AxisID 	= 5;	// BPV2 - WF

	//////////////////////////////////////////////////////

	TML_MTR_INST[TML_AX_TVL].Disable = 0;
	TML_MTR_INST[TML_AX_OSC].Disable = 0;
	TML_MTR_INST[TML_AX_INDX].Disable = 0;
	TML_MTR_INST[TML_AX_AHC].Disable = 0;
	TML_MTR_INST[TML_AX_WF].Disable = 0;

	VectorStateMach[VECT_AX_TVL].simul = 0;
	VectorStateMach[VECT_AX_OSC].simul = 0;
	VectorStateMach[VECT_AX_INDX].simul = 0;
	VectorStateMach[VECT_AX_AHC].simul = 0;
	VectorStateMach[VECT_AX_WF].simul = 0;

	VectorStateMach[VECT_AX_TVL].testMode = 0;
	VectorStateMach[VECT_AX_OSC].testMode = 0;
	VectorStateMach[VECT_AX_INDX].testMode = 0;
	VectorStateMach[VECT_AX_AHC].testMode = 0;
	VectorStateMach[VECT_AX_WF].testMode = 0;

	///////////////////////////////////////////////////////////
	/// Tak jest w BPV2 

	/*
	AxisCh[TML_AX_TVL].AxisID	= 25;		
	AxisCh[TML_AX_OSC].AxisID 	= 21;	
	AxisCh[TML_AX_AVC].AxisID 	= 26;
	AxisCh[TML_AX_WRST].AxisID 	= 23;

	AxisCh[TML_AX_INDX].AxisID 	= 22;
	AxisCh[TML_AX_INDX_1].AxisID 	= 24;

	
	AxisCh[TML_AX_WF].AxisID 	= 5;
	AxisCh[TML_AX_WP].AxisID 	= 6;
	AxisCh[TML_AX_WF_1].AxisID 	= 7;
	AxisCh[TML_AX_WP_1].AxisID 	= 8;
	*/
	
}

#if 0
void _INIT ProgramInit(void)
{int i;

	//hardware.Inputs[5] = 1; //ControlModule_Estop_Active
	//hardware.Outputs[7];  // Estop Output

	ESTOP_out = 1;

	TML_mgr[0].pLog = TML_mgr[0].pLog;
	VectorStateMach[0].fParam[0] = VectorStateMach[0].fParam[0];
	TML_MTR_INST[0].AnInp[0] = TML_MTR_INST[0].AnInp[0] ;
	AxisCh[0].AxisID = 0;
	LogMgr.Cmd.Refresh = LogMgr.Cmd.Refresh;

	/***********************************************************/	
	//LogTxt(&LogMgr , "Start TML Cfg %d " , 1);
	LoggerInit(&LogMgr, &LogID, "LogBook1", 2000);
	///////////////////////////////////////
	/***********************************************************/

	
	/////////////////////////////////

	i = TML_AX_OSC;
	
	strcpy(TML_MTR_INST[i].Name , "OSC");
											
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
				
	TML_MTR_INST[i]._cfg.units = 1024 * 4;

	//This is from BPV
	TML_MTR_INST[i]._cfg.rev_motor = (-75.0/19.0) * 50.0;
	//TML_MTR_INST[i]._cfg.rev_scale = -1.0/ (0.625 * 25.4 *M_PI) ; // To jest dobrze
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/ (0.625 * 25.4 *M_PI) ; // A to tylko do eksperymentu

	
	TML_MTR_INST[i]._cfg.maxVel = 8000.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale;
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[TML_AX_OSC];
	TML_MTR_INST[i]._cfg.pAbsLastPos = &OLD_MTR_POS[i];
	
	TML_MTR_INST[i]._int.kVolt = 1365.0;
	
	TML_MTR_INST[i]._cfg.absFbk = 10;

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;
	//TML_MTR_INST[i]._cfg.PVTPeriod = 0.08;
	
	strcpy(TML_MTR_INST[i]._cfg.programName , "1821_BPV2_OSC.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1821");					
	
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("674C");  //0x674B
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;


	////////////////////////////////////////
	i = TML_AX_TVL;
	
	strcpy(TML_MTR_INST[i].Name , "TVL");

	// Corrected 29May19
	//TML_MTR_INST[i]._cfg.loopPeriod = 0.0005; // Slow loop
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001; // Slow loop
	// It was in BPV
	TML_MTR_INST[i]._cfg.units = 1024 * 4; // 					
	TML_MTR_INST[i]._cfg.rev_motor = 156;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0 /( 1.25 * 25.4 * M_PI) ;

				
	TML_MTR_INST[i]._cfg.maxVel = fabsf(4500.0/60.0/TML_MTR_INST[i]._cfg.rev_motor/ TML_MTR_INST[i]._cfg.rev_scale);
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0 ;
	TML_MTR_INST[i]._cfg.absLastPos = OLD_MTR_POS[i];
	TML_MTR_INST[i]._cfg.absFbk = 2;

	TML_MTR_INST[i]._cfg.HeartBeat = 0.2;
					
	strcpy(TML_MTR_INST[i]._cfg.programName , "1625_BPV2_TVL.sw");											
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1625");

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674B
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;


	///////////////////////////////////////////////////
	i = TML_AX_INDX;
	
	strcpy(TML_MTR_INST[i].Name , "INDX"); 					
												
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
				
	TML_MTR_INST[i]._cfg.units = 500 * 4;
	TML_MTR_INST[i]._cfg.rev_motor = 24;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/5.0;
	TML_MTR_INST[i]._cfg.maxVel = 10000.0; //
	TML_MTR_INST[i]._cfg.maxAcc = 100.0;
	TML_MTR_INST[i]._cfg.absFbk = 0;	

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;

	strcpy(TML_MTR_INST[i]._cfg.programName , "0326_BPV_WP.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0326");
						

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;

	////////////////////////////////////////////
	i = TML_AX_AHC;
	
	strcpy(TML_MTR_INST[i].Name , "AHC");
							
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
	TML_MTR_INST[i]._cfg.units = 4096;
	TML_MTR_INST[i]._cfg.rev_motor = 1;
	TML_MTR_INST[i]._cfg.rev_scale = 1.0/5.0;					
	TML_MTR_INST[i]._cfg.maxVel = 1500.0;
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 10.0;	
	//TML_MTR_INST[i]._cfg.satpMax = 27853; // KPP bylo Stauration of proportional gain
	TML_MTR_INST[i]._cfg.satpMax = 32000; // KPP bylo Stauration of proportional gain
	//TML_MTR_INST[i]._cfg.kppMax = 32000; // KPP bylo Stauration of proportional gain
	//TML_MTR_INST[i]._cfg.kipMax = 200; // KPP bylo Stauration of proportional gain
	TML_MTR_INST[i]._cfg.kppMax = 32000;
	TML_MTR_INST[i]._cfg.kipMax = 1000;
	TML_MTR_INST[i]._cfg.absFbk = 0; 

	TML_MTR_INST[i]._cfg.HeartBeat = 0.8;
													
	//TML_MTR_INST[i]._cfg.signFilter = curCfg->Param[parAVC_FbkFilter].val[0];

	strcpy(TML_MTR_INST[i]._cfg.programName , "1526_SMH_AVC.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("1526");							
	
	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  //0x674C , 514D
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;

	//TML_MTR_INST[i]._cfg.sftLimitPos = 0.0;
	//TML_MTR_INST[i]._cfg.sftLimitNeg = 0.0;	

	TML_MTR_INST[i]._cfg.AnalLimitPos = 3900;
	TML_MTR_INST[i]._cfg.AnalLimitNeg = 100;

	TML_MTR_INST[i]._cfg.MtrType = 1 ; // 3phase brusshless

	////////////////////////////////////////////
	i = TML_AX_WF;
	strcpy(TML_MTR_INST[i].Name , "WF");
	
	// Analog Feedback from REF
	TML_MTR_INST[i]._cfg.loopPeriod = 0.001;
	TML_MTR_INST[i]._cfg.units = 1024 * 4; //					

	TML_MTR_INST[i]._cfg.rev_scale = 1.0 /( 0.75 * 25.4 * M_PI) ;
	TML_MTR_INST[i]._cfg.rev_motor = 35.0;				

	TML_MTR_INST[i]._cfg.maxVel = 520 * 25.4/ 60.0; //// 520 ipm
	TML_MTR_INST[i]._cfg.maxAcc = TML_MTR_INST[i]._cfg.maxVel * 4.0;
	TML_MTR_INST[i]._cfg.absFbk = 0;	

	TML_MTR_INST[i]._cfg.HeartBeat = 0.4;

	strcpy(TML_MTR_INST[i]._cfg.programName , "0325_BPV_WF.sw");
	TML_MTR_INST[i].ProgramVersion = ASC2UDINT("0325");
						

	TML_MTR_INST[i].FirmwareVersion = ASC2UDINT("6A2B");  
	TML_MTR_INST[i]._cfg.Appl_ID_adr = 0x7FCF;
	TML_MTR_INST[i]._cfg.NotAutoDownload = 1;

	/***********************************************************/
	/***************************************************************/
	/////////////////////////////////////////////////////////////////

	TML_MTR_INST[TML_AX_TVL].pState = &VectorStateMach[VECT_AX_TVL];
	TML_MTR_INST[TML_AX_OSC].pState = &VectorStateMach[VECT_AX_OSC];
	TML_MTR_INST[TML_AX_INDX].pState = &VectorStateMach[VECT_AX_INDX];
	TML_MTR_INST[TML_AX_AHC].pState = &VectorStateMach[VECT_AX_AHC];
	TML_MTR_INST[TML_AX_WF].pState = &VectorStateMach[VECT_AX_WF];

	strcpy(VectorStateMach[VECT_AX_TVL].Name , TML_MTR_INST[TML_AX_TVL].Name);
	strcpy(VectorStateMach[VECT_AX_OSC].Name , TML_MTR_INST[TML_AX_OSC].Name);
	strcpy(VectorStateMach[VECT_AX_INDX].Name , TML_MTR_INST[TML_AX_INDX].Name);
	strcpy(VectorStateMach[VECT_AX_AHC].Name , TML_MTR_INST[TML_AX_AHC].Name);
	strcpy(VectorStateMach[VECT_AX_WF].Name , TML_MTR_INST[TML_AX_WF].Name);
	
	TML_mgr[0].hrdw[0].AxisNum = 0; 
	TML_mgr[0].hrdw[1].AxisNum = 5;

	TML_mgr[0].pAxis[0] = 0;
	TML_mgr[0].pAxis[1] = &AxisCh[0];

	TML_mgr[0].pTML[0] = 0;
	TML_mgr[0].pTML[1] = &TML_MTR_INST[0];

	
	AxisCh[TML_AX_TVL].AxisID	= 25;	// BPV2 - TVL	
	AxisCh[TML_AX_OSC].AxisID 	= 21;	// BPV2 - OSC	
	AxisCh[TML_AX_INDX].AxisID 	= 6;	// BPV2 - WP	
	AxisCh[TML_AX_AHC].AxisID 	= 26; 	// BPV2 - AVC	
	AxisCh[TML_AX_WF].AxisID 	= 5;	// BPV2 - WF	

	/// Tak jest w BPV2 

	/*
	AxisCh[TML_AX_TVL].AxisID	= 25;		
	AxisCh[TML_AX_OSC].AxisID 	= 21;	
	AxisCh[TML_AX_AVC].AxisID 	= 26;
	AxisCh[TML_AX_WRST].AxisID 	= 23;

	AxisCh[TML_AX_INDX].AxisID 	= 22;
	AxisCh[TML_AX_INDX_1].AxisID 	= 24;

	
	AxisCh[TML_AX_WF].AxisID 	= 5;
	AxisCh[TML_AX_WP].AxisID 	= 6;
	AxisCh[TML_AX_WF_1].AxisID 	= 7;
	AxisCh[TML_AX_WP_1].AxisID 	= 8;
	*/
	
}
#endif
void _CYCLIC ProgramCyclic(void)
{int i , k ;


	LoggerSrv(&LogMgr);
	
/*
	if(ErrorMgr.Error){

		k = ErrorMgr.lastItemIndx - 5;
		if( k < 0)
			k =0;
	
		for(i = 0 ; i < 5 ; i++){					

			if((k + i)<= ErrorMgr.lastItemIndx){

				if((ErrorMgr.history[k + i]>=ERROR_DRV_01)
					&& (ErrorMgr.history[k + i] < ERROR_DRV_41)){

					AssignMtrErrorText(ErrorMgr_Txt[i] , ErrorMgr.history[k + i] , ErrorMgr.logs[ErrorMgr.history[k + i]].value);
				
				}
				else					
					xsprintf(ErrorMgr_Txt[i], "%s: %X" , ErrorMgr.logs[ErrorMgr.history[k + i]].txt , ErrorMgr.logs[ErrorMgr.history[k + i]].value);
			}
			else
				strcpy(ErrorMgr_Txt[i] , " -------------- ");

		}	
	}
*/	
}

void _EXIT ProgramExit(void)
{

}

void AssignMtrErrorText(char* txt , UDINT i , UDINT val ){

plcstring errortxt[64];

	if(i < ERROR_DRV_11)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i-1].Name);
	else if(i < ERROR_DRV_21)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i - ERROR_DRV_11].Name);
	else if(i < ERROR_DRV_31)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i- ERROR_DRV_21].Name);
	else if(i < ERROR_DRV_41)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i- ERROR_DRV_31].Name);

	switch(val){


		case 1: strcat(txt , "CANbus Error"); 			break;

		case 2: strcat(txt , "Short-Circuit Error"); 		break;

		case 3: strcat(txt , "Invalid Setup Error"); 		break;

		case 4: strcat(txt , "Control (Position) Error "); 		break;

		case 5: strcat(txt , "Serial Comm Error"); 		break;

		case 6: strcat(txt , "Feedback Error"); 		break;

		case 7: strcat(txt , "POT Limit Error"); 			break;

		case 8: strcat(txt , "NOT Limit Error"); 			break;

		case 9: strcat(txt , "Instantanious Over-Current Error"); 			break;

		case 10: strcat(txt , "I2t Over-Current Error"); 					break;

		case 11: strcat(txt , "Over-Temperature Error - Motor"); 	break;

		case 12: strcat(txt , "Over-Temperature Error - Drive"); 	break;

		case 13: strcat(txt , "Over-Voltage Error"); 		break;

		case 14: strcat(txt , "Under-Voltage Error"); 		break;

		case 15: strcat(txt , "Command Error"); 		break;

		case 16: strcat(txt , "E-Stop Enable Inactive Error"); 	break;

		case 17: strcat(txt , "Init Error"); 	break;

		case 18: strcat(txt , "No Comunnication"); 	break;

		default: xsprintf(txt+strlen(txt), "Unknown Error %X " , val);

	}


}



