#pragma once

namespace vgui {
	typedef unsigned long HFont;
	typedef unsigned int VPANEL;
};

enum FontFeature {
	FONT_FEATURE_ANTIALIASED_FONTS = 1,
	FONT_FEATURE_DROPSHADOW_FONTS = 2,
	FONT_FEATURE_OUTLINE_FONTS = 6,
};

enum FontDrawType {
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum FontFlags {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

struct IntRect {
	int x0;
	int y0;
	int x1;
	int y1;
};

struct Vertex_t {
	Vertex_t() {}

	Vertex_t( const vec3_t &pos, const vec3_t &coord = vec3_t( 0, 0, 0 ) ) {
		m_Position = pos;
		m_TexCoord = coord;
	}

	void Init( const vec3_t &pos, const vec3_t &coord = vec3_t( 0, 0, 0 ) ) {
		m_Position = pos;
		m_TexCoord = coord;
	}

	vec3_t m_Position;
	vec3_t m_TexCoord;
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class ISurface {
public:
	void DrawSetColor( Color color ) {
		using original_fn = void( __thiscall *)( void *, Color );
		util::misc::vfunc< original_fn >( this, 14 )( this, color );
	}

	void DrawFilledRect( int x0, int y0, int x1, int y1 ) {
		using original_fn = void( __thiscall *)( void *, int, int, int, int );
		util::misc::vfunc< original_fn >( this, 16 )( this, x0, y0, x1, y1 );
	}

	void DrawLine( int x0, int y0, int x1, int y1 ) {
		using original_fn = void( __thiscall *)( void *, int, int, int, int );
		util::misc::vfunc< original_fn >( this, 19 )( this, x0, y0, x1, y1 );
	}
	void UnlockCursor(  ) {
		using original_fn = void( __thiscall *)( void * );
		util::misc::vfunc< original_fn >( this, 66 )( this );
	}
};
