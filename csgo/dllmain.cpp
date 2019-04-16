#include "inc.h"

#include "features/misc/misc.h"
#include "features/animations/anim.h"
#include "features/entity listener/ent_listener.h"

c_csgo g_csgo{ };
c_netvars g_netvars{ };
c_renderer g_renderer{ };
c_menu g_menu{ };
c_config g_configs{ };
c_client g_cl{ };
c_vars g_vars{ };

bool unload() {
#ifdef _DEBUG
	console::detach( );
#endif

	g_misc.transparent_props( 100.f );

	if( !g_events.remove( ) )
		return false;

	g_anim.remove( );

	g_listener.remove( );

	if( OSHGui::Application::HasBeenInitialized( ) )
		g_renderer.get_renderer( ).PreD3DReset( );

	if( !g_hooks.release( ) )
		return false;

	if( !g_input.remove( ) )
		return false;

	return true;
}

static ulong_t __stdcall cheat_init( void *arg ) {
	try {
		while( !pe::get_module( CT_HASH32( "serverbrowser.dll" ) ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

#ifdef _DEBUG
		if( !console::allocate( "debug" ) )
			std::abort( );

		console::print( "initializing deadcell.\n" );

		// redirect warnings to a window similar to errors.
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_WNDW );
#endif

		if( !g_configs.init( ) )
			throw std::runtime_error( "Failed to initialize profile system" );

		if( !g_configs.save( "default" ) )
			throw std::runtime_error( "Failed to save default profile" );

		if( !g_csgo.init( ) )
			throw std::runtime_error( "Failed to initialize interfaces" );

		if( !g_events.setup( ) )
			throw std::runtime_error( "Failed to setup event(s)" );

		if( !g_netvars.init( ) )
			throw std::runtime_error( "Sanity check failed on netvar intialization" );

		g_anim.init( );

		g_listener.init( );

		if( !g_hooks.init( ) )
			throw std::runtime_error( "Failed to initialize hook(s)" );

		if( !g_hooks.hook( ) )
			throw std::runtime_error( "Failed to hook function(s)" );

		g_misc.transparent_props( g_vars.misc.prop_transparency );

#ifdef _DEBUG
		while( !&g_renderer.get_renderer( ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

		console::print( "game resolution ==> %x%.\n", g_renderer.get_renderer( ).GetDisplaySize( ).Width, g_renderer.get_renderer( ).GetDisplaySize( ).Height );
#endif

		if( !g_csgo.m_command_line->find_param( "-insecure" ) ) {
#ifdef _DEBUG
			console::print( "-insecure launch parameter not found.\n" );
#endif
		}
		
		return 1;
	}
	catch( const std::runtime_error &ex ) {
		UNREFERENCED_PARAMETER( ex );

#ifndef _DEBUG // only show error message in release configuration(s).
		MessageBoxA( nullptr, ex.what( ), nullptr, MB_OK | MB_ICONERROR | MB_TOPMOST );

		// attempt to safely unload dll.
		if( g_csgo.m_interfaces_successful && g_hooks.m_hooked_successful ) {
			if( !unload( ) ) // unable to safely unload dll.
				std::abort( );

			Beep( 500, 350 );

			FreeLibraryAndExitThread( static_cast< HMODULE >( arg ), 0 );
		}
		else {
			std::abort( );
		}
#endif
	}

	return 1;
}

static ulong_t __stdcall cheat_free( void *arg ) {
	while( !g_input.key_pressed( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );

#ifdef _DEBUG
	console::detach( );
#endif

	if( !unload( ) ) // unable to safely unload dll.
		std::abort( );

	Beep( 500, 350 );

	FreeLibraryAndExitThread( static_cast< HMODULE >( arg ), 0 );
}

int __stdcall DllMain( const HMODULE self, const ulong_t reason_for_call, void *reserved ) {
	if( reason_for_call == DLL_PROCESS_ATTACH ) {
		//util::mem::disable_thread_lib_calls( self );

		HANDLE cheat_thread = CreateThread( nullptr, 0, &cheat_init, nullptr, 0, nullptr );
		if( !cheat_thread )
			return 0;

		HANDLE free_thread = CreateThread( nullptr, 0, &cheat_free, self, 0, nullptr );
		if( !free_thread )
			return 0;

		util::mem::close_handle( cheat_thread );
		util::mem::close_handle( free_thread );

		return 1;
	}

	return 0;
}
