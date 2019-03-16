#pragma once

class OverlayText_t;

class IVDebugOverlay {
public:
	virtual void            AddEntityTextOverlay( int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char *format, ... ) = 0;
	virtual void            AddBoxOverlay( const vec3_t& origin, const vec3_t& mins, const vec3_t& max, vec3_t const& orientation, int r, int g, int b, int a, float duration ) = 0;
	virtual void            AddSphereOverlay( const vec3_t& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void            AddTriangleOverlay( const vec3_t& p1, const vec3_t& p2, const vec3_t& p3, int r, int g, int b, int a, bool noDepthTest, float duration ) = 0;
	virtual void            AddLineOverlay( const vec3_t& origin, const vec3_t& dest, int r, int g, int b, bool noDepthTest, float duration ) = 0;
	virtual void            AddTextOverlay( const vec3_t& origin, float duration, const char *format, ... ) = 0;
	virtual void            AddTextOverlay( const vec3_t& origin, int line_offset, float duration, const char *format, ... ) = 0;
	virtual void            AddScreenTextOverlay( float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char *text ) = 0;
	virtual void            AddSweptBoxOverlay( const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& max, const vec3_t & angles, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void            AddGridOverlay( const vec3_t& origin ) = 0;
	virtual void            AddCoordFrameOverlay( const matrix3x4_t& frame, float flScale, int vColorTable[ 3 ][ 3 ] = NULL ) = 0;
	virtual int             ScreenPosition( const vec3_t& point, vec3_t& screen ) = 0;
	virtual int             ScreenPosition( float flXPos, float flYPos, vec3_t& screen ) = 0;
	virtual OverlayText_t*  GetFirst( void ) = 0;
	virtual OverlayText_t*  GetNext( OverlayText_t *current ) = 0;
	virtual void            ClearDeadOverlays( void ) = 0;
	virtual void            ClearAllOverlays( void ) = 0;
	virtual void            AddTextOverlayRGB( const vec3_t& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char *format, ... ) = 0;
	virtual void            AddTextOverlayRGB( const vec3_t& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ... ) = 0;
	virtual void            AddLineOverlayAlpha( const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a, bool noDepthTest, float duration ) = 0;
	virtual void            AddBoxOverlay2( const vec3_t& origin, const vec3_t& mins, const vec3_t& max, vec3_t const& orientation, const uint8_t* faceColor, const uint8_t* edgeColor, float duration ) = 0;
	virtual void			AddLineOverlay( const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a, float, float ) = 0;
	virtual void            PurgeTextOverlays() = 0;
	VFUNC( 23, AddCapsuleOverlay( vec3_t& mins, vec3_t& maxs, float radius, int r, int g, int b, int a, float duration ), void( __thiscall* )( void*, vec3_t&, vec3_t&, float&, int, int, int, int, float ) )( mins, maxs, radius, r, g, b, a, duration )
	VFUNC( 24, AddCapsuleOverlayVisible( vec3_t& mins, vec3_t& maxs, float radius, int r, int g, int b, int a, float duration ), void( __thiscall* )( void*, vec3_t&, vec3_t&, float&, int, int, int, int, float ) )( mins, maxs, radius, r, g, b, a, duration )
};
