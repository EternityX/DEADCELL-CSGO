#pragma once

class IWeaponSystem {
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual const WeaponInfo_t *GetWpnData( unsigned int ItemDefinitionIndex ) = 0;
};
