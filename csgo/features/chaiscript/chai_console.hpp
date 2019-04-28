#pragma once
#include "../../../oshgui/Controls/Control.hpp"
#include <sstream>

using namespace OSHGui;
using namespace Drawing;

class chai_console_text {
public:
	std::string text = "";
	OSHGui::Drawing::Color back_color = OSHGui::Drawing::Color::FromARGB(50, 255, 255, 255); // default color
	OSHGui::Drawing::Color side_color = OSHGui::Drawing::Color::FromARGB(0, 255, 255, 255); // side thing to indicate error or other
	OSHGui::Drawing::Color fore_color = OSHGui::Drawing::Color::FromARGB(255, 255, 255, 255); // text color
};

class chai_console : public OSHGui::Control {
public:
	chai_console();
	 
	std::vector<chai_console_text> *text_list = new std::vector<chai_console_text>();
	void AppendText(std::string str);
	void AppendText(chai_console_text cstxt);
protected:
	virtual void PopulateGeometry() override;

private:
	static const OSHGui::Drawing::SizeI DefaultSize;
	void DrawConsole(OSHGui::Drawing::Graphics &g);
	int LineSpacing = 20;
	int PaddingFromLeft = 10;
};

