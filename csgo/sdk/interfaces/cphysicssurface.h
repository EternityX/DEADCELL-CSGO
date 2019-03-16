#pragma once

class CPhysicsSurface {
public:
	SurfaceData_t *GetSurfaceData( int data ) {
		return util::misc::vfunc< SurfaceData_t *( __thiscall *)( decltype( this ), int ) >( this, 5 )( this, data );
	}
};
