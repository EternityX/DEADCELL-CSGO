#pragma once

typedef void *light_cache_handle_t;

class iv_model_render {
public:
	virtual int draw_model( int flags, i_client_renderable *pRenderable, model_instance_handle_t instance, int entity_index,
	                       const model_t *model, vec3_t const &origin, vec3_t const &angles, int skin, int body,
	                       int hitboxset, const matrix3x4_t *modelToWorld = NULL,
	                       const matrix3x4_t *pLightingOffset = NULL ) = 0;

	virtual void forced_material_override( i_material *newMaterial, override_type_t nOverrideType = OVERRIDE_NORMAL, int nOverrides = 0 ) = 0;
	virtual bool is_forced_material_override( void ) = 0;
	virtual void set_view_target( const int *pStudioHdr, int nBodyIndex, const vec3_t &target ) = 0;
	virtual model_instance_handle_t create_instance( i_client_renderable *pRenderable, light_cache_handle_t *pCache = NULL ) = 0;
	virtual void destroy_instance( model_instance_handle_t handle ) = 0;
};
