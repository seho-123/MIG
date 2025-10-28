/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1757074935_4_
#define _BUR_1757074935_4_

#include <bur/plctypes.h>

/* Constants */
#ifdef _REPLACE_CONST
#else
#endif


/* Variables */
_BUR_LOCAL struct TON pelcoSrvTimer;
_BUR_LOCAL plcbit pelcoMsgSend;
_BUR_LOCAL unsigned short pelcoSrvPort;
_BUR_LOCAL plctime pelcoSrvTimeout;
_BUR_LOCAL plcstring pelcoSrvIP[21];
_BUR_LOCAL unsigned char pelcoMsg[7];
_BUR_LOCAL struct UdpOpen open;
_BUR_LOCAL struct UdpClose close;
_BUR_LOCAL struct UdpSend send;





__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/UDP/Variables.var\\\" scope \\\"local\\\"\\n\"");
__asm__(".ascii \"iecfile \\\"Logical/Libraries/standard/standard.fun\\\" scope \\\"global\\\"\\n\"");
__asm__(".ascii \"iecfile \\\"Logical/Libraries/AsUDP/AsUDP.fun\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1757074935_4_ */

