// BiQuad.cpp

#include <math.h>
#include "Biquad.h"

BiQuad::BiQuad()
{
    
}

BiQuad::~BiQuad()
{
    
}

void BiQuad::flushDelays()
{
    m_f_Xz_1 = 0;
    m_f_Xz_2 = 0;
    m_f_Yz_1 = 0;
    m_f_Yz_2 = 0;
}

float BiQuad::doBiQuad(float f_xn)
{
    // just do the difference equation: y(n) = a0x(n) + a1x(n-1) + a2x(n-2) - b1y(n-1) - b2y(n-2)
    float yn = a0*f_xn + a1*m_f_Xz_1 + a2*m_f_Xz_2 - b1*m_f_Yz_1 - b2*m_f_Yz_2;
    
    // underflow check
    if(yn > 0.0 && yn < FLT_MIN_PLUS) yn = 0;
    if(yn < 0.0 && yn > FLT_MIN_MINUS) yn = 0;
    
    // shuffle delays
    // Y delays
    m_f_Yz_2 = m_f_Yz_1;
    m_f_Yz_1 = yn;
    
    // X delays
    m_f_Xz_2 = m_f_Xz_1;
    m_f_Xz_1 = f_xn;
    
    return  yn;
}

