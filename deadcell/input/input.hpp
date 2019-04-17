#pragma once

class input_mngr {
private:
	HWND                    m_window_handle;
	WNDPROC                 m_original_wndproc;
	std::array< bool, 255 > m_key_pressed;

public:
	input_mngr();

	bool init( const std::string &window_name );
	bool init( HWND wnd );

	bool handle( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
	bool remove();

	HWND get_window_handle( ) const;
	WNDPROC get_original_wndproc() const;
	bool key_pressed( int virtual_key );
};

extern input_mngr g_input;