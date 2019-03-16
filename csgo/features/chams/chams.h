#pragma once
#include "../../inc.hpp"

class c_chams {
	void    push_players( );
	IMaterial *create_material( bool shade, bool wireframe, bool ignorez, int rimlight_boost = 0 );
	std::vector< std::pair< C_CSPlayer *, float > > m_players;
	bool applied;
public:
	void on_sceneend( );
	bool on_dme( IMatRenderContext *ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld );
};

extern c_chams g_chams;