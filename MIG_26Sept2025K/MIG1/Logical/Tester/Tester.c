
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


_LOCAL INT MainCounter , CountPeriod , CountDelta , CountPeriod_SP;

INT _mainCounter;

INT Cntr_Incr(INT newVal , INT prevVal );

void _INIT ProgramInit(void)
{

  CountPeriod_SP =	2000;

}

void _CYCLIC ProgramCyclic(void)
{

	if(CountPeriod > CountPeriod_SP){
		CountPeriod = 0;

		CountDelta = Cntr_Incr(MainCounter , _mainCounter);
		
		_mainCounter = MainCounter;

	}
	else
		CountPeriod++;

	MainCounter--;
}


INT Cntr_Incr(INT newVal , INT prevVal ){
INT ret = 0;

	
	if(newVal < prevVal)
		return ((0xFFFF - prevVal)+ newVal + 1);
	else
		return (newVal - prevVal);
	

	return ret;
}


