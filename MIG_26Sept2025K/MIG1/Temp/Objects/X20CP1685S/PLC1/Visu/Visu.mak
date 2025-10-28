######################################################
#                                                    #
# Automatic generated Makefile for Visual Components #
#                                                    #
#                  Do NOT edit!                      #
#                                                    #
######################################################

VCC:=@'$(AS_BIN_PATH)/br.vc.pc.exe'
LINK:=@'$(AS_BIN_PATH)/BR.VC.Link.exe'
MODGEN:=@'$(AS_BIN_PATH)/BR.VC.ModGen.exe'
VCPL:=@'$(AS_BIN_PATH)/BR.VC.PL.exe'
VCHWPP:=@'$(AS_BIN_PATH)/BR.VC.HWPP.exe'
VCDEP:=@'$(AS_BIN_PATH)/BR.VC.Depend.exe'
VCFLGEN:=@'$(AS_BIN_PATH)/BR.VC.lfgen.exe'
VCREFHANDLER:=@'$(AS_BIN_PATH)/BR.VC.CrossReferenceHandler.exe'
VCXREFEXTENDER:=@'$(AS_BIN_PATH)/BR.AS.CrossRefVCExtender.exe'
RM=CMD /C DEL
PALFILE_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Palette.vcr
VCCFLAGS_Visu=-server -proj Visu -vc '$(AS_PROJECT_PATH)/Logical/Visu/VCObject.vc' -prj_path '$(AS_PROJECT_PATH)' -temp_path '$(AS_TEMP_PATH)' -cfg $(AS_CONFIGURATION) -plc $(AS_PLC) -plctemp $(AS_TEMP_PLC) -cpu_path '$(AS_CPU_PATH)'
VCFIRMWARE=4.73.2
VCFIRMWAREPATH=$(AS_VC_PATH)/Firmware/V4.73.2/SG4
VCOBJECT_Visu=$(AS_PROJECT_PATH)/Logical/Visu/VCObject.vc
VCSTARTUP='vcstart.br'
VCLOD='vclod.br'
VCSTPOST='vcstpost.br'
TARGET_FILE_Visu=$(AS_CPU_PATH)/Visu.br
OBJ_SCOPE_Visu=
PRJ_PATH_Visu=$(AS_PROJECT_PATH)
SRC_PATH_Visu=$(AS_PROJECT_PATH)/Logical/$(OBJ_SCOPE_Visu)/Visu
TEMP_PATH_Visu=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu
TEMP_PATH_Shared=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared
TEMP_PATH_ROOT_Visu=$(AS_TEMP_PATH)
VC_LIBRARY_LIST_Visu=$(TEMP_PATH_Visu)/libraries.vci
VC_XREF_BUILDFILE_Visu=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcxref.build
VC_XREF_CLEANFILE=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcxref.clean
VC_LANGUAGES_Visu=$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr
CPUHWC='$(TEMP_PATH_Visu)/cpuhwc.vci'
VC_STATIC_OPTIONS_Visu='$(TEMP_PATH_Visu)/vcStaticOptions.xml'
VC_STATIC_OPTIONS_Shared='$(TEMP_PATH_Shared)/vcStaticOptions.xml'
# include Shared and Font Makefile (only once)
	include $(AS_TEMP_PATH)/objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCFntDat/Font_Visu.mak
ifneq ($(VCINC),1)
	VCINC=1
	include $(AS_TEMP_PATH)/objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/VCShared.mak
endif

DEPENDENCIES_Visu=-d vcgclass -profile 'False'
DEFAULT_STYLE_SHEET_Visu='Source[local].StyleSheet[Color]'
SHARED_MODULE=$(TEMP_PATH_ROOT_Visu)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcshared.br
LFNTFLAGS_Visu=-P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)'
BDRFLAGS_Visu=-P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)'

# Local Libs
LIB_LOCAL_OBJ_Visu=$(TEMP_PATH_Visu)/localobj.vca

# Hardware sources
PANEL_HW_OBJECT_Visu=$(TEMP_PATH_ROOT_Visu)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu/dis.Hardware.vco
PANEL_HW_VCI_Visu=$(TEMP_PATH_ROOT_Visu)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu/dis.Hardware.vci
PANEL_HW_SOURCE_Visu=C:/projects/MIG/MIG_26Sept2025K/MIG1/Physical/X20CP1685S/Hardware.hw 
DIS_OBJECTS_Visu=$(PANEL_HW_OBJECT_Visu) $(KEYMAP_OBJECTS_Visu)

