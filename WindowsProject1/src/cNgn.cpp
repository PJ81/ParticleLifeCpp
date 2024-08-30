//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#include "../inc/stdafx.h"
#include "../inc/misc.h"
#include "../inc/cNgn.h"

//--------------------------------------------------------------------------------------------------
//#define TIMING 1

//--------------------------------------------------------------------------------------------------
cNgn* cNgn::m_pApp = 0;

//--------------------------------------------------------------------------------------------------
void cNgn::MainLoop(void) {

#ifdef TIMING 
    m_fNow = GetTime();
    if (m_fNow - m_fStart > 1.0f) {
        m_fps = m_uiFrames / (m_fNow - m_fStart);
        m_fStart = m_fNow;
        m_uiFrames = 0;
    }

#endif

    // call virtual function to render the frame
    RenderFrame();

    // blit back buffer
    HDC hdc = GetDC(m_hWnd);
    BitBlt(hdc, 0, 0, m_iWndWid, m_iWndHei, m_hBackDC, 0, 0, SRCCOPY);
    ReleaseDC(m_hWnd, hdc);

#ifdef TIMING
    m_uiFrames++;
    m_fFrameTime = GetTime() - m_fNow;
    if (m_fFrameTime > 1.0f) m_fFrameTime = 1.0f;

    WCHAR txt[64];
    swprintf_s(txt, 64, L"+++ %.2f +++", m_fps);
    SetWindowText(m_hWnd, txt);
    m_fTimeCount += m_fFrameTime;

#endif
}
//--------------------------------------------------------------------------------------------------
void cNgn::OnPaint(HWND hWnd) {

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    BitBlt(hdc, 0, 0, m_iWndWid, m_iWndHei, m_hBackDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
}
//--------------------------------------------------------------------------------------------------
void cNgn::OnDestroy(HWND hWnd) {

    DeleteDC(m_hBackDC);
    DeleteObject(m_hBitmap);
    PostQuitMessage(0);
}
//--------------------------------------------------------------------------------------------------
bool cNgn::IsRunning(void) {
    // nothing for now
    return false;
}
//--------------------------------------------------------------------------------------------------
BOOL cNgn::RegisterMyClass(void) {

    WNDCLASSEX			wcex;
    CLEAR_THIS(&wcex);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)ngnMain;
    wcex.cbWndExtra = DLGWINDOWEXTRA;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, (LPCTSTR)IDI_ICON);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszClassName = L"DA_BOX";

    return RegisterClassEx(&wcex);
}
//--------------------------------------------------------------------------------------------------
void cNgn::SetupWindow(int w, int h) {

    RECT rc = { 0, 0, w, h };

    AdjustWindowRectEx(&rc, WS_SYSMENU | WS_CAPTION, FALSE, WS_EX_CLIENTEDGE);
    m_iWndWid = rc.right - rc.left;
    m_iWndHei = rc.bottom - rc.top;

    int posX = (GetSystemMetrics(SM_CXSCREEN) >> 1) - (m_iWndWid >> 1),
        posY = (GetSystemMetrics(SM_CYSCREEN) >> 1) - (m_iWndHei >> 1);

    SetWindowPos(m_hWnd, HWND_TOP, posX, posY, m_iWndWid, m_iWndHei, SWP_NOZORDER);
}
//--------------------------------------------------------------------------------------------------
bool cNgn::CreateBackbuffer(void)
{
    BITMAPINFO	bi;
    void* pBits;
    CLEAR_THIS(&bi);

    if (m_hBitmap) DeleteObject(m_hBitmap);
    if (m_hBackDC) DeleteDC(m_hBackDC);

    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biWidth = m_iWndWid;
    bi.bmiHeader.biHeight = -m_iWndHei;

    HDC hdc = GetDC(m_hWnd);
    m_hBitmap = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &pBits, NULL, 0);
    if (!m_hBitmap) return false;

    m_hBackDC = CreateCompatibleDC(hdc);
    ReleaseDC(m_hWnd, hdc);
    SelectObject(m_hBackDC, m_hBitmap);

    return true;
}
//--------------------------------------------------------------------------------------------------
int cNgn::Run(HINSTANCE hInst) {

    if (IsRunning()) return 0;

    m_fTimeCount = 0;
    m_hInst = hInst;

    IS_CREATED(RegisterMyClass());

    m_hWnd = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_BOX), GetDesktopWindow(), 0);
    IS_CREATED(m_hWnd);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    MSG msg;
    CLEAR_THIS(&msg);

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            DispatchMessage(&msg);
        else MainLoop();
    }

    UnregisterClass(L"DA_BOX", m_hInst);

    return 0;
}
//--------------------------------------------------------------------------------------------------
int WINAPI cNgn::ngnMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {
        HANDLE_MSG(hWnd, WM_CREATE, m_pApp->OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, m_pApp->OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, m_pApp->OnPaint);
    default:
        return (int)DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
BOOL cNgn::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {

    m_hWnd = hWnd;

    SetupWindow(m_iWndWid, m_iWndHei);
    CreateBackbuffer();

    // call virtual function to initialize the program
    Initialize();

    return TRUE;
}
//--------------------------------------------------------------------------------------------------