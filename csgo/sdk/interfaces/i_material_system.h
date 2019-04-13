#pragma once
#include "i_matrendercontext.h"

class i_material_system {
public:
	const char *get_name() {
		return util::misc::vfunc< const char *( __thiscall *)( decltype( this ) ) >( this, 0 )( this );
	}

	i_material *create_material( const char *material_name, key_values *kv ) {
		return util::misc::vfunc< i_material *( __thiscall *)( decltype( this ), const char *, key_values * ) >( this, 83 )(
			this, material_name, kv );
	}

	i_material *get_material( const char *material_name, const char *texture_group_name, bool complain = true, const char *complain_prefix = nullptr ) {
		return util::misc::vfunc< i_material *( __thiscall *)( decltype( this ), const char *, const char *, bool, const char * ) >( this, 84 )( this, material_name, texture_group_name, complain, complain_prefix );
	}

	int first_material() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 86 )( this );
	}

	int next_material( int handle ) {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ), int ) >( this, 87 )( this, handle );
	}

	int invalid_material() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 88 )( this );
	}

	i_material *find_material( int handle ) {
		return util::misc::vfunc< i_material *( __thiscall *)( decltype( this ), int ) >( this, 89 )( this, handle );
	}

	i_texture *find_texture( char const *pTextureName, const char *pTextureGroupName, bool complain = true ) {
		return util::misc::vfunc< i_texture *( __thiscall *)( decltype( this ), char const *, const char *, bool )
		>( this, 91 )( this, pTextureName, pTextureGroupName, complain );
	}

	IMatRenderContext *get_render_context() {
		return util::misc::vfunc< IMatRenderContext *( __thiscall *)( decltype( this ) ) >( this, 115 )( this );
	}
};
