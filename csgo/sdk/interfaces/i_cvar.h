#pragma once

class cvar {
public:
	float get_float() {
		return util::misc::vfunc< float(__thiscall *)( decltype( this ) ) >( this, 12 )( this );
	}

	int get_int() {
		return util::misc::vfunc< int(__thiscall *)( decltype( this ) ) >( this, 13 )( this );
	}

	void set_value( int value ) {
		util::misc::vfunc< void(__thiscall *)( decltype( this ), int ) >( this, 16 )( this, value );
	}
};

class i_cvar {
public:
	cvar *find_var( const char *var ) {
		return util::misc::vfunc< cvar *(__thiscall *)( decltype( this ), const char * ) >( this, 15 )( this, var );
	}

	template< typename ... t >
	void print_to_console( const Color &clr, const char *format, t ... args ) {
		util::misc::vfunc< void(__cdecl *)( decltype( this ), const Color &, const char *, ... ) >( this, 25 )(
			this, clr, format, args... );
	}
};
