#include "../inc.hpp"
#include "math.hpp"

// degrees to radians.
constexpr float math::deg_to_rad( float val ) {
	return val * ( pi / 180.f );
}

// radians to degrees.
constexpr float math::rad_to_deg( float val ) {
	return val * ( 180.f / pi );
}

vec3_t math::vector_transform( vec3_t in, matrix3x4_t matrix ) {
	return vec3_t( in.dot( matrix[ 0 ] ) + matrix[ 0 ][ 3 ], in.dot( matrix[ 1 ] ) + matrix[ 1 ][ 3 ],
		in.dot( matrix[ 2 ] ) + matrix[ 2 ][ 3 ] );
}

void math::vector_transform( vec3_t &in, const matrix3x4_t &matrix, vec3_t &out ) {
	out.x = in.dot( matrix.m_mat_val[ 0 ] ) + matrix.m_mat_val[ 0 ][ 3 ];
	out.y = in.dot( matrix.m_mat_val[ 1 ] ) + matrix.m_mat_val[ 1 ][ 3 ];
	out.z = in.dot( matrix.m_mat_val[ 2 ] ) + matrix.m_mat_val[ 2 ][ 3 ];
}

void math::vector_itransform( const vec3_t *in1, const matrix3x4_t& in2, vec3_t *out ) {
	float in1t[ 3 ];

	in1t[ 0 ] = in1->x - in2[ 0 ][ 3 ];
	in1t[ 1 ] = in1->y - in2[ 1 ][ 3 ];
	in1t[ 2 ] = in1->z - in2[ 2 ][ 3 ];

	out->x = in1t[ 0 ] * in2[ 0 ][ 0 ] + in1t[ 1 ] * in2[ 1 ][ 0 ] + in1t[ 2 ] * in2[ 2 ][ 0 ];
	out->y = in1t[ 0 ] * in2[ 0 ][ 1 ] + in1t[ 1 ] * in2[ 1 ][ 1 ] + in1t[ 2 ] * in2[ 2 ][ 1 ];
	out->z = in1t[ 0 ] * in2[ 0 ][ 2 ] + in1t[ 1 ] * in2[ 1 ][ 2 ] + in1t[ 2 ] * in2[ 2 ][ 2 ];
}

void math::vector_irotate( const vec3_t *in1, const matrix3x4_t& in2, vec3_t *out ) {
	out->x = in1->dot( in2[ 0 ] );
	out->y = in1->dot( in2[ 1 ] );
	out->z = in1->dot( in2[ 2 ] );
}

bool math::clamp_angles( vec3_t &angles ) {
	if( std::isfinite( angles.x ) && std::isfinite( angles.y ) && std::isfinite( angles.z ) ) {
		clamp( angles.x, -89.f, 89.f );
		clamp( angles.y, -180.f, 180.f );
		angles.z = 0.f;
		return true;
	}

	return false;
}

bool math::normalize_angles( vec3_t &angles ) {
	if( std::isfinite( angles.x ) && std::isfinite( angles.y ) && std::isfinite( angles.z ) ) {
		angles.x = std::remainder( angles.x, 360.f );
		angles.y = std::remainder( angles.y, 360.f );
		return true;
	}

	return false;
}

vec3_t math::to_angle( vec3_t vec ) {
	vec3_t angles;

	if( vec.x == 0.f && vec.y == 0.f ) {
		angles.x = ( vec.z > 0.f ) ? 270.f : 90.f;
		angles.y = 0.f;
	}
	else {
		angles.x = std::atan2f( -vec.z, vec.length_2d() ) * ( 180.f / pi );
		if( angles.x < 0.f )
			angles.x += 360.f;

		angles.y = std::atan2f( vec.y, vec.x ) * ( 180.f / pi );
		if( angles.y < 0.f )
			angles.y += 360.f;
	}

	angles.z = 0.0f;

	return angles;
}

void math::sin_cos( float radian, float *sin, float *cos ){
	*sin = std::sin( radian );
	*cos = std::cos( radian );
}

