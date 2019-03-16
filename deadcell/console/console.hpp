#pragma once

enum class console_colour {
	BLACK,
	DARKBLUE,
	DARKGREEN,
	DARKCYAN,
	DARKRED,
	DARKPINK,
	DARKYELLOW,
	LIGHTGREY,
	DARKGREY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PINK,
	YELLOW,
	WHITE
};

namespace console {
	void allocate( const char *window_name );
	void detach( );
	void set_text_colour( console_colour colour );
	void print( std::string str, ... );
	void error( std::string text, ... );
}