# KeyMapping flags
$(TEMP_PATH_Visu)/dis.PS2-Keyboard.vco: $(AS_PROJECT_PATH)/Physical/X20CP1685S/PLC1/VC/PS2-Keyboard.dis $(PANEL_HW_SOURCE_Visu)
	$(VCHWPP) -f '$(PANEL_HW_SOURCE_Visu)' -o '$(subst .vco,.vci,$(TEMP_PATH_Visu)/dis.PS2-Keyboard.vco)' -n Visu -d Visu -pal '$(PALFILE_Visu)' -c '$(AS_CONFIGURATION)' -p '$(AS_PLC)' -ptemp '$(AS_TEMP_PLC)' -B 'J4.93' -L 'visapi: V*' -hw '$(CPUHWC)' -warninglevel 2 -so $(VC_STATIC_OPTIONS_Visu) -sos $(VC_STATIC_OPTIONS_Shared) -keyboard '$(AS_PROJECT_PATH)/Physical/X20CP1685S/PLC1/VC/PS2-Keyboard.dis' -fp '$(AS_VC_PATH)/Firmware/V4.73.2/SG4' -prj '$(AS_PROJECT_PATH)' -apj 'MIG1' -sfas -vcob '$(VCOBJECT_Visu)'
	$(VCC) -f '$(subst .vco,.vci,$@)' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -k '$(VCVK_SOURCES_Visu)' $(VCCFLAGS_Visu) -p Visu -sfas

KEYMAP_SOURCES_Visu=$(AS_PROJECT_PATH)/Physical/X20CP1685S/PLC1/VC/PS2-Keyboard.dis 
KEYMAP_OBJECTS_Visu=$(TEMP_PATH_Visu)/dis.PS2-Keyboard.vco 

# All Source Objects
TXTGRP_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Passwords.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Jog_Txt.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/AHC_Type.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Calibration_Indx.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Wel_Mode.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/TXTGRP_Misc.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/TXTGRP_YES_NO.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Txt_Files.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Txt_StartBtn.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Txt_StopBtn.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/TextGroups/ProcessErrorTxt.txtgrp 

FNINFO_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Fonts/DefaultFont.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial9px.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial9pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial10pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial12px.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial9pxValue.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial14pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial16pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial24pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial12pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial48pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial36pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/Arial64pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/A_12_B_I.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/A_14.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/A_20_B_I.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/BVS_11_B.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/BVS_12_B.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/BVS_13_B.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/BVS_9_B.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/A_13.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Fonts/A_18_B.fninfo 

BMINFO_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA1.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA2.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA3.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadVGA_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA2_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA3_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA1_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadVGA.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/NumPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/NumPad.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AcknowledgeReset.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmActive.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmBypassOFF.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmBypassON.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmInactive.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmLatched.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmNotQuit.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmQuit.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Reset.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ResetAcknowledge.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Triggered.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadHor.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadHor_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadVer.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadVer_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/backward_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/backward_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/forward_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/forward_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/control_button_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/control_button_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/right_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/right_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/left_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/left_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_up_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_up_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_down_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_down_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ProgressBorder.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_active_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_pressed_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_active_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_gradient_upside.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_gradient_downside.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/frame_header.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_pressed_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/EditPadVGA.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/EditPadVGA_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneNumPad_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadVer_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPad_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA1_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA1_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA2_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA3_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditpadQVGA2_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadQVGA3_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadVga_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadVga_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadHor_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadHor_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadVer_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneNumPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/azzlogo.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Bitmap_1.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Bitmap_2.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Button_Small.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Arrow_Right.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_Big.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_Big_Pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_pressed.bminfo 

BMGRP_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlphaPadQVGA.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/NumPad.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlphaPad.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlarmEvent.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlarmState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/BypassState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AcknowledgeState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/NavigationPad.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/Borders.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/Up.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/Down.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/CurtiusPads.bmgrp 

PAGE_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Pages/Init_Page.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Jog.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Pendant.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Setup.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Camera.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Maint.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Test.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/WeldCameras.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Diagnostic.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/LogBook.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Param2.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Calibr.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Param3.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Main_CALIBR.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Set_Limits.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/Param1.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/SCR40_Recipe1.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/AMPS_CALIBR.page \
	$(AS_PROJECT_PATH)/Logical/Visu/Pages/PG_StandBy.page 

