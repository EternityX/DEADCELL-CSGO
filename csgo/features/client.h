#pragma once

class c_client {
private:
public:
	c_user_cmd     *m_cmd = nullptr;
	c_csplayer   *m_local = nullptr;
	cvar       *c4_timer = nullptr;
	void         *m_context = nullptr;

	vec3_t       m_last_sent_origin;
	int          m_predicted_flags;
	int          m_ping;
	int          m_rtt;
	int          m_ur;
	bool         m_should_update_materials = false;
	int          m_server_update_rate;
	bool         m_under_tickrate;
	int          m_client_framerate;
	float        m_hitmarker_alpha;
	int          m_pen_crosshair = 0;

	bool m_sendpacket;
};

extern c_client g_cl;