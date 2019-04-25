#pragma once
#include "../../../inc.h"

class chai_wrapper_ucmd
{
private:
	c_user_cmd* c_cmd;
public:
	chai_wrapper_ucmd(c_user_cmd* cmd)
	{
		c_cmd = cmd;
	}
	void set_forward_move(float f)
	{
		c_cmd->m_forwardmove = f;
	}
	void set_side_move(float f)
	{
		c_cmd->m_sidemove = f;
	}
	void set_up_move(float f)
	{
		c_cmd->m_upmove = f;
	}
	void set_buttons(int buttons)
	{
		c_cmd->m_buttons = buttons;
	}
	void set_viewangles(vec3_t va)
	{
		c_cmd->m_viewangles = va;
	}
	int get_buttons()
	{
		return c_cmd->m_buttons;
	}
	int get_tick_count()
	{
		return c_cmd->m_tick_count;
	}
	vec3_t get_viewangles()
	{
		return c_cmd->m_viewangles;
	}
	float get_forward_move()
	{
		return c_cmd->m_forwardmove;
	}
	float get_side_move()
	{
		return c_cmd->m_sidemove;
	}
	float get_up_move()
	{
		return c_cmd->m_upmove;
	}
};