LAYER_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Layers/Menu.layer \
	$(AS_PROJECT_PATH)/Logical/Visu/Layers/Estop.layer \
	$(AS_PROJECT_PATH)/Logical/Visu/Layers/MaintPop.layer \
	$(AS_PROJECT_PATH)/Logical/Visu/Layers/HardwareFailure.layer \
	$(AS_PROJECT_PATH)/Logical/Visu/Layers/Seq_Status.layer \
	$(AS_PROJECT_PATH)/Logical/Visu/Layers/EditBox.layer \
	$(AS_PROJECT_PATH)/Logical/Visu/Layers/ProcessErrors.layer 

VCS_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/StyleSheets/Gray.vcs \
	$(AS_PROJECT_PATH)/Logical/Visu/StyleSheets/Color.vcs 

BDR_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Borders/Raised.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/Sunken.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/Etched.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/Bump.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/SunkenOuter.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/RaisedInner.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/Flat_black.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/Flat_grey.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/BackwardActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/BackwardPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ControlActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ControlPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/DownActiveControl.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/DownPressedControl.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ForwardActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ForwardPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/GlobalAreaActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/GlobalAreaPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollDownActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollDownPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollUpActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollUpPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ProgressBarBorder.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollDownActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollDownPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollUpActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollUpPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollLeftActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollLeftPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollRightActive.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollRightPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/UpActiveControl.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/UpPressedControl.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu/Borders/FrameHeader.bdr 

TPR_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NumPad.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu/TouchPads/AlphaPadQVGA.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu/TouchPads/AlphaPad.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NavigationPad_ver.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NavigationPad_hor.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu/TouchPads/EditPad.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NumPadBig.tpr 

TDC_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData.tdc 

TRD_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData_Amps.trd \
	$(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData_Volts.trd \
	$(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData_Weld_Stat.trd 

TRE_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Trends/Trend_1.tre 

CLM_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/RedBlack.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/BtnColor.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/LimitSw.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/DotBlink.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/Status_Color.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/LED_GRAY_GRN.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/ON_OFF.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/LED_RED_GRAY.clm \
	$(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/GRY_ON.clm 

VCVK_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/VirtualKeys.vcvk 

VCR_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Palette.vcr 

# Runtime Object sources
VCR_OBJECT_Visu=$(TEMP_PATH_Visu)/vcrt.vco
VCR_SOURCE_Visu=$(SRC_PATH_Visu)/package.vcp
# All Source Objects END

#Panel Hardware
$(PANEL_HW_VCI_Visu): $(PANEL_HW_SOURCE_Visu) $(VC_LIBRARY_LIST_Visu) $(KEYMAP_SOURCES_Visu) $(PALFILE_Visu)
	$(VCHWPP) -f '$<' -o '$@' -n Visu -d Visu -pal '$(PALFILE_Visu)' -c '$(AS_CONFIGURATION)' -p '$(AS_PLC)' -ptemp '$(AS_TEMP_PLC)' -B 'J4.93' -L 'visapi: V*' -verbose 'False' -profile 'False' -hw '$(CPUHWC)' -warninglevel 2 -so $(VC_STATIC_OPTIONS_Visu) -sos $(VC_STATIC_OPTIONS_Shared) -fp '$(AS_VC_PATH)/Firmware/V4.73.2/SG4' -sfas -prj '$(AS_PROJECT_PATH)' -apj 'MIG1' -vcob '$(VCOBJECT_Visu)'

$(PANEL_HW_OBJECT_Visu): $(PANEL_HW_VCI_Visu) $(VC_LIBRARY_LIST_Visu)
	$(VCC) -f '$(subst .vco,.vci,$@)' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -k '$(VCVK_SOURCES_Visu)' $(VCCFLAGS_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


# Pages
PAGE_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/page., $(notdir $(PAGE_SOURCES_Visu:.page=.vco)))

$(TEMP_PATH_Visu)/page.Init_Page.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Init_Page.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Jog.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Jog.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Pendant.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Pendant.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Setup.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Setup.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Camera.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Camera.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Maint.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Maint.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Test.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Test.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.WeldCameras.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/WeldCameras.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Diagnostic.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Diagnostic.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.LogBook.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/LogBook.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Param2.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Param2.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Calibr.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Calibr.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Param3.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Param3.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Main_CALIBR.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Main_CALIBR.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Set_Limits.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Set_Limits.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.Param1.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/Param1.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.SCR40_Recipe1.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/SCR40_Recipe1.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.AMPS_CALIBR.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/AMPS_CALIBR.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/page.PG_StandBy.vco: $(AS_PROJECT_PATH)/Logical/Visu/Pages/PG_StandBy.page $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu)/StyleSheets/Color.vcs' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Pages END




# Stylesheets
VCS_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/vcs., $(notdir $(VCS_SOURCES_Visu:.vcs=.vco)))

$(TEMP_PATH_Visu)/vcs.Gray.vco: $(AS_PROJECT_PATH)/Logical/Visu/StyleSheets/Gray.vcs
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/vcs.Color.vco: $(AS_PROJECT_PATH)/Logical/Visu/StyleSheets/Color.vcs
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -P '$(AS_PROJECT_PATH)' -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Stylesheets END




# Layers
LAYER_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/layer., $(notdir $(LAYER_SOURCES_Visu:.layer=.vco)))

