#include "geometry.h"

geometry::c_intersection g_intersection;

void geometry::c_intersection::setup( const vec3_t & min, const vec3_t & max, float radius, std::vector< c_sphere >& spheres ) {
	vec3_t delta = ( max - min ).normalized( );

	spheres.push_back( c_sphere { min, radius } );

	for ( size_t i = 1; i < std::floor( min.distance( max ) ); ++i ) {
		spheres.push_back( c_sphere { min + delta * static_cast< float >( i ), radius } );
	}

	spheres.push_back( c_sphere { max, radius } );
}

bool geometry::c_intersection::intersect_ray_with_obb( const ray_t & ray, vec3_t & min, vec3_t & max, const matrix3x4_t &matrix ) {
	vec3_t ray_trans, dir_trans;
	math::vector_itransform( &ray.m_start, matrix, &ray_trans );
	math::vector_irotate( &ray.m_delta, matrix, &dir_trans );

	return intersect_ray_with_aabb( ray_trans, dir_trans, min, max );
}

bool geometry::c_intersection::intersect_ray_with_aabb( vec3_t & origin, vec3_t & dir, vec3_t & min, vec3_t & max ) {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	if ( dir.x >= 0 ) {
		tmin = ( min.x - origin.x ) / dir.x;
		tmax = ( max.x - origin.x ) / dir.x;
	}
	else {
		tmin = ( max.x - origin.x ) / dir.x;
		tmax = ( min.x - origin.x ) / dir.x;
	}

	if ( dir.y >= 0 ) {
		tymin = ( min.y - origin.y ) / dir.y;
		tymax = ( max.y - origin.y ) / dir.y;
	}
	else {
		tymin = ( max.y - origin.y ) / dir.y;
		tymax = ( min.y - origin.y ) / dir.y;
	}

	if ( tmin > tymax || tymin > tmax )
		return false;

	if ( tymin > tmin )
		tmin = tymin;

	if ( tymax < tmax )
		tmax = tymax;

	if ( dir.z >= 0 ) {
		tzmin = ( min.z - origin.z ) / dir.z;
		tzmax = ( max.z - origin.z ) / dir.z;
	}
	else {
		tzmin = ( max.z - origin.z ) / dir.z;
		tzmax = ( min.z - origin.z ) / dir.z;
	}

	if ( tmin > tzmax || tzmin > tmax )
		return false;

	//behind us
	if ( tmin < 0 || tmax < 0 )
		return false;

	return true;
}

bool geometry::c_sphere::intersects_ray( const ray_t & ray ) {
	float t1, t2;
	if ( !intersect_infinite_ray_with_sphere( ray.m_start, ray.m_delta, m_center, m_radius, &t1, &t2 ) )
		return false;

	return t2 >= 0.0f && t1 <= 1.0f;
}

bool geometry::c_sphere::intersects_ray( const ray_t & ray, vec3_t & intersection ) {
	float t1, t2;
	if ( !intersect_infinite_ray_with_sphere( ray.m_start, ray.m_delta, m_center, m_radius, &t1, &t2 ) )
		return false;

	if ( t1 > 1.0f || t2 < 0.0f )
		return false;

	// Clamp it!
	if ( t1 < 0.0f )
		t1 = 0.0f;
	if ( t2 > 1.0f )
		t2 = 1.0f;

	intersection = ray.m_start + ray.m_delta * t1;

	return true;
}

bool geometry::c_sphere::intersect_infinite_ray_with_sphere( const vec3_t & origin, const vec3_t & delta, const vec3_t & sphere_center, float radius, float * t1, float * t2 ) {
	vec3_t sphere_to_ray = origin - sphere_center;

	float a = delta.dot( delta );

	if ( a == 0.0f ) {
		*t1 = *t2 = 0.0f;
		return sphere_to_ray.length_sqr( ) <= radius * radius;
	}

	float b = 2 * sphere_to_ray.dot( delta );
	float c = sphere_to_ray.dot( sphere_to_ray ) - radius * radius;
	float discrim = b * b - 4 * a * c;
	if ( discrim < 0.0f )
		return false;

	discrim = sqrt( discrim );
	float oo2a = 0.5f / a;
	*t1 = ( -b - discrim ) * oo2a;
	*t2 = ( -b + discrim ) * oo2a;
	return true;
}