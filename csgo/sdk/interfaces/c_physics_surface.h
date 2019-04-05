#pragma once

class c_physics_surface {
public:
	surface_data_t *get_surface_data( int data ) {
		return util::misc::vfunc< surface_data_t *( __thiscall *)( decltype( this ), int ) >( this, 5 )( this, data );
	}
};
