
FUNCTION XMLp_CallParse : DINT
	VAR_INPUT
		m : XML_Method_typ;
	END_VAR
END_FUNCTION

FUNCTION XMLp_CallCreate : DINT
	VAR_INPUT
		m : XML_Method_typ;
		PVNames : REFERENCE TO USINT;
	END_VAR
END_FUNCTION

FUNCTION XMLp_Init : BOOL
	VAR_INPUT
		m : XML_Method_typ;
		BufferSize : UDINT;
	END_VAR
END_FUNCTION

FUNCTION XMLp_Destroy : BOOL (*TODO: Add your comment here*)
	VAR_INPUT
		m : XML_Method_typ;
	END_VAR
END_FUNCTION
