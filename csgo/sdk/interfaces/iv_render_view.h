#pragma once

class iv_render_view {
public:
	void set_blend( float blend ) {
		util::misc::vfunc< void( __thiscall *)( decltype( this ), float ) >( this, 4 )( this, blend );
	}

	void set_color_modulation( float *col ) {
		util::misc::vfunc< void( __thiscall*)( void *, const float * ) >( this, 6 )( this, col );
	}
};

class i_view_render {
public:
};