$(TEMP_PATH_Visu)/layer.Menu.vco: $(AS_PROJECT_PATH)/Logical/Visu/Layers/Menu.layer $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/layer.Estop.vco: $(AS_PROJECT_PATH)/Logical/Visu/Layers/Estop.layer $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/layer.MaintPop.vco: $(AS_PROJECT_PATH)/Logical/Visu/Layers/MaintPop.layer $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/layer.HardwareFailure.vco: $(AS_PROJECT_PATH)/Logical/Visu/Layers/HardwareFailure.layer $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/layer.Seq_Status.vco: $(AS_PROJECT_PATH)/Logical/Visu/Layers/Seq_Status.layer $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/layer.EditBox.vco: $(AS_PROJECT_PATH)/Logical/Visu/Layers/EditBox.layer $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/layer.ProcessErrors.vco: $(AS_PROJECT_PATH)/Logical/Visu/Layers/ProcessErrors.layer $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -ds $(DEFAULT_STYLE_SHEET_Visu) -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Layers END




# Virtual Keys
VCVK_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/vcvk., $(notdir $(VCVK_SOURCES_Visu:.vcvk=.vco)))

$(TEMP_PATH_Visu)/vcvk.VirtualKeys.vco: $(AS_PROJECT_PATH)/Logical/Visu/VirtualKeys.vcvk
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas

$(VCVK_OBJECTS_Visu): $(VC_LANGUAGES_Visu)

#Virtual Keys END




# Touch Pads
TPR_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/tpr., $(notdir $(TPR_SOURCES_Visu:.tpr=.vco)))

$(TEMP_PATH_Visu)/tpr.NumPad.vco: $(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NumPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -prj 'C:/projects/MIG/MIG_26Sept2025K/MIG1/Logical/Visu' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/tpr.AlphaPadQVGA.vco: $(AS_PROJECT_PATH)/Logical/Visu/TouchPads/AlphaPadQVGA.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -prj 'C:/projects/MIG/MIG_26Sept2025K/MIG1/Logical/Visu' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/tpr.AlphaPad.vco: $(AS_PROJECT_PATH)/Logical/Visu/TouchPads/AlphaPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -prj 'C:/projects/MIG/MIG_26Sept2025K/MIG1/Logical/Visu' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/tpr.NavigationPad_ver.vco: $(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NavigationPad_ver.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -prj 'C:/projects/MIG/MIG_26Sept2025K/MIG1/Logical/Visu' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/tpr.NavigationPad_hor.vco: $(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NavigationPad_hor.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -prj 'C:/projects/MIG/MIG_26Sept2025K/MIG1/Logical/Visu' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/tpr.EditPad.vco: $(AS_PROJECT_PATH)/Logical/Visu/TouchPads/EditPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -prj 'C:/projects/MIG/MIG_26Sept2025K/MIG1/Logical/Visu' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/tpr.NumPadBig.vco: $(AS_PROJECT_PATH)/Logical/Visu/TouchPads/NumPadBig.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu) -prj 'C:/projects/MIG/MIG_26Sept2025K/MIG1/Logical/Visu' -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Touch Pads END




# Text Groups
TXTGRP_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/txtgrp., $(notdir $(TXTGRP_SOURCES_Visu:.txtgrp=.vco)))

$(TEMP_PATH_Visu)/txtgrp.Passwords.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Passwords.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.Jog_Txt.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Jog_Txt.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.AHC_Type.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/AHC_Type.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.Calibration_Indx.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Calibration_Indx.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.Wel_Mode.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Wel_Mode.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.TXTGRP_Misc.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/TXTGRP_Misc.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.TXTGRP_YES_NO.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/TXTGRP_YES_NO.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.Txt_Files.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Txt_Files.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.Txt_StartBtn.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Txt_StartBtn.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.Txt_StopBtn.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/Txt_StopBtn.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/txtgrp.ProcessErrorTxt.vco: $(AS_PROJECT_PATH)/Logical/Visu/TextGroups/ProcessErrorTxt.txtgrp $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Text Groups END




# BitmapGroups
BMGRP_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/bmgrp., $(notdir $(BMGRP_SOURCES_Visu:.bmgrp=.vco)))

