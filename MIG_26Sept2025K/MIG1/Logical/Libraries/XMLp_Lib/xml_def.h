/*********************************************************
* FILE		:		xml_def.h(Header)
*
* OBJECTs	:	 
*
* OVERVIEW	:	
*			
*	
* HISTORY : 			08.31.07	- Version 1.0
					09.18.07	- Version 1.1
					02.14.08	- STRING_LENGTH_MAX 40
					02.28.08	- STRING Null
					
				
*
* DEPENDANCIES:
*********************************************************/
#include <bur/plc.h>			
#include <bur/plctypes.h>
#include <xmlp_lib.h>

/* DEFINES:
*********************************************************/
#ifndef OFF
	#define OFF 0
	#define ON 1
	#define YES 1
	#define NO 0
#endif
#ifndef STRING_LENGTH_MAX
	#define STRING_LENGTH_MAX			40
#endif

/* MACROS:
*********************************************************/
#define XML_BODY_VERSION				"<?xml version=\"1.0\" encoding=\"utf-8\"?>" 
#define XML_BODY_OPEN					"<"
#define XML_BODY_CLOSE					"</"
#define XML_BODY_END					">"

#define XML_BODY_METHOD_CALL			"methodCall"
#define XML_BODY_METHOD_RESPONSE		"methodResponse"
#define XML_BODY_METHOD_NAME			"methodName"
#define XML_BODY_PARAMS					"variables"
#define XML_BODY_PARAM					"variable"
#define XML_BODY_VALUE					"value"
#define XML_BODY_STRUCT				"struct"
#define XML_BODY_ARRAY					"array"
#define XML_BODY_DATA					"data"
#define XML_BODY_MEMBER 				"member"
#define XML_BODY_NAME					"name"
#define XML_BODY_FAULT					"fault"
#define XML_BODY_FAULT_CODE			"faultCode"
#define XML_BODY_FAULT_STRING 		"faultString"

#define XML_BODY_TYPE_I4				"i4"
#define XML_BODY_TYPE_I4_V1			"integer"			/* Variant */
#define XML_BODY_TYPE_BOOL			"boolean"
#define XML_BODY_TYPE_STRING			"string"
#define XML_BODY_TYPE_DOUBLE			"double"
#define XML_BODY_TYPE_DATE_TIME			"date_time"			//"dateTime.iso8601"

#define XML_OP_READ					"Read"
#define XML_OP_READ_2 					"read"
#define XML_OP_READ_3 					"READ"
#define XML_OP_READ_IDX				1
#define XML_OP_WRITE 					"Write"
#define XML_OP_WRITE_2					"write"
#define XML_OP_WRITE_3					"WRITE"
#define XML_OP_WRITE_IDX 				2
#define XML_OP_METHOD					"Method"
#define XML_OP_METHOD_2				"method"
#define XML_OP_METHOD_3 				"METHOD"
#define XML_OP_METHOD_IDX 				3
#define XML_OP_FILEWRITE				"FileWrite"
#define XML_OP_FILEWRITE_IDX			12
#define XML_OP_FILEREAD				"FileRead"
#define XML_OP_FILEREAD_IDX			13

#define XML_CLOSE							1
#define XML_OPEN							0

#define XML_BODY_TYPE_NULL_IDX			0
#define XML_BODY_TYPE_I4_IDX				1
#define XML_BODY_TYPE_BOOL_IDX			2
#define XML_BODY_TYPE_STRING_IDX			3
#define XML_BODY_TYPE_DOUBLE_IDX			4
#define XML_BODY_TYPE_DATE_TIME_IDX			5

#define XML_BODY_PARAM_IDX				30
#define XML_BODY_PARAMS_IDX				31
#define XML_BODY_METHOD_CALL_IDX			32
#define XML_BODY_STRUCT_IDX				33
#define XML_BODY_VALUE_IDX					34
#define XML_BODY_METHOD_RESPONSE_IDX	35
#define XML_BODY_FAULT_IDX					36
#define XML_BODY_MEMBER_IDX				37
#define XML_BODY_METHOD_NAME_IDX			38
#define XML_BODY_NAME_IDX					39
#define XML_BODY_ARRAY_IDX				40
#define XML_BODY_DATA_IDX					41

#define XML_BODY_INDENT_1					"	"

#define XML_VERSION 						999


#ifndef TYPE_BOOL
	#define TYPE_BOOL						1
	#define TYPE_SINT						2
	#define TYPE_INT						3
	#define TYPE_DINT						4
	#define TYPE_USINT						5
	#define TYPE_UINT						6
	#define TYPE_UDINT						7
	#define TYPE_REAL						8
	#define TYPE_STRING						9
	#define TYPE_DATE_TIME					11
	#define TYPE_TIME						12
