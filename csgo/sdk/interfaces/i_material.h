#pragma once
#include "i_material_var.h"

struct image_format;

class i_material {
public:
	// Get the name of the material.  This is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char *get_name() const = 0;
	virtual const char *get_texture_group_name() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual int /*PreviewImageRetVal_t*/ get_preview_image_properties( int *width, int *height,
	                                                                image_format *imageFormat,
	                                                                bool *isTranslucent ) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual int /*PreviewImageRetVal_t*/ get_preview_image( unsigned char *data,
	                                                      int width, int height,
	                                                      image_format imageFormat ) const = 0;
	// 
	virtual int get_mapping_width() = 0;
	virtual int get_mapping_height() = 0;

	virtual int get_num_animation_frames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool in_material_page( void ) = 0;
	virtual void get_material_offset( float *pOffset ) = 0;
	virtual void get_material_scale( float *pScale ) = 0;
	virtual i_material *get_material_page( void ) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual i_material_var *find_var( const char *varName, bool *found, bool complain = true ) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// i_material_system::UncacheUnusedMaterials.
	virtual void increment_reference_count( void ) = 0;
	virtual void decrement_reference_count( void ) = 0;

	inline void add_ref() {
		increment_reference_count();
	}

	inline void release() {
		decrement_reference_count();
	}

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int get_enumeration_id( void ) const = 0;

	virtual void get_low_res_color_sample( float s, float t, float *color ) const = 0;

	// This computes the state snapshots for this material
	virtual void recompute_state_snapshots() = 0;

	// Are we translucent?
	virtual bool is_translucent() = 0;

	// Are we alphatested?
	virtual bool is_alpha_tested() = 0;

	// Are we vertex lit?
	virtual bool is_vertex_lit() = 0;

	// Gets the vertex format
	virtual void get_vertex_format() const = 0;

	// returns true if this material uses a material proxy
	virtual bool has_proxy( void ) const = 0;

	virtual bool uses_env_cubemap( void ) = 0;

	virtual bool needs_tangent_space( void ) = 0;

	virtual bool needs_power_of_two_frame_buffer_texture( bool bCheckSpecificToThisFrame = true ) = 0;
	virtual bool needs_full_frame_buffer_texture( bool bCheckSpecificToThisFrame = true ) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool needs_software_skinning( void ) = 0;

	// Apply constant color or alpha modulation
	virtual void alpha_modulate( float alpha ) = 0;
	virtual void color_modulate( float r, float g, float b ) = 0;

	// Material Var flags...
	virtual void set_material_var_flag( MaterialFlags_t flag, bool on ) = 0;
	virtual bool get_material_var_flag( MaterialFlags_t flag ) const = 0;

	// Gets material reflectivity
	virtual void get_reflectivity( vec3_t &reflect ) = 0;

	// Gets material property flags
	virtual bool get_property_flag( int /*MaterialPropertyTypes_t*/ type ) = 0;

	// Is the material visible from both sides?
	virtual bool is_two_sided() = 0;

	// Sets the shader associated with the material
	virtual void set_shader( const char *pShaderName ) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int get_num_passes( void ) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int get_texture_memory_bytes( void ) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool needs_lightmap_blend_alpha( void ) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool needs_software_lighting( void ) = 0;

	// Gets at the shader parameters
	virtual int shader_param_count() const = 0;
	virtual i_material_var **get_shader_params( void ) = 0;

	// Returns true if this is the error material you get back from i_material_system::FindMaterial if
	// the material can't be found.
	virtual bool is_error_material() const = 0;

	virtual void Unused() = 0;

	// Gets the current alpha modulation
	virtual float get_alpha_modulation() = 0;
	virtual void get_color_modulation( float *r, float *g, float *b ) = 0;

	// Is this translucent given a particular alpha modulation?
	virtual bool is_translucent_under_modulation( float fAlphaModulation = 1.0f ) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual i_material_var *find_var_fast( char const *pVarName, unsigned int *pToken ) = 0;

	// Sets new VMT shader parameters for the material
	virtual void set_shader_and_params( key_values *pKeyValues ) = 0;
	virtual const char *get_shader_name() const = 0;

	virtual void delete_if_unreferenced() = 0;

	virtual bool is_sprite_card() = 0;

	virtual void call_bind_proxy( void *proxyData ) = 0;

	virtual void refresh_preserving_material_vars() = 0;

	virtual bool was_reloaded_from_whitelist() = 0;

	virtual bool set_temp_excluded( bool bSet, int nExcludedDimensionLimit ) = 0;

	virtual int get_reference_count() const = 0;
};
