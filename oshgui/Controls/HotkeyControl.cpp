/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include <sstream>
#include <algorithm>
#include <iterator>
#include "HotkeyControl.hpp"
#include "TextBox.hpp"
#include "../Misc/Exceptions.hpp"
#include "../Misc/Intersection.hpp"

namespace OSHGui {
	std::map< Key, Misc::AnsiString > HotkeyControl::HotkeyNames;
	const Drawing::SizeI HotkeyControl::DefaultSize( 100, 24 );
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	HotkeyControl::HotkeyControl()
		: textBox_( new TextBox() ) {
		type_ = ControlType::HotkeyControl;

		if( HotkeyNames.empty() ) {
#define EnumToString(x) #x
			HotkeyNames[ Key::None ] = EnumToString(None);
			HotkeyNames[ Key::LButton ] = EnumToString(LButton);
			HotkeyNames[ Key::RButton ] = EnumToString(RButton);
			HotkeyNames[ Key::Cancel ] = EnumToString(Cancel);
			HotkeyNames[ Key::MButton ] = EnumToString(MButton);
			HotkeyNames[ Key::XButton1 ] = "Mouse 4";
			HotkeyNames[ Key::XButton2 ] = "Mouse 5";
			HotkeyNames[ Key::Back ] = EnumToString(Back);
			HotkeyNames[ Key::Tab ] = EnumToString(Tab);
			HotkeyNames[ Key::LineFeed ] = EnumToString(LineFeed);
			HotkeyNames[ Key::Clear ] = EnumToString(Clear);
			HotkeyNames[ Key::Enter ] = EnumToString(Enter);
			HotkeyNames[ Key::Return ] = EnumToString(Return);
			HotkeyNames[ Key::ShiftKey ] = EnumToString(ShiftKey);
			HotkeyNames[ Key::ControlKey ] = EnumToString(ControlKey);
			HotkeyNames[ Key::Menu ] = EnumToString(Menu);
			HotkeyNames[ Key::Pause ] = EnumToString(Pause);
			HotkeyNames[ Key::CapsLock ] = EnumToString(CapsLock);
			HotkeyNames[ Key::Capital ] = EnumToString(Capital);
			HotkeyNames[ Key::KanaMode ] = EnumToString(KanaMode);
			HotkeyNames[ Key::HanguelMode ] = EnumToString(HanguelMode);
			HotkeyNames[ Key::HangulMode ] = EnumToString(HangulMode);
			HotkeyNames[ Key::JunjaMode ] = EnumToString(JunjaMode);
			HotkeyNames[ Key::FinalMode ] = EnumToString(FinalMode);
			HotkeyNames[ Key::KanjiMode ] = EnumToString(KanjiMode);
			HotkeyNames[ Key::HanjaMode ] = EnumToString(HanjaMode);
			HotkeyNames[ Key::Escape ] = EnumToString(Escape);
			HotkeyNames[ Key::IMEConvert ] = EnumToString(IMEConvert);
			HotkeyNames[ Key::IMENonconvert ] = EnumToString(IMENonconvert);
			HotkeyNames[ Key::IMEAceept ] = EnumToString(IMEAceept);
			HotkeyNames[ Key::IMEAccept ] = EnumToString(IMEAccept);
			HotkeyNames[ Key::IMEModeChange ] = EnumToString(IMEModeChange);
			HotkeyNames[ Key::Space ] = EnumToString(Space);
			HotkeyNames[ Key::Prior ] = EnumToString(Prior);
			HotkeyNames[ Key::PageUp ] = EnumToString(PageUp);
			HotkeyNames[ Key::Next ] = EnumToString(Next);
			HotkeyNames[ Key::PageDown ] = EnumToString(PageDown);
			HotkeyNames[ Key::End ] = EnumToString(End);
			HotkeyNames[ Key::Home ] = EnumToString(Home);
			HotkeyNames[ Key::Left ] = EnumToString(Left);
			HotkeyNames[ Key::Up ] = EnumToString(Up);
			HotkeyNames[ Key::Right ] = EnumToString(Right);
			HotkeyNames[ Key::Down ] = EnumToString(Down);
			HotkeyNames[ Key::Select ] = EnumToString(Select);
			HotkeyNames[ Key::Print ] = EnumToString(Print);
			HotkeyNames[ Key::Execute ] = EnumToString(Execute);
			HotkeyNames[ Key::PrintScreen ] = EnumToString(PrintScreen);
			HotkeyNames[ Key::Snapshot ] = EnumToString(Snapshot);
			HotkeyNames[ Key::Insert ] = EnumToString(Insert);
			HotkeyNames[ Key::Delete ] = EnumToString(Delete);
			HotkeyNames[ Key::Help ] = EnumToString(Help);
			HotkeyNames[ Key::D0 ] = EnumToString(D0);
			HotkeyNames[ Key::D1 ] = EnumToString(D1);
			HotkeyNames[ Key::D2 ] = EnumToString(D2);
			HotkeyNames[ Key::D3 ] = EnumToString(D3);
			HotkeyNames[ Key::D4 ] = EnumToString(D4);
			HotkeyNames[ Key::D5 ] = EnumToString(D5);
			HotkeyNames[ Key::D6 ] = EnumToString(D6);
			HotkeyNames[ Key::D7 ] = EnumToString(D7);
			HotkeyNames[ Key::D8 ] = EnumToString(D8);
			HotkeyNames[ Key::D9 ] = EnumToString(D9);
			HotkeyNames[ Key::A ] = EnumToString(A);
			HotkeyNames[ Key::B ] = EnumToString(B);
			HotkeyNames[ Key::C ] = EnumToString(C);
			HotkeyNames[ Key::D ] = EnumToString(D);
			HotkeyNames[ Key::E ] = EnumToString(E);
			HotkeyNames[ Key::F ] = EnumToString(F);
			HotkeyNames[ Key::G ] = EnumToString(G);
			HotkeyNames[ Key::H ] = EnumToString(H);
			HotkeyNames[ Key::I ] = EnumToString(I);
			HotkeyNames[ Key::J ] = EnumToString(J);
			HotkeyNames[ Key::K ] = EnumToString(K);
			HotkeyNames[ Key::L ] = EnumToString(L);
			HotkeyNames[ Key::M ] = EnumToString(M);
			HotkeyNames[ Key::N ] = EnumToString(N);
			HotkeyNames[ Key::O ] = EnumToString(O);
			HotkeyNames[ Key::P ] = EnumToString(P);
			HotkeyNames[ Key::Q ] = EnumToString(Q);
			HotkeyNames[ Key::R ] = EnumToString(R);
			HotkeyNames[ Key::S ] = EnumToString(S);
			HotkeyNames[ Key::T ] = EnumToString(T);
			HotkeyNames[ Key::U ] = EnumToString(U);
			HotkeyNames[ Key::V ] = EnumToString(V);
			HotkeyNames[ Key::W ] = EnumToString(W);
			HotkeyNames[ Key::X ] = EnumToString(X);
			HotkeyNames[ Key::Y ] = EnumToString(Y);
			HotkeyNames[ Key::Z ] = EnumToString(Z);
			HotkeyNames[ Key::LWin ] = EnumToString(LWin);
			HotkeyNames[ Key::RWin ] = EnumToString(RWin);
			HotkeyNames[ Key::Apps ] = EnumToString(Apps);
			HotkeyNames[ Key::Sleep ] = EnumToString(Sleep);
			HotkeyNames[ Key::NumPad0 ] = EnumToString(NumPad0);
			HotkeyNames[ Key::NumPad1 ] = EnumToString(NumPad1);
			HotkeyNames[ Key::NumPad2 ] = EnumToString(NumPad2);
			HotkeyNames[ Key::NumPad3 ] = EnumToString(NumPad3);
			HotkeyNames[ Key::NumPad4 ] = EnumToString(NumPad4);
			HotkeyNames[ Key::NumPad5 ] = EnumToString(NumPad5);
			HotkeyNames[ Key::NumPad6 ] = EnumToString(NumPad6);
			HotkeyNames[ Key::NumPad7 ] = EnumToString(NumPad7);
			HotkeyNames[ Key::NumPad8 ] = EnumToString(NumPad8);
			HotkeyNames[ Key::NumPad9 ] = EnumToString(NumPad9);
			HotkeyNames[ Key::Multiply ] = EnumToString(Multiply);
			HotkeyNames[ Key::Add ] = EnumToString(Add);
			HotkeyNames[ Key::Separator ] = EnumToString(Separator);
			HotkeyNames[ Key::Subtract ] = EnumToString(Subtract);
			HotkeyNames[ Key::Decimal ] = EnumToString(Decimal);
			HotkeyNames[ Key::Divide ] = EnumToString(Divide);
			HotkeyNames[ Key::F1 ] = EnumToString(F1);
			HotkeyNames[ Key::F2 ] = EnumToString(F2);
			HotkeyNames[ Key::F3 ] = EnumToString(F3);
			HotkeyNames[ Key::F4 ] = EnumToString(F4);
			HotkeyNames[ Key::F5 ] = EnumToString(F5);
			HotkeyNames[ Key::F6 ] = EnumToString(F6);
			HotkeyNames[ Key::F7 ] = EnumToString(F7);
			HotkeyNames[ Key::F8 ] = EnumToString(F8);
			HotkeyNames[ Key::F9 ] = EnumToString(F9);
			HotkeyNames[ Key::F10 ] = EnumToString(F10);
			HotkeyNames[ Key::F11 ] = EnumToString(F11);
			HotkeyNames[ Key::F12 ] = EnumToString(F12);
			HotkeyNames[ Key::F13 ] = EnumToString(F13);
			HotkeyNames[ Key::F14 ] = EnumToString(F14);
			HotkeyNames[ Key::F15 ] = EnumToString(F15);
			HotkeyNames[ Key::F16 ] = EnumToString(F16);
			HotkeyNames[ Key::F17 ] = EnumToString(F17);
			HotkeyNames[ Key::F18 ] = EnumToString(F18);
			HotkeyNames[ Key::F19 ] = EnumToString(F19);
			HotkeyNames[ Key::F20 ] = EnumToString(F20);
			HotkeyNames[ Key::F21 ] = EnumToString(F21);
			HotkeyNames[ Key::F22 ] = EnumToString(F22);
			HotkeyNames[ Key::F23 ] = EnumToString(F23);
			HotkeyNames[ Key::F24 ] = EnumToString(F24);
			HotkeyNames[ Key::NumLock ] = EnumToString(NumLock);
			HotkeyNames[ Key::Scroll ] = EnumToString(Scroll);
			HotkeyNames[ Key::LShiftKey ] = EnumToString(LShiftKey);
			HotkeyNames[ Key::RShiftKey ] = EnumToString(RShiftKey);
			HotkeyNames[ Key::LControlKey ] = EnumToString(LControlKey);
			HotkeyNames[ Key::RControlKey ] = EnumToString(RControlKey);
			HotkeyNames[ Key::LMenu ] = EnumToString(LMenu);
			HotkeyNames[ Key::RMenu ] = EnumToString(RMenu);
			HotkeyNames[ Key::BrowserBack ] = EnumToString(BrowserBack);
			HotkeyNames[ Key::BrowserForward ] = EnumToString(BrowserForward);
			HotkeyNames[ Key::BrowserRefresh ] = EnumToString(BrowserRefresh);
			HotkeyNames[ Key::BrowserStop ] = EnumToString(BrowserStop);
			HotkeyNames[ Key::BrowserSearch ] = EnumToString(BrowserSearch);
			HotkeyNames[ Key::BrowserFavorites ] = EnumToString(BrowserFavorites);
			HotkeyNames[ Key::BrowserHome ] = EnumToString(BrowserHome);
			HotkeyNames[ Key::VolumeMute ] = EnumToString(VolumeMute);
			HotkeyNames[ Key::VolumeDown ] = EnumToString(VolumeDown);
			HotkeyNames[ Key::VolumeUp ] = EnumToString(VolumeUp);
			HotkeyNames[ Key::MediaNextTrack ] = EnumToString(MediaNextTrack);
			HotkeyNames[ Key::MediaPreviousTrack ] = EnumToString(MediaPreviousTrack);
			HotkeyNames[ Key::MediaStop ] = EnumToString(MediaStop);
			HotkeyNames[ Key::MediaPlayPause ] = EnumToString(MediaPlayPause);
			HotkeyNames[ Key::LaunchMail ] = EnumToString(LaunchMail);
			HotkeyNames[ Key::SelectMedia ] = EnumToString(SelectMedia);
			HotkeyNames[ Key::LaunchApplication1 ] = EnumToString(LaunchApplication1);
			HotkeyNames[ Key::LaunchApplication2 ] = EnumToString(LaunchApplication2);
			HotkeyNames[ Key::Oem1 ] = EnumToString(Oem1);
			HotkeyNames[ Key::OemSemicolon ] = EnumToString(OemSemicolon);
			HotkeyNames[ Key::OemPlus ] = EnumToString(OemPlus);
			HotkeyNames[ Key::OemComma ] = EnumToString(OemComma);
			HotkeyNames[ Key::OemMinus ] = EnumToString(OemMinus);
			HotkeyNames[ Key::OemPeriod ] = EnumToString(OemPeriod);
			HotkeyNames[ Key::OemQuestion ] = EnumToString(OemQuestion);
			HotkeyNames[ Key::Oem2 ] = EnumToString(Oem2);
			HotkeyNames[ Key::OemTilde ] = EnumToString(OemTilde);
			HotkeyNames[ Key::Oem3 ] = EnumToString(Oem3);
			HotkeyNames[ Key::Oem4 ] = EnumToString(Oem4);
			HotkeyNames[ Key::OemOpenBrackets ] = EnumToString(OemOpenBrackets);
			HotkeyNames[ Key::OemPipe ] = EnumToString(OemPipe);
			HotkeyNames[ Key::Oem5 ] = EnumToString(Oem5);
			HotkeyNames[ Key::Oem6 ] = EnumToString(Oem6);
			HotkeyNames[ Key::OemCloseBrackets ] = EnumToString(OemCloseBrackets);
			HotkeyNames[ Key::Oem7 ] = EnumToString(Oem7);
			HotkeyNames[ Key::OemQuotes ] = EnumToString(OemQuotes);
			HotkeyNames[ Key::Oem8 ] = EnumToString(Oem8);
			HotkeyNames[ Key::Oem102 ] = EnumToString(Oem102);
			HotkeyNames[ Key::OemBackslash ] = EnumToString(OemBackslash);
			HotkeyNames[ Key::ProcessKey ] = EnumToString(ProcessKey);
			HotkeyNames[ Key::Packet ] = EnumToString(Packet);
			HotkeyNames[ Key::Attn ] = EnumToString(Attn);
			HotkeyNames[ Key::Crsel ] = EnumToString(Crsel);
			HotkeyNames[ Key::Exsel ] = EnumToString(Exsel);
			HotkeyNames[ Key::EraseEof ] = EnumToString(EraseEof);
			HotkeyNames[ Key::Play ] = EnumToString(Play);
			HotkeyNames[ Key::Zoom ] = EnumToString(Zoom);
			HotkeyNames[ Key::NoName ] = EnumToString(NoName);
			HotkeyNames[ Key::Pa1 ] = EnumToString(Pa1);
			HotkeyNames[ Key::OemClear ] = EnumToString(OemClear);
		}

		textBox_->SetLocation( Drawing::PointI( 0, 0 ) );
		textBox_->ShowCaret( true );
		textBox_->SetParent( this );

		auto text = textBox_->GetText();
		if( text.empty() )
			textBox_->SetText( "Click to set a hotkey..." );

		SetSize( DefaultSize );

		ApplyStyle( Application::Instance().GetStyle() );

		ClearHotkey();
	}

	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void HotkeyControl::SetSize( const Drawing::SizeI &size ) {
		Control::SetSize( size );

		textBox_->SetSize( size.Width, 24 );

		clearButtonLocation_ = Drawing::PointI( 160 - 12, GetHeight() * 0.5f - 2 );
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::SetFont( const Drawing::FontPtr &font ) {
		Control::SetFont( font );

		textBox_->SetFont( GetFont() );
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::SetForeColor( const Drawing::Color &color ) {
		Control::SetForeColor( color );

		textBox_->SetForeColor( Drawing::Color::FromARGB( 255, 205, 205, 205 ) );
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::SetBackColor( const Drawing::Color &color ) {
		Control::SetBackColor( color );

		textBox_->SetBackColor( GetBackColor() );
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::SetHotkey( Key hotkey ) {
		if( hotkey_ != hotkey ) {
			hotkey_ = hotkey;

			HotkeyToText();

			hotkeyChangedEvent_.Invoke( this );
		}
	}

	//---------------------------------------------------------------------------
	Key HotkeyControl::GetHotkey() const {
		return hotkey_;
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::SetHotkeyModifier( Key modifier ) {
		if( modifier_ != modifier ) {
			modifier_ = modifier;

			HotkeyToText();

			hotkeyChangedEvent_.Invoke( this );
		}
	}

	//---------------------------------------------------------------------------
	Key HotkeyControl::GetHotkeyModifier() const {
		return modifier_;
	}

	//---------------------------------------------------------------------------
	HotkeyChangedEvent &HotkeyControl::GetHotkeyChangedEvent() {
		return hotkeyChangedEvent_;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void HotkeyControl::ClearHotkey() {
		SetHotkey( Key::None );
		SetHotkeyModifier( Key::None );
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::HotkeyToText( ) {
		auto ModifierToString = []( Key modifier ) -> Misc::AnsiString {
			Misc::AnsiString modifierName;
			if( ( modifier & Key::Control ) == Key::Control )
				modifierName = "Control";
			if( ( modifier & Key::Alt ) == Key::Alt )
				modifierName = "Alt";
			if( ( modifier & Key::Shift ) == Key::Shift )
				modifierName = "Shift";

			//std::stringstream s;
			//for (auto i = 0; i < modifierNames.size(); ++i)
			//{
			//	/*if (i > 0)
			//	{
			//		s << " + ";
			//	}*/
			//	s << modifierNames[i];
			//}
			return modifierName;
		};

		if( modifier_ == Key::None && hotkey_ == Key::None ) {
			textBox_->SetText( HotkeyNames[ hotkey_ ] );
			textBox_->SetForeColor( Drawing::Color::FromRGB( 201, 201, 201 ) );
		}
		else if( hotkey_ != Key::None && ( hotkey_ != Key::ShiftKey && hotkey_ != Key::Menu && hotkey_ != Key::ControlKey ) ) {
			const auto hotkeyText = HotkeyNames[ hotkey_ ];
			textBox_->SetText( hotkeyText );
			textBox_->SetForeColor( Drawing::Color::FromRGB( 201, 201, 201 ) );
		}
		else {
			auto modifierText = ModifierToString( modifier_ );
			textBox_->SetText( modifierText );
			textBox_->SetForeColor( Drawing::Color::FromRGB( 201, 201, 201 ) );
		}
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::CalculateAbsoluteLocation() {
		Control::CalculateAbsoluteLocation();

		textBox_->CalculateAbsoluteLocation();
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::DrawSelf( Drawing::RenderContext &context ) {
		textBox_->Render();

		Control::DrawSelf( context );
	}

	//---------------------------------------------------------------------------
	void HotkeyControl::PopulateGeometry() {
		using namespace Drawing;

		Graphics g( *geometry_ );

		const auto color = isFocused_ || isInside_ ? Color::FromARGB( 0, 0, 0, 0 ) : Color::FromARGB( 255, 156, 156, 156 );

		for( auto i = 0; i < 4; ++i ) {
			g.FillRectangle( color, clearButtonLocation_ + PointF( i, i ), SizeF( 2, 1 ) );
			g.FillRectangle( color, clearButtonLocation_ + PointF( 4 - i, i ), SizeF( 2, 1 ) );
			g.FillRectangle( color, clearButtonLocation_ + PointF( i, 4 - i ), SizeF( 2, 1 ) );
			g.FillRectangle( color, clearButtonLocation_ + PointF( 4 - i, 4 - i ), SizeF( 2, 1 ) );
		}

		if( isFocused_ ) {
			g.DrawRectangle( Color::FromARGB( 255, 206, 115, 136 ), PointF( 0, 0 ), textBox_->GetSize() );
		}
	}

	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool HotkeyControl::ProcessMouseMessage( const MouseMessage &mouse ) {
		if( mouse.GetState() == MouseState::Down ) {
			if( isFocusable_ && !isFocused_ ) {
				if( Intersection::TestRectangle( absoluteLocation_, Drawing::SizeI( 140, 24 ), mouse.GetLocation() ) )
					if( mouse.GetButton() == MouseButton::Left && isEnabled_ )
						OnGotFocus( this );

				if( Intersection::TestRectangle( absoluteLocation_ + clearButtonLocation_, Drawing::SizeI( 9, 8 ),
					mouse.GetLocation() ) )
					ClearHotkey();

				return false;
			}

			if( isFocused_ ) {
				textBox_->SetText( "Press a key..." );

				if( mouse.GetButton() == MouseButton::Left && isEnabled_ )
					SetHotkey( Key::LButton );

				if( mouse.GetButton() == MouseButton::Right && isEnabled_ )
					SetHotkey( Key::RButton );

				if( mouse.GetButton() == MouseButton::XButton1 && isEnabled_ )
					SetHotkey( Key::XButton1 );

				if( mouse.GetButton() == MouseButton::XButton2 && isEnabled_ )
					SetHotkey( Key::XButton2 );

				if( mouse.GetButton() == MouseButton::Middle && isEnabled_ )
					SetHotkey( Key::MButton );
			}

			OnLostFocus( this );
			return true;
		}

		return false;
	}

	//---------------------------------------------------------------------------
	bool HotkeyControl::OnKeyDown( const KeyboardMessage &keyboard ) {
		if( !Control::OnKeyDown( keyboard ) ) {
			KeyEventArgs args( keyboard );

			/*if ( args.GetKeyCode( ) == Key::Enter || args.GetKeyCode( ) == Key::Escape )
			{
			OnLostFocus( this );
			return true;
			}*/

			SetHotkey( args.GetKeyCode() );
			SetHotkeyModifier( args.GetModifier() );

			OnLostFocus( this );
			return true;
		}

		return true;
	}

	//---------------------------------------------------------------------------
	bool HotkeyControl::OnKeyPress( const KeyboardMessage &keyboard ) {
		return true;
	}

	//---------------------------------------------------------------------------
	bool HotkeyControl::OnKeyUp( const KeyboardMessage &keyboard ) {
		return true;
	}

	//---------------------------------------------------------------------------
}
