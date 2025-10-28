/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1757074935_6_
#define _BUR_1757074935_6_

#include <bur/plctypes.h>

/* Constants */
#ifdef _REPLACE_CONST
#else
#endif


/* Variables */
_BUR_LOCAL plcbit cameraSend[2];
_BUR_LOCAL plcstring cameraIP[2][21];
_BUR_LOCAL struct TcpSend send[2];
_BUR_LOCAL struct TcpRecv receive[2];
_BUR_LOCAL plcbit connect[2];
_BUR_LOCAL struct TcpClient client[2];
_BUR_LOCAL unsigned char response[40];
_BUR_LOCAL plcstring message[41];
_BUR_LOCAL struct TcpOpen open[2];
_BUR_LOCAL struct TcpClose close[2];





__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/TcpCam/Variables.var\\\" scope \\\"local\\\"\\n\"");
__asm__(".ascii \"iecfile \\\"Logical/Libraries/AsTCP/AsTCP.fun\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1757074935_6_ */

