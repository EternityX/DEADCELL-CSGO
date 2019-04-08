#pragma once
#include "../../csgo/sdk/custom classes/vector.h"
#include "../../csgo/sdk/custom classes/angle.h"
#include "../../csgo/sdk/custom classes/matrix.h"

namespace math {
	// pi constants.
	constexpr float pi{ 3.141592654f };
	constexpr float pi_2{ pi * 2.f };
	constexpr float pi_1divpi{ 1.f / pi };
	constexpr float pi_1div2pi{ 1.f / pi_2 };
	constexpr float pi_div2{ pi / 2.f };
	constexpr float pi_div4{ pi / 4.f };
	constexpr float rad_pi{ 57.295779513082f };

	constexpr float deg_to_rad( float val );
	constexpr float rad_to_deg( float val );

	// sse min.
	template< typename t = float >
	t min( const t &a, const t &b ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "math::min only supports integral types." );
		return (t)_mm_cvtss_f32(
			_mm_min_ss( _mm_set_ss( (float)a ),
			            _mm_set_ss( (float)b ) )
		);
	}

	// sse max.
	template< typename t = float >
	t max( const t &a, const t &b ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "math::max only supports integral types." );
		return (t)_mm_cvtss_f32(
			_mm_max_ss( _mm_set_ss( (float)a ),
			            _mm_set_ss( (float)b ) )
		);
	}

	// sse clamp.
	template< typename t = float >
	void clamp( t &val, const t &min, const t &max ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "math::clamp only supports integral types." );
		val = (t)_mm_cvtss_f32(
			_mm_min_ss( _mm_max_ss( _mm_set_ss( (float)val ), _mm_set_ss( (float)min ) ),
			            _mm_set_ss( (float)max ) )
		);
	}

	// sse sqrt.
	template< typename t = float >
	t sqrt( const t &val ) {
		// check type.
		static_assert( std::is_arithmetic< t >::value, "math::sqrt only supports integral types." );
		return _mm_cvtss_f32( _mm_sqrt_ss( _mm_set_ss( (float)val ) ) );
	}

	vec3_t vector_transform( vec3_t in, matrix3x4_t matrix );

	void vector_transform( vec3_t &in, const matrix3x4_t &matrix, vec3_t &out );
	bool clamp_angles( vec3_t & angles);
	bool normalize_angles( vec3_t & angles);
	float normalize_angle( float angle);
	float get_fov( const vec3_t & viewangle, const vec3_t & aimangle );
	void angle_matrix( const vec3_t & angles, matrix3x4_t & matrix );
	vec3_t to_angle( vec3_t vec);
	void sin_cos( float radian, float * sin, float * cos );
	void angle_to_vector( const vec3_t & angles, vec3_t & forward );
	void matrix_position( const matrix3x4_t & matrix, vec3_t & out );
	float normalize_vector( vec3_t & v );
	void angle_to_vectors( const vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up );
	void vector_angle( const vec3_t & forward, vec3_t & angles );
	vec3_t calc_angle( vec3_t v1, vec3_t v2 );
	float random_float( float min, float max );

	void vector_itransform( const vec3_t *in1, const matrix3x4_t& in2, vec3_t *out );

	// assume in2 is a rotation and rotate the input vector
	void vector_irotate( const vec3_t *in1, const matrix3x4_t& in2, vec3_t *out );

}