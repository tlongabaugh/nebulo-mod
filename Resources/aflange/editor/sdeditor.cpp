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
#include "../aflange.h"
#endif

#include <stdio.h>

//-----------------------------------------------------------------------------
// resource id's
enum {
	// bitmaps
	kBackgroundId = 128,
	kFaderBodyId,
	kFaderHandleId,
	
	// fader positions
	kFaderX = 91,
	kFaderY = 64,
	kFaderInc = (75-16),

	// multi-select positions
	kSelectX = 155,
	kSelectY = 240,
	kSelectWidth = 80,
	kSelectHeight = 20,
	kSelectInc = 30,

	kDisplayX = 10,
	kDisplayY = 184,
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


//
//								SDEditor
//
SDEditor::SDEditor (AudioEffect *effect)
 : AEffGUIEditor (effect) 
{
	rateFader    = 0;
	widthFader = 0;
	feedbackFader   = 0;
	delayFader = 0;
	mixFader = 0;
	delayDisplay  = 0;
	feedbackDisplay = 0;
	volumeDisplay = 0;

	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	hBackground = new CBitmap (kBackgroundId);

	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = (short)hBackground->getWidth ();
	rect.bottom = (short)hBackground->getHeight ();
}

//
//								~SDEditor
//
SDEditor::~SDEditor ()
{
	// free the background bitmap
	if (hBackground)
		hBackground->forget ();
	hBackground = 0;
}

//
//								open
//
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

	//--init the faders------------------------------------------------
	int minPos = kFaderY;
	int maxPos = kFaderY + hFaderBody->getHeight () - hFaderHandle->getHeight () - 1;
	CPoint point (0, 0);
	CPoint offset (1, 0);

	// Rate
	size (kFaderX, kFaderY,
          kFaderX + hFaderBody->getWidth (), kFaderY + hFaderBody->getHeight ());
	rateFader = new CVerticalSlider (size, this, kRate, minPos, maxPos, hFaderHandle, hFaderBody, point);
	rateFader->setOffsetHandle (offset);
	rateFader->setValue (effect->getParameter (kRate));
	lFrame->addView (rateFader);

	// Width
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	widthFader = new CVerticalSlider (size, this, kWidth, minPos, maxPos, hFaderHandle, hFaderBody, point);
	widthFader->setOffsetHandle (offset);
	widthFader->setValue (effect->getParameter (kWidth));
	lFrame->addView (widthFader);

	// Feedback
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	feedbackFader = new CVerticalSlider (size, this, kFeedback, minPos, maxPos, hFaderHandle, hFaderBody, point);
	feedbackFader->setOffsetHandle (offset);
	feedbackFader->setValue (effect->getParameter (kFeedback));
	lFrame->addView (feedbackFader);

	// Delay
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	delayFader = new CVerticalSlider (size, this, kDelay, minPos, maxPos, hFaderHandle, hFaderBody, point);
	delayFader->setOffsetHandle (offset);
	delayFader->setValue (effect->getParameter (kDelay));
	lFrame->addView (delayFader);

	// Mix
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	mixFader = new CVerticalSlider (size, this, kMix, minPos, maxPos, hFaderHandle, hFaderBody, point);
	mixFader->setOffsetHandle (offset);
	mixFader->setValue (effect->getParameter (kMix));
	lFrame->addView (mixFader);

	// Mix mode
	//	size.offset( 0, kSelectInc );
	size (kSelectX, kSelectY, kSelectX + kSelectWidth, kSelectY + kSelectHeight);
	mixOption = new COptionMenu(size, this, kMixMode);
	mixOption->addEntry("mono");
	mixOption->addEntry("mono minus");
	mixOption->addEntry("mono +/-");
	mixOption->addEntry("stereo");
	mixOption->addEntry("stereo minus");
	mixOption->addEntry("stereo +/-");
	mixOption->setCurrent((int)(effect->getParameter(kMixMode)*kNumMixModes));
	lFrame->addView (mixOption);

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

//
//								close
//
void SDEditor::close ()
{
	delete frame;
	frame = 0;
}

//
//								setParameter
//
void SDEditor::setParameter (VstInt32 index, float value)
{
	if (frame == 0)
		return;

	switch (index)
	{
	case kRate:
		if (rateFader)
			rateFader->setValue (effect->getParameter (index));
		break;

	case kWidth:
		if (widthFader)
			widthFader->setValue (effect->getParameter (index));
		break;

	case kFeedback:
		if(feedbackFader)
			feedbackFader->setValue (effect->getParameter (index));
		break;

	case kDelay:
		if (delayOption)
		{
			// DEBUG
			delayOption->setValue (effect->getParameter (index));
			//delayOption->setValue (effect->getParameter (index)* NUM_DELAYS);
		}
		break;
	
	case kMix:
		mixFader->setValue( effect->getParameter( index ));
		break;

	case kMixMode:
		if (mixOption)
			mixOption->setValue (effect->getParameter (index));
		break;
	}
}

//
//								valueChanged
//
void SDEditor::valueChanged (CDrawContext* context, CControl* control)
{
	long tag = control->getTag ();
	switch (tag)
	{
	case kRate:
	case kWidth:
	case kFeedback:
	case kDelay:
	case kMix:
		effect->setParameterAutomated (tag, control->getValue ());
		control->setDirty ();
		break;

	//case kDelay:
	//	effect->setParameterAutomated (tag, control->getValue () / NUM_DELAYS);
	//	control->setDirty ();
	//	break;

	case kMixMode:
		effect->setParameterAutomated (tag, control->getValue ()/kNumMixModes);
		control->setDirty ();
		break;
	}
}

