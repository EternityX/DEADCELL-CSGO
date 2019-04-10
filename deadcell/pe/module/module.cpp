#include "../../inc.hpp"
#include "module.hpp"

// get DOS / NT headers.
bool pe::get_file_headers( uintptr_t base, PIMAGE_DOS_HEADER &out_dos, PIMAGE_NT_HEADERS &out_nt ) {
	// get DOS header and check for invalid DOS / DOS signature.
	auto dos = reinterpret_cast< IMAGE_DOS_HEADER * >( base );
	if( !dos || dos->e_magic != IMAGE_DOS_SIGNATURE /* "MZ" */ )
		return false;

	// get NT headers and check for invalid NT / NT signature.
	auto nt = reinterpret_cast< IMAGE_NT_HEADERS * >( reinterpret_cast< uintptr_t >( dos ) + dos->e_lfanew );
	if( !nt || nt->Signature != IMAGE_NT_SIGNATURE /* "PE\0\0" */ )
		return false;

	out_dos = dos;
	out_nt = nt;

	return true;
}

pe::Module::Module(): m_ldr_entry{ nullptr }, m_dos{ nullptr }, m_nt{ nullptr }, m_base{ 0 } { }

bool pe::Module::init( types::LDR_DATA_TABLE_ENTRY *ldr_entry ) {
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *nt;

	if( !ldr_entry )
		return false;

	uintptr_t base = ldr_entry->DllBase;

	if( !get_file_headers( base, dos, nt ) )
		return false;

	m_ldr_entry = ldr_entry;
	m_dos = dos;
	m_nt = nt;
	m_base = base;
	m_pathW = ldr_entry->FullDllName.Buffer;

	std::transform( m_pathW.begin(), m_pathW.end(), m_pathW.begin(), tolower );

	return true;
}

uintptr_t pe::Module::get_base() const {
	return m_base;
}

IMAGE_DOS_HEADER *pe::Module::get_dos_header() const {
	return ( m_dos ) ? m_dos : nullptr;
}

IMAGE_NT_HEADERS *pe::Module::get_nt_headers() const {
	return ( m_nt ) ? m_nt : nullptr;
}

size_t pe::Module::get_code_size() const {
	return ( m_nt ) ? m_nt->OptionalHeader.SizeOfCode : 0;
}

size_t pe::Module::get_image_size() const {
	return ( m_nt ) ? m_nt->OptionalHeader.SizeOfImage : 0;
}

uintptr_t pe::Module::get_code_base() const {
	return ( m_nt ) ? RVA( m_nt->OptionalHeader.BaseOfCode ) : 0;
}

std::wstring pe::Module::get_pathW() const {
	return m_pathW;
}

std::string pe::Module::get_pathA() const {
	return util::misc::wide_to_multibyte( get_pathW() );
}

std::wstring pe::Module::get_module_nameW() const {
	if( m_pathW.empty() )
		return { };

	std::wstring out = get_pathW( );

	size_t delim = out.find_last_of( '\\' );
	if( delim == std::wstring::npos )
		return { };

	// keep only name + extension.
	return out.substr( delim + 1 );
}

std::string pe::Module::get_module_nameA() const {
	return util::misc::wide_to_multibyte( get_module_nameW() );
}