$(TEMP_PATH_Visu)/bmgrp.AlphaPadQVGA.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlphaPadQVGA.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.NumPad.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/NumPad.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.AlphaPad.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlphaPad.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.AlarmEvent.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlarmEvent.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.AlarmState.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AlarmState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.BypassState.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/BypassState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.AcknowledgeState.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/AcknowledgeState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.NavigationPad.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/NavigationPad.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.Borders.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/Borders.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.Up.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/Up.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.Down.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/Down.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bmgrp.CurtiusPads.vco: $(AS_PROJECT_PATH)/Logical/Visu/BitmapGroups/CurtiusPads.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#BitmapGroups END




# Bitmaps
BMINFO_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/bminfo., $(notdir $(BMINFO_SOURCES_Visu:.bminfo=.vco)))

$(TEMP_PATH_Visu)/bminfo.AlphaPadQVGA1.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA1.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA1.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlphaPadQVGA2.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA2.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA2.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlphaPadQVGA3.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA3.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA3.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlphaPadVGA_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadVGA_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadVGA_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlphaPadQVGA2_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA2_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA2_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlphaPadQVGA3_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA3_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA3_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlphaPadQVGA1_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA1_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadQVGA1_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlphaPadVGA.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadVGA.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlphaPadVGA.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.NumPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/NumPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/NumPad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.NumPad.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/NumPad.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/NumPad.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AcknowledgeReset.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AcknowledgeReset.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AcknowledgeReset.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlarmActive.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmActive.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmActive.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlarmBypassOFF.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmBypassOFF.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmBypassOFF.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlarmBypassON.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmBypassON.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmBypassON.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlarmInactive.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmInactive.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmInactive.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlarmLatched.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmLatched.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmLatched.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlarmNotQuit.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmNotQuit.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmNotQuit.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.AlarmQuit.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmQuit.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/AlarmQuit.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Reset.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Reset.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Reset.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.ResetAcknowledge.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ResetAcknowledge.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ResetAcknowledge.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Triggered.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Triggered.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Triggered.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.ListPadHor.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadHor.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadHor.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.ListPadHor_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadHor_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadHor_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.ListPadVer.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadVer.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadVer.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.ListPadVer_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadVer_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ListPadVer_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.backward_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/backward_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/backward_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.backward_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/backward_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/backward_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.global_area_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.global_area_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.forward_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/forward_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/forward_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.forward_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/forward_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/forward_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.control_button_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/control_button_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/control_button_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.control_button_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/control_button_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/control_button_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.right_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/right_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/right_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.right_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/right_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/right_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.left_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/left_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/left_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.left_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/left_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/left_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.up_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.up_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.down_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.down_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.multi_up_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_up_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_up_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.multi_up_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_up_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_up_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.multi_down_active.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_down_active.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_down_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.multi_down_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_down_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/multi_down_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.ProgressBorder.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ProgressBorder.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/ProgressBorder.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.down_active_control.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_active_control.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_active_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.down_pressed_control.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_pressed_control.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/down_pressed_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.up_active_control.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_active_control.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_active_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.global_area_gradient_upside.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_gradient_upside.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_gradient_upside.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.global_area_gradient_downside.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_gradient_downside.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/global_area_gradient_downside.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.frame_header.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/frame_header.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/frame_header.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.up_pressed_control.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_pressed_control.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/up_pressed_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.EditPadVGA.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/EditPadVGA.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/EditPadVGA.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.EditPadVGA_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/EditPadVGA_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/EditPadVGA_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneNumPad_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneNumPad_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneNumPad_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneListPadVer_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadVer_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadVer_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneAlphaPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneAlphaPad_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPad_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPad_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneAlphaPadQVGA1_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA1_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA1_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneAlphaPadQVGA1_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA1_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA1_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneAlphaPadQVGA2_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA2_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA2_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneAlphaPadQVGA3_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA3_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneAlphaPadQVGA3_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneEditpadQVGA2_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditpadQVGA2_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditpadQVGA2_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneEditPadQVGA3_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadQVGA3_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadQVGA3_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneEditPadVga_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadVga_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadVga_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneEditPadVga_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadVga_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneEditPadVga_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneListPadHor_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadHor_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadHor_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneListPadHor_released.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadHor_released.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadHor_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneListPadVer_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadVer_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneListPadVer_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.zuneNumPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneNumPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/zuneNumPad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.azzlogo.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/azzlogo.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/azzlogo.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Bitmap_1.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Bitmap_1.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Bitmap_1.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Bitmap_2.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Bitmap_2.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Bitmap_2.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Button_Small.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Button_Small.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Button_Small.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Arrow_Right.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Arrow_Right.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Arrow_Right.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Curtius_Key_Numpad.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Curtius_Key_Numpad_Big.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_Big.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_Big.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Curtius_Key_Numpad_Big_Pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_Big_Pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_Big_Pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/bminfo.Curtius_Key_Numpad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu/Bitmaps/Curtius_Key_Numpad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Bitmaps END