#endif

#define cTAB		9
#define cLF		10
#define cCR 		13
#define cSLASH	47
#define cLT		60
#define cGT		62
#define cQUESTION	63

#define LINE_LENGTH_MAX			120
#define XML_CLIENT					"client"
#define XML_SERVER					"server"
#define XML_FAULT_CODE				"fault.Code"
#define XML_FAULT_STRING			"fault.String"
#define XML_TRUE					"true"
#define XML_FALSE					"false"
#define XML_STRING_NULL			"NULL"

/* Client Status */
#define XMLd_STATUS_ERROR			-100
#define XML_STATUS_RESP_ERROR		-2
#define XML_STATUS_CALL_ERROR		-1
#define XML_STATUS_READY			0
#define XML_STATUS_SENDNOW		1
#define XML_STATUS_METHOD			2
#define XML_STATUS_FAULT 			3
#define XML_STATUS_FAULT_AUTO		4
#define XML_STATUS_COMPLETE		5
#define XML_STATUS_PARSE_OK	 	6

#define XML_BUFFER_REMAIN			500

/* TYPES:
*********************************************************/
typedef struct {
	STRING 	Name[STRING_LENGTH_MAX];
	UDINT	Length;
	UDINT	Type;
	UINT	Dim;
} DataElement_typ;

typedef struct {
	DINT	Key[4];

	UINT	NumKeys;
	UINT	NumOpenKeys;
	UINT	NumCloseKeys;
	UINT	NumValues;
	STRING	sValue[STRING_LENGTH_MAX];
	REAL	rValue;
	DINT	iValue;
	BOOL	bValue;
} LineElement_typ;

/* HELPER PROTOTYPES:
*********************************************************/
DINT xml_FaultDetected( XML_Method_typ *m,	DINT FaultCode, STRING FaultString[STRING_LENGTH_MAX], UDINT offset );
DINT xml_FaultDetectedInResponse( XML_Method_typ *m,	DINT FaultCode, STRING FaultString[STRING_LENGTH_MAX] );
BOOL xml_MessageViewer( XML_Method_typ *m, UINT line, USINT *text );
DINT xml_AddItems( USINT *msg, char* PVBaseName, UINT *Indent, UDINT	*offset, UDINT BufferSize, BOOL Logger );
DINT xml_AppendIndex( STRING Dest[100], USINT num );
DINT xml_GetLineInformation( XML_Method_typ *m, UDINT	*offset, LineElement_typ *line, BOOL *lastlinedected );
DINT xml_GetElement( XML_Method_typ *m, STRING *pBaseName, UDINT	*offset, UINT idx, LineElement_typ *line);
DINT xml_ProcessValue( STRING *pName, LineElement_typ *line, UINT idx );
DINT xml_SetErrorFlag( XML_Method_typ *m  );
DINT xml_CreateFaultMessage( XML_Method_typ *m,  UDINT	*offset, DINT FaultCode, STRING FaultString[STRING_LENGTH_MAX] );
DINT xml_InsertComplexLine( USINT *msg, UINT Type, UINT size, UDINT  *offset, void *pData, UINT Indent);
DINT xml_InsertKey( USINT *msg, UINT Type, UDINT  *offset, UINT Indent, BOOL Option );
DINT xml_GetKeyIndex( USINT *pItemText, BOOL *SlashFound );
DINT xml_GetOpIndex( USINT *pOpName );
UINT xml_DataTypeToValueType(  UDINT type );
DINT xml_MakeCallName(	USINT *pDest, USINT *pOpName, USINT *pMethodName );
DINT _xmlGetStringFromFile( USINT *f0, UDINT  *offset, USINT *dest );
DINT _xmlGetLine( USINT *f0, UDINT	*offset, USINT *dest, BOOL *lastline, UINT *levels );
BOOL _GetStringFromFile( USINT *f0, UDINT  *offset, USINT *dest, USINT *endchar, BOOL ignore );
void _AddIntToFile( USINT *f0, UDINT  *offset, INT x, USINT *endchar );
void _AddDintToFile( USINT *f0, UDINT  *offset, DINT x, USINT *endchar );
void _AddFloatToFile( USINT *f0, UDINT	*offset, REAL x, USINT *endchar );
void _AddStringToFile( USINT *f0, UDINT  *offset, USINT *s, USINT *endchar );
signed long AddIndex(plcstring* Dest, plcstring* Name, unsigned char num);
/* EOF */
