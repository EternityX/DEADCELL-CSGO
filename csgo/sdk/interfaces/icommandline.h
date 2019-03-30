#pragma once

class ICommandLine {
private:
public:
	int ParmCount( ) {
		return util::misc::vfunc< int( __thiscall * )( decltype( this ) ) >( this, 11 )( this );
	}
	bool FindParm( const char *param ) {
		return util::misc::vfunc< bool( __thiscall * )( decltype( this ), const char * ) >( this, 11 )( this, param );
	}
};
