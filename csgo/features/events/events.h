#pragma once
#include "../../inc.h"

class c_event_listener : public i_game_event_listener {
public:
	c_event_listener( );
	~c_event_listener( );

	bool setup( );
	bool remove( );

	void fire_game_event( i_game_event *m_event ) override;

	int get_event_debug_id( ) override {
		return 0x2A;
	};
private:
	c_event_listener *m_eventlistener = nullptr;
};

extern c_event_listener g_events;