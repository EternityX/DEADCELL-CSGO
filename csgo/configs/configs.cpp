#include "config.h"
#include "..\json.h"
#include <experimental/filesystem>

nlohmann::json json;

void assign_item( item &item ) {
	if( json[ item.name.c_str() ].empty( ) )
		return;

	if( item.type == "float" )
		*reinterpret_cast< float* >( item.value ) = json[ item.name.c_str( ) ].get< float >( );
	if( item.type == "int" )
		*reinterpret_cast< int* >( item.value ) = json[ item.name.c_str( ) ].get< int >( );
	if( item.type == "bool" )
		*reinterpret_cast< bool* >( item.value ) = json[ item.name.c_str( ) ].get< bool >( );
	if( item.type == "float [4]" )
		for( int i = 0; i < 4; i++ )
			reinterpret_cast< float* >( item.value )[ i ] = json[ item.name.c_str( ) ][ std::to_string( i ).c_str( ) ].get< float >( );
}

void save_item( item &item ) {
	if( item.type == "float" )
		json[ item.name.c_str( ) ] = *reinterpret_cast< float* >( item.value );
	if( item.type == "int" )
		json[ item.name.c_str( ) ] = *reinterpret_cast< int* >( item.value );
	if( item.type == "bool" )
		json[ item.name.c_str( ) ] = *reinterpret_cast< bool* >( item.value );
	if( item.type == "float [4]" )
		for( int i = 0; i < 4; i++ )
			json[ item.name.c_str( ) ][ std::to_string( i ).c_str( ) ] = reinterpret_cast< float* >( item.value )[ i ];
}

void reset_item( item &item ) {
	if( json[ item.name.c_str( ) ].empty( ) )
		return;

	if( json[ item.name.c_str( ) ].is_array( ) )
		for( int i = 0; i < 4; i++ )
			json[ item.name.c_str( ) ][ i ].clear( );

	json[ item.name.c_str( ) ].clear( );

	json[ "g_vars.misc.nightmode" ] = 100;
	json[ "g_vars.misc.prop_transparency" ] = 100;
	json[ "g_vars.visuals.effects.camera_fov" ] = 90;
	json[ "g_vars.visuals.effects.weapon_fov" ] = 68;
}

