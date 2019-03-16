#pragma once
#include "imatrendercontext.h"

class IMaterialSystem {
public:
	const char *GetName() {
		return util::misc::vfunc< const char *( __thiscall *)( decltype( this ) ) >( this, 0 )( this );
	}

	IMaterial *CreateMaterial( const char *material_name, KeyValues *key_values ) {
		return util::misc::vfunc< IMaterial *( __thiscall *)( decltype( this ), const char *, KeyValues * ) >( this, 83 )(
			this, material_name, key_values );
	}

	IMaterial *GetMaterial( const char *material_name, const char *texture_group_name, bool complain = true,
	                        const char *complain_prefix = nullptr ) {
		return util::misc::vfunc< IMaterial *( __thiscall *)( decltype( this ), const char *, const char *, bool,
		                                                      const char * ) >( this, 84 )(
			this, material_name, texture_group_name, complain, complain_prefix );
	}

	int FirstMaterial() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 86 )( this );
	}

	int NextMaterial( int handle ) {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ), int ) >( this, 87 )( this, handle );
	}

	int InvalidMaterial() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 88 )( this );
	}

	IMaterial *FindMaterial( int handle ) {
		return util::misc::vfunc< IMaterial *( __thiscall *)( decltype( this ), int ) >( this, 89 )( this, handle );
	}

	ITexture *FindTexture( char const *pTextureName, const char *pTextureGroupName, bool complain = true ) {
		return util::misc::vfunc< ITexture *( __thiscall *)( decltype( this ), char const *, const char *, bool )
		>( this, 91 )( this, pTextureName, pTextureGroupName, complain );
	}

	IMatRenderContext *GetRenderContext() {
		return util::misc::vfunc< IMatRenderContext *( __thiscall *)( decltype( this ) ) >( this, 115 )( this );
	}
};
