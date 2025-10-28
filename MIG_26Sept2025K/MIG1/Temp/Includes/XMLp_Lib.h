/* Automation Studio generated header file */
/* Do not edit ! */
/* XMLp_Lib 1.54.0 */

#ifndef _XMLP_LIB_
#define _XMLP_LIB_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _XMLp_Lib_VERSION
#define _XMLp_Lib_VERSION 1.54.0
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG4
		#include "asstring.h"
		#include "sys_lib.h"
#endif


/* Constants */
#ifdef _REPLACE_CONST
 #define XMLp_STATUS_FAULT 3
 #define XMLp_STATUS_SUCCESS 5
#else
 _GLOBAL_CONST signed char XMLp_STATUS_FAULT;
 _GLOBAL_CONST signed char XMLp_STATUS_SUCCESS;
#endif




/* Datatypes and datatypes of function blocks */
typedef struct XML_ParseInfo_typ
{	unsigned short LinesProcessed;
	unsigned long LengthInBytes;
	signed long ErrorNum;
	unsigned short ItemsNotFound;
	plcstring ErrorMessage[10][51];
	plcbit IgnoreNotFound;
} XML_ParseInfo_typ;

typedef struct XML_CreateInfo_typ
{	unsigned long LengthInBytes;
	signed long ErrorNum;
} XML_CreateInfo_typ;

typedef struct XML_DiagInfo_typ
{	plcbit Reset;
	plcbit Logger;
	unsigned long TotalCalls;
	unsigned long TotalResponses;
	unsigned long TotalFaults;
	unsigned long TotalErrors;
} XML_DiagInfo_typ;

typedef struct XML_Method_typ
{	signed long InitStatus;
	signed long Status;
	plcstring MethodName[41];
	plcstring PVServer[41];
	plcstring PVClient[41];
	signed short OpIndex;
	struct XML_ParseInfo_typ ParseInfo;
	struct XML_CreateInfo_typ CreateInfo;
	struct XML_DiagInfo_typ DiagInfo;
	plcbit Fault;
	plcstring FaultString[41];
	signed long FaultCode;
	plcstring ViewLineString[120];
	plcbit ViewInc;
	unsigned short ViewLine;
	unsigned short ViewLevel;
	unsigned short _viewline;
	unsigned long BufferSize;
	unsigned long pMessage;
} XML_Method_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC signed long XMLp_CallParse(struct XML_Method_typ* m);
_BUR_PUBLIC signed long XMLp_CallCreate(struct XML_Method_typ* m, unsigned char* PVNames);
_BUR_PUBLIC plcbit XMLp_Init(struct XML_Method_typ* m, unsigned long BufferSize);
_BUR_PUBLIC plcbit XMLp_Destroy(struct XML_Method_typ* m);


#ifdef __cplusplus
};
#endif
#endif /* _XMLP_LIB_ */

