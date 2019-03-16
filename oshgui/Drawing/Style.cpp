#include "Style.hpp"

namespace OSHGui {
	namespace Drawing {
		void Style::SetControlStyle( ControlType type, Style::ControlStyle &&style ) {
			controlStyles[ type ] = std::move( style );
		}

		//---------------------------------------------------------------------------
		const Style::ControlStyle &Style::GetControlStyle( ControlType type ) const {
			const auto it = controlStyles.find( type );
			if( it == controlStyles.end() ) {
				return DefaultColor;
			}

			return it->second;
		}

		//---------------------------------------------------------------------------
	}
}