bool c_config::init( ) {
	if( !std::experimental::filesystem::exists( m_directory ) ) {
		if( !std::experimental::filesystem::create_directory( m_directory ) ) {
			_RPT1( _CRT_WARN, "Failed to create profile directory. Ignoring this error will result in not being able to create or save profiles.\n\n%s", m_directory );
			return false;
		}
	}

	item( g_vars.visuals.activation_type,"g_vars.visuals.activation_type", m_items );
	item( g_vars.visuals.activation_key,"g_vars.visuals.activation_key", m_items );
	item( g_vars.visuals.bbox,"g_vars.visuals.bbox", m_items );
	item( g_vars.visuals.box_color,"g_vars.visuals.box_color", m_items );
	item( g_vars.visuals.name,"g_vars.visuals.name", m_items );
	item( g_vars.visuals.name_color, "g_vars.visuals.name_color", m_items );
	item( g_vars.visuals.weapon, "g_vars.visuals.weapon", m_items );
	item( g_vars.visuals.weapon_color, "g_vars.visuals.weapon_color", m_items );
	item( g_vars.visuals.healthbar, "g_vars.visuals.healthbar", m_items );
	item( g_vars.visuals.ammo_bar, "g_vars.visuals.ammo_bar", m_items );
	item( g_vars.visuals.ammo_bar_color, "g_vars.visuals.ammo_bar_color", m_items );
	item( g_vars.visuals.flags, "g_vars.visuals.flags", m_items );
	item( g_vars.visuals.flag_color, "g_vars.visuals.flag_color", m_items );
	item( g_vars.visuals.money, "g_vars.visuals.money", m_items );
	item( g_vars.visuals.money_color, "g_vars.visuals.money_color", m_items );
	item( g_vars.visuals.flash_bar, "g_vars.visuals.flash_bar", m_items );
	item( g_vars.visuals.flash_bar_color, "g_vars.visuals.flash_bar_color", m_items );
	item( g_vars.visuals.glow, "g_vars.visuals.glow", m_items );
	item( g_vars.visuals.glow_color, "g_vars.visuals.glow_color", m_items );
	item( g_vars.visuals.filter, "g_vars.visuals.filter", m_items );
	item( g_vars.visuals.chicken, "g_vars.visuals.chicken", m_items );
	item( g_vars.visuals.projectiles, "g_vars.visuals.projectiles", m_items );
	item( g_vars.visuals.objectives, "g_vars.visuals.objectives", m_items );
	item( g_vars.visuals.dropped_weapons, "g_vars.visuals.dropped_weapons", m_items );
	item( g_vars.visuals.dropped_ammo, "g_vars.visuals.dropped_ammo", m_items );
	item( g_vars.visuals.dropped_ammo_color, "g_vars.visuals.dropped_ammo_color", m_items );
	item( g_vars.visuals.radar, "g_vars.visuals.radar", m_items );
	item( g_vars.visuals.visualize_spread, "g_vars.visuals.visualize_spread", m_items );
	item( g_vars.visuals.visualize_spread_color, "g_vars.visuals.visualize_spread_color", m_items );
	item( g_vars.visuals.hitmarker, "g_vars.visuals.hitmarker", m_items );
	item( g_vars.visuals.impact, "g_vars.visuals.impact", m_items );
	item( g_vars.visuals.impact_color, "g_vars.visuals.impact_color", m_items );
	item( g_vars.visuals.misc.remove_scope, "g_vars.visuals.misc.remove_scope", m_items );
	item( g_vars.visuals.misc.remove_scope_zoom, "g_vars.visuals.misc.remove_scope_zoom", m_items );
	item( g_vars.visuals.misc.remove_smoke, "g_vars.visuals.misc.remove_smoke", m_items );
	item( g_vars.visuals.misc.remove_blur, "g_vars.visuals.misc.remove_blur", m_items );
	item( g_vars.visuals.misc.scope_color, "g_vars.visuals.misc.scope_color", m_items );
	item( g_vars.visuals.misc.fog, "g_vars.visuals.misc.fog", m_items );
	item( g_vars.visuals.misc.no_flash, "g_vars.visuals.visuals.misc.no_flash", m_items );
	item( g_vars.visuals.dormancy_fade, "g_vars.visuals.dormancy_fade", m_items );
	item( g_vars.visuals.effects.camera_fov, "g_vars.visuals.effects.camera_fov", m_items );
	item( g_vars.visuals.effects.weapon_fov, "g_vars.visuals.effects.weapon_fov", m_items );
	item( g_vars.visuals.extra.speclist, "g_vars.visuals.extra.speclist", m_items );
	item( g_vars.visuals.extra.antiaim_direction, "g_vars.visuals.extra.antiaim_direction", m_items );
	item( g_vars.visuals.extra.antiaim_direction_color, "g_vars.visuals.extra.antiaim_direction_color", m_items );
	item( g_vars.visuals.extra.misc_flags, "g_vars.visuals.extra.misc_flags", m_items );
	item( g_vars.visuals.extra.points, "g_vars.visuals.extra.points", m_items );
	item( g_vars.visuals.misc.scope_color, "g_vars.visuals.misc.scope_color", m_items );
	item( g_vars.visuals.grenade_pred, "g_vars.visuals.grenade_pred", m_items );
	item( g_vars.visuals.chams.enabled, "g_vars.visuals.chams.enabled", m_items );
	item( g_vars.visuals.chams.alpha, "g_vars.visuals.chams.alpha", m_items );
	item( g_vars.visuals.chams.hid_color, "g_vars.visuals.chams.hid_color", m_items );
	item( g_vars.visuals.chams.vis_color, "g_vars.visuals.chams.vis_color", m_items );
	item( g_vars.visuals.chams.teammates, "g_vars.visuals.chams.teammates", m_items );
	item( g_vars.visuals.chams.twopass, "g_vars.visuals.chams.twopass", m_items );
	item( g_vars.visuals.chams.type, "g_vars.visuals.chams.type", m_items );
	item( g_vars.visuals.chams.local, "g_vars.visuals.chams.local", m_items );
	item( g_vars.visuals.chams.local_col, "g_vars.visuals.chams.local_col", m_items );
	item( g_vars.rage.enabled, "g_vars.rage.enabled", m_items );
	item( g_vars.rage.key, "g_vars.rage.key", m_items );
	item( g_vars.rage.target_selection, "g_vars.rage.target_selection", m_items );
	item( g_vars.rage.auto_fire, "g_vars.rage.auto_fire", m_items );
	item( g_vars.rage.hitchance, "g_vars.rage.hitchance", m_items );
	item( g_vars.rage.pointscale, "g_vars.rage.pointscale", m_items );
	item( g_vars.rage.min_dmg, "g_vars.rage.min_dmg", m_items );
	item( g_vars.rage.silent, "g_vars.rage.silent", m_items );
	item( g_vars.rage.bodyaim_lethal, "g_vars.rage.bodyaim_lethal", m_items );
	item( g_vars.rage.prefer_bodyaim, "g_vars.rage.prefer_bodyaim", m_items );
	item( g_vars.rage.autoscope, "g_vars.rage.autoscope", m_items );
	item( g_vars.rage.autostop, "g_vars.rage.autostop", m_items );
	item( g_vars.rage.head, "g_vars.rage.head", m_items );
	item( g_vars.rage.pelvis, "g_vars.rage.pelvis", m_items );
	item( g_vars.rage.arms, "g_vars.rage.arms", m_items );
	item( g_vars.rage.legs, "g_vars.rage.legs", m_items );
	item( g_vars.rage.hitbox_head, "g_vars.rage.hitbox_head", m_items );
	item( g_vars.rage.hitbox_pelvis, "g_vars.rage.hitbox_pelvis", m_items );
	item( g_vars.rage.hitbox_arms, "g_vars.rage.hitbox_arms", m_items );
	item( g_vars.rage.hitbox_legs, "g_vars.rage.hitbox_legs", m_items );
	item( g_vars.rage.teammate, "g_vars.rage.teammate", m_items );
	item( g_vars.rage.dynamic_hitbox, "g_vars.rage.dynamic_hitbox", m_items );
	item( g_vars.rage.primary_hitbox, "g_vars.rage.primary_hitbox", m_items );
	item( g_vars.rage.save_fps, "g_vars.rage.save_fps", m_items );
	item( g_vars.antiaim.enabled, "g_vars.antiaim.enabled", m_items );
	item( g_vars.antiaim.jitter, "g_vars.antiaim.jitter", m_items );
	item( g_vars.antiaim.pitch, "g_vars.antiaim.pitch", m_items );
	item( g_vars.antiaim.yaw, "g_vars.antiaim.yaw", m_items );
	item( g_vars.antiaim.side_switch_key, "g_vars.antiaim.side_switch_key", m_items );
	item( g_vars.antiaim.left_override_key, "g_vars.antiaim.left_override_key", m_items );
	item( g_vars.antiaim.right_override_key, "g_vars.antiaim.right_override_key", m_items );
	item( g_vars.antiaim.backwards_override_key, "g_vars.antiaim.backwards_override_key", m_items );
	item( g_vars.antiaim.reset_override_key, "g_vars.antiaim.reset_override_key", m_items );
	item( g_vars.misc.fakelag.type, "g_vars.misc.fakelag.type", m_items );
	item( g_vars.misc.fakelag.amount, "g_vars.misc.fakelag.amount", m_items );
	item( g_vars.misc.fakelag.lagjump, "g_vars.misc.fakelag.lagjump", m_items );
	item( g_vars.misc.fakelag.enabled, "g_vars.misc.fakelag.enabled", m_items );
	item( g_vars.misc.bhop, "g_vars.misc.bhop", m_items );
	item( g_vars.misc.air_strafe, "g_vars.misc.air_strafe", m_items );
	item( g_vars.misc.thirdperson_dead, "g_vars.misc.thirdperson_dead", m_items );
	item( g_vars.misc.thirdperson, "g_vars.misc.thirdperson", m_items );
	item( g_vars.misc.thirdperson_key, "g_vars.misc.thirdperson_key", m_items );
	item( g_vars.misc.flashlight_key, "g_vars.misc.flashlight_key", m_items );
	item( g_vars.misc.autozeus, "g_vars.misc.autozeus", m_items );
	item( g_vars.misc.log_damage, "g_vars.misc.log_damage", m_items );
	item( g_vars.misc.log_purchases, "g_vars.misc.log_purchases", m_items );
	item( g_vars.misc.nightmode, "g_vars.misc.nightmode", m_items );
	item( g_vars.misc.prop_transparency, "g_vars.misc.prop_transparency", m_items );
	item( g_vars.misc.fast_duck, "g_vars.misc.fast_duck", m_items );
	item( g_vars.misc.dangerzone_menu, "g_vars.visuals.dangerzone_menu", m_items );
	item( g_vars.misc.client_hitboxes, "g_vars.misc.client_hitboxes", m_items );
	item( g_vars.misc.client_hitboxes_duration, "g_vars.visuals.client_hitboxes_duration", m_items );
	item( g_vars.misc.bullet_impacts, "g_vars.visuals.bullet_impacts", m_items );
	item( g_vars.misc.bullet_impacts_duration, "g_vars.visuals.bullet_impacts_duration", m_items );
	item( g_vars.dz.healthshot, "g_vars.dz.healthshot", m_items );
	item( g_vars.dz.drone, "g_vars.dz.drone", m_items );
	item( g_vars.dz.turret, "g_vars.dz.turret", m_items );
	item( g_vars.dz.cash, "g_vars.dz.cash", m_items );
	item( g_vars.dz.safe, "g_vars.dz.safe", m_items );
	item( g_vars.dz.weapon_case, "g_vars.dz.weapon_case", m_items );
	item( g_vars.dz.tool_case, "g_vars.dz.tool_case", m_items );
	item( g_vars.dz.tablet_upgrade, "g_vars.dz.tablet_upgrade", m_items );
	item( g_vars.dz.airdrop, "g_vars.dz.airdrop", m_items );
	item( g_vars.dz.armor, "g_vars.dz.armor", m_items );
	item( g_vars.dz.contract, "g_vars.dz.contract", m_items );
	item( g_vars.dz.parachute, "g_vars.dz.parachute", m_items );
	item( g_vars.dz.ammobox, "g_vars.dz.ammobox", m_items );
	item( g_vars.dz.jammer, "g_vars.dz.jammer", m_items );
	item( g_vars.dz.melee_weapon, "g_vars.dz.melee_weapon", m_items );
	item( g_vars.dz.breach_charge, "g_vars.dz.breach_charge", m_items );

	return true;
}

