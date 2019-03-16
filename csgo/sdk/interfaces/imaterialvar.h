#pragma once

class IMaterialVar {
private:
public:
	void SetFloatValue( float value ) {
		util::misc::vfunc< float(__thiscall *)( decltype( this ), float ) >( this, 4 )( this, value );
	}
};
