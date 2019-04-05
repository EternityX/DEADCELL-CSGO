#pragma once

class i_panel {
public:
	const char *get_name( int vguiPanel ) {
		return util::misc::vfunc< const char *(__thiscall *)( decltype( this ), int ) >( this, 36 )( this, vguiPanel );
	}
};
