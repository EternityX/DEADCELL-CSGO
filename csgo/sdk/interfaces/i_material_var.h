#pragma once

class i_material_var {
private:
public:
	void set_float_value( float value ) {
		util::misc::vfunc< void(__thiscall *)( decltype( this ), float ) >( this, 4 )( this, value );
	}

	void set_vec_value( float r, float g, float b ) {
		util::misc::vfunc< void(__thiscall *)( decltype( this ), float, float, float ) >( this, 11 )( this, r, g, b );
	}

	void set_string_value( char const* value ) {
		util::misc::vfunc< void(__thiscall *)( decltype( this ), char const* ) >( this, 6 )( this, value );
	}
};
