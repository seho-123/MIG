export AS_BUILD_MODE := Build
export AS_SYSTEM_PATH := C:/Program\ Files/BRAutomation4/AS/System
export AS_BIN_PATH := C:/Program Files/BRAutomation4/AS412/bin-en
export AS_INSTALL_PATH := C:/Program\ Files/BRAutomation4/AS412
export AS_PATH := C:/Program Files/BRAutomation4/AS412
export AS_VC_PATH := C:/Program\ Files/BRAutomation4/AS412/AS/VC
export AS_GNU_INST_PATH := C:/Program\ Files/BRAutomation4/AS412/AS/gnuinst/V4.1.2
export AS_STATIC_ARCHIVES_PATH := C:/projects/MIG/MIG_26Sept2025K/MIG1/Temp/Archives/X20CP1685S/PLC1
export AS_CPU_PATH := C:/projects/MIG/MIG_26Sept2025K/MIG1/Temp/Objects/X20CP1685S/PLC1
export AS_CPU_PATH_2 := C:/projects/MIG/MIG_26Sept2025K/MIG1/Temp/Objects/X20CP1685S/PLC1
export AS_TEMP_PATH := C:/projects/MIG/MIG_26Sept2025K/MIG1/Temp
export AS_BINARIES_PATH := C:/projects/MIG/MIG_26Sept2025K/MIG1/Binaries
export AS_PROJECT_CPU_PATH := C:/projects/MIG/MIG_26Sept2025K/MIG1/Physical/X20CP1685S/PLC1
export AS_PROJECT_CONFIG_PATH := C:/projects/MIG/MIG_26Sept2025K/MIG1/Physical/X20CP1685S
export AS_PROJECT_PATH := C:/projects/MIG/MIG_26Sept2025K/MIG1
export AS_PROJECT_NAME := MIG1
export AS_PLC := PLC1
export AS_TEMP_PLC := PLC1
export AS_USER_NAME := SPark
export AS_CONFIGURATION := X20CP1685S
export AS_COMPANY_NAME := \ 
export AS_VERSION := 4.12.7.113\ SP
export AS_WORKINGVERSION := 4.12


default: \
	$(AS_CPU_PATH)/Visu.br \
	vcPostBuild_Visu \
	$(AS_CPU_PATH)/Visu1.br \
	vcPostBuild_Visu1 \



include $(AS_CPU_PATH)/Visu/Visu.mak
include $(AS_CPU_PATH)/Visu1/Visu1.mak
