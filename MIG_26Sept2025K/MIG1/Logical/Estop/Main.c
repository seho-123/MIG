
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT EstopInit(void)
{

}

void _CYCLIC EstopCyclic(void)
{
	int i;
	
	for( i=0; i<=maxStation; i++ ) {
		// ESD is low if ESTOP is low, else check for reset
		if (!estop[i].ESTOP) {
			estop[i].ESD = 0;
		} else {
			// Look for positive edge on reset
			if (estop[i].Reset && !estop[i]._Reset) {
				estop[i].ESD = 1;
			}
		}

		// Save Reset status to look for positive edge
		estop[i]._Reset = estop[i].Reset;

	}
	

}

void _EXIT EstopExit(void)
{

}

