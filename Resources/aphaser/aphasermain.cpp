//
//								aphasermain.cpp
//

#ifndef __aphaser__
#include "aphaser.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new APhaser (audioMaster);
}

