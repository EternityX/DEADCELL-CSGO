#pragma once

class input_mngr : public OSHGui::Input::Input {
private:
	HWND                    m_window_handle;
	WNDPROC                 m_original_wndproc;
	std::array< bool, 256 > m_key_pressed;
	OSHGui::Application     *m_instance;

public:
	input_mngr();

	bool init( const std::string &window_name, OSHGui::Application *instance );
	bool init( HWND wnd );

	bool handle( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
	bool remove();
	bool process_message( LPMSG msg, WPARAM wparam, LPARAM lparam );

	WNDPROC get_original_wndproc() const;
	bool key_pressed( int virtual_key );
};

extern input_mngr g_input;