# Trend Configuration
TRE_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/tre., $(notdir $(TRE_SOURCES_Visu:.tre=.vco)))

$(TEMP_PATH_Visu)/tre.Trend_1.vco: $(AS_PROJECT_PATH)/Logical/Visu/Trends/Trend_1.tre
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Trend Configuration END




# Trend Data
TRD_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/trd., $(notdir $(TRD_SOURCES_Visu:.trd=.vco)))

$(TEMP_PATH_Visu)/trd.TrendData_Amps.vco: $(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData_Amps.trd
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/trd.TrendData_Volts.vco: $(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData_Volts.trd
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/trd.TrendData_Weld_Stat.vco: $(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData_Weld_Stat.trd
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Trend Data END




# Trend Data Configuration
TDC_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/tdc., $(notdir $(TDC_SOURCES_Visu:.tdc=.vco)))

$(TEMP_PATH_Visu)/tdc.TrendData.vco: $(AS_PROJECT_PATH)/Logical/Visu/Trends/TrendData.tdc
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#Trend Data Configuration END




# ColorMap Table
CLM_OBJECTS_Visu = $(addprefix $(TEMP_PATH_Visu)/clm., $(notdir $(CLM_SOURCES_Visu:.clm=.vco)))

$(TEMP_PATH_Visu)/clm.RedBlack.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/RedBlack.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.BtnColor.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/BtnColor.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.LimitSw.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/LimitSw.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.DotBlink.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/DotBlink.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.Status_Color.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/Status_Color.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.LED_GRAY_GRN.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/LED_GRAY_GRN.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.ON_OFF.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/ON_OFF.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.LED_RED_GRAY.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/LED_RED_GRAY.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


$(TEMP_PATH_Visu)/clm.GRY_ON.vco: $(AS_PROJECT_PATH)/Logical/Visu/ColorMaps/GRY_ON.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu)' $(VCCFLAGS_Visu)  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas


#ColorMap Table END


#
# Borders
#
BDR_SOURCES_Visu=$(AS_PROJECT_PATH)/Logical/Visu/Borders/Raised.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/Sunken.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/Etched.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/Bump.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/SunkenOuter.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/RaisedInner.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/Flat_black.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/Flat_grey.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/BackwardActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/BackwardPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ControlActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ControlPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/DownActiveControl.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/DownPressedControl.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ForwardActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ForwardPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/GlobalAreaActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/GlobalAreaPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollDownActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollDownPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollUpActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/MultiScrollUpPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ProgressBarBorder.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollDownActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollDownPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollUpActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollUpPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollLeftActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollLeftPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollRightActive.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/ScrollRightPressed.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/UpActiveControl.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/UpPressedControl.bdr $(AS_PROJECT_PATH)/Logical/Visu/Borders/FrameHeader.bdr 
BDR_OBJECTS_Visu=$(TEMP_PATH_Visu)/bdr.Bordermanager.vco
$(TEMP_PATH_Visu)/bdr.Bordermanager.vco: $(BDR_SOURCES_Visu)
	$(VCC) -f '$<' -o '$@' -pkg '$(SRC_PATH_Visu)' $(BDRFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu$(SRC_PATH_Visu)
#
# Logical fonts
#
$(TEMP_PATH_Visu)/lfnt.sv.vco: $(TEMP_PATH_Visu)/sv.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
$(TEMP_PATH_Visu)/lfnt.fr.vco: $(TEMP_PATH_Visu)/fr.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
$(TEMP_PATH_Visu)/lfnt.zh-CN.vco: $(TEMP_PATH_Visu)/zh-CN.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
$(TEMP_PATH_Visu)/lfnt.de.vco: $(TEMP_PATH_Visu)/de.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
$(TEMP_PATH_Visu)/lfnt.pt-BR.vco: $(TEMP_PATH_Visu)/pt-BR.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
$(TEMP_PATH_Visu)/lfnt.es.vco: $(TEMP_PATH_Visu)/es.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
$(TEMP_PATH_Visu)/lfnt.en.vco: $(TEMP_PATH_Visu)/en.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
$(TEMP_PATH_Visu)/lfnt.pt.vco: $(TEMP_PATH_Visu)/pt.lfnt $(VC_LANGUAGES_Visu)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu) $(VCCFLAGS_Visu) -p Visu -sfas
LFNT_OBJECTS_Visu=$(TEMP_PATH_Visu)/lfnt.sv.vco $(TEMP_PATH_Visu)/lfnt.fr.vco $(TEMP_PATH_Visu)/lfnt.zh-CN.vco $(TEMP_PATH_Visu)/lfnt.de.vco $(TEMP_PATH_Visu)/lfnt.pt-BR.vco $(TEMP_PATH_Visu)/lfnt.es.vco $(TEMP_PATH_Visu)/lfnt.en.vco $(TEMP_PATH_Visu)/lfnt.pt.vco 

#Runtime Object
$(VCR_OBJECT_Visu) : $(VCR_SOURCE_Visu)
	$(VCC) -f '$<' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -sl en $(VCCFLAGS_Visu) -rt  -p Visu -so $(VC_STATIC_OPTIONS_Visu) -vcr 4732 -sfas
# Local resources Library rules
LIB_LOCAL_RES_Visu=$(TEMP_PATH_Visu)/localres.vca
$(LIB_LOCAL_RES_Visu) : $(TEMP_PATH_Visu)/Visu02.ccf

# Bitmap Library rules
LIB_BMP_RES_Visu=$(TEMP_PATH_Visu)/bmpres.vca
$(LIB_BMP_RES_Visu) : $(TEMP_PATH_Visu)/Visu03.ccf
$(BMGRP_OBJECTS_Visu) : $(PALFILE_Visu) $(VC_LANGUAGES_Visu)
$(BMINFO_OBJECTS_Visu) : $(PALFILE_Visu)

BUILD_FILE_Visu=$(TEMP_PATH_Visu)/BuildFiles.arg
$(BUILD_FILE_Visu) : BUILD_FILE_CLEAN_Visu $(BUILD_SOURCES_Visu)
BUILD_FILE_CLEAN_Visu:
	$(RM) /F /Q '$(BUILD_FILE_Visu)' 2>nul
#All Modules depending to this project
PROJECT_MODULES_Visu=$(AS_CPU_PATH)/Visu01.br $(AS_CPU_PATH)/Visu02.br $(AS_CPU_PATH)/Visu03.br $(FONT_MODULES_Visu) $(SHARED_MODULE)

# General Build rules

$(TARGET_FILE_Visu): $(PROJECT_MODULES_Visu) $(TEMP_PATH_Visu)/Visu.prj
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu) -fw '$(VCFIRMWAREPATH)' -m $(VCSTPOST) -v V1.00.0 -f '$(TEMP_PATH_Visu)/Visu.prj' -o '$@' -vc '$(VCOBJECT_Visu)' $(DEPENDENCIES_Visu) $(addprefix -d ,$(notdir $(PROJECT_MODULES_Visu:.br=)))

