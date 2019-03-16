#pragma once

class c_client {
private:
public:
	CUserCmd     *m_cmd = nullptr;
	C_CSPlayer   *m_local = nullptr;
	ConVar       *c4_timer = nullptr;
	void         *m_context = nullptr;

	vec3_t       m_last_sent_origin;
	int          m_predicted_flags;
	int          m_ping;
	int          m_rtt;
	int          m_ur;
	bool         m_should_update_materials = false;
	int          m_server_update_rate;
	bool         m_under_server_tick_rate;
	float        m_hitmarker_alpha;
	int          m_pen_crosshair = 0;

	bool m_sendpacket;
};

extern c_client g_cl;