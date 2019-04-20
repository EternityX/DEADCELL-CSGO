#include "../inc.hpp"
#include "input.hpp"

#include "../../csgo/menu/menu.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

input_mngr g_input;

ulong_t __stdcall hook( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
	if( g_input.handle( hwnd, msg, wparam, lparam ) )
		return true;

	return CallWindowProcA( g_input.get_original_wndproc(), hwnd, msg, wparam, lparam );
}

input_mngr::input_mngr( )
	: m_window_handle{ }, m_original_wndproc{ }, m_key_pressed{ } { }

bool input_mngr::init( const std::string &window_name ) {
	if( m_window_handle )
		return false;

	m_window_handle = FindWindowA( window_name.c_str(), nullptr );
	m_original_wndproc = reinterpret_cast< WNDPROC >( SetWindowLongA( m_window_handle, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( hook ) ) );

	return m_original_wndproc != nullptr;
}

bool input_mngr::init( HWND wnd ) {
	return false;
}

bool input_mngr::handle( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
	if( !(GetForegroundWindow( ) == m_window_handle) )
		return false;

	switch( msg ) {
		// left mouse
		case WM_LBUTTONDOWN:
			m_key_pressed[ VK_LBUTTON ] = true;
			break;
		case WM_LBUTTONUP:
			m_key_pressed[ VK_LBUTTON ] = false;
			break;

		// right mouse
		case WM_RBUTTONDOWN:
			m_key_pressed[ VK_RBUTTON ] = true;
			break;
		case WM_RBUTTONUP:
			m_key_pressed[ VK_RBUTTON ] = false;
			break;

		//middle mouse
		case WM_MBUTTONDOWN:
			m_key_pressed[ VK_MBUTTON ] = true;
			break;
		case WM_MBUTTONUP:
			m_key_pressed[ VK_MBUTTON ] = false;
			break;

		// regular key
		case WM_KEYDOWN:
			m_key_pressed[ wparam ] = true;
			break;
		case WM_KEYUP:
			m_key_pressed[ wparam ] = false;
			break;

		// side mouse
		case WM_XBUTTONDOWN: {
			switch( GET_XBUTTON_WPARAM( wparam ) ) {
			case XBUTTON1:
				m_key_pressed[ VK_XBUTTON1 ] = true;
				break;
			case XBUTTON2:
				m_key_pressed[ VK_XBUTTON2 ] = true;
				break;
			default:
				break;
			}

			break;
		}
		case WM_XBUTTONUP: {
			switch( GET_XBUTTON_WPARAM( wparam ) ) {
			case XBUTTON1:
				m_key_pressed[ VK_XBUTTON1 ] = false;
				break;
			case XBUTTON2:
				m_key_pressed[ VK_XBUTTON2 ] = false;
				break;
			default:
				break;
			}

			break;
		}

		default: break;
	}

	static bool is_down = false;
	static bool is_clicked = false;

	if( m_key_pressed[ VK_INSERT ] ) {
		is_clicked = false;
		is_down = true;
	}
	else if( !m_key_pressed[ VK_INSERT ] && is_down ) {
		is_clicked = true;
		is_down = false;
	}
	else {
		is_clicked = false;
		is_down = false;
	}

	if( is_clicked )
		g_menu.is_active() = !g_menu.is_active();

	return g_menu.is_active() && ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam );
}

bool input_mngr::remove() {
	if( !m_window_handle || !m_original_wndproc )
		return false;

	SetWindowLongA( m_window_handle, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( m_original_wndproc ) );

	return true;
}

HWND input_mngr::get_window_handle( ) const {
	return m_window_handle;
}

WNDPROC input_mngr::get_original_wndproc( ) const {
	return m_original_wndproc;
}

bool input_mngr::key_pressed( int virtual_key ) {
	if( !m_window_handle )
		return false;

	return m_key_pressed.at( virtual_key );
}