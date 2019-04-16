#pragma once
#include "../../inc.h"

enum resolve_mode_e {
	RESOLVE_MODE_NONE,
	RESOLVE_MODE_LBYT
};

struct resolver_data_t
{
	resolver_data_t( ) { }

	resolve_mode_e m_resolve_mode;
	float m_resolved_yaw;
};

class c_resolver {
public:
	void frame_stage_notify( );
private:
	void resolve_player( c_csplayer* e );
	resolver_data_t m_resolver_data[ 65 ];
};

extern c_resolver g_resolver;