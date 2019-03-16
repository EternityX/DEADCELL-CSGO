#pragma once

#include "Control.hpp"
#include "../Drawing/Image.hpp"
#include "ColorPicker.hpp"

namespace OSHGui {
	class ColorPicker;

	typedef Event< void( Control *, const Drawing::Color &color ) > ColorChangedEvent;
	typedef EventHandler< void( Control *, const Drawing::Color &color ) > ColorChangedEventHandler;

	class OSHGUI_EXPORT ColorButton : public Control {
	public:
		ColorButton();
		Drawing::Color GetColor();
		void SetColor( Drawing::Color color );
		void Expand();
		void Collapse();
		void OnMouseDown( const MouseMessage &mouse );
		bool Intersect( const Drawing::PointI &point ) const;
		void HandleMouseEvent( const MouseMessage &mouse );
		void DrawSelf( Drawing::RenderContext &context );
		void PopulateGeometry();
		ColorChangedEvent &GetColorChangedEvent();

	private:
		bool droppedDown_;
		Drawing::Color color_;
		ColorPicker *picker;
		ColorChangedEvent colorChangedEvent_;
	};
}
