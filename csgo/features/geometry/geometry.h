#pragma once
#include "../../inc.h"


class geometry {
public:
	class c_obb {
	public:
		vec3_t m_min;
		vec3_t m_max;
		matrix3x4_t m_matrix;

		c_obb( void ) {};
		c_obb( const vec3_t& min, const vec3_t& max, matrix3x4_t matrix ) { m_min = min; m_max = max; m_matrix = matrix; };
	};

	class c_sphere {
	public:
		vec3_t m_center;
		float   m_radius = 0.f;

		c_sphere( void ) {};
		c_sphere( const vec3_t& vecCenter, float flRadius ) { m_center = vecCenter; m_radius = flRadius; };

		bool intersects_ray( const ray_t& ray );
		bool intersects_ray( const ray_t& ray, vec3_t& intersection );
		bool intersect_infinite_ray_with_sphere( const vec3_t &origin, const vec3_t &delta, const vec3_t &speher_center, float radius, float *t1, float *t2 );
	};

	class c_intersection {
	public:
		void setup( const vec3_t& min, const vec3_t& max, float radius, std::vector< c_sphere >&spheres );
		bool intersect_ray_with_obb( const ray_t& ray, vec3_t& min, vec3_t& max, const matrix3x4_t &matrix );
		bool intersect_ray_with_aabb( vec3_t& origin, vec3_t& dir, vec3_t& min, vec3_t& max );
	};
};

extern geometry::c_intersection g_intersection;