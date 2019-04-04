#include "fakelag.h"

void c_fakelag::think(CUserCmd* cmd) {
	g_cl.m_sendpacket = true;

	static int choke = 0;
	auto local = C_CSPlayer::get_local();
	if (g_vars.misc.fakelag.enabled && !g_csgo.m_engine->IsVoiceRecording() && !(cmd->m_buttons & IN_ATTACK)) {
			switch (g_vars.misc.fakelag.type) {
			case 0: { // Maximum
				choke = g_vars.misc.fakelag.amount;
				break;
			}
			case 1: { // Adaptive
				choke = std::min< int >(static_cast<int>(std::ceilf(64 / (g_cl.m_local->velocity().Length() * g_csgo.m_global_vars->m_interval_per_tick))), g_vars.misc.fakelag.amount);
				break;
			}
			default:
				choke = 1;
				break;
		}
	}
	else {
		choke = 1;
	}
	if (choke > static_cast<int>(g_csgo.m_clientstate->m_nChokedCommands))
		g_cl.m_sendpacket = false;
}

c_fakelag g_fakelag;
