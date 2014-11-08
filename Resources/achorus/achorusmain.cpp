//
//								achorusmain.cpp
//

#ifndef __achorus__
#include "achorus.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new AChorus (audioMaster);
}