$(AS_CPU_PATH)/Visu01.br: $(TEMP_PATH_Visu)/Visu01.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu)' -f '$<' -o '$@' $(DEPENDENCIES_Visu)

$(AS_CPU_PATH)/Visu02.br: $(TEMP_PATH_Visu)/Visu02.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu)' -f '$<' -o '$@' $(DEPENDENCIES_Visu)

$(AS_CPU_PATH)/Visu03.br: $(TEMP_PATH_Visu)/Visu03.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu)' -f '$<' -o '$@' $(DEPENDENCIES_Visu)

# General Build rules END
$(LIB_LOCAL_OBJ_Visu) : $(TEMP_PATH_Visu)/Visu01.ccf

# Main Module
$(TEMP_PATH_ROOT_Visu)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu.vcm:
$(TEMP_PATH_Visu)/Visu.prj: $(TEMP_PATH_ROOT_Visu)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu.vcm
	$(VCDEP) -m '$(TEMP_PATH_ROOT_Visu)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu.vcm' -s '$(AS_CPU_PATH)/VCShared/Shared.vcm' -p '$(AS_PATH)/AS/VC/Firmware' -c '$(AS_CPU_PATH)' -fw '$(VCFIRMWAREPATH)' -hw '$(CPUHWC)' -so $(VC_STATIC_OPTIONS_Visu) -o Visu -proj Visu
	$(VCPL) $(notdir $(PROJECT_MODULES_Visu:.br=,4)) Visu,2 -o '$@' -p Visu -vc 'Visu' -verbose 'False' -fl '$(TEMP_PATH_ROOT_Visu)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu.vcm' -vcr '$(VCR_SOURCE_Visu)' -prj '$(AS_PROJECT_PATH)' -warningLevel2 -sfas

