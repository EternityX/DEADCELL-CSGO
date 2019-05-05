/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include "TabControl.hpp"
#include "TabPage.hpp"
#include "Label.hpp"
#include "../Misc/TextHelper.hpp"
#include "../Misc/Exceptions.hpp"
#include <algorithm>
#include "Timer.hpp"

namespace OSHGui {
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeI TabControl::DefaultSize( 500, 500 );

	int button_width;

	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TabControl::TabControl()
		: startIndex_( 0 ),
		  maxIndex_( 0 ),
		  selected_( nullptr ) {
		type_ = ControlType::TabControl;

		lastSwitchButton_ = new TabControlSwitchButton( TabControlSwitchButton::TabControlSwitchButtonDirection::Left );
		lastSwitchButton_->GetClickEvent() += ClickEventHandler( [ this ]( Control *control )
		{
			if( startIndex_ > 0 ) {
				--startIndex_;
				CalculateButtonLocationAndCount();
			}
		} );
		AddSubControl( lastSwitchButton_ );

		nextSwitchButton_ = new TabControlSwitchButton( TabControlSwitchButton::TabControlSwitchButtonDirection::Right );
		nextSwitchButton_->GetClickEvent() += ClickEventHandler( [ this ]( Control *control )
		{
			if( maxIndex_ < (int)bindings_.size() ) {
				++startIndex_;
				CalculateButtonLocationAndCount();
			}
		} );
		AddSubControl( nextSwitchButton_ );

		SetSize( DefaultSize );

		ApplyStyle( Application::Instance().GetStyle() );

		canRaiseEvents_ = false;
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void TabControl::SetSize( const Drawing::SizeI &size ) {
		Control::SetSize( size );

		CalculateButtonLocationAndCount();

		lastSwitchButton_->SetLocation( GetWidth() - TabControlSwitchButton::DefaultSize.Width, 0 );
		nextSwitchButton_->SetLocation( GetWidth() - TabControlSwitchButton::DefaultSize.Width,
		                                TabControlSwitchButton::DefaultSize.Height + 1 );

		if( selected_ != nullptr ) {
			const auto tabPageSize = size.InflateEx( 0, -selected_->Button->GetHeight() );

			for( auto &binding : bindings_ ) {
				binding->TabPage->SetSize( tabPageSize );
			}
		}
	}

	//---------------------------------------------------------------------------
	void TabControl::SetForeColor( const Drawing::Color &color ) {
		Control::SetForeColor( color );

		lastSwitchButton_->SetForeColor( color );
		nextSwitchButton_->SetForeColor( color );

		for( auto &binding : bindings_ ) {
			binding->Button->SetForeColor( color );
			binding->TabPage->SetForeColor( color );
		}
	}

	//---------------------------------------------------------------------------
	void TabControl::SetBackColor( const Drawing::Color &color ) {
		Control::SetBackColor( color );

		/*lastSwitchButton_->SetBackColor( color );
		nextSwitchButton_->SetBackColor( color );

		for( auto &binding : bindings_ )
		{
			binding->Button->SetBackColor( color );
			binding->TabPage->SetBackColor( color );
		}*/
	}

	//---------------------------------------------------------------------------
	TabPage *TabControl::GetTabPage( const Misc::AnsiString &text ) const {
		for( auto &binding : bindings_ ) {
			if( binding->TabPage->GetText() == text ) {
				return binding->TabPage;
			}
		}

		return 0;
	}

	//---------------------------------------------------------------------------
	TabPage *TabControl::GetTabPage( int index ) const {
		if( index > 0 && index < static_cast< int >(bindings_.size()) ) {
			return bindings_[ index ]->TabPage;
		}

		return nullptr;
	}

	//---------------------------------------------------------------------------
	void TabControl::SetSelectedIndex( int index ) {
		for( auto &binding : bindings_ ) {
			if( binding->Index == index ) {
				SelectBinding( *binding );

				return;
			}
		}
	}

	//---------------------------------------------------------------------------
	int TabControl::GetSelectedIndex() const {
		return selected_->Index;
	}

	//---------------------------------------------------------------------------
	void TabControl::SetButtonWidth( int width ) {
		button_width = width;
		buttonWidth_ = width;
	}

	//---------------------------------------------------------------------------
	void TabControl::SetSelectedTabPage( TabPage *tabPage ) {
		for( auto &binding : bindings_ ) {
			if( binding->TabPage == tabPage ) {
				SelectBinding( *binding );

				return;
			}
		}
	}

	//---------------------------------------------------------------------------
	TabPage *TabControl::GetSelectedTabPage() const {
		return selected_->TabPage;
	}

	//---------------------------------------------------------------------------
	SelectedIndexChangedEvent &TabControl::GetSelectedIndexChangedEvent() {
		return selectedIndexChangedEvent_;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void TabControl::AddTabPage( TabPage *tabPage ) {
		if( tabPage == nullptr ) {
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException( "tabPage" );
#endif
			return;
		}

		for( auto &binding : bindings_ ) {
			if( binding->TabPage == tabPage ) {
				return;
			}
		}

		auto binding = std::make_unique< TabPageButtonBinding >();
		binding->Index = bindings_.size();
		binding->TabPage = tabPage;

		auto button = new TabControlButton( *binding );
		button->SetLocation( Drawing::PointI( 0, 0 ) );
		button->SetSize( Drawing::SizeF( buttonWidth_, 25 ) );
		button->SetForeColor( GetForeColor() );
		button->SetBackColor( GetBackColor() );

		AddSubControl( button );

		tabPage->SetSize( size_.InflateEx( 0, -button->GetHeight() ) );

		AddSubControl( tabPage );

		tabPage->button_ = button;
		binding->Button = button;

		if( bindings_.empty() ) {
			button->SetActive( true );
			tabPage->SetVisible( true );
			selected_ = binding.get();
			tabPage->SetLocation( 0, button->GetSize().Height );
		}
		else {
			tabPage->SetVisible( false );
		}

		bindings_.push_back( std::move( binding ) );

		CalculateButtonLocationAndCount();
	}

	//---------------------------------------------------------------------------
	void TabControl::RemoveTabPage( TabPage *tabPage ) {
		if( tabPage == nullptr ) {
#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException( "tabPage" );
#endif
			return;
		}

		for( auto &binding : bindings_ ) {
			if( binding->TabPage == tabPage ) {
				RemoveControl( binding->Button );
				delete binding->Button;
				binding->TabPage->button_ = nullptr;
				RemoveControl( binding->TabPage );

				bindings_.erase( std::remove( std::begin( bindings_ ), std::end( bindings_ ), binding ), std::end( bindings_ ) );

				if( selected_->TabPage == tabPage ) {
					if( !bindings_.empty() ) {
						selected_ = bindings_.front().get();
						selected_->Button->SetActive( true );
					}
					else {
						selected_->Index = -1;
						selected_->TabPage = nullptr;
						selected_->Button = nullptr;
					}
				}

				break;
			}
		}

		CalculateButtonLocationAndCount();
	}

	//---------------------------------------------------------------------------
	void TabControl::CalculateAbsoluteLocation() {
		Control::CalculateAbsoluteLocation();

		CalculateButtonLocationAndCount();
	}

	//---------------------------------------------------------------------------
	void TabControl::CalculateButtonLocationAndCount() {
		if( !bindings_.empty() ) {
			maxIndex_ = startIndex_;

			auto tempWidth = 0;
			const auto maxWidth = size_.Width - TabControlSwitchButton::DefaultSize.Width;
			for( auto &binding : bindings_ ) {
				auto button = binding->Button;
				button->SetVisible( false );

				if( tempWidth + button->GetSize().Width <= 900 ) {
					button->SetLocation( tempWidth, 0 );
					button->SetVisible( true );

					++maxIndex_;
					tempWidth += button->GetSize().Width - 1;
				}
				else {
					break;
				}
			}

			if( selected_ != nullptr ) {
				selected_->TabPage->SetLocation( 0, selected_->Button->GetSize().Height );
			}

			if( startIndex_ != 0 ) {
				lastSwitchButton_->SetVisible( true );
			}
			else {
				lastSwitchButton_->SetVisible( false );
			}
			if( maxIndex_ < static_cast< int >(bindings_.size()) ) {
				nextSwitchButton_->SetVisible( true );
			}
			else {
				nextSwitchButton_->SetVisible( false );
			}

			Invalidate();
		}
	}

	//---------------------------------------------------------------------------
	void TabControl::SelectBinding( TabPageButtonBinding &binding ) {
		selected_->TabPage->SetVisible( false );
		selected_->Button->SetActive( false );
		selected_ = &binding;
		selected_->Button->SetActive( true );
		selected_->TabPage->SetVisible( true );
		CalculateButtonLocationAndCount();

		selectedIndexChangedEvent_.Invoke( this );

		Invalidate();
	}

	//---------------------------------------------------------------------------
	void TabControl::ApplyStyle( const Drawing::Style &style ) {
		Control::ApplyStyle( style );

		for( auto &binding : bindings_ ) {
			binding->TabPage->ApplyStyle( style );
		}
	}

	//---------------------------------------------------------------------------
	void TabControl::DrawSelf( Drawing::RenderContext &context ) {
		Control::DrawSelf( context );

		if( selected_ != nullptr && selected_->TabPage != nullptr ) {
			for( auto i = startIndex_; i < maxIndex_; ++i ) {
				bindings_[ i ]->Button->Render();
			}

			nextSwitchButton_->Render();
			lastSwitchButton_->Render();

			selected_->TabPage->Render();
		}
	}

	//---------------------------------------------------------------------------
	//TabControl::TabControlButton
	//---------------------------------------------------------------------------
	const Drawing::PointI TabControl::TabControlButton::DefaultLabelOffset( 4, 6 );
	//---------------------------------------------------------------------------
	TabControl::TabControlButton::TabControlButton( TabPageButtonBinding &binding )
		: binding_( binding ),
		  label_( new Label() ) {
		active_ = false;

		label_->SetText( binding.TabPage->GetText() );
		label_->SetLocation( Drawing::PointI( button_width / 2.f - label_->GetSize().Width / 2.f,
		                                      25 / 2.f - label_->GetSize().Height / 2.f ) );
		label_->SetBackColor( Drawing::Color::Empty() );

		//size_ = label_->GetSize().InflateEx(DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2);
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetForeColor( const Drawing::Color &color ) {
		Control::SetForeColor( color );

		label_->SetForeColor( Drawing::Color::FromARGB( 255, 205, 205, 205 ) );
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetText( const Misc::AnsiString &text ) {
		label_->SetText( text );

		size_ = label_->GetSize().InflateEx( DefaultLabelOffset.Left * 2, DefaultLabelOffset.Top * 2 );
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::SetActive( bool active ) {
		active_ = active;

		Invalidate();
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::CalculateAbsoluteLocation() {
		Control::CalculateAbsoluteLocation();

		label_->SetParent( this );
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::OnMouseClick( const MouseMessage &mouse ) {
		Control::OnMouseClick( mouse );

		if( !active_ ) {
			if( parent_ != nullptr ) {
				static_cast< TabControl* >(parent_)->SetSelectedIndex( binding_.Index );
			}
		}
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::DrawSelf( Drawing::RenderContext &context ) {
		Control::DrawSelf( context );

		label_->Render();
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlButton::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		if( active_ ) {
			const auto color = isFocused_ || isInside_
			                   ? Color::FromARGB( 255, 55, 55, 64 ) + GetMouseOverFocusColor()
			                   : Color::FromARGB( 255, 55, 55, 64 );
			
			g.FillRectangleGradient( Color::FromARGB( 255, 55, 55, 64 ), PointF( 0, 0 ), GetSize() );
			g.FillRectangleGradient( ColorRectangle( color, Color::FromARGB( 255, 35, 35, 43 ) ), PointF( 1, 1 ),
			                         GetSize() - SizeF( 2, 0 ) );
			g.FillRectangle( OSHGui::Drawing::Color::FromARGB( 255, 206, 115, 136 ), PointF( 1, 24 ), SizeF( GetWidth() - 2, 1 ) );
		}
		else {
			const auto backInactive = ( isInside_
				                            ? Color::FromARGB( 255, 32, 32, 42 ) + Color::FromARGB( 0, 55, 55, 64 )
				                            : Color::FromARGB( 255, 32, 32, 42 ) + Color::FromARGB( 0, 50, 50, 58 ) ) - Color::
				FromARGB( 0, 35, 35, 43 );

			g.FillRectangle( backInactive + Color::FromARGB( 0, 9, 9, 12 ), PointF( 0, 0 ), GetSize() );
			g.FillRectangleGradient( ColorRectangle( backInactive, backInactive - Color::FromARGB( 0, 23, 23, 30 ) ),
			                         PointF( 1, 1 ), GetSize() - SizeF( 2, 1 ) );
		}
	}

	//---------------------------------------------------------------------------
	//TabControl::TabControlSwitchButton
	//---------------------------------------------------------------------------
	const Drawing::SizeI TabControl::TabControlSwitchButton::DefaultSize( 9, 9 );
	//---------------------------------------------------------------------------
	TabControl::TabControlSwitchButton::TabControlSwitchButton( TabControlSwitchButtonDirection direction )
		: direction_( direction ) {
		SetSize( DefaultSize );
	}

	//---------------------------------------------------------------------------
	void TabControl::TabControlSwitchButton::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		const auto base = isInside_ ? GetBackColor() : GetBackColor() - Color::FromARGB( 0, 47, 47, 47 );
		const auto borderColor = GetBackColor() + Color::FromARGB( 0, 9, 9, 9 );

		g.FillRectangle( borderColor, PointF( 0, 0 ), GetSize() );
		g.FillRectangle( base, PointF( 1, 1 ), GetSize() - SizeF( 2, 2 ) );

		if( direction_ == TabControlSwitchButtonDirection::Left ) {
			for( auto i = 0; i < 3; ++i ) {
				g.FillRectangle( GetForeColor(), PointF( 3 + i, 4 - i ), SizeF( 1, 1 + i * 2 ) );
			}
		}
		else {
			for( auto i = 0; i < 3; ++i ) {
				g.FillRectangle( GetForeColor(), PointF( 3 + i, 2 + i ), SizeF( 1, 5 - i * 2 ) );
			}
		}
	}

	//---------------------------------------------------------------------------
}
