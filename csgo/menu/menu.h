#pragma once

class c_menu
{
public:
	c_menu();
	~c_menu() = default;

	enum e_tabs
	{
		TAB_AIMBOT,
		TAB_VISUALS,
		TAB_MISC,
		TAB_SKINS,
		TAB_DEBUG
	};

	struct tab_t
	{
		tab_t( const char* name, const char* icon, int index ) : m_name( name ), m_icon( icon ), m_index( index ) {}

		const char* m_name;
		const char* m_icon;
		int m_index;
	};

	void init( IDirect3DDevice9 * device );

	void render();

	bool& is_active();
	int& get_active_tab();
private:
	bool m_is_active;
	bool m_init;
	int m_current_tab = 0;

	IDirect3DDevice9 * m_device;
};

extern c_menu g_menu;