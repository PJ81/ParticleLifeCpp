
//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#include "../inc/stdafx.h"
#include "../inc/cParticleLife.h"
#include "../inc/misc.h"

//--------------------------------------------------------------------------------------------------
cParticleLife* cParticleLife::m_pApp = 0;

//--------------------------------------------------------------------------------------------------
void cParticleLife::MainLoop(void) {

    StartRender();

    FillRect(m_hBackDC, &m_Rect, m_BlackBrush);

    UpdateParticles();

    cParticle p;
    for (int x = 0; x < MAX_POP; x++) {
        p = m_Particles[x];
        BitBlt(m_hBackDC, (int)p.pos.x, (int)p.pos.y, 3, 3, m_hDCs[p.type], 0, 0, SRCCOPY);
    }

    FinishRender();

    m_fTimeCount += m_fFrameTime;
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::StartRender(void) {

    m_fNow = GetTime();
    if (m_fNow - m_fStart > 1.0f) {
        m_fps = m_uiFrames / (m_fNow - m_fStart);
        m_fStart = m_fNow;
        m_uiFrames = 0;
    }
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::FinishRender(void) {

    HDC hdc = GetDC(m_hWnd);
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, m_hBackDC, 0, 0, SRCCOPY);
    ReleaseDC(m_hWnd, hdc);

    SHORT space = GetAsyncKeyState(VK_SPACE);
    if ((1 << 15) & space) {
        CreateRules();
    }

    m_uiFrames++;
    m_fFrameTime = GetTime() - m_fNow;
    if (m_fFrameTime > 1.0f) m_fFrameTime = 1.0f;

#ifdef DEBUG
    WCHAR txt[64];
    swprintf_s(txt, 64, L"+++ %.2f +++", m_fps);
    SetWindowText(m_hWnd, txt);
#endif
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::OnPaint(HWND hWnd) {

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, m_hBackDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::OnDestroy(HWND hWnd) {

    DeleteDC(m_hBackDC);
    DeleteObject(m_hBitmap);

    for (int x = 0; x < 6; x++) {
        DeleteObject(m_hBits[x]);
        DeleteDC(m_hDCs[x]);
    }

    PostQuitMessage(0);
}
//--------------------------------------------------------------------------------------------------
bool cParticleLife::IsRunning(void) {

    return false;
}
//--------------------------------------------------------------------------------------------------
BOOL cParticleLife::RegisterMyClass(void) {

    WNDCLASSEX			wcex;
    CLEAR_THIS(&wcex);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)lifeMain;
    wcex.cbWndExtra = DLGWINDOWEXTRA;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, (LPCTSTR)IDI_ICON);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszClassName = L"DA_BOX";

    return RegisterClassEx(&wcex);
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::SetupWindow(int w, int h) {

    RECT rc = { 0, 0, w, h };

    AdjustWindowRectEx(&rc, WS_SYSMENU | WS_CAPTION, FALSE, WS_EX_CLIENTEDGE);
    m_iWndWid = rc.right - rc.left;
    m_iWndHei = rc.bottom - rc.top;

    int posX = (GetSystemMetrics(SM_CXSCREEN) >> 1) - (m_iWndWid >> 1),
        posY = (GetSystemMetrics(SM_CYSCREEN) >> 1) - (m_iWndHei >> 1);

    SetWindowPos(m_hWnd, HWND_TOP, posX, posY, m_iWndWid, m_iWndHei, SWP_NOZORDER);
}
//--------------------------------------------------------------------------------------------------
bool cParticleLife::CreateBackbuffer(void)
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
void cParticleLife::UpdateParticles() {
    cParticle* pj, *pi;
    float d;
    cBehavior behavior;
    cVector dir, force, totForce;

    for (int i = 0; i < MAX_POP; i++) {
        pi = &m_Particles[i];

        for (int j = 0; j < MAX_POP; j++) {
            if (j == i) continue;

            pj = &m_Particles[j];

            behavior = m_Rules[pi->type][pj->type];

            dir = pj->pos - pi->pos;
            
            dir.x += dir.x > HW ? -WIDTH : dir.x < -HW ? WIDTH : 0;
            dir.y += dir.y > HH ? -HEIGHT : dir.y < -HH ? HEIGHT : 0;

            d = dir.length();
            dir.normalize();

            if (d < behavior.attract.radius) {
                force = dir * behavior.attract.force;
                force *= .05f;
                totForce += force;
            }

            if (d < behavior.repel.radius) {
                force = dir * behavior.repel.force*3;
                force *= .05f;
                totForce += force;
            }
        }
        
        totForce *= .85f;
        pi->vel += totForce;
        pi->pos += pi->vel;
        pi->vel *= .5f;

        pi->pos.x = (float)fmod((pi->pos.x + WIDTH), WIDTH);
        pi->pos.y = (float)fmod((pi->pos.y + HEIGHT), HEIGHT);
    }
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::CreateParticles(void) {
    for (int x = 0; x < MAX_POP; x++) {
        m_Particles[x].type = rand() % MAX_TYPES;
        m_Particles[x].pos.set((float)(rand() % WIDTH), (float)(rand() % HEIGHT));
    }
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::CreateRules(void) {
    for (int y = 0; y < MAX_TYPES; y++) {
        for (int x = 0; x < MAX_TYPES; x++) {

            m_Rules[x][y].attract.radius = m_Rand.randF(70.f, 150.f);
            m_Rules[x][y].attract.force = m_Rand.randF(.3f, 1.f);
            if (m_Rand.randF() < .5) m_Rules[x][y].attract.force = -m_Rules[x][y].attract.force;

            m_Rules[x][y].repel.radius = m_Rand.randF(5.f, 25.f);
            m_Rules[x][y].repel.force = -m_Rand.randF(.3f, 1.f);
        }
    }
}
//--------------------------------------------------------------------------------------------------
int cParticleLife::Run(HINSTANCE hInst) {

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

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            DispatchMessage(&msg);
        else MainLoop();
    }

    UnregisterClass(L"DA_BOX", m_hInst);

    return 0;
}
//--------------------------------------------------------------------------------------------------
int WINAPI cParticleLife::lifeMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

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
BOOL cParticleLife::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {

    m_hWnd = hWnd;

    for (int x = 0; x < 6; x++) {
        m_hBits[x] = LoadImage(m_hInst, MAKEINTRESOURCE(x + IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
        m_hDCs[x] = CreateCompatibleDC(0);

        SelectObject(m_hDCs[x], m_hBits[x]);
    }

    SetupWindow(WIDTH, HEIGHT);
    CreateBackbuffer();
    CreateParticles();
    CreateRules();

    m_BlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    m_Rect = { 0, 0, WIDTH, HEIGHT };

    return TRUE;
}
//--------------------------------------------------------------------------------------------------