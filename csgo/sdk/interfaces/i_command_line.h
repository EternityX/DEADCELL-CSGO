#pragma once

class i_command_line {
private:
public:
	int param_count( ) {
		return util::misc::vfunc< int( __thiscall * )( decltype( this ) ) >( this, 11 )( this );
	}
	bool find_param( const char *param ) {
		return util::misc::vfunc< bool( __thiscall * )( decltype( this ), const char * ) >( this, 11 )( this, param );
	}
};
