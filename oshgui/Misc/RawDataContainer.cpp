/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include "RawDataContainer.hpp"
#include "Exceptions.hpp"
#include <fstream>
#include <iterator>

namespace OSHGui {
	namespace Misc {
		RawDataContainer::RawDataContainer() { }
		//---------------------------------------------------------------------------
		RawDataContainer::RawDataContainer( size_t size )
			: data_( size ) { }

		RawDataContainer::RawDataContainer( std::vector< uint8_t > data )
			: data_( data ) { }

		//---------------------------------------------------------------------------
		uint8_t *RawDataContainer::GetDataPointer() {
			return data_.data();
		}

		//---------------------------------------------------------------------------
		const uint8_t *RawDataContainer::GetDataPointer() const {
			return data_.data();
		}

		//---------------------------------------------------------------------------
		std::vector< uint8_t > &RawDataContainer::GetData() {
			return data_;
		}

		//---------------------------------------------------------------------------
		const std::vector< uint8_t > &RawDataContainer::GetData() const {
			return data_;
		}

		//---------------------------------------------------------------------------
		size_t RawDataContainer::GetSize() const {
			return data_.size();
		}

		//---------------------------------------------------------------------------
		void RawDataContainer::LoadFromFile( const Misc::AnsiString &filename ) {
			Clear();

			std::ifstream in( filename, std::ios::binary );
			if( in ) {
				in.unsetf( std::ios::skipws );

				in.seekg( 0, std::ios::end );
				auto fileSize = static_cast< unsigned int >(in.tellg());
				in.seekg( 0, std::ios::beg );

				data_.reserve( fileSize );

				data_.insert( data_.begin(), std::istream_iterator< uint8_t >( in ), std::istream_iterator< uint8_t >() );
			}
			else {
				throw Misc::FileNotFoundException();
			}
		}

		//---------------------------------------------------------------------------
		void RawDataContainer::Clear() {
			data_.clear();
		}

		//---------------------------------------------------------------------------
	}
}
