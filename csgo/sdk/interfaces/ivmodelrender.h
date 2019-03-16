#pragma once

typedef void *LightCacheHandle_t;

class IVModelRender {
public:
	virtual int DrawModel( int flags, IClientRenderable *pRenderable, ModelInstanceHandle_t instance, int entity_index,
	                       const model_t *model, vec3_t const &origin, vec3_t const &angles, int skin, int body,
	                       int hitboxset, const matrix3x4_t *modelToWorld = NULL,
	                       const matrix3x4_t *pLightingOffset = NULL ) = 0;
	virtual void ForcedMaterialOverride( IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL,
	                                     int nOverrides = 0 ) = 0;
	virtual bool IsForcedMaterialOverride( void ) = 0;
	virtual void SetViewTarget( const int *pStudioHdr, int nBodyIndex, const vec3_t &target ) = 0;
	virtual ModelInstanceHandle_t CreateInstance( IClientRenderable *pRenderable, LightCacheHandle_t *pCache = NULL ) = 0;
	virtual void DestroyInstance( ModelInstanceHandle_t handle ) = 0;
};
