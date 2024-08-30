//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------------------------
#include <Windows.h>
#include "../res/resource.h"
#include "../inc/cRand.h"

//--------------------------------------------------------------------------------------------------
class cNgn {

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ctor / dtor
public:
    cNgn(int w, int h);
    ~cNgn(void);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ public functions
    int										Run                                             (HINSTANCE hInst);
    virtual void                            RenderFrame                                     (void) = 0;
    virtual void                            Initialize                                      (void) = 0;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ private functions
private:
    BOOL                                    OnCreate                                        (HWND hWnd, LPCREATESTRUCT lpCreateStruct);
    void                                    OnDestroy                                       (HWND hWnd);
    void                                    OnPaint                                         (HWND hWnd);
    BOOL                                    RegisterMyClass                                 (void);
    void                                    SetupWindow                                     (int w, int h);
    bool                                    IsRunning                                       (void);
    bool									CreateBackbuffer                                (void);
    float                                   GetTime                                         (void);
    void									MainLoop                                        (void);

    static int WINAPI						ngnMain                                         (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ private variables
private:
    static cNgn*                                                                            m_pApp;
    HWND				                                            						m_hWnd;
    HBITMAP																					m_hBitmap;
    LARGE_INTEGER	                                                                        m_liFrequency;
    float			                                                                        m_fFrameTime, m_fps, m_fNow, m_fStart, m_fTimeCount;
    unsigned			                                                                    m_uiFrames;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ protected variables
protected:
    HINSTANCE			                                            						m_hInst;
    HDC																						m_hBackDC;
    cRand                                                                                   m_Rand;
    int                                                                                     m_iWndWid, m_iWndHei;
};
//--------------------------------------------------------------------------------------------------
inline cNgn::cNgn(int w, int h) : m_fFrameTime(0), m_fps(0), m_fNow(0), m_fStart(0), m_uiFrames(0), m_hBitmap(0), m_hBackDC(0)
{
    QueryPerformanceFrequency(&m_liFrequency);
    m_pApp = this;

    m_iWndWid = w;
    m_iWndHei = h;
}
//--------------------------------------------------------------------------------------------------
inline cNgn::~cNgn(void)
{
    m_pApp = 0;
}
//--------------------------------------------------------------------------------------------------
inline float cNgn::GetTime(void)
{
    LARGE_INTEGER liTime;

    QueryPerformanceCounter(&liTime);
    return liTime.QuadPart / (float)m_liFrequency.QuadPart;
}
//--------------------------------------------------------------------------------------------------