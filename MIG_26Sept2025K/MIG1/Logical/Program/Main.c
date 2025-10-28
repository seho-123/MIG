
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	open.enable = 1;
	open.device = (UDINT)"IF1";
	open.mode = (UDINT)"/PHY=RS232 /PA=N /DB=8 /SB=1 /BD=9600";
	open.config = 0;

	strcpy((UDINT)buffer, (UDINT)"TEST ");
	
	write.enable = 1;
	write.ident = 0;
	write.buffer = (UDINT)buffer;
	write.buflng = strlen(buffer);
}

void _CYCLIC ProgramCyclic(void)
{
	if (open.enable) {
		FRM_xopen(&open);
	}
	
	if (!open.status) {
		write.ident = open.ident;
		open.enable = 0;
	}
	
	if (write.ident != 0) {
		FRM_write(&write);
		
		if (write.status == 0) {
			//write.enable = 0;
		}
	}
	
	
}

void _EXIT ProgramExit(void)
{

}