# 01 Module

DEL_TARGET01_LFL_Visu=$(TEMP_PATH_Visu)\Visu01.ccf.lfl
$(TEMP_PATH_Visu)/Visu01.ccf: $(LIB_SHARED) $(SHARED_CCF) $(LIB_BMP_RES_Visu) $(TEMP_PATH_Visu)/Visu03.ccf $(LIB_LOCAL_RES_Visu) $(TEMP_PATH_Visu)/Visu02.ccf $(DIS_OBJECTS_Visu) $(PAGE_OBJECTS_Visu) $(VCS_OBJECTS_Visu) $(VCVK_OBJECTS_Visu) $(VCRT_OBJECTS_Visu) $(TPR_OBJECTS_Visu) $(TXTGRP_OBJECTS_Visu) $(LAYER_OBJECTS_Visu) $(VCR_OBJECT_Visu) $(TDC_OBJECTS_Visu) $(TRD_OBJECTS_Visu) $(TRE_OBJECTS_Visu) $(PRC_OBJECTS_Visu) $(SCR_OBJECTS_Visu)
	-@CMD /Q /C if exist "$(DEL_TARGET01_LFL_Visu)" DEL /F /Q "$(DEL_TARGET01_LFL_Visu)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_BMP_RES_Visu)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_LOCAL_RES_Visu)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(DIS_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .page -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCS_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .vcvk -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCRT_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(TPR_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .txtgrp -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .layer -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCR_OBJECT_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .tdc -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .trd -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(TEMP_PATH_Visu)/tre.Trend_1.vco' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(SCR_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu -lib '$(LIB_LOCAL_OBJ_Visu)' -P '$(AS_PROJECT_PATH)' -m 'local objects' -profile 'False' -warningLevel2 -vcr 4732 -sfas
# 01 Module END

# 02 Module

DEL_TARGET02_LFL_Visu=$(TEMP_PATH_Visu)\Visu02.ccf.lfl
$(TEMP_PATH_Visu)/Visu02.ccf: $(LIB_SHARED) $(SHARED_CCF) $(LIB_BMP_RES_Visu) $(TEMP_PATH_Visu)/Visu03.ccf $(BDR_OBJECTS_Visu) $(LFNT_OBJECTS_Visu) $(CLM_OBJECTS_Visu)
	-@CMD /Q /C if exist "$(DEL_TARGET02_LFL_Visu)" DEL /F /Q "$(DEL_TARGET02_LFL_Visu)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_BMP_RES_Visu)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(BDR_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LFNT_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(CLM_OBJECTS_Visu:.vco=.vco|)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu -lib '$(LIB_LOCAL_RES_Visu)' -P '$(AS_PROJECT_PATH)' -m 'local resources' -profile 'False' -warningLevel2 -vcr 4732 -sfas
# 02 Module END

# 03 Module

DEL_TARGET03_LFL_Visu=$(TEMP_PATH_Visu)\Visu03.ccf.lfl
$(TEMP_PATH_Visu)/Visu03.ccf: $(LIB_SHARED) $(SHARED_CCF) $(BMGRP_OBJECTS_Visu) $(BMINFO_OBJECTS_Visu) $(PALFILE_Visu)
	-@CMD /Q /C if exist "$(DEL_TARGET03_LFL_Visu)" DEL /F /Q "$(DEL_TARGET03_LFL_Visu)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .bmgrp -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .bminfo -vcp '$(AS_PROJECT_PATH)/Logical/Visu/Package.vcp' -temp '$(TEMP_PATH_Visu)' -prj '$(PRJ_PATH_Visu)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu -lib '$(LIB_BMP_RES_Visu)' -P '$(AS_PROJECT_PATH)' -m 'bitmap resources' -profile 'False' -warningLevel2 -vcr 4732 -sfas
# 03 Module END

# Post Build Steps

.PHONY : vcPostBuild_Visu

vcPostBuild_Visu :
	$(VCC) -pb -vcm '$(TEMP_PATH_Visu)/MODULEFILES.vcm' -fw '$(VCFIRMWAREPATH)' $(VCCFLAGS_Visu) -p Visu -vcr 4732 -sfas

# Post Build Steps END
