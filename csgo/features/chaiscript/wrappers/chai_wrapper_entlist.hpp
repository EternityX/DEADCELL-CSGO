#pragma once
#include "../../../inc.h"
#include "chai_wrapper_player.hpp"

class chai_wrapper_entlist
{
private:
public:
	i_client_entity_list* list;
	chai_wrapper_entlist(i_client_entity_list* list_var) {
		list = list_var;
	}
	chai_wrapper_player get_player(int index)
	{
		c_csplayer* player = g_csgo.m_entity_list->get<c_csplayer>(index);
		if (player)
			return chai_wrapper_player(g_csgo.m_entity_list->get<c_csplayer>(index));
		else
			return false;
	}

	bool is_valid(int index) {
		c_csplayer* player = g_csgo.m_entity_list->get<c_csplayer>(index);
		if (player)
			return true;
		else
			return false;
	}

	chai_wrapper_player get_local_player() 
	{
		return chai_wrapper_player(g_cl.m_local);
	}
	int get_max_clients() {
		return g_csgo.m_global_vars->m_max_clients;
	}
};