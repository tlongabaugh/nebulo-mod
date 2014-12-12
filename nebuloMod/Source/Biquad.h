// --- BiQuad ---
// This class can be used alone or as the base class for a derived object
// Note the virtual destructor, required for proper clean-up of derived
// classes
//
// Implements a single Bi-Quad Structure
//
// From Will Pirkle's "Designing Audio Effects in C++"

#ifndef __NebuloMod__BiQuad__
#define __NebuloMod__BiQuad__

#define FLT_MIN_PLUS          1.175494351e-38         /* min positive value */
#define FLT_MIN_MINUS        -1.175494351e-38         /* min negative value */

class BiQuad
{
public:
    BiQuad(void);
    virtual ~BiQuad(void);
    
protected:
    float m_f_Xz_1; // x z-1 delay element
    float m_f_Xz_2; // x z-2 delay element
    float m_f_Yz_1; // y z-1 delay element
    float m_f_Yz_2; // y z-2 delay element
    
    // allow other objects to set these directly since we have no cooking
    // function or intelligence
    
public:
    float a0;
    float a1;
    float a2;
    float b1;
    float b2;
    
    // flush Delays
    void flushDelays();
    
    // Do the filter: given input xn, calculate output yn and return it
    float doBiQuad(float f_xn);
    
};
#endif // Biquad_h
