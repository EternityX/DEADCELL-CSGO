/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include <locale>
#include "ListBox.hpp"
#include "ScrollBar.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"
#include "Timer.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI ListBox::DefaultSize( 120, 106 );
	const Drawing::SizeI ListBox::DefaultItemAreaPadding( 8, 8 );
	const int ListBox::DefaultItemPadding( 2 );
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ListBox::ListBox()
		: selectedIndex_( -1 ),
		  firstVisibleItemIndex_( 0 ),
		  autoScrollEnabled_( false ) {
		type_ = ControlType::ListBox;

		scrollBar_ = new ScrollBar();
		scrollBar_->SetVisible( false );
		scrollBar_->GetScrollEvent() += ScrollEventHandler( [ this ]( Control *, ScrollEventArgs &args )
		{
			firstVisibleItemIndex_ = args.NewValue;
			Invalidate();
		} );
		scrollBar_->GetFocusLostEvent() += FocusLostEventHandler( [ this ]( Control *, Control *newFocusedControl )
		{
			if( newFocusedControl != this ) {
				OnLostFocus( newFocusedControl );
			}
		} );
		AddSubControl( scrollBar_ );

		SetSize( DefaultSize );

		maxVisibleItems_ = GetHeight() / ( GetFont()->GetFontHeight() + DefaultItemPadding );

		ApplyStyle( Application::Instance().GetStyle() );
	}

	//---------------------------------------------------------------------------
	ListBox::~ListBox() {
		Clear();
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void ListBox::SetSize( const Drawing::SizeI &size ) {
		Control::SetSize( size );

		itemAreaSize_ = size.InflateEx( -8, -8 );
		if( scrollBar_->GetVisible() ) {
			itemAreaSize_.Width -= scrollBar_->GetWidth();
		}

		scrollBar_->SetLocation( size.Width - scrollBar_->GetWidth() - 1, -14 );
		scrollBar_->SetSize( scrollBar_->GetWidth(), size.Height + 28 );

		CheckForScrollBar();
	}

	//---------------------------------------------------------------------------
	void ListBox::SetFont( const Drawing::FontPtr &font ) {
		Control::SetFont( font );

		CheckForScrollBar();
	}

	//---------------------------------------------------------------------------
	void ListBox::SetAutoScrollEnabled( bool autoScrollEnabled ) {
		autoScrollEnabled_ = autoScrollEnabled;
	}

	//---------------------------------------------------------------------------
	bool ListBox::GetAutoScrollEnabled() const {
		return autoScrollEnabled_;
	}

	//---------------------------------------------------------------------------
	ListItem *ListBox::GetItem( int index ) const {
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if( index < 0 || index >= static_cast<int>( items_.size() ) )
		{
			throw Misc::ArgumentOutOfRangeException( "index" );
		}
#endif

		return items_[ index ];
	}

	//---------------------------------------------------------------------------
	void ListBox::SetSelectedIndex( int index ) {
		if( selectedIndex_ == index ) {
			return;
		}

#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if( index < 0 || index >= static_cast<int>( items_.size() ) )
		{
			throw Misc::ArgumentOutOfRangeException( "index" );
		}
#endif

		selectedIndex_ = index;

		selectedIndexChangedEvent_.Invoke( this );

		if( index - firstVisibleItemIndex_ >= maxVisibleItems_ || index - firstVisibleItemIndex_ < 0 ) {
			for( firstVisibleItemIndex_ = 0; firstVisibleItemIndex_ <= index; firstVisibleItemIndex_ += maxVisibleItems_ );
			firstVisibleItemIndex_ -= maxVisibleItems_;
			if( firstVisibleItemIndex_ < 0 ) {
				firstVisibleItemIndex_ = 0;
			}
			scrollBar_->SetValue( firstVisibleItemIndex_ );
		}

		Invalidate();
	}

	//---------------------------------------------------------------------------
	int ListBox::GetSelectedIndex() const {
		return selectedIndex_;
	}

	//---------------------------------------------------------------------------
	void ListBox::SetSelectedItem( const Misc::AnsiString &text ) {
		auto index = 0;
		for( auto &&item : items_ ) {
			if( item->GetItemText() == text ) {
				SetSelectedIndex( index );
				return;
			}
			++index;
		}
	}

	//---------------------------------------------------------------------------
	ListItem *ListBox::GetSelectedItem() const {
		return GetItem( GetSelectedIndex() );
	}

	//---------------------------------------------------------------------------
	int ListBox::GetItemsCount() const {
		return items_.size();
	}

	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent &ListBox::GetSelectedIndexChangedEvent() {
		return selectedIndexChangedEvent_;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool ListBox::Intersect( const Drawing::PointI &point ) const {
		return Intersection::TestRectangle( absoluteLocation_,
		                                    scrollBar_->GetVisible() ? size_.InflateEx( -scrollBar_->GetWidth(), 0 ) : size_,
		                                    point );
	}

	//---------------------------------------------------------------------------
	void ListBox::ExpandSizeToShowItems( int count ) {
		const auto itemHeight = GetFont()->GetFontHeight() + DefaultItemPadding;
		const auto newHeight = count * itemHeight;

		SetSize( GetWidth(), newHeight + DefaultItemAreaPadding.Height );
	}

	//---------------------------------------------------------------------------
	void ListBox::AddItem( const Misc::AnsiString &text ) {
		AddItem( new StringListItem( text ) );
	}

	//---------------------------------------------------------------------------
	void ListBox::AddItem( ListItem *item ) {
		InsertItem( !items_.empty() ? items_.size() : 0, item );
	}

	//---------------------------------------------------------------------------
	void ListBox::InsertItem( int index, const Misc::AnsiString &text ) {
		InsertItem( index, new StringListItem( text ) );
	}

	//---------------------------------------------------------------------------
	void ListBox::InsertItem( int index, ListItem *item ) {
		items_.insert( items_.begin() + index, item );

		CheckForScrollBar();

		if( autoScrollEnabled_ ) {
			scrollBar_->SetValue( index );
		}

		Invalidate();
	}

	//---------------------------------------------------------------------------
	void ListBox::RemoveItem( int index ) {
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if( index < 0 || index >= ( int ) items_.size() )
		{
			throw Misc::ArgumentOutOfRangeException( "index" );
		}
#endif

		items_.erase( items_.begin() + index );

		if( scrollBar_->GetVisible() ) {
			scrollBar_->SetMaximum( items_.size() - maxVisibleItems_ );
		}
		if( selectedIndex_ >= static_cast< int >(items_.size()) ) {
			selectedIndex_ = items_.size() - 1;

			selectedIndexChangedEvent_.Invoke( this );
		}

		Invalidate();
	}

	//---------------------------------------------------------------------------
	void ListBox::Clear() {
		items_.clear();

		scrollBar_->SetMaximum( 1 );

		selectedIndex_ = -1;

		CheckForScrollBar();

		Invalidate();
	}

	//---------------------------------------------------------------------------
	void ListBox::CheckForScrollBar() {
		const auto itemHeight = static_cast< int >(GetFont()->GetFontHeight()) + DefaultItemPadding;

		maxVisibleItems_ = std::max( 1, itemAreaSize_.Height / itemHeight );

		if( !items_.empty() && items_.size() * itemHeight > itemAreaSize_.Height ) {
			if( !scrollBar_->GetVisible() ) {
				itemAreaSize_.Width -= scrollBar_->GetWidth();
			}
			scrollBar_->SetMaximum( items_.size() - maxVisibleItems_ );
			scrollBar_->SetVisible( true );
		}
		else if( scrollBar_->GetVisible() ) {
			scrollBar_->SetVisible( false );
			itemAreaSize_.Width += scrollBar_->GetWidth();
		}
	}

	//---------------------------------------------------------------------------
	void ListBox::DrawSelf( Drawing::RenderContext &context ) {
		Control::DrawSelf( context );

		scrollBar_->Render();
	}

	//---------------------------------------------------------------------------
	void ListBox::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		g.FillRectangle( Color::FromARGB( 255, 40, 40, 50 ), PointF( 1, 1 ), GetSize() - SizeF( 2, 2 ) );

		const auto color = GetBackColor() + Color::FromARGB( 0, 54, 53, 60 );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 24 ), PointF( 0, 0 ), SizeF( GetWidth(), 1 ) );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 24 ), PointF( 0, 1 ), SizeF( 1, GetHeight() - 1 ) );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 24 ), PointF( GetWidth() - 1, 1 ), SizeF( 1, GetHeight() - 1 ) );
		g.FillRectangle( Color::FromARGB( 255, 18, 18, 24 ), PointF( 1, GetHeight() - 1 ), SizeF( GetWidth() - 2, 1 ) );

		const auto itemX = 4;
		const auto itemY = 5;
		const auto padding = GetFont()->GetFontHeight() + DefaultItemPadding;
		for( int i = 0; i < maxVisibleItems_ && i + firstVisibleItemIndex_ < static_cast<int>( items_.size() ); ++i ) {
			if( firstVisibleItemIndex_ + i == selectedIndex_ ) {
				g.FillRectangle( Color::FromARGB( 255, 54, 54, 64 ), PointF( itemX - 1, itemY + i * padding - 1 ),
				                 SizeF( itemAreaSize_.Width + 2, padding ) );
				g.DrawString( items_[ firstVisibleItemIndex_ + i ]->GetItemText(), GetFont(), Color::FromARGB( 255, 206, 115, 136 ),
				              PointF( itemX + 1, itemY + i * padding ) );
			}
			else {
				g.DrawString( items_[ firstVisibleItemIndex_ + i ]->GetItemText(), GetFont(), Color::FromARGB( 255, 201, 201, 218 ),
				              PointF( itemX + 1, itemY + i * padding ) );
			}
		}
	}

	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void ListBox::OnMouseClick( const MouseMessage &mouse ) {
		Control::OnMouseClick( mouse );

		if( Intersection::TestRectangle( absoluteLocation_.OffsetEx( 4, 4 ), itemAreaSize_, mouse.GetLocation() ) ) {
			const auto clickedIndex = firstVisibleItemIndex_ + ( mouse.GetLocation().Y - absoluteLocation_.Y - 4 ) / ( GetFont()
				->GetFontHeight() + DefaultItemPadding );
			if( clickedIndex < items_.size() ) {
				SetSelectedIndex( clickedIndex );
			}
		}
	}

	//---------------------------------------------------------------------------
	void ListBox::OnMouseScroll( const MouseMessage &mouse ) {
		Control::OnMouseScroll( mouse );

		int newScrollValue = scrollBar_->GetValue() + mouse.GetDelta();
		if( newScrollValue < 0 ) {
			newScrollValue = 0;
		}
		else if( newScrollValue > items_.size() - maxVisibleItems_ ) {
			newScrollValue = items_.size() - maxVisibleItems_;
		}
		scrollBar_->SetValue( newScrollValue );
	}

	//---------------------------------------------------------------------------
	bool ListBox::OnKeyDown( const KeyboardMessage &keyboard ) {
		if( !Control::OnKeyDown( keyboard ) ) {
			switch( keyboard.GetKeyCode() ) {
			case Key::Up:
			case Key::Down:
			case Key::Home:
			case Key::End:
			case Key::PageUp:
			case Key::PageDown: {
				int newSelectedIndex = selectedIndex_;

				switch( keyboard.GetKeyCode() ) {
				case Key::Up:
					--newSelectedIndex;
					break;
				case Key::Down:
					++newSelectedIndex;
					break;
				case Key::Home:
					newSelectedIndex = 0;
					break;
				case Key::End:
					newSelectedIndex = items_.size() - 1;
					break;
				case Key::PageUp:
					newSelectedIndex += maxVisibleItems_;
					break;
				case Key::PageDown:
					newSelectedIndex -= maxVisibleItems_;
					break;
				}

				if( newSelectedIndex < 0 ) {
					newSelectedIndex = 0;
				}
				if( newSelectedIndex >= (int)items_.size() ) {
					newSelectedIndex = items_.size() - 1;
				}

				SetSelectedIndex( newSelectedIndex );

				return true;
			}
			}
		}

		return false;
	}

	//---------------------------------------------------------------------------
	bool ListBox::OnKeyPress( const KeyboardMessage &keyboard ) {
		if( !Control::OnKeyPress( keyboard ) ) {
			if( keyboard.IsAlphaNumeric() ) {
				std::locale loc;
				const auto keyChar = std::tolower( keyboard.GetKeyChar(), loc );
				int foundIndex = 0;
				for( auto &c : items_ ) {
					const auto check = std::tolower( c->GetItemText()[ 0 ], loc );
					if( check == keyChar && foundIndex != selectedIndex_ ) {
						break;
					}

					++foundIndex;
				}

				if( foundIndex < static_cast< int >(items_.size()) ) {
					SetSelectedIndex( foundIndex );
				}
			}
		}

		return true;
	}

	//---------------------------------------------------------------------------
	// StringListItem
	//---------------------------------------------------------------------------
	StringListItem::StringListItem( const Misc::AnsiString &_text )
		: text( _text ) { }

	//---------------------------------------------------------------------------
	const Misc::AnsiString &StringListItem::GetItemText() const {
		return text;
	}

	//---------------------------------------------------------------------------
}
