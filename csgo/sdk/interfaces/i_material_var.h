#pragma once

class i_material_var {
private:
public:
	void set_float_value( float value ) {
		util::misc::vfunc< float(__thiscall *)( decltype( this ), float ) >( this, 4 )( this, value );
	}
};
