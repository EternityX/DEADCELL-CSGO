#include "chai_console.hpp"
const Drawing::SizeI chai_console::DefaultSize(200, 200);

chai_console::chai_console() {
	type_ = ControlType::Panel;

	SetSize(DefaultSize);

	ApplyStyle(Application::Instance().GetStyle());

	isFocusable_ = false;
}

void chai_console::DrawConsole(OSHGui::Drawing::Graphics &g) // dunno if this should be draw_console, but its like this to keep it like oshgui classes
{
	// set up line array
	std::vector<chai_console_text> line_array = *text_list;
	std::reverse(std::begin(line_array), std::end(line_array)); // we want the console going up from the bottom

	// set up drawing
	int max_lines = chai_console::GetSize().Height / LineSpacing;
	PointI start_point = PointI(GetLocation().X, GetLocation().Y + GetSize().Height);
	if (max_lines > line_array.size())
		max_lines = line_array.size();

	// start drawing
	for (int i = 0; i < max_lines; i++) {
		chai_console_text line = line_array.at(i);
		PointI line_location = PointI(start_point.X + 1, (start_point.Y - LineSpacing * i) - LineSpacing);

		g.FillRectangleGradient(ColorRectangle(line.back_color, line.back_color, line.back_color, line.back_color), line_location, Size(GetSize().Width - 2, LineSpacing)); // back color
		g.FillRectangleGradient(ColorRectangle(line.side_color, line.side_color, line.side_color, line.side_color), line_location, Size(3, LineSpacing)); // side line

		// draw text
		g.DrawString(line.text, GetFont(), line.fore_color, PointI(PaddingFromLeft + line_location.X, line_location.Y + 5)); 

	}
}


void chai_console::PopulateGeometry() {

	Graphics g(*geometry_);

	if (GetBackColor().GetAlpha() > 0) {
		g.FillRectangle(GetBackColor() - Color::FromARGB(0, 100, 100, 110), GetBounds());
		const auto color = GetBackColor() - Color::FromARGB(0, 90, 90, 100);
		g.FillRectangleGradient(ColorRectangle(GetBackColor(), GetBackColor(), GetBackColor(), GetBackColor()),
			RectangleF(GetLocation() + PointF(1, 1), GetSize() - SizeF(2, 2)));
	}

	DrawConsole(g);
}


void chai_console::AppendText(std::string str) {
	chai_console_text new_txt;
	new_txt.text = str;
	text_list->push_back(new_txt);
}

void chai_console::AppendText(chai_console_text cc_txt)
{
	text_list->push_back(cc_txt);
}