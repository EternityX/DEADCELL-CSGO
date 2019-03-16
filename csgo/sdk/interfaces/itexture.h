#pragma once

class ITexture {
private:
public:
	int GetActualWidth() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 3 )( this );
	}

	int GetActualHeight() {
		return util::misc::vfunc< int( __thiscall *)( decltype( this ) ) >( this, 4 )( this );
	}
};
