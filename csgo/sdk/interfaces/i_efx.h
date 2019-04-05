#pragma once

enum DlightFlags {
	DLIGHT_NO_WORLD_ILLUMINATION = 0x1,
	DLIGHT_NO_MODEL_ILLUMINATION = 0x2,
	DLIGHT_ADD_DISPLACEMENT_ALPHA = 0x4,
	DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA = 0x8,
	DLIGHT_DISPLACEMENT_MASK = ( DLIGHT_ADD_DISPLACEMENT_ALPHA | DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA ),
};

struct ColorRGBExp32 {
	unsigned char r, g, b;
	signed char exponent;
};

struct dlight_t {
	int flags;
	vec3_t origin;
	float radius;
	ColorRGBExp32 color;
	float die;
	float decay;
	float minlight;
	int key;
	int style;
	vec3_t direction;
	float innerAngle;
	float outerAngle;
};

class iv_efx {
public:
	dlight_t *alloc_dlight( int key ) {
		using original_fn = dlight_t *( __thiscall *)( void *, int );
		return util::misc::vfunc< original_fn >( this, 4 )( this, key );
	}

	dlight_t *alloc_elight( int key ) {
		using original_fn = dlight_t *( __thiscall *)( void *, int );
		return util::misc::vfunc< original_fn >( this, 5 )( this, key );
	}

	dlight_t *get_elight_by_key( int key ) {
		using original_fn = dlight_t *( __thiscall *)( void *, int );
		return util::misc::vfunc< original_fn >( this, 8 )( this, key );
	}
};
