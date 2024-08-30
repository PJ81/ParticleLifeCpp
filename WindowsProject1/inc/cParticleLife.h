//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------------------------
#include <Windows.h>
#include "../res/resource.h"
#include "../inc/cParticle.h"
#include "../inc/cBehavior.h"
#include "../inc/cRand.h"

//--------------------------------------------------------------------------------------------------
class cParticleLife {

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ctor / dtor
public:
    cParticleLife(void);
    ~cParticleLife(void);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ public functions
    int										Run(HINSTANCE hInst);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ private functions
private:
    BOOL                                    OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
    void                                    OnDestroy(HWND hWnd);
    void                                    OnPaint(HWND hWnd);
    BOOL                                    RegisterMyClass(void);
    void                                    SetupWindow(int w, int h);
    bool                                    IsRunning(void);
    bool									CreateBackbuffer(void);
    float                                   GetTime(void);
    void                                    StartRender(void);
    void                                    FinishRender(void);
    void									MainLoop(void);
    void                                    CreateRules(void);
    void                                    CreateParticles(void);
    void                                    UpdateParticles(void);

    static int WINAPI						lifeMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ private variables
private:
    static cParticleLife*                                                                   m_pApp;
    HINSTANCE			                                            						m_hInst;
    HWND				                                            						m_hWnd;
    HANDLE                                                                                  m_hBits[6];
    HBITMAP																					m_hBitmap;
    HDC																						m_hBackDC, m_hDCs[6];
    LARGE_INTEGER	                                                                        m_liFrequency;

    cRand                                                                                   m_Rand;

    float			                                                                        m_fFrameTime, m_fps, m_fNow, m_fStart, m_fTimeCount;
    unsigned			                                                                    m_uiFrames;
    int                                                                                     m_iWndWid, m_iWndHei;
    bool                                                                                    m_bWrapAround;

    cParticle                                                                               m_Particles[MAX_POP];
    cBehavior                                                                               m_Rules[MAX_TYPES][MAX_TYPES];

    HBRUSH                                                                                  m_BlackBrush;
    RECT                                                                                    m_Rect;
};
//--------------------------------------------------------------------------------------------------
inline cParticleLife::cParticleLife(void) : m_fFrameTime(0), m_fps(0), m_fNow(0), m_fStart(0), m_uiFrames(0), m_bWrapAround(false), m_hBitmap(0), m_hBackDC(0)
{
    QueryPerformanceFrequency(&m_liFrequency);
    m_pApp = this;
}
//--------------------------------------------------------------------------------------------------
inline cParticleLife::~cParticleLife(void)
{
    m_pApp = 0;
}
//--------------------------------------------------------------------------------------------------
inline float cParticleLife::GetTime(void)
{
    LARGE_INTEGER liTime;

    QueryPerformanceCounter(&liTime);
    return liTime.QuadPart / (float)m_liFrequency.QuadPart;
}
//--------------------------------------------------------------------------------------------------