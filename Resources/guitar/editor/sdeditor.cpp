//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:28 $
//
// Category     : VST 2.x SDK Samples
// Filename     : sdeditor.cpp
// Created by   : Steinberg Media Technologies
// Description  : Simple Surround Delay plugin with Editor using VSTGUI
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __sdeditor__
#include "sdeditor.h"
#endif

#ifndef __adelay__
#include "../aguitar.h"
#endif

#include <stdio.h>

//-----------------------------------------------------------------------------
// resource id's
enum {
	// bitmaps
	kBackgroundId = 128,
	kFaderBodyId,
	kFaderHandleId,
	
	// positions
	kFaderX = 160,
	kFaderY = 50,

	kFaderInc = 18,

	kDisplayX = 10,
	kDisplayY = 144,
	kDisplayXWidth = 30,
	kDisplayHeight = 14
};

//-----------------------------------------------------------------------------
// prototype string convert float -> percent
void percentStringConvert (float value, char* string);
void percentStringConvert (float value, char* string)
{
	 sprintf (string, "%d%%", (int)(100 * value + 0.5f));
}


//-----------------------------------------------------------------------------
// SDEditor class implementation
//-----------------------------------------------------------------------------
SDEditor::SDEditor (AudioEffect *effect)
 : AEffGUIEditor (effect) 
{
	toneFader    = 0;

	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	hBackground = new CBitmap (kBackgroundId);

	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = (short)hBackground->getWidth ();
	rect.bottom = (short)hBackground->getHeight ();
}

//-----------------------------------------------------------------------------
SDEditor::~SDEditor ()
{
	// free the background bitmap
	if (hBackground)
		hBackground->forget ();
	hBackground = 0;
}

//-----------------------------------------------------------------------------
bool SDEditor::open (void *ptr)
{
	// !!! always call this !!!
	AEffGUIEditor::open (ptr);
	
	//--load some bitmaps
	CBitmap* hFaderBody   = new CBitmap (kFaderBodyId);
	CBitmap* hFaderHandle = new CBitmap (kFaderHandleId);

	//--init background frame-----------------------------------------------
	// We use a local CFrame object so that calls to setParameter won't call into objects which may not exist yet. 
	// If all GUI objects are created we assign our class member to this one. See bottom of this method.
	CRect size (0, 0, hBackground->getWidth (), hBackground->getHeight ());
	CFrame* lFrame = new CFrame (size, ptr, this);
	lFrame->setBackground (hBackground);

	//--init the fader ------------------------------------------------
	int minPos = kFaderY;
	int maxPos = kFaderY + hFaderBody->getHeight () - hFaderHandle->getHeight () - 1;
	CPoint point (0, 0);
	CPoint offset (1, 0);

	// Tone
	size (kFaderX, kFaderY,
          kFaderX + hFaderBody->getWidth (), kFaderY + hFaderBody->getHeight ());
	toneFader = new CVerticalSlider (size, this, kTone, minPos, maxPos, hFaderHandle, hFaderBody, point);
	toneFader->setOffsetHandle (offset);
	toneFader->setValue (effect->getParameter (kTone));
	lFrame->addView (toneFader);


	//--init the display------------------------------------------------
	// Threshhold


	// Note : in the constructor of a CBitmap, the number of references is set to 1.
	// Then, each time the bitmap is used (for hinstance in a vertical slider), this
	// number is incremented.
	// As a consequence, everything happens as if the constructor by itself was adding
	// a reference. That's why we need til here a call to forget ().
	// You mustn't call delete here directly, because the bitmap is used by some CControls...
	// These "rules" apply to the other VSTGUI objects too.
	hFaderBody->forget ();
	hFaderHandle->forget ();

	frame = lFrame;
	return true;
}

//-----------------------------------------------------------------------------
void SDEditor::close ()
{
	delete frame;
	frame = 0;
}

//-----------------------------------------------------------------------------
void SDEditor::setParameter (VstInt32 index, float value)
{
	if (frame == 0)
		return;

	// called from ADelayEdit
	switch (index)
	{
		case kTone:
			if (toneFader)
				toneFader->setValue (effect->getParameter (index));
			break;

	}
}

//-----------------------------------------------------------------------------
void SDEditor::valueChanged (CDrawContext* context, CControl* control)
{
	long tag = control->getTag ();
	switch (tag)
	{
		case kTone:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		break;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
