
//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//-------------------------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------------------------
#define										SAFE_DELETE_ARRAY( p )							{ if( p ){ delete[] ( p ); ( p ) = NULL; } }
#define										SAFE_RELEASE( p )								{ if( p ) { ( p )->Release(); ( p ) = NULL; } }
#define										SAFE_DELETE( p )								{ if( p ){ delete ( p ); ( p ) = NULL; } }
#define										IS_CREATED( p )									{ if( !p ){ return 0; } }
#define										CLEAR_THIS( p )									{ ZeroMemory( p, sizeof( *p ) ); }

constexpr auto								WIDTH =											1280;
constexpr auto								HW =											WIDTH / 2;
constexpr auto								HEIGHT =										800;
constexpr auto								HH =											HEIGHT / 2;
constexpr auto								MAX_POP =										2000;
constexpr auto								MAX_TYPES =										6;
//--------------------------------------------------------------------------------------------------