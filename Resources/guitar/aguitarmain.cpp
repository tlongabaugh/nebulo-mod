//
//								aguitarmain.cpp
//

#ifndef __aguitar__
#include "aguitar.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new AGuitar (audioMaster);
}

