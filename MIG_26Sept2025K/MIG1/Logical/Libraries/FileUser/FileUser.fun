
FUNCTION FU_init : DINT
	VAR_INPUT
		fm_adr : UDINT;
		sDevice : STRING[20];
		Path : STRING[80];
		data_adr : UDINT;
		size : UDINT;
		Type : USINT;
		_if_in_adr : UDINT;
		_if_out_adr : UDINT;
		sDevice2 : STRING[20];
		_fname_adr : UDINT;
		_ftp_adr : UDINT;
		MaxFiles : UINT;
	END_VAR
END_FUNCTION

FUNCTION FU_serv : DINT
	VAR_INPUT
		fm_adr : UDINT;
	END_VAR
END_FUNCTION

FUNCTION FU_setBufPointer : BOOL (*TODO: Add your comment here*)
	VAR_INPUT
		fm_adr : UDINT;
		pMem : UDINT;
		size : UDINT;
	END_VAR
END_FUNCTION
