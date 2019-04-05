#pragma once

class c_base_handle;

class i_handle_entity {
public:
	virtual ~i_handle_entity() {}
	virtual void set_handle( const c_base_handle &handle ) = 0;
	virtual const c_base_handle &get_handle() const = 0;
};

class i_collideable;
class i_client_networkable;
class i_client_renderable;
class i_client_entity;
class c_base_entity;
class i_client_thinkable;
class i_client_alpha_property;

class i_collideable {
public:
	virtual void pad0();
	virtual vec3_t &mins() const;
	virtual vec3_t &maxs() const;
};

class i_client_unknown : public i_handle_entity {
public:
	virtual i_collideable *get_collideable() = 0;
	virtual i_client_networkable *get_client_networkable() = 0;
	virtual i_client_renderable *get_client_renderable() = 0;
	virtual i_client_entity *get_client_entity() = 0;
	virtual c_base_entity *get_base_entity() = 0;
	virtual i_client_thinkable *get_client_thinkable() = 0;
	// virtual IClientModelRenderable  *GetClientModelRenderable() = 0;
	virtual i_client_alpha_property *get_client_alpha_property() = 0;
};
