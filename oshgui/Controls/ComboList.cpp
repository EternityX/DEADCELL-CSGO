#include "ComboList.h"

namespace OSHGui {
	constexpr int DEFAULT_MAX_ITEMS = 5;

	ComboList::ComboList() : m_max_shown_items( DEFAULT_MAX_ITEMS ), m_dropped_down( false ) {
		m_control_type = ControlType::ComboList;

		m_button = new ComboListButton();
		m_button->SetLocation( Drawing::PointI( 0, 0 ) );
		m_button->GetClickEvent() += ClickEventHandler( [ this ]( Control * )
		{
			!m_dropped_down ? Expand() : Collapse();
		} );

		m_button->GetFocusLostEvent() += FocusLostEventHandler( [ this ]( Control *, Control *new_focused_control )
		{
			if( new_focused_control == nullptr || new_focused_control->GetParent() == this || new_focused_control
			                                                                                  ->GetParent()->GetParent() == this
			)
				return;

			Collapse();
		} );

		AddSubControl( m_button );

		m_list_box = new ListBox();
		m_list_box->SetLocation( Drawing::PointI( 0, m_button->GetBottom() + 2 ) );
		m_list_box->SetSize( Drawing::SizeI( m_list_box->GetWidth(), 4 ) );
		m_list_box->SetVisible( false );
		m_list_box->ExpandSizeToShowItems( 4 );
		m_list_box->GetSelectedIndexChangedEvent() += SelectedIndexChangedEventHandler( [ this ]( Control * )
		{
			m_button->SetText( m_list_box->GetSelectedItem()->GetItemText() );
			if( m_list_box->GetVisible() ) {
				Collapse();
				m_list_box->Focus();
			}
		} );
	}

	void ComboList::Expand() {
		m_dropped_down = true;
		m_list_box->SetVisible( true );
		m_list_box->Focus();

		Invalidate();
	}

	void ComboList::Collapse() {
		m_dropped_down = false;
		m_list_box->SetVisible( false );

		Invalidate();
	}
}
