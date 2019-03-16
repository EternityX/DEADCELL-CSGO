#pragma once
#include "../inc.hpp"

struct item
{
	template <typename T>
	item( T &value, const std::string& sz, std::vector<item>& items )
	{
		this->name = sz;
		this->value = ( void* ) &value;
		this->type = typeid( T ).name();

		items.push_back( *this );
	}
	void*		value;
	std::string	name;
	std::string type;
};


class c_config
{
private:
	std::vector< item > m_items;
	std::string m_directory = "profiles";
public:
	std::vector< std::string > get_configs( );
	bool import_from_clipboard( std::string file );
	void export_to_clipboard( std::string file );

	const void		init( );
	const bool		load( std::string file );
	const bool		save( std::string file );

	const bool		reset( std::string file );
	const void		remove( std::string file );
};

extern c_config g_configs;