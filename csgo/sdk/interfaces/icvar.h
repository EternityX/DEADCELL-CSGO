#pragma once

class ConVar {
public:
	float GetFloat() {
		return util::misc::vfunc< float(__thiscall *)( decltype( this ) ) >( this, 12 )( this );
	}

	int GetInt() {
		return util::misc::vfunc< int(__thiscall *)( decltype( this ) ) >( this, 13 )( this );
	}

	void SetValue( int value ) {
		util::misc::vfunc< void(__thiscall *)( decltype( this ), int ) >( this, 16 )( this, value );
	}
};

class ICVar {
public:
	ConVar *FindVar( const char *var ) {
		return util::misc::vfunc< ConVar *(__thiscall *)( decltype( this ), const char * ) >( this, 15 )( this, var );
	}

	template< typename ... t >
	void PrintToConsole( const Color &clr, const char *format, t ... args ) {
		util::misc::vfunc< void(__cdecl *)( decltype( this ), const Color &, const char *, ... ) >( this, 25 )(
			this, clr, format, args... );
	}
};
