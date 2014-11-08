//
//								aphaser.h
//

#ifndef __aphaser__
#define __aphaser__

#include "public.sdk/source/vst2.x/audioeffectx.h"

#define	SAMPLE_RATE	44100
#define BOTTOM_FREQ 100
#define M_PI 3.141592653589793
#define M_PI_2 (M_PI/2.0)
#define PIN(n,min,max) ((n) > (max) ? max : ((n) < (min) ? (min) : (n)))

enum
{
	// Global
	kNumPrograms = 8,

	// Parameters Tags
	kRate = 0,
	kWidth,
	kFeedback,
	kStages,
	kMixMode,

	kNumParams
};

enum
{
	kMixMono,
	kMixMonoMinus,
	kMixWetLeft,
	kMixWetRight,
	kMixStereo
};

class APhaser;

//------------------------------------------------------------------------
class APhaserProgram
{
friend class APhaser;
public:
	APhaserProgram ();
	APhaserProgram( double sweep, double width, double feedback, double stages, double mixMode, char *name );
	~APhaserProgram () {}

private:	
	double paramSweepRate;
	double paramWidth;
	double paramFeedback;
	double paramStages;
	double paramMixMode;
	char name[64];
};

//------------------------------------------------------------------------
class APhaser : public AudioEffectX
{
public:
	APhaser (audioMasterCallback audioMaster);
	~APhaser ();

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
	void setRate (float v);
	void setWidth (float v);
	void setFeedback (float v);
	void setStages (float v);
	void setMixMode (float v);
	void setSweep(void);

	APhaserProgram* pPrograms[kNumPrograms];
	
	float _paramSweepRate;		// 0.0-1.0 passed in
	float _paramWidth;			// ditto
	float _paramFeedback;		// ditto
	float _paramStages;			// ditto
	float _paramMixMode;		// ditto
	double _sweepRate;			// actual calc'd sweep rate
	double _width;				// 0-100%
	double _feedback;			// 0.0 to 1.0
	double _feedbackPhase;		// -1.0 or 1.0
	int   _stages;				// calc'd # of stages 2-10
	int	  _mixMode;				// mapped to supported mix modes

	double _wp;					// freq param for equation
	double  _minwp;
	double  _maxwp;
	double _sweepFactor;		// amount to multiply the freq by with each sample

	// the all pass line
	double _lx1;
	double _ly1;
	double _lx2;
	double _ly2;
	double _lx3;
	double _ly3;
	double _lx4;
	double _ly4;
	double _lx5;
	double _ly5;
	double _lx6;
	double _ly6;
	double _lx7;
	double _ly7;
	double _lx8;
	double _ly8;
	double _lx9;
	double _ly9;
	double _lx10;
	double _ly10;

	// output mixing
	double _mixLeftWet;
	double _mixLeftDry;
	double _mixRightWet;
	double _mixRightDry;
};

#endif