void math::angle_to_vector( const vec3_t &angles, vec3_t &forward ) {
	float sp, sy, cp, cy;

	sin_cos( deg_to_rad( angles.y ), &sy, &cy );
	sin_cos( deg_to_rad( angles.x ), &sp, &cp );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void math::matrix_position( const matrix3x4_t &matrix, vec3_t &out ) {
	out.x = matrix.m_mat_val[ 0 ][ 3 ];
	out.y = matrix.m_mat_val[ 1 ][ 3 ];
	out.z = matrix.m_mat_val[ 2 ][ 3 ];
}

float math::normalize_vector( vec3_t &v ) {
	float l = v.length( );
	if( l != 0.0f )
		v /= l;
	else
		v.x = v.y = 0.0f, v.z = 1.f;

	return l;
}

void math::angle_to_vectors( const vec3_t &angles, vec3_t *forward, vec3_t *right, vec3_t *up ) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos( deg_to_rad( angles.x ), &sp, &cp );
	sin_cos( deg_to_rad( angles.y ), &sy, &cy );
	sin_cos( deg_to_rad( angles.z ), &sr, &cr );

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;

	right->x = -1 * sr * sp * cy + -1 * cr * -sy;
	right->y = -1 * sr * sp * sy + -1 * cr * cy;
	right->z = -1 * sr * cp;

	up->x = cr * sp * cy + -sr * -sy;
	up->y = cr * sp * sy + -sr * cy;
	up->z = cr * cp;
}

void math::vector_angle( const vec3_t &forward, vec3_t &angles ) {
	if( forward.x == 0.f && forward.y == 0.f ) {
		angles.x = forward.z > 0.f ? -90.f : 90.f;
		angles.y = 0.f;
	}
	else {
		angles.x = rad_to_deg( std::atan2( -forward.z, forward.length_2d() ) );
		angles.y = rad_to_deg( std::atan2( forward.y, forward.x ) );
	}

	angles.z = 0.f;
}

vec3_t math::calc_angle( vec3_t v1, vec3_t v2 ) {
	vec3_t delta = v1 - v2;

	float len = delta.length();

	if ( delta.z == 0.0f && len == 0.0f )
		return vec3_t{};

	if ( delta.y == 0.0f && delta.x == 0.0f )
		return vec3_t{};

	vec3_t angles;
	angles.x = ( asinf( delta.z / delta.length() ) * rad_pi );

	angles.y = ( atanf( delta.y / delta.x ) * rad_pi );

	angles.z = 0.0f;
	if ( delta.x >= 0.0f ) { angles.y += 180.0f; }

	angles.clamp( );

	return angles;
}

float math::normalize_angle( float angle ) {
	if( angle > 180.f || angle < -180.f ) {
		auto revolutions = std::round( std::abs( angle / 360.f ) );

		if( angle < 0.f )
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}

	return angle;
}

float math::get_fov( const vec3_t &viewangle, const vec3_t &aimangle ) {
	vec3_t ang, aim;

	angle_to_vector( viewangle, aim );
	angle_to_vector( aimangle, ang );

	return ( rad_to_deg( acos( aim.dot( ang ) / aim.length_sqr( ) ) ) );
}

void math::angle_matrix( const vec3_t &angles, matrix3x4_t &matrix ) {
	float sr, sp, sy, cr, cp, cy;

	sin_cos( deg_to_rad( angles.x ), &sp, &cp );
	sin_cos( deg_to_rad( angles.y ), &sy, &cy );
	sin_cos( deg_to_rad( angles.z ), &sr, &cr );

	matrix[ 0 ][ 0 ] = cp * cy;
	matrix[ 1 ][ 0 ] = cp * sy;
	matrix[ 2 ][ 0 ] = -sp;

	auto crcy = cr * cy, crsy = cr * sy, srcy = sr * cy, srsy = sr * sy;

	matrix[ 0 ][ 1 ] = sp * srcy - crsy;
	matrix[ 1 ][ 1 ] = sp * srsy + crcy;
	matrix[ 2 ][ 1 ] = sr * cp;

	matrix[ 0 ][ 2 ] = ( sp * crcy + srsy );
	matrix[ 1 ][ 2 ] = ( sp * crsy - srcy );
	matrix[ 2 ][ 2 ] = cr * cp;

	matrix[ 0 ][ 3 ] = 0.f;
	matrix[ 1 ][ 3 ] = 0.f;
	matrix[ 2 ][ 3 ] = 0.f;
}

float math::random_float( float min, float max ) {
	static auto o_random_float = reinterpret_cast< float( *)( float, float ) >( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomFloat" ) );
	return o_random_float( min, max );
}