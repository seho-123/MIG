export AS_SYSTEM_PATH := C:/BrAutomation/AS/System
export AS_BIN_PATH := C:/BrAutomation/AS45/bin-en
export AS_INSTALL_PATH := C:/BrAutomation/AS45
export AS_PATH := C:/BrAutomation/AS45
export AS_VC_PATH := C:/BrAutomation/AS45/AS/VC
export AS_GNU_INST_PATH := C:/BrAutomation/AS45/AS/GnuInst/V4.1.2
export AS_STATIC_ARCHIVES_PATH := C:/projects/MIG/MIG/Temp/Archives/X20CP0484/X20CP0484_1
export AS_CPU_PATH := C:/projects/MIG/MIG/Temp/Objects/X20CP0484/X20CP0484_1
export AS_CPU_PATH_2 := C:/projects/MIG/MIG/Temp/Objects/X20CP0484/X20CP0484_1
export AS_TEMP_PATH := C:/projects/MIG/MIG/Temp
export AS_BINARIES_PATH := C:/projects/MIG/MIG/Binaries
export AS_PROJECT_CPU_PATH := C:/projects/MIG/MIG/Physical/X20CP0484/X20CP0484_1
export AS_PROJECT_CONFIG_PATH := C:/projects/MIG/MIG/Physical/X20CP0484
export AS_PROJECT_PATH := C:/projects/MIG/MIG
export AS_PROJECT_NAME := MIG
export AS_PLC := X20CP0484_1
export AS_TEMP_PLC := X20CP0484_1
export AS_USER_NAME := CKaczkowski
export AS_CONFIGURATION := X20CP0484
export AS_COMPANY_NAME := \ 
export AS_VERSION := 4.5.2.102


default: \
	$(AS_CPU_PATH)/Visu.br \
	vcPostBuild_Visu \



include $(AS_CPU_PATH)/Visu/Visu.mak
