#pragma once
#pragma once
#include "../../../inc.h"

class chai_wrapper_renderer
{
public:
	c_renderer* renderer;
	chai_wrapper_renderer(c_renderer* renderer_var)
	{
		renderer = renderer_var;
	}
	void ansi_text(int font, OSHGui::Drawing::Color color, OSHGui::Drawing::Color shadow_color, int x, int y, int flags, const std::string str)
	{
		renderer->ansi_text(g_renderer.m_fonts.at(font), color, shadow_color, x, y, flags, str);
	}
	void rect(OSHGui::Drawing::Color color, int x, int y, int w, int h)
	{
		renderer->rect(color, x, y, w, h);
	}
	void circle(OSHGui::Drawing::Color color, int x, int y, int radius)
	{
		renderer->circle(color, x, y, radius);
	}
	void filled_rect(OSHGui::Drawing::Color color, int x, int y, int w, int h )
	{
		renderer->filled_rect(color, x, y, w, h);
	}
	void line(OSHGui::Drawing::Color color, float x, float y, float x1, float y1) 
	{
		renderer->line(color, x, y, x1, y1);
	}
};