bool c_config::save( const std::string &file ) {
	try {
		std::ofstream output_file = std::ofstream( m_directory + "/" + file );

		if( !output_file.good() )
			return false;

		for( auto &item : m_items )
			save_item( item );

		output_file << std::setw( 4 ) << json << std::endl;
		output_file.close();

		return true;
	} 
	catch( std::ofstream::failure &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_WARN, "Failed to save the default profile. Ignoring this warning will most likely result in future profiles not being saved correctly.\n\n%s", ex.what( ) );
		return false;
	}
}

bool c_config::reset( const std::string &file ) {
	std::ofstream output_file = std::ofstream( m_directory + "/" + file );

	if( !output_file.good( ) )
		return false;

	for( auto &item : m_items )
		reset_item( item );

	output_file << std::setw( 4 ) << json << std::endl;
	output_file.close( );

	return true;
}

bool c_config::load( const std::string &file ) {
	std::ifstream input_file = std::ifstream( m_directory + "/" + file );
	if( !input_file.good( ) )
		return false;

	try {
		json << input_file;
	}
	catch( const std::exception &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT2( _CRT_ERROR, "Failed to load %s profile, profile could be corrupted. Ignoring this error may prevent you from loading profiles.\n\n%s", file.c_str( ), ex.what( ) );

		input_file.close( );
		return false;
	}

	for( auto &item : m_items )
		assign_item( item );

	input_file.close( );

	return true;
}

void c_config::remove( const std::string &file ) const {
	std::string path = m_directory  + "/" + file;
	std::remove( path.c_str( ) );
}

std::vector< std::string > c_config::get_configs( ) const {
	std::vector< std::string > output{ };

	for( auto &file_path : std::experimental::filesystem::directory_iterator( m_directory ) ) {
		if( file_path.path( ).string( ).empty( ) )
			continue;

		if( file_path.path( ).string( ).find('.') != std::string::npos )
			continue;

		output.emplace_back( file_path.path( ).string( ).erase( 0, m_directory.length( ) + 1 )  );
	}

	return output;
}

bool c_config::import_from_clipboard( const std::string &file ) {
	const auto get_clipboard_data = [ ]( ) -> std::string {
		OpenClipboard( nullptr );

		HANDLE handle = GetClipboardData( CF_TEXT );
		const auto data = static_cast< char* >( GlobalLock( handle ) );

		std::string text( data );
		GlobalUnlock( handle );
		CloseClipboard( );

		return text;
	};

	const std::string clipboard = get_clipboard_data( );

	std::ofstream output = std::ofstream( m_directory + "/" + file );
	if( !output.good( ) )
		return false;
	
	// clear our config
	output.clear( );

	// write clipboard data to the file
	output << clipboard;

	// close the file after writing
	output.close( );

	// re-load the imported config
	load( file );

	return true;
}

void c_config::export_to_clipboard( const std::string &file ) const {
	std::ifstream input_file = std::ifstream( m_directory + "/" + file );
	std::string str( ( std::istreambuf_iterator< char >( input_file ) ), std::istreambuf_iterator< char >( ) );

	const char* output = str.c_str( );
	const size_t len = strlen( output ) + 1;

	HGLOBAL mem = GlobalAlloc( GMEM_MOVEABLE, len );
	memcpy( GlobalLock( mem ), output, len );

	GlobalUnlock( mem );
	OpenClipboard( nullptr );
	EmptyClipboard( );
	SetClipboardData( CF_TEXT, mem );
	CloseClipboard( );
}
