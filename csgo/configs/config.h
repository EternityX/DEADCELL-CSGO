#pragma once
#include "../inc.h"

struct item
{
	template <typename T>
	item( T &value, const std::string& sz, std::vector<item>& items )
	{
		this->name = sz;
		this->value = reinterpret_cast< void* >( &value );
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
	std::vector< std::string > get_configs( ) const;
	bool import_from_clipboard( const std::string &file );
	void export_to_clipboard( const std::string &file ) const;

	bool init( );
	bool load( const std::string &file );
	bool save( const std::string &file );

	bool reset( const std::string &file );
	void remove( const std::string &file ) const;
};

extern c_config g_configs;