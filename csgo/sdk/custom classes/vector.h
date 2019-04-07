#pragma once

class vec3_t
{
public:
	vec3_t( void )
	{
		invalidate( );
	}

	vec3_t( float X, float Y, float Z )
	{
		x = X;
		y = Y;
		z = Z;
	}

	vec3_t( const float* clr )
	{
		x = clr [ 0 ];
		y = clr [ 1 ];
		z = clr [ 2 ];
	}

	void init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f )
	{
		x = ix; y = iy; z = iz;
	}

	bool is_valid( ) const
	{
		return std::isfinite( x ) && std::isfinite( y ) && std::isfinite( z );
	}

	void invalidate( )
	{
		x = y = z = std::numeric_limits< float >::infinity( );
	}

	float& operator[]( int i )
	{
		return ( ( float* ) this ) [ i ];
	}

	float operator[]( int i ) const
	{
		return ( ( float* ) this ) [ i ];
	}

	void zero( )
	{
		x = y = z = 0.0f;
	}

	bool operator==( const vec3_t& src ) const
	{
		return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
	}

	bool operator!=( const vec3_t& src ) const
	{
		return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
	}

	vec3_t& operator+=( const vec3_t& v )
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	vec3_t& operator-=( const vec3_t& v )
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	vec3_t& operator*=( float fl )
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	vec3_t& operator*=( const vec3_t& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	vec3_t& operator/=( const vec3_t& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	vec3_t& operator+=( float fl )
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	vec3_t& operator/=( float fl )
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	vec3_t& operator-=( float fl )
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	void normalize( ) {
		*this /= length( );
	}

	float distance( const vec3_t &vOther ) const
	{
		vec3_t delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.length( );
	}

	vec3_t normalized( ) const {
		vec3_t vec = *this;
		vec.normalize();
		return vec;
	}

	float dist_to_sqr( const vec3_t &vOther ) const
	{
		vec3_t delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.length_sqr( );
	}

	float dot( const vec3_t &vOther ) const
	{
		return ( x * vOther.x + y * vOther.y + z * vOther.z );
	}

	float length( ) const
	{
		return sqrt( x * x + y * y + z * z );
	}

	float length_sqr( void ) const
	{
		return ( x * x + y * y + z * z );
	}

	float Length2D( ) const
	{
		return sqrt( x * x + y * y );
	}

	vec3_t& operator=( const vec3_t &vOther )
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	vec3_t operator-( void ) const
	{
		return vec3_t( -x, -y, -z );
	}

	vec3_t operator+( const vec3_t& v ) const
	{
		return vec3_t( x + v.x, y + v.y, z + v.z );
	}

	vec3_t operator-( const vec3_t& v ) const
	{
		return vec3_t( x - v.x, y - v.y, z - v.z );
	}

	vec3_t operator*( float fl ) const
	{
		return vec3_t( x * fl, y * fl, z * fl );
	}

	vec3_t operator*( const vec3_t& v ) const
	{
		return vec3_t( x * v.x, y * v.y, z * v.z );
	}

	vec3_t operator/( float fl ) const
	{
		return vec3_t( x / fl, y / fl, z / fl );
	}

	vec3_t operator/( const vec3_t& v ) const
	{
		return vec3_t( x / v.x, y / v.y, z / v.z );
	}
	inline float normalize_l()
	{
		vec3_t res = *this;
		float length = res.length();
		if ( length != 0.0f )
			res /= length;
		else
			res.x = res.y = res.z = 0.0f;
		return length;
	}
	inline vec3_t clamp()
	{
		if ( this->x < -89.0f )
			this->x = -89.0f;

		if ( this->x > 89.0f )
			this->x = 89.0f;

		while ( this->y < -180.0f )
			this->y += 360.0f;

		while ( this->y > 180.0f )
			this->y -= 360.0f;

		this->z = 0.0f;

		return *this;
	}

	float x, y, z;
};

class __declspec( align( 16 ) ) vector_aligned : public vec3_t
{
public:
	inline vector_aligned( void )
	{ };

	inline vector_aligned( float X, float Y, float Z )
	{
		init( X, Y, Z );
	}

public:
	explicit vector_aligned( const vec3_t &vOther )
	{
		init( vOther.x, vOther.y, vOther.z );
	}

	vector_aligned& operator=( const vec3_t &vOther )
	{
		init( vOther.x, vOther.y, vOther.z );
		return *this;
	}

	vector_aligned& operator=( const vector_aligned &vOther )
	{
		init( vOther.x, vOther.y, vOther.z );
		return *this;
	}

	float w;
};