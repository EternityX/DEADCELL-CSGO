/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#ifndef OSHGUI_TRACKBAR_HPP
#define OSHGUI_TRACKBAR_HPP

#include "Control.hpp"

namespace OSHGui
{
	class Label;

	/**
	* Tritt auf, wenn der TrackBar-Schieberegler verschoben wird.
	*/
	typedef Event<void( Control* )> ValueChangedEvent;
	typedef EventHandler<void( Control* )> ValueChangedEventHandler;

	/**
	* Stellt eine Standardtrackleiste dar.
	*/
	class OSHGUI_EXPORT TrackBar : public Control
	{
	public:
		using Control::SetSize;

		/**
		* Konstruktor der Klasse.
		*/
		TrackBar();

		/**
		* Legt die Höhe und Breite des Steuerelements fest.
		*
		* \param size
		*/
		virtual void SetSize( const Drawing::SizeI &size ) override;
		/**
		* Legt den minimalen Wert für die Schiebereglerposition auf der TrackBar fest.
		*
		* \param minimum
		*/
		void SetText( const Misc::AnsiString &text );
		void SetMinimum( float minimum );
		void SetPrecision( int precision );
		int GetPrecision() const;
		/**
		* Ruft den minimalen Wert für die Schiebereglerposition auf der TrackBar ab.
		*
		* \return minimum
		*/
		int GetMinimum() const;
		/**
		* Legt den maximalen Wert für die Schiebereglerposition auf der TrackBar fest.
		*
		* \param maximum
		*/
		void SetMaximum( float maximum );
		/**
		* Ruft den maximalen Wert für die Schiebereglerposition auf der TrackBar ab.
		*
		* \return maximum
		*/
		int GetMaximum() const;
		/**
		* Legt die Anzahl der Positionen zwischen den Teilstrichen fest.
		*
		* \param tickFrequency
		*/
		void SetTickFrequency( float tickFrequency );
		/**
		* Ruft die Anzahl der Positionen zwischen den Teilstrichen ab
		*
		* \return tickFrequency
		*/
		int GetTickFrequency() const;
		/**
		* Legt den aktuellen Wert fest.
		*
		* \param value
		*/
		void SetValue( float value );
		/**
		* Ruft den aktuellen Wert ab.
		*
		* \return der aktuelle Wert
		*/
		float GetValue() const;
		void SetAppendText( std::string text );
		/**
		* Ruft das ValueChangedEvent für das Steuerelement ab.
		*
		* \return valueChangedEvent
		*/
		ValueChangedEvent& GetValueChangedEvent();

		/**
		* Berechnet die absolute Position des Steuerelements.
		*/
		virtual void CalculateAbsoluteLocation() override;

	protected:
		virtual void PopulateGeometry() override;

		virtual void OnMouseDown( const MouseMessage &mouse ) override;
		virtual void OnMouseUp( const MouseMessage &mouse ) override;
		virtual void OnMouseClick( const MouseMessage &mouse ) override;
		virtual void OnMouseMove( const MouseMessage &mouse ) override;
		virtual void OnMouseScroll( const MouseMessage &mouse ) override;
		virtual bool OnKeyDown( const KeyboardMessage &keyboard ) override;

	private:
		static const Drawing::SizeI GrabSize;
		static const Drawing::SizeI SliderSize;
		static const Drawing::SizeI DefaultSize;
		static const Drawing::PointI DefaultLabelOffset;
		static const int DefaultTickOffset = 7;

		void SetValueInternal( float value );
		void DrawSelf( Drawing::RenderContext &context );

		void HandleMouseEvent( const MouseMessage &mouse );

		float minimum_;
		float maximum_;
		float value_;
		float tickFrequency_;
		int precision_;
		float pixelsPerTick_;
		bool drag_;
		std::string appendText_;

		Drawing::PointI sliderLocation_;
		Drawing::PointI sliderAbsoluteLocation_;

		std::unique_ptr<Label> label_;
		std::unique_ptr<Label> label_value_;

		ValueChangedEvent valueChangedEvent_;
	};
}

#endif