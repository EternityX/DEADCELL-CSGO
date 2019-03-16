/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_MISC_RAWDATACONTAINER_HPP
#define OSHGUI_MISC_RAWDATACONTAINER_HPP

#include <vector>
#include <cstdint>
#include "../Exports.hpp"
#include "Strings.hpp"

namespace OSHGui {
	namespace Misc {
		/**
		 * Dient zum Laden von rohen Dateien wie Schriftarten und Bildern.
		 */
		class OSHGUI_EXPORT RawDataContainer {
		public:
			RawDataContainer();
			/**
			 * Reserviert Speicher mit size Bytes.
			 */
			RawDataContainer( size_t size );
			RawDataContainer( std::vector< uint8_t > data );

			uint8_t *GetDataPointer();
			const uint8_t *GetDataPointer() const;
			std::vector< uint8_t > &GetData();
			const std::vector< uint8_t > &GetData() const;
			size_t GetSize() const;

			void LoadFromFile( const Misc::AnsiString &filename );

			void Clear();

		private:
			std::vector< uint8_t > data_;
		};
	}
}

#endif
