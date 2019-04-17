#pragma once
#include "../../inc.h"

struct notifications_t {
	notifications_t( const ImU32 &c, const ImU32 &c1, float time, std::string buf )
		: color( c ), background_color( c1 ), m_time( time ), m_buf( buf ) { }
	ImU32 color;
	ImU32 background_color;
	float m_time;
	std::string m_buf;
};

class c_notifications {
private:
	float m_text_duration = 5.f;

	std::deque< notifications_t > m_notifications;
public:
	void add( bool display, const ImU32 &color, const std::string message, ... );
	void clear( );
	void draw( );
};

extern c_notifications g_notify;