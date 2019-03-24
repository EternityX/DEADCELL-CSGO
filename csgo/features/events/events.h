#pragma once
#include "../../inc.hpp"

class c_event_listener : public IGameEventListener2 {
public:
	c_event_listener( );
	~c_event_listener( );

	void setup( );
	void remove( );

	void FireGameEvent( IGameEvent *m_event ) override;

	int GetEventDebugID( ) override {
		return 0x2A;
	};
private:
	c_event_listener *m_eventlistener = nullptr;
};

extern c_event_listener g_events;