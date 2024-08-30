
//--------------------------------------------------------------------------------------------------
// ++ Author: Paulo Jorente
// ++ Date: 22.9.2008
//--------------------------------------------------------------------------------------------------
#include "../inc/stdafx.h"
#include "../inc/cPraiseBox.h"
#include <math.h>

//--------------------------------------------------------------------------------------------------
cPraiseBox* cPraiseBox::m_pPraiseBox = 0;

//--------------------------------------------------------------------------------------------------
void cPraiseBox::FormatTime( float time, char* str )
{
    int h = 0, m = 0, s = ( int )time;

    if( s > 59 )
    {
        m = ( int )time / 60;
        s = ( int )time - m * 60;
        if( m > 59 )
        {
            h = m / 60;
            m = m - h * 60;
        }
    }
    
    wsprintf( str, "%.2d:%.2d:%.2d", h, m, s );
}
//--------------------------------------------------------------------------------------------------
void cPraiseBox::SetTimeVars( float time, int level )
{
   	float av[7];
   	ZeroMemory( av, sizeof( av ) );
   	av[level] = time;
	
    FormatTime( time, m_szTime );
	
    FILE* pFile;
    if( 0 == fopen_s( &pFile, "data.dat", "r" ) )
    {
        for( int x = 0; x < 7; x++ )
            fread( &av[x], sizeof( float ), 1, pFile );

        fclose( pFile );
    }

	FormatTime( av[level], m_szAverage );
	
	av[level] = ( av[level] + time ) / 2.0f;
	
	if( 0 == fopen_s( &pFile, "data.dat", "w" ) )
	{
	    for( int x = 0; x < 7; x++ )
	        fwrite( &av[x], sizeof( float ), 1, pFile );

	    fclose( pFile );
	}
	
}
//--------------------------------------------------------------------------------------------------
int cPraiseBox::Run( HINSTANCE hInst, HWND hWnd, float time, int level )
{
    m_bHover = false;
    m_hInst = hInst;

    SetTimeVars( time, level );
    IS_CREATED( RegisterMyClass() );
    
    m_hWnd = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_PRAISE ), hWnd, 0 );
    IS_CREATED( m_hWnd );

    // prepares the link button
    HWND hBut = GetDlgItem( m_hWnd, IDOK );
    m_lLinkProc = GetWindowLong( hBut, GWL_WNDPROC );
    SetWindowLong( hBut, GWL_WNDPROC, ( LONG )m_pPraiseBox->ButtonClass );

    CenterWindow();
    CreateBackbuffer();
    
    HDC hdc = GetDC( m_hWnd );
    SetTextColor ( hdc, 0xffffff );
    SetBkColor( hdc, 0 );
    ReleaseDC( m_hWnd, hdc );
    
    ShowWindow( m_hWnd, SW_SHOW );
    UpdateWindow( m_hWnd );

    MSG msg;
    CLEAR_THIS( &msg );

    while( msg.message != WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            DispatchMessage( &msg );
        else DrawFrame();
    }
    return UnregisterClass( "PRAISE", m_hInst );
}
//--------------------------------------------------------------------------------------------------
void cPraiseBox::DrawFrame( void )
{
	float now = GetTime();

	ZeroMemory( m_pBack, m_uiSize );

    static double	mysin = 0;
    double			col = 0, cos1 = 400, hgt = 16, a, b;
    BITMAP          bit;
    
    {
        col = 0;
        int sc = 0;
        do
        {
            a = ( 4 + col * 32 + ( cos( cos1 ) * ( col - 6 ) * cos( mysin ) ) );
            b = ( 24 + cos( mysin + col ) * hgt + 17 );

            GetObject( m_hBit[sc], sizeof( bit ), &bit );
            BitBlt( m_hBackDC, ( int )a, ( int )b, bit.bmWidth, bit.bmHeight, m_hDC[sc], 0, 0, SRCCOPY );
            
            if( sc == 3 || sc == 10 ) col += 0.4;
            else if( sc == 5 || sc == 9 ) col += 1;
            else if( sc == 6 ) col += 1.1;
            else col += 0.8;
        }
        while( ++sc < 13 );		
        mysin += 4 * m_fFrameTime;
    }
    
    HDC hdc = GetDC( m_hWnd );
    BitBlt( hdc, 11, 10, 328, 110, m_hBackDC, 0, 0, SRCCOPY );
    ReleaseDC( m_hWnd, hdc );

	m_fFrameTime = GetTime() - now;
}
//--------------------------------------------------------------------------------------------------
void cPraiseBox::OnDrawItem( HWND hWnd, const DRAWITEMSTRUCT * lpDrawItem )
{
    char text[64];
    int iUp = 0;
    HBRUSH bb = ( HBRUSH )GetStockObject( BLACK_BRUSH );

    GetWindowText( lpDrawItem->hwndItem, text, 64 );
    SetTextColor ( lpDrawItem->hDC, RGB( 255, 255, 255 ) );
    SetBkColor( lpDrawItem->hDC, 0 );
    FillRect( lpDrawItem->hDC, ( LPRECT )&lpDrawItem->rcItem, bb );
    SelectObject( lpDrawItem->hDC, ( HPEN )GetStockObject( WHITE_PEN ) );
    SelectObject( lpDrawItem->hDC, bb );
    
    InflateRect( ( LPRECT )&lpDrawItem->rcItem, -1, -1 );
    SelectObject( lpDrawItem->hDC, ( HBRUSH )GetStockObject( HOLLOW_BRUSH ) );

    if( m_bHover ) iUp = -1;
    if( lpDrawItem->itemAction & ODA_SELECT && lpDrawItem->itemState & ODS_SELECTED )  iUp = 1;

    Rectangle( lpDrawItem->hDC, lpDrawItem->rcItem.left + iUp, lpDrawItem->rcItem.top + iUp,
        lpDrawItem->rcItem.right + iUp, lpDrawItem->rcItem.bottom + iUp );
    InflateRect( ( LPRECT )&lpDrawItem->rcItem, -1, -1 );
    Rectangle( lpDrawItem->hDC, lpDrawItem->rcItem.left + iUp, lpDrawItem->rcItem.top + iUp,
        lpDrawItem->rcItem.right + iUp, lpDrawItem->rcItem.bottom + iUp );

    OffsetRect( ( LPRECT )&lpDrawItem->rcItem, iUp, iUp );
    DrawText( lpDrawItem->hDC, text, lstrlen( text ), ( LPRECT )&lpDrawItem->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
}
//--------------------------------------------------------------------------------------------------
void cPraiseBox::CenterWindow( void )
{
	RECT rc;
	POINT pt, parent;

	GetWindowRect( GetParent( m_hWnd ), &rc );
	parent.x = rc.left; parent.y = rc.top;

	pt.x = ( rc.right - rc.left ) / 2;
	pt.y = ( rc.bottom - rc.top ) / 2;

	pt.x -= 350 / 2;
	pt.y -= 321 / 2;

    SetWindowPos( m_hWnd, 0, parent.x + pt.x, parent.y + pt.y, 350, 321, SWP_NOZORDER | SWP_SHOWWINDOW );
}
//--------------------------------------------------------------------------------------------------
void cPraiseBox::OnClose( HWND hWnd )
{
    for( int x = 0; x < 14; x++ )
    {
        DeleteDC( m_hDC[x] );
        DeleteObject( m_hBit[x] );
    }
    
    DeleteDC( m_hBackDC );
    DeleteObject( m_hBitmap );
    
    PostQuitMessage( 0 );
}
//--------------------------------------------------------------------------------------------------
bool cPraiseBox::CreateBackbuffer( void )
{
    BITMAPINFO	bi;
    void		*pBits;
    CLEAR_THIS( &bi );

    bi.bmiHeader.biSize			= sizeof( bi.bmiHeader );
    bi.bmiHeader.biBitCount		= 32;
    bi.bmiHeader.biCompression	= BI_RGB;
    bi.bmiHeader.biPlanes		= 1;
    bi.bmiHeader.biWidth		=  328;
    bi.bmiHeader.biHeight		= -110;

    HDC hdc = GetDC( m_hWnd );
    m_hBitmap = CreateDIBSection( hdc, &bi, DIB_RGB_COLORS, &pBits, NULL, 0 );
    if( !m_hBitmap ) return false;

    m_hBackDC = CreateCompatibleDC( hdc );
    ReleaseDC( m_hWnd, hdc ); 
    SelectObject( m_hBackDC, m_hBitmap );
    
    m_pBack = ( uint* )pBits;
    m_uiSize = sizeof( uint ) * 328 * 110;

    return true;
}
//--------------------------------------------------------------------------------------------------
LRESULT cPraiseBox::OnInitDialog( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
    // set the icon for the box
	SetClassLong( hWnd, GCL_HICON, ( long )LoadIcon( m_hInst, MAKEINTRESOURCE( IDI_ICON ) ) );
    
    for( int x = 0; x < 14; x++ )
    {
        m_hDC[x] = CreateCompatibleDC( 0 );
        m_hBit[x] = LoadImage( m_hInst, MAKEINTRESOURCE( x + IDB_BITMAP46 ), IMAGE_BITMAP, 0, 0, 0 );
        SelectObject( m_hDC[x], m_hBit[x] );
    }

	return 1;
}
//--------------------------------------------------------------------------------------------------
BOOL cPraiseBox::RegisterMyClass( void )
{
    WNDCLASSEX			wcex;
    CLEAR_THIS( &wcex );

    wcex.cbSize			= sizeof( WNDCLASSEX ); 
    wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc	= ( WNDPROC )PraiseBox;
    wcex.cbWndExtra		= DLGWINDOWEXTRA;
    wcex.hInstance		= m_hInst;
    wcex.hIcon			= LoadIcon( m_hInst, ( LPCTSTR )IDI_ICON );
    wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground	= ( HBRUSH )( COLOR_BTNFACE + 1 );
    wcex.lpszClassName	= "PRAISE";

    return RegisterClassEx( &wcex );
}
//--------------------------------------------------------------------------------------------------
void cPraiseBox::OnPaint( HWND hWnd )
{
    PAINTSTRUCT ps;
    cRect r;

    HDC hdc = BeginPaint( hWnd, &ps );
    BitBlt( hdc, 0, 0, 350, 321, m_hDC[13], 0, 0, SRCCOPY );

    r.Set( 93, 153, 256, 178 );
    DrawText( hdc, m_szTime, lstrlen( m_szTime ), &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
    r.Set( 76, 224, 273, 249 );
    DrawText( hdc, m_szAverage, lstrlen( m_szAverage ), &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
    
    EndPaint( hWnd, &ps );
}
//--------------------------------------------------------------------------------------------------
int WINAPI cPraiseBox::PraiseBox( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
        HANDLE_MSG( hWnd, WM_DESTROY, m_pPraiseBox->OnClose );
        HANDLE_MSG( hWnd, WM_PAINT, m_pPraiseBox->OnPaint );
		HANDLE_MSG( hWnd, WM_COMMAND, m_pPraiseBox->OnCommand );
		HANDLE_MSG( hWnd, WM_CREATE, m_pPraiseBox->OnInitDialog );
		HANDLE_MSG( hWnd, WM_DRAWITEM, m_pPraiseBox->OnDrawItem );
        default:
            return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}
//------------------------------------------------------------------------------
LRESULT CALLBACK cPraiseBox::ButtonClass( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_MOUSEMOVE:
        {
            TRACKMOUSEEVENT	tme;
            tme.cbSize = sizeof( tme );
            tme.dwFlags = TME_LEAVE | TME_HOVER;
            tme.dwHoverTime = 1;
            tme.hwndTrack = hWnd;

            TrackMouseEvent( &tme );
            return TRUE;
        }
        case WM_MOUSEHOVER:
        {
            m_pPraiseBox->m_bHover = TRUE;
            InvalidateRect( hWnd, NULL, FALSE );
            return TRUE;
        }
        case WM_MOUSELEAVE:
        {
            m_pPraiseBox->m_bHover = FALSE;
            InvalidateRect( hWnd, NULL, FALSE );
            return TRUE;
        }
    }
    return CallWindowProc( ( WNDPROC )m_pPraiseBox->m_lLinkProc, hWnd, msg, wParam, lParam );
}
//--------------------------------------------------------------------------------------------------