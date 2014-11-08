//
//								aflangemain.cpp
//

#ifndef __aflange__
#include "aflange.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new AFlange (audioMaster);
}

