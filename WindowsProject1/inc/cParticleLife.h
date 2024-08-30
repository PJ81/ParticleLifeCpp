//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------------------------
#include "../inc/cNgn.h"
#include "../inc/cParticle.h"
#include "../inc/cBehavior.h"

//--------------------------------------------------------------------------------------------------
constexpr auto								MAX_POP =                                       2100;
constexpr auto								MAX_TYPES =                                     6;

//--------------------------------------------------------------------------------------------------
class cParticleLife : public cNgn {

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ctor / dtor
public:
    cParticleLife                                                                           (int w, int h);
    ~cParticleLife                                                                          (void);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ private functions
protected:
    void                                    RenderFrame                                     (void);
    void                                    Initialize                                      (void);
    void                                    CreateRules                                     (void);
    void                                    CreateParticles                                 (void);
    void                                    UpdateParticles                                 (void);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ private variables
protected:
    cParticle                                                                               m_Particles[MAX_POP];
    cBehavior                                                                               m_Rules[MAX_TYPES][MAX_TYPES];
    HBRUSH                                                                                  m_BlackBrush;
    RECT                                                                                    m_Rect;
    HDC                                                                                     m_hDCs[7];
    HANDLE                                                                                  m_hBits[7];
    int                                                                                     m_ihWid, m_ihHei;
};
//--------------------------------------------------------------------------------------------------
inline cParticleLife::cParticleLife(int w, int h) : cNgn(w, h) {
    //
}
//--------------------------------------------------------------------------------------------------
inline cParticleLife::~cParticleLife(void) {
    for (int x = 0; x < 7; x++) {
        DeleteObject(m_hBits[x]);
        DeleteDC(m_hDCs[x]);
    }
}
//--------------------------------------------------------------------------------------------------