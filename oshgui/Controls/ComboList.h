#pragma once

#ifndef OSHGUI_COMBOLIST_HPP
#define OSHGUI_COMBOLIST_HPP

#include "Control.hpp"
#include "Button.hpp"
#include "ListBox.hpp"

namespace OSHGui {
	class OSHGUI_EXPORT ComboList : public Control {
	private:
		int m_max_shown_items;
		bool m_dropped_down;
		std::vector< ListItem * > m_items;

		class ComboListButton : public Button {
		public:
			using Button::SetSize;

			void SetSize( const Drawing::SizeI &size ) override;
			bool Intersect( const Drawing::PointI &point ) const override;

		protected:
			void CalculateLabelLocation() override;

			void DrawSelf( Drawing::RenderContext &context ) override;
			void PopulateGeometry() override;

			bool OnKeyDown( const KeyboardMessage &keyboard ) override;

		private:
			Drawing::SizeI m_real_size;
			Drawing::PointI m_arrow_absolute_loc;
		};

		ComboListButton *m_button;
		ListBox *m_list_box;

	protected:
		ControlType m_control_type;

	public:
		ComboList();
		void Expand();
		void Collapse();
	};
}

#endif
