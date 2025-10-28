
TYPE
	XML_ParseInfo_typ : 	STRUCT 
		LinesProcessed : UINT;
		LengthInBytes : UDINT;
		ErrorNum : DINT;
		ItemsNotFound : UINT;
		ErrorMessage : ARRAY[0..9] OF STRING[50];
		IgnoreNotFound : BOOL;
	END_STRUCT;
	XML_CreateInfo_typ : 	STRUCT 
		LengthInBytes : UDINT;
		ErrorNum : DINT;
	END_STRUCT;
	XML_DiagInfo_typ : 	STRUCT 
		Reset : BOOL;
		Logger : BOOL;
		TotalCalls : UDINT;
		TotalResponses : UDINT;
		TotalFaults : UDINT;
		TotalErrors : UDINT;
	END_STRUCT;
	XML_Method_typ : 	STRUCT 
		InitStatus : DINT;
		Status : DINT;
		MethodName : STRING[40];
		PVServer : STRING[40];
		PVClient : STRING[40];
		OpIndex : INT;
		ParseInfo : XML_ParseInfo_typ;
		CreateInfo : XML_CreateInfo_typ;
		DiagInfo : XML_DiagInfo_typ;
		Fault : BOOL;
		FaultString : STRING[40];
		FaultCode : DINT;
		ViewLineString : STRING[119];
		ViewInc : BOOL;
		ViewLine : UINT;
		ViewLevel : UINT;
		_viewline : UINT;
		BufferSize : UDINT;
		pMessage : UDINT;
	END_STRUCT;
END_TYPE
