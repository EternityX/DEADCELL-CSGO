#pragma once

#include <map>

#include "../Exports.hpp"

#include "../Controls/Types.hpp"

#include "Color.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT Style
		{
		public:
			struct ControlStyle
			{
				Color ForeColor;
				Color BackColor;
			};

			ControlStyle DefaultColor;

			/**
			 * Legt das Farbschema für eine Control Klasse fest.
			 *
			 * \param type
			 * \param style
			 */
			void SetControlStyle(ControlType type, ControlStyle &&style);
			/**
			 * Ruft das Farbschema für eine Control Klasse ab.
			 *
			 * \param controlClass
			 * \return style
			 */
			const ControlStyle& GetControlStyle(ControlType type) const;

		private:
			std::map<ControlType, ControlStyle> controlStyles;
		};
	}
}
