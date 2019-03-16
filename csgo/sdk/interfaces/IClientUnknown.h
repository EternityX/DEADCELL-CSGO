#pragma once

class CBaseHandle;

class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle( const CBaseHandle &handle ) = 0;
	virtual const CBaseHandle &GetRefEHandle() const = 0;
};

class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class IClientThinkable;
class IClientAlphaProperty;

class ICollideable {
public:
	virtual void pad0();
	virtual vec3_t &OBBMins() const;
	virtual vec3_t &OBBMaxs() const;
};

class IClientUnknown : public IHandleEntity {
public:
	virtual ICollideable *get_collideable() = 0;
	virtual IClientNetworkable *get_client_networkable() = 0;
	virtual IClientRenderable *get_client_renderable() = 0;
	virtual IClientEntity *GetIClientEntity() = 0;
	virtual C_BaseEntity *GetBaseEntity() = 0;
	virtual IClientThinkable *GetClientThinkable() = 0;
	// virtual IClientModelRenderable  *GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty *GetClientAlphaProperty() = 0;
};
