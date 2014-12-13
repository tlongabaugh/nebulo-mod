//
//  LFOtest.h
//  NebuloMod
//
//  Created by Tom Longabaugh on 12/13/14.
//
//

#ifndef __NebuloMod__LFOtest__
#define __NebuloMod__LFOtest__

#include <stdio.h>

// --- CWaveTable ---
// generic WT Oscillator for you to use
class LFOtest
{
public:	// Functions
    //
    // One Time Initialization
    LFOtest();
    
    // One Time Destruction
    ~LFOtest(void);
    
    // The Prepare For Play Function is called just before audio streams
    bool prepareForPlay();
    
    // --- MEMBER FUNCTIONS
    //  function to do render one sample of the Oscillator
    //  call this once per sample period
    //
    //	pYn is the normal output
    //	pYqn is the quad phase output
    void doOscillate(float* pYn, float* pYqn);
    
    // reset the read index
    void reset()
    {
        m_fReadIndex = 0.0;
        m_fQuadPhaseReadIndex = 256.0;	// 1/4 of our 1024 point buffer
    }
    
    // set the sample rate: NEEDED to calcualte the increment value from frequency
    void setSampleRate(int nSampleRate)
    {
        m_nSampleRate = nSampleRate;
    }
    
    // our cooking function
    void cookFrequency()
    {
        // inc = L*fd/fs
        m_f_inc = 1024.0*m_fFrequency_Hz/(float)m_nSampleRate;
    }
    
    // --- MEMBER OBJECTS
    // Array for the Table
    float m_SinArray[1024];			// 1024 Point Sinusoid
    float m_SawtoothArray[1024];    // saw
    float m_TriangleArray[1024];	// tri
    float m_SquareArray[1024];		// sqr
    
    // band limited to 5 partials
    float m_SawtoothArray_BL5[1024];    // saw, BL = 5
    float m_TriangleArray_BL5[1024];	// tri, BL = 5
    float m_SquareArray_BL5[1024];		// sqr, BL = 5
    
    // current read location
    float m_fReadIndex;					// NOTE its a FLOAT!
    float m_fQuadPhaseReadIndex;		// NOTE its a FLOAT!
    
    // our inc value
    float m_f_inc;
    
    // fs value
    int   m_nSampleRate;
    
    // user-controlled variables:
    // Frequency
    float m_fFrequency_Hz;
    
    // Inverted Output
    bool m_bInvert;
    
    // Type
    unsigned int m_uOscType;
    enum{sine,saw,tri,square};
    
    // Mode
    unsigned int m_uTableMode;
    enum{normal,bandlimit};
    
    // Polarity
    unsigned int m_uPolarity;
    enum{bipolar,unipolar};
};

#endif /* defined(__NebuloMod__LFOtest__) */
