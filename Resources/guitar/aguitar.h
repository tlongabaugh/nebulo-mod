//
//								aguitar.h
//

#ifndef __aguitar__
#define __aguitar__

#include "public.sdk/source/vst2.x/audioeffectx.h"

#define	SAMPLE_RATE	44100
#define HIPASS_FREQ 2000
#define M_PI 3.141592653589793
#define M_PI_2 (M_PI/2.0)

enum
{
	// Global
	kNumPrograms = 16,

	// Parameters Tags
	kTone = 0,
	kNumParams
};

class AGuitar;

//------------------------------------------------------------------------
class AGuitarProgram
{
friend class AGuitar;
public:
	AGuitarProgram ();
	~AGuitarProgram () {}

private:	
	float paramTone;
	char name[24];
};

class OnePoleHighPassFilter;
class OnePoleLowPassFilter;
class TwoPoleHighPassFilter;
class Limiter;
class Phaser;
class Chorus;
class CurveApprox;


//------------------------------------------------------------------------
class AGuitar : public AudioEffectX
{
public:
	AGuitar (audioMasterCallback audioMaster);
	~AGuitar ();

	//---from AudioEffect-----------------------
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

	virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual void resume ();

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
	void setTone(float v);

	AGuitarProgram* programs;
	TwoPoleHighPassFilter* _highPass;
	Limiter *_limiter;
	Phaser *_phaser;
	Chorus *_chorus;
	OnePoleLowPassFilter* _controlFilter;
	CurveApprox *_caLimiterGain;
	CurveApprox *_caFilterGain;
	CurveApprox *_caPhaserGain;
	CurveApprox *_caOverdrive;
	CurveApprox *_caOutGain;

	float _paramTone;		// 0.0-1.0 passed in
	double _tone;			// actual calc'd threshhold
};

#endif
