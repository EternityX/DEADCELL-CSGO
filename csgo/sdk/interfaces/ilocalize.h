#pragma once

class ILocalize {
public:
	wchar_t *find( const char *name ) {
		return util::misc::vfunc< wchar_t *( __thiscall *)( void *, const char * ) >( this, 11 )( this, name );
	}
};
