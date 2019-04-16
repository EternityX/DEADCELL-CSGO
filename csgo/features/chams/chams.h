#pragma once
#include "../../inc.h"

enum shader_type_t {
	VertexLitGeneric,
	UnlitGeneric,
	Modulate
};

class c_chams {
private:
	std::pair< i_material *, i_material * > m_shaded_mat;
	std::pair< i_material *, i_material * > m_flat_mat;
	std::pair< i_material *, i_material * > m_modulate_mat;

	std::vector< std::pair< c_csplayer *, float > > m_players;

	void push_players( );
	i_material *create_material( shader_type_t shade, bool ignorez, bool wireframe = false );

public:
	bool m_kv_needs_update = false;

	void on_sceneend( );
	bool on_dme( uintptr_t ecx, IMatRenderContext *ctx, void *state, model_render_info_t &pInfo, matrix3x4_t *pCustomBoneToWorld, hook::fn::DrawModelExecute_t orig );
};

extern c_chams g_chams;