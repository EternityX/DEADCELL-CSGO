#pragma once
#include "../../../inc.h"
#include "chai_wrapper_player.hpp"

class chai_wrapper_entlist
{
private:
public:
	chai_wrapper_player get_player(int index)
	{
		auto player = g_csgo.m_entity_list->get<c_csplayer>(index);
		chai_wrapper_player local = chai_wrapper_player(player);
		return local;
	}
	chai_wrapper_player get_local_player() 
	{
		chai_wrapper_player local = chai_wrapper_player(g_cl.m_local);
		return local;
	}
	int get_max_clients() {
		return g_csgo.m_global_vars->m_max_clients;
	}
};