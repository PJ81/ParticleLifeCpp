
//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#include "../inc/stdafx.h"
#include "../inc/misc.h"
#include "../inc/cParticleLife.h"
#include <crtdbg.h>

//--------------------------------------------------------------------------------------------------
int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// start searching for memory leaks
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF );
	{
		srand( GetTickCount() );

		cParticleLife* life  = new cParticleLife();

		if( life ) {
			int p = 1;
			do {
				p = life->Run( hInstance );
			} while( p );

			SAFE_DELETE( life );
		}
	}

	// check the memory leaks
	_CrtDumpMemoryLeaks();

	return 0;
}
//--------------------------------------------------------------------------------------------------