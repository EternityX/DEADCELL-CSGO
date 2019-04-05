#pragma once

class i_texture {
private:
public:
	int get_actual_width() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 3 )( this );
	}

	int get_actual_height() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 4 )( this );
	}
};
