#include "menu.h"
#include <mutex>

#include "../features/misc/misc.h"
#include "../features/chams/chams.h"
#include "../features/chaiscript/chai_wrapper.hpp"
#include "../features/chaiscript/chai_console.hpp"
#include <experimental/filesystem>

using namespace controls;

constexpr int default_x_pos = 27;
constexpr int default_y_pos = 10;

OSHGui::Drawing::Color m_primary_color = OSHGui::Drawing::Color::FromARGB( 255, 113, 59, 198 );

c_menu::c_menu( )
	: m_control_x_pos{ default_x_pos }, m_control_y_pos{ default_y_pos } { }

void c_menu::init( ) {
	m_form = std::static_pointer_cast< OSHGui::Form >( std::make_shared< c_main_form >( ) );
	m_form->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );

	m_dangerzone_form = std::static_pointer_cast< OSHGui::Form >( std::make_shared< c_dangerzone_form >( ) );
	m_dangerzone_form->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );

	m_chaiscript_form = std::static_pointer_cast< OSHGui::Form >( std::make_shared< c_chaiscript_form >( ) );
	m_chaiscript_form->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );

	g_renderer.get_instance( )->Run( m_form );

	m_dangerzone_form->SetEnabled( true );
	m_dangerzone_form->SetVisible( g_vars.misc.dangerzone_menu );
	m_dangerzone_form->SetLocation( 50, 500 );

	m_chaiscript_form->SetEnabled( true );
	m_chaiscript_form->SetVisible( g_vars.misc.chaiscript_enabled );
	m_chaiscript_form->SetLocation( 50, 500 );

	m_form->Show( m_dangerzone_form );
	m_form->Show( m_chaiscript_form );

	g_renderer.get_instance( )->Enable( );
}

/* CONTROL PLACEMENT */
void c_menu::set_x_pos( int x ) {
	m_control_x_pos = x;
}

void c_menu::set_y_pos( int y ) {
	m_control_y_pos = y;
}

int c_menu::get_x_pos( ) const {
	return m_control_x_pos;
}

int c_menu::get_y_pos( ) const {
	return m_control_y_pos;
}

void c_menu::push_x_pos( int x ) {
	m_control_x_pos += x;
}

void c_menu::push_y_pos( int y ) {
	m_control_y_pos += y;
}

void c_main_form::init_component( ) {
	SetSize( OSHGui::Drawing::SizeI( 600, 400 ) );
	init_tabs( );

	ragebot_tab( );
	visuals_tab( );
	misc_tab( );
	config_tab( );
	playerlist_tab( );
}

void c_main_form::init_tabs( ) {
	m_tab_control = new OSHGui::TabControl( );

	// create pages.
	for( int i = 0; i < PAGE_MAX; ++i )
		m_pages.push_back( std::make_shared< OSHGui::TabPage >( ) );

	m_tab_control->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );

	// set page titles.
	m_pages.at( PAGE_RAGEBOT )->SetText( "Ragebot" );
	m_pages.at( PAGE_VISUALS )->SetText( "Visuals" );
	m_pages.at( PAGE_MISC )->SetText( "Miscellaneous" );
	m_pages.at( PAGE_CONFIG )->SetText( "Configuration" );
	m_pages.at( PAGE_PLAYERLIST )->SetText( "Skinchanger" );

	// set tab style.
	m_tab_control->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	m_tab_control->SetSize( 576, 380 );
	m_tab_control->SetBackColor( OSHGui::Drawing::Color::FromRGB( 32, 32, 38 ) );
	m_tab_control->SetLocation( 6, -15 );
	m_tab_control->SetButtonWidth( 116 );

	// add all pages to tab control.
	for( auto &item : m_pages )
		m_tab_control->AddTabPage( item.get( ) );

	AddControl( m_tab_control );
}

void c_main_form::playerlist_tab( ) { }

std::vector< std::string > get_antiaims( const std::string dir, std::vector< std::string > &vec ) {
	std::vector< std::string > str_list = vec;
	for( auto &file_path : std::experimental::filesystem::directory_iterator( dir ) ) {
		if( file_path.path( ).string( ).empty( ) )
			continue;

		if( file_path.path( ).string( ).find( '.' ) != std::string::npos )
			continue;

		std::string file_name = file_path.path( ).string( ).erase( 0, dir.length( ) + 1 );
		str_list.push_back( file_name );
	}

	return str_list;
}

void c_main_form::ragebot_tab( ) {
	auto aimbot_tab = new OSHGui::TabControl( );
	aimbot_tab->SetSize( 241, 310 );
	aimbot_tab->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	aimbot_tab->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	aimbot_tab->SetButtonWidth( 121 );

	auto aimbot_page = new OSHGui::TabPage( );
	aimbot_page->SetText( "General" );
	aimbot_page->SetBorder( false );

	auto target_page = new OSHGui::TabPage( );
	target_page->SetText( "Target" );
	target_page->SetBorder( false );

	aimbot_tab->AddTabPage( aimbot_page );
	aimbot_tab->AddTabPage( target_page );

	auto aimbot_groupbox = new c_groupbox( "Ragebot", 17, 6, 260, 334 );
	aimbot_groupbox->AddControl( aimbot_tab );

	g_menu.set_x_pos( 19 );

	auto enabled = new c_checkbox( "Enabled", aimbot_page, &g_vars.rage.enabled );
	//auto activation_hotkey = new c_hotkey( "Activation key", aimbot_page, &g_vars.visuals.activation_key, aimbot_groupbox->GetWidth() - 15 );
	auto teammate = new c_checkbox( "Teammate", aimbot_page, &g_vars.rage.teammate );
	auto silent = new c_checkbox( "Silent", aimbot_page, &g_vars.rage.silent );
	auto autofire = new c_checkbox( "Auto fire", aimbot_page, &g_vars.rage.auto_fire );
	auto autostop = new c_checkbox( "Quickstop", aimbot_page, &g_vars.rage.autostop );
	auto autoscope = new c_combo( "Automatic scope", { "Off", "Always on", "Hitchance fail" }, aimbot_page, 4, &g_vars.rage.autoscope, aimbot_groupbox->GetWidth( ) - 15 );
	auto save_fps = new c_checkbox( "Check bounding box", aimbot_page, &g_vars.rage.save_fps );
	auto bodyaim_lethal = new c_checkbox( "Bodyaim if lethal", aimbot_page, &g_vars.rage.bodyaim_lethal );
	auto bodyaim_prefer = new c_checkbox( "Prefer bodyaim", aimbot_page, &g_vars.rage.prefer_bodyaim );

	g_menu.set_y_pos( 10 );
	g_menu.set_x_pos( 38 );
	auto selection = new c_combo( "Target selection", { "Highest damage", "Closest distance", "Cycle" }, target_page, 4, &g_vars.rage.target_selection, aimbot_groupbox->GetWidth( ) - 15 );
	auto head_check = new c_checkbox( "Head", target_page, &g_vars.rage.hitbox_head );
	g_menu.set_x_pos( 125 );
	g_menu.push_y_pos( -18 );
	auto body_check = new c_checkbox( "Body", target_page, &g_vars.rage.hitbox_pelvis );
	g_menu.set_x_pos( 38 );
	auto arms_check = new c_checkbox( "Arms", target_page, &g_vars.rage.hitbox_arms );
	g_menu.set_x_pos( 125 );
	g_menu.push_y_pos( -18 );
	auto legs_check = new c_checkbox( "Legs", target_page, &g_vars.rage.hitbox_legs );
	auto primaryhitbox = new c_combo( "Primary hitbox", { "Head", "Neck", "Pelvis", "Stomach", "Thorax" }, target_page, 5, &g_vars.rage.primary_hitbox, aimbot_groupbox->GetWidth( ) - 15 );
	g_menu.set_x_pos( 38 );
	auto dynamic_hitbox = new c_checkbox( "Dynamic scanning", target_page, &g_vars.rage.dynamic_hitbox );
	auto nigger = new c_checkbox( "Selectively multipoint records", target_page, &g_vars.rage.selectively_multipoint_records );
	auto multipoint_head_check = new c_checkbox( "Scan head", target_page, &g_vars.rage.head );
	g_menu.set_x_pos( 125 );
	g_menu.push_y_pos( -18 );
	auto multipoint_body_check = new c_checkbox( "Scan body", target_page, &g_vars.rage.pelvis );
	g_menu.set_x_pos( 38 );
	auto multipoint_arms_check = new c_checkbox( "Scan arms", target_page, &g_vars.rage.arms );
	g_menu.set_x_pos( 125 );
	g_menu.push_y_pos( -18 );
	auto multipoint_legs_check = new c_checkbox( "Scan legs", target_page, &g_vars.rage.legs );
	g_menu.set_x_pos( 19 );
	auto pointscame = new c_slider( "Multipoint scale", target_page, 0.f, 100.f, &g_vars.rage.pointscale, 0, 100.f, "%" );
	auto hitchance = new c_slider( "Hitchance", target_page, 0, 100, &g_vars.rage.hitchance, 0, "%" );
	auto min_dmg = new c_slider( "Minimum damage", target_page, 0, 100, &g_vars.rage.min_dmg, 0, "" );

	auto antiaim_tab = new OSHGui::TabControl( );
	antiaim_tab->SetSize( 241, 310 );
	antiaim_tab->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	antiaim_tab->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	antiaim_tab->SetButtonWidth( 121 );

	auto antiaim_page = new OSHGui::TabPage( );
	antiaim_page->SetText( "Anti-aim" );
	antiaim_page->SetBorder( false );

	auto fakelag_page = new OSHGui::TabPage( );
	fakelag_page->SetText( "Fake lag" );
	fakelag_page->SetBorder( false );

	antiaim_tab->AddTabPage( antiaim_page );
	antiaim_tab->AddTabPage( fakelag_page );

	auto antiaim_groupbox = new c_groupbox( "Anti-aim", aimbot_groupbox->GetRight( ) + 19, 6, 260, 334 );
	antiaim_groupbox->AddControl( antiaim_tab );
	g_menu.set_x_pos( 19 );

	std::vector< std::string > output{};

	std::vector< std::string > yaw_vec = { "Off", "180" };
	std::vector< std::string > pitch_vec = { "Off", "Default" };
	yaw_vec = get_antiaims( "profiles//aa//y", yaw_vec );
	pitch_vec = get_antiaims( "profiles//aa//x", pitch_vec );

	chai_manager::chai_aa_yaw = yaw_vec;
	chai_manager::chai_aa_pitch = pitch_vec;

	auto aa_enabled = new c_checkbox( "Enabled", antiaim_page, &g_vars.antiaim.enabled );
	auto pitch = new c_combo( "Pitch", pitch_vec, antiaim_page, 2, &g_vars.antiaim.pitch, antiaim_groupbox->GetWidth( ) - 15 );
	auto yaw = new c_combo( "Yaw", yaw_vec, antiaim_page, 2, &g_vars.antiaim.yaw, antiaim_groupbox->GetWidth( ) - 15 );
	auto switch1 = new c_hotkey( "Switch AA", antiaim_page, &g_vars.antiaim.side_switch_key, antiaim_groupbox->GetWidth( ) - 15 );
	//auto jitter = new c_slider( "", antiaim_page, -60.f, 60.f, &g_vars.antiaim.jitter, 0, 0.f, u8"�" );

	g_menu.set_y_pos( 10 );
	auto fakelag_check = new c_checkbox( "Enabled", fakelag_page, &g_vars.misc.fakelag.enabled );
	auto fakelag_type = new c_combo( "Type", { "Maximum", "Adaptive" }, fakelag_page, 4, &g_vars.misc.fakelag.type, antiaim_groupbox->GetWidth( ) - 15 );
	auto fakelag_on_jump_check = new c_combo( "Air", { "Default", "On land", "On jump" }, fakelag_page, 3, &g_vars.misc.fakelag.lagjump, antiaim_groupbox->GetWidth( ) - 15 );
	auto fakelag_choke_check = new c_slider( "Choke", fakelag_page, 0, 14, &g_vars.misc.fakelag.amount, 0 );

	m_pages.at( PAGE_RAGEBOT )->AddControl( aimbot_groupbox );
	m_pages.at( PAGE_RAGEBOT )->AddControl( antiaim_groupbox );
}

void c_main_form::visuals_tab( ) {
	auto player_tab = new OSHGui::TabControl( );
	player_tab->SetSize( 241, 310 );
	player_tab->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	player_tab->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	player_tab->SetButtonWidth( 81 );

	auto generic_esp_page = new OSHGui::TabPage( );
	generic_esp_page->SetText( "General" );
	generic_esp_page->SetBorder( false );

	auto player_chams_page = new OSHGui::TabPage( );
	player_chams_page->SetText( "Models" );
	player_chams_page->SetBorder( false );

	auto extra_esp_page = new OSHGui::TabPage( );
	extra_esp_page->SetText( "Extra" );
	extra_esp_page->SetBorder( false );

	player_tab->AddTabPage( generic_esp_page );
	player_tab->AddTabPage( player_chams_page );
	player_tab->AddTabPage( extra_esp_page );

	// player esp groupbox.
	auto player_esp_groupbox = new c_groupbox( "Player ESP", 17, 6, 260, 334 );
	player_esp_groupbox->AddControl( player_tab );

	g_menu.set_x_pos( 19 );

	auto activation_type_combo = new c_combo( "Activation type", { "Always", "On-key", "Toggle" }, generic_esp_page, 3, &g_vars.visuals.activation_type, player_esp_groupbox->GetWidth( ) - 15 );
	auto activation_hotkey = new c_hotkey( "Activation key", generic_esp_page, &g_vars.visuals.activation_key, player_esp_groupbox->GetWidth( ) - 15 );
	auto dormacy = new c_checkbox( "Dormant", generic_esp_page, &g_vars.visuals.dormancy_fade );
	auto bbox_check = new c_checkbox( "Bounding box", generic_esp_page, &g_vars.visuals.bbox );
	auto name_check = new c_checkbox( "Name", generic_esp_page, &g_vars.visuals.name );
	auto weapon_check = new c_checkbox( "Weapon", generic_esp_page, &g_vars.visuals.weapon );
	auto health_check = new c_checkbox( "Health bar", generic_esp_page, &g_vars.visuals.healthbar );
	auto ammo_check = new c_checkbox( "Ammo bar", generic_esp_page, &g_vars.visuals.ammo_bar );
	auto flag_check = new c_checkbox( "Flags", generic_esp_page, &g_vars.visuals.flags );
	auto money_check = new c_checkbox( "Money", generic_esp_page, &g_vars.visuals.money );
	auto flashed_check = new c_checkbox( "Flashed", generic_esp_page, &g_vars.visuals.flash_bar );

	g_menu.set_y_pos( 10 );
	auto enabled_chams = new c_checkbox( "Enabled", player_chams_page, &g_vars.visuals.chams.enabled );
	auto chams_type = new c_combo( "Shader", { "Vertex lit", "Unlit", "Modulate" }, player_chams_page, 3, &g_vars.visuals.chams.type, player_esp_groupbox->GetWidth( ) - 15 );
	auto chams_twopass = new c_checkbox( "Two-pass", player_chams_page, &g_vars.visuals.chams.twopass );
	auto chams_teammates = new c_checkbox( "Teammate", player_chams_page, &g_vars.visuals.chams.teammates );
	auto chams_local = new c_checkbox( "Local player", player_chams_page, &g_vars.visuals.chams.local );
	auto chams_blend = new c_checkbox( "Blend when scoped", player_chams_page, &g_vars.visuals.chams.blend_scope );
	auto chams_alpha = new c_slider( "Transparency", player_chams_page, 0.f, 100.f, &g_vars.visuals.chams.alpha, 0, 100.f, "%" );

	auto chams_reflect = new c_slider( "Reflectivity", player_chams_page, 0.f, 100.f, &g_vars.visuals.chams.reflectivity, 1, 50.f, "%" );
	chams_reflect->GetValueChangedEvent( ) += OSHGui::ValueChangedEventHandler( []( Control *sender ) {
		g_chams.m_kv_needs_update = true;
	} );

	auto chams_luminance = new c_slider( "Shine", player_chams_page, 0.f, 100.f, &g_vars.visuals.chams.luminance, 1, 25.f, "%" );
	chams_luminance->GetValueChangedEvent( ) += OSHGui::ValueChangedEventHandler( []( Control *sender ) {
		g_chams.m_kv_needs_update = true;
	} );

	auto local_color = new c_colorpicker( player_chams_page, chams_local, g_vars.visuals.chams.local_col );
	auto hid_color = new c_colorpicker( player_chams_page, chams_twopass, g_vars.visuals.chams.hid_color );
	auto vis_color = new c_colorpicker( player_chams_page, chams_type, g_vars.visuals.chams.vis_color );

	g_menu.set_y_pos( 10 );
	auto filter_combo = new c_combo( "Filter", { "Everyone", "Enemy only" }, extra_esp_page, 3, &g_vars.visuals.filter, player_esp_groupbox->GetWidth( ) - 15 );
	auto speclist_check = new c_checkbox( "Spectator list", extra_esp_page, &g_vars.visuals.extra.speclist );
	auto glow_check = new c_checkbox( "Glow", extra_esp_page, &g_vars.visuals.glow );
	//auto aa_manual = new c_checkbox( "Anti-aim manual direction", extra_esp_page, &g_vars.visuals.extra.antiaim_direction );
	auto points_check = new c_checkbox( "Scan points", extra_esp_page, &g_vars.visuals.extra.points );

	auto flashed_color = new c_colorpicker( generic_esp_page, flashed_check, g_vars.visuals.flash_bar_color );
	auto money_color = new c_colorpicker( generic_esp_page, money_check, g_vars.visuals.money_color );
	auto flag_color = new c_colorpicker( generic_esp_page, flag_check, g_vars.visuals.flag_color );
	auto ammo_color = new c_colorpicker( generic_esp_page, ammo_check, g_vars.visuals.ammo_bar_color );
	auto weapon_color = new c_colorpicker( generic_esp_page, weapon_check, g_vars.visuals.weapon_color );
	auto name_color = new c_colorpicker( generic_esp_page, name_check, g_vars.visuals.name_color );
	auto box_color = new c_colorpicker( generic_esp_page, bbox_check, g_vars.visuals.box_color );

	auto glow_color = new c_colorpicker( extra_esp_page, glow_check, g_vars.visuals.glow_color );

	//auto manualdirection_color = new c_colorpicker( extra_esp_page, aa_manual, g_vars.visuals.extra.antiaim_direction_color );

	// other visuals groupbox.
	auto world_tab = new OSHGui::TabControl( );
	world_tab->SetSize( 241, 100 );
	world_tab->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	world_tab->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	world_tab->SetButtonWidth( 81 );

	auto generic_world = new OSHGui::TabPage( );
	generic_world->SetText( "General" );
	generic_world->SetBorder( false );

	auto extra_removals = new OSHGui::TabPage( );
	extra_removals->SetText( "Removals" );
	extra_removals->SetBorder( false );

	auto extra_world = new OSHGui::TabPage( );
	extra_world->SetText( "Extra" );
	extra_world->SetBorder( false );

	world_tab->AddTabPage( generic_world );
	world_tab->AddTabPage( extra_removals );
	world_tab->AddTabPage( extra_world );

	auto other_esp_groupbox = new c_groupbox( "World ESP", player_esp_groupbox->GetRight( ) + 19, 6, 259, 334 );

	g_menu.set_x_pos( 19 );

	auto dropped_weapon_check = new c_checkbox( "Dropped weapons", generic_world, &g_vars.visuals.dropped_weapons );
	auto dropped_ammo_check = new c_checkbox( "Dropped weapons ammo", generic_world, &g_vars.visuals.dropped_ammo );
	auto objective_check = new c_checkbox( "Objective", generic_world, &g_vars.visuals.objectives );
	auto grenade_check = new c_checkbox( "Projectiles", generic_world, &g_vars.visuals.projectiles );
	auto grenadepred_check = new c_checkbox( "Projectile prediction", generic_world, &g_vars.visuals.grenade_pred );
	auto hitmarker = new c_checkbox( "Hitmarker", generic_world, &g_vars.visuals.hitmarker );
	auto extraflags = new c_checkbox( "Flags", generic_world, &g_vars.visuals.extra.misc_flags );
	auto impact_beams = new c_checkbox( "Bullet impact beams", generic_world, &g_vars.visuals.impact );

	auto overridefov_slider = new c_slider( "Camera FOV", generic_world, 0, 180, &g_vars.visuals.effects.camera_fov, 90, "" );
	auto weaponfov_slider = new c_slider( "Weapon FOV", generic_world, 0, 180, &g_vars.visuals.effects.weapon_fov, 64, "" );

	g_menu.set_y_pos( 10 );
	auto remove_flash = new c_checkbox( "Flash", extra_removals, &g_vars.visuals.misc.no_flash );
	auto remove_fog = new c_checkbox( "Fog", extra_removals, &g_vars.visuals.misc.fog );
	auto remove_scope_zoom = new c_checkbox( "Scope zoom", extra_removals, &g_vars.visuals.misc.remove_scope_zoom );
	auto remove_scope = new c_combo( "Scope overlay", { "None", "Static", "Dynamic" }, extra_removals, 4, &g_vars.visuals.misc.remove_scope, other_esp_groupbox->GetWidth( ) - 15 );
	auto remove_smoke_type = new c_combo( "Smoke", { "None", "Remove", "Wireframe" }, extra_removals, 3, &g_vars.visuals.misc.remove_smoke, other_esp_groupbox->GetWidth( ) - 15 );
	auto nightmode = new c_slider( "World brightness", extra_removals, 0, 100, &g_vars.misc.nightmode, 100, "%" );
	nightmode->GetValueChangedEvent( ) += OSHGui::ValueChangedEventHandler( []( Control *sender ) {
		//g_misc.nightmode( );
	} );

	auto translucent_props = new c_slider( "Prop transparency", extra_removals, 0, 100, &g_vars.misc.prop_transparency, 100, "%" );
	translucent_props->GetValueChangedEvent( ) += OSHGui::ValueChangedEventHandler( []( Control *sender ) {
		g_cl.m_should_update_materials = true;
	} );
	//auto remove_blue = new c_checkbox( "Aug scope blur", removals_page, &g_vars.visuals.misc.remove_blur );

	auto scope_color = new c_colorpicker( extra_removals, remove_scope, g_vars.visuals.misc.scope_color );

	g_menu.set_y_pos( 10 );
	auto visualize_spread = new c_combo( "Visualize spread", { "None", "Circle", "Dots" }, extra_world, 3, &g_vars.visuals.visualize_spread, other_esp_groupbox->GetWidth( ) - 15 );
	auto client_hitboxes = new c_checkbox( "Client hitboxes", extra_world, &g_vars.misc.client_hitboxes );
	auto client_hitboxes_duration = new c_slider( "", extra_world, 0.f, 5.f, &g_vars.misc.client_hitboxes_duration, 0, 2.f, "s" );
	auto bullet_impacts = new c_checkbox( "Bullet impacts", extra_world, &g_vars.misc.bullet_impacts );
	auto bullet_impacts_duration = new c_slider( "", extra_world, 0.f, 5.f, &g_vars.misc.bullet_impacts_duration, 0, 4.f, "s" );
	auto flashlight_hotkey = new c_hotkey( "Flashlight key", extra_world, &g_vars.misc.flashlight_key, other_esp_groupbox->GetWidth( ) - 15 );

	//auto impact_color = new c_colorpicker( extra_world, impact_beams, g_vars.visuals.impact_color );
	auto visualize_spread_color = new c_colorpicker( extra_world, visualize_spread, g_vars.visuals.visualize_spread_color );
	auto dropped_ammo_color = new c_colorpicker( generic_world, dropped_ammo_check, g_vars.visuals.dropped_ammo_color );

	other_esp_groupbox->AddControl( world_tab );

	m_pages.at( PAGE_VISUALS )->AddControl( other_esp_groupbox );
	m_pages.at( PAGE_VISUALS )->AddControl( player_esp_groupbox );
}

void c_main_form::misc_tab( ) {
	auto misc_tab = new OSHGui::TabControl( );
	misc_tab->SetSize( 241, 310 );
	misc_tab->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	misc_tab->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	misc_tab->SetButtonWidth( 121 );

	auto generic_misc_page = new OSHGui::TabPage( );
	generic_misc_page->SetText( "General" );
	generic_misc_page->SetBorder( false );

	auto extra_misc_page = new OSHGui::TabPage( );
	extra_misc_page->SetText( "Extra" );
	extra_misc_page->SetBorder( false );

	misc_tab->AddTabPage( generic_misc_page );
	misc_tab->AddTabPage( extra_misc_page );

	auto general_groupbox = new c_groupbox( "General", 17, 6, 260, 334 );
	general_groupbox->AddControl( misc_tab );

	g_menu.set_x_pos( 19 );
	auto bhop = new c_checkbox( "Bunny hop", generic_misc_page, &g_vars.misc.bhop );
	auto air_strafe = new c_checkbox( "Air strafe", generic_misc_page, &g_vars.misc.air_strafe );
	auto thirdperson_check = new c_checkbox( "Thirdperson", generic_misc_page, &g_vars.misc.thirdperson );
	auto activation_hotkey = new c_hotkey( "Activation key", generic_misc_page, &g_vars.misc.thirdperson_key, general_groupbox->GetWidth( ) - 15 );
	auto thirdpersondead = new c_checkbox( "Force thirdperson while spectating", generic_misc_page, &g_vars.misc.thirdperson_dead );
	auto thirdpersongrenade = new c_checkbox( "Disable thirdperson on grenade", generic_misc_page, &g_vars.misc.thirdperson_grenade );
	auto autozeus = new c_checkbox( "Automatic zeus", generic_misc_page, &g_vars.misc.autozeus );
	auto radar = new c_checkbox( "Radar", generic_misc_page, &g_vars.visuals.radar );
	auto fast_duck = new c_checkbox( "Fast duck", generic_misc_page, &g_vars.misc.fast_duck );
	auto dangerzone_check = new c_checkbox( "Danger Zone menu", generic_misc_page, &g_vars.misc.dangerzone_menu );
	dangerzone_check->GetCheckedChangedEvent( ) += OSHGui::CheckedChangedEventHandler( [ & ]( Control *sender ) {
		g_menu.m_dangerzone_form->SetVisible( g_vars.misc.dangerzone_menu );
	} );

	auto chaiscript_check = new c_checkbox( "ChaiScript menu", generic_misc_page, &g_vars.misc.chaiscript_enabled );
	chaiscript_check->GetCheckedChangedEvent( ) += OSHGui::CheckedChangedEventHandler( [&]( Control *sender ) {
		g_menu.m_chaiscript_form->SetVisible( g_vars.misc.chaiscript_enabled );
	} );

	auto sounds = new c_sound_combo( "Hitsound", { "None" }, generic_misc_page, 5, &g_vars.misc.hitmarker_sound, general_groupbox->GetWidth( ) - 15 );

	const auto get_sound_files = [ & ]( ) {
		std::vector< std::string > names = {};
		std::string dir = "hitsounds";

		for( auto &file_path : std::experimental::filesystem::directory_iterator( dir ) ) {
			if( !file_path.path( ).string( ).empty( ) ) {
				if( file_path.path( ).string( ).find( ".wav" ) != std::string::npos )
					names.emplace_back( file_path.path( ).string( ).erase( 0, dir.length( ) + 1 ) );
			}
		}

		for( auto &item : names ) {
			std::string final = item;

			if( item.length( ) > 17 )
				final = item.substr( 0, 17 ) + "...";

			// reinit list view items.
			for( int i = 0; i < sounds->GetItemsCount( ); i++ ) {
				const OSHGui::ListItem *list_item = sounds->GetItem( i );
				if( !list_item )
					continue;

				if( final == list_item->GetItemText( ) )
					sounds->RemoveItem( i );
			}

			sounds->AddItem( final );
		}
	};

	get_sound_files( );

	g_menu.push_y_pos( 40 );

	auto log_groupbox = new c_groupbox( "Logging", general_groupbox->GetRight( ) + 19, 6, 259, 64 );
	auto log_dmg = new c_checkbox( "Log damage", log_groupbox, &g_vars.misc.log_damage );
	auto log_purchases = new c_checkbox( "Log purchases", log_groupbox, &g_vars.misc.log_purchases );

	auto other_groupbox = new c_groupbox( "Other", general_groupbox->GetRight( ) + 19, log_groupbox->GetBottom( ) + 19, 259, 126 );

	m_pages.at( PAGE_MISC )->AddControl( general_groupbox );
	m_pages.at( PAGE_MISC )->AddControl( log_groupbox );
	m_pages.at( PAGE_MISC )->AddControl( other_groupbox );
}

void c_main_form::config_tab( ) {
	// profile groupbox.
	auto config_groupbox = new c_groupbox( "Profiles", 17, 6, 260, 334 );

	// list index.
	static int index = 0;

	// list view.
	auto list = new OSHGui::ListBox( );
	list->SetSize( 234, 306 );
	list->SetLocation( config_groupbox->GetWidth( ) / 2 - list->GetWidth( ) / 2 - 3, 7 );
	//list->ExpandSizeToShowItems( 22 );
	list->SetAutoScrollEnabled( true );
	list->SetFont( g_renderer.m_fonts.at( 0 ) );

	// get index from event.
	list->GetSelectedIndexChangedEvent( ) += OSHGui::SelectedIndexChangedEventHandler( [ list ]( Control *sender ) {
		index = list->GetSelectedIndex( );
	} );

	// items.
	static std::vector< std::string > items = g_configs.get_configs( );

	// fill list with configs.
	for( auto const &item : items )
		list->AddItem( item );

	// add list to groupbox.
	config_groupbox->AddControl( list );

	m_pages.at( PAGE_CONFIG )->AddControl( config_groupbox );

	auto config2_groupbox = new c_groupbox( "Configurations", config_groupbox->GetRight( ) + 19, 6, 259, 334 );

	// config name textbox.
	auto name_textbox = new c_text_box( "", config2_groupbox );

	// new button.
	auto button_new = new c_button( "New", config2_groupbox );
	button_new->GetClickEvent( ) += OSHGui::ClickEventHandler( [ this, name_textbox, list ]( Control *sender ) {
		if( name_textbox->GetText( ).empty( ) ) {
			OSHGui::MessageBox::Show( "You must set a name." );
			return;
		}

		if( name_textbox->GetText( ).length( ) > 25 ) {
			OSHGui::MessageBox::Show( "Profile name cannot exceed 25 characters in length." );
			return;
		}

		if( items.size( ) > 100 ) {
			OSHGui::MessageBox::Show( "You cannot create anymore profiles." );
			return;
		}

		if( name_textbox->GetText( ) == "default" ) {
			OSHGui::MessageBox::Show( "You cannot replace the default profile." );
			return;
		}

		g_configs.save( name_textbox->GetText( ) );
		g_configs.load( name_textbox->GetText( ) );

		items = g_configs.get_configs( );

		for( auto &item : items ) {
			// reinit list view items.
			for( int i = 0; i < list->GetItemsCount( ); i++ ) {
				auto list_item = list->GetItem( i );
				if( !list_item )
					continue;

				if( item == list_item->GetItemText( ) ) {
					list->RemoveItem( i );
					continue;
				}
			}

			list->AddItem( item );

			// dumb hack to show the form without having to switch to misc tab.
			g_menu.m_dangerzone_form->SetVisible( g_vars.misc.dangerzone_menu );

			g_cl.m_should_update_materials = true;
		}
	} );
	config2_groupbox->AddControl( button_new );

	// save button.
	auto button_save = new c_button( "Save", config2_groupbox );
	button_save->GetClickEvent( ) += OSHGui::ClickEventHandler( [ this, list ]( Control *sender ) {
		if( items.empty( ) || list->GetSelectedIndex( ) == -1 )
			return;

		OSHGui::MessageBox::ShowDialog( "Saving will override any changes, are you sure you want to save?", "", OSHGui::MessageBoxButtons::YesNo, [ this, list ]( OSHGui::DialogResult result ) {
			if( result == OSHGui::DialogResult::Yes )
				g_configs.save( items.at( index ) );
		} );

	} );
	config2_groupbox->AddControl( button_save );

	// load button.
	auto button_load = new c_button( "Load", config2_groupbox );
	button_load->GetClickEvent( ) += OSHGui::ClickEventHandler( [ this, list ]( Control *sender ) {
		if( items.empty( ) || list->GetSelectedIndex( ) == -1 )
			return;

		OSHGui::MessageBox::ShowDialog( "Are you sure you want to load the selected profile?", "", OSHGui::MessageBoxButtons::YesNo, [ this ]( OSHGui::DialogResult result ) {
			if( result == OSHGui::DialogResult::Yes ) {
				g_configs.load( items.at( index ) );

				// dumb hack to show the form without having to switch to misc tab.
				g_menu.m_dangerzone_form->SetVisible( g_vars.misc.dangerzone_menu );

				g_cl.m_should_update_materials = true;
			}
		} );

	} );
	config2_groupbox->AddControl( button_load );

	auto button_reset = new c_button( "Reset", config2_groupbox );
	button_reset->GetClickEvent( ) += OSHGui::ClickEventHandler( [ this, list ]( Control *sender ) {
		if( items.empty( ) || list->GetSelectedIndex( ) == -1 )
			return;

		OSHGui::MessageBox::ShowDialog( "Are you sure you want to reset the selected profile?", "", OSHGui::MessageBoxButtons::YesNo, [ this, list ]( OSHGui::DialogResult result ) {
			if( result == OSHGui::DialogResult::Yes ) {
				g_configs.reset( items.at( index ) );
				g_configs.load( items.at( index ) );

				// dumb hack to show the form without having to switch to misc tab.
				g_menu.m_dangerzone_form->SetVisible( g_vars.misc.dangerzone_menu );

				g_cl.m_should_update_materials = true;
			}

		} );

	} );
	config2_groupbox->AddControl( button_reset );

	// delete button.
	auto button_delete = new c_button( "Delete", config2_groupbox );
	button_delete->GetClickEvent( ) += OSHGui::ClickEventHandler( [ this, list ]( Control *sender ) {
		if( items.empty( ) || list->GetSelectedIndex( ) == -1 )
			return;

		if( items.at( index ) == "default" ) {
			OSHGui::MessageBox::Show( "You cannot delete the default profile." );
			return;
		}

		OSHGui::MessageBox::ShowDialog( "Are you sure you want to delete the selected profile?", "", OSHGui::MessageBoxButtons::YesNo, [ list ]( OSHGui::DialogResult result ) {
			if( result == OSHGui::DialogResult::Yes ) {
				g_configs.remove( items.at( index ) );
				list->RemoveItem( index );
				items = g_configs.get_configs( );

				for( auto &item : items ) {
					// reinit list view items.
					for( int i = 0; i < list->GetItemsCount( ); i++ ) {
						auto list_item = list->GetItem( i );
						if( !list_item )
							continue;

						if( item == list_item->GetItemText( ) ) {
							list->RemoveItem( i );
							continue;
						}
					}

					list->AddItem( item );
				}
			}
		} );
	} );
	config2_groupbox->AddControl( button_delete );

	auto button_import = new c_button( "Import", config2_groupbox );
	button_import->GetClickEvent( ) += OSHGui::ClickEventHandler( [ this, list ]( Control *sender ) {
		if( items.empty( ) || list->GetSelectedIndex( ) == -1 )
			return;

		if( g_configs.import_from_clipboard( items.at( index ) ) )
			OSHGui::MessageBox::ShowDialog( "Profile imported from clipboard.", "",
			                                OSHGui::MessageBoxButtons::OK, [ this, list ]( OSHGui::DialogResult result ) { } );

	} );
	config2_groupbox->AddControl( button_import );

	auto button_export = new c_button( "Export", config2_groupbox );
	button_export->GetClickEvent( ) += OSHGui::ClickEventHandler( [ this, list ]( Control *sender ) {
		if( items.empty( ) || list->GetSelectedIndex( ) == -1 )
			return;

		g_configs.export_to_clipboard( items.at( index ) );

		OSHGui::MessageBox::ShowDialog( "Profile exported to clipboard.", "", OSHGui::MessageBoxButtons::OK, [ this, list ]( OSHGui::DialogResult result ) { } );
	} );
	config2_groupbox->AddControl( button_export );

	m_pages.at( PAGE_CONFIG )->AddControl( config2_groupbox );
}

/* DANGERZONE FORM */
void c_dangerzone_form::init_component( ) {
	SetSize( SizeI( 301, 400 ) );
	init_tabs( );

	visuals_tab( );
	misc_tab( );

	c_label *form_name = new c_label( "Danger Zone", this );
	form_name->SetLocation( PointI( 6, -14 ) );
}

void c_dangerzone_form::init_tabs( ) {
	m_tab_control = new OSHGui::TabControl( );

	// create pages.
	for( int i = 0; i < PAGE_MAX; ++i )
		m_pages.push_back( std::make_shared< OSHGui::TabPage >( ) );

	m_tab_control->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );

	// set page titles.
	m_pages.at( PAGE_VISUALS )->SetText( "Visuals" );
	m_pages.at( PAGE_MISC )->SetText( "Miscellaneous" );

	// set tab style.
	m_tab_control->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	m_tab_control->SetSize( 277, 360 );
	m_tab_control->SetBackColor( OSHGui::Drawing::Color::FromRGB( 32, 32, 38 ) );
	m_tab_control->SetLocation( 6, 5 );
	m_tab_control->SetButtonWidth( 139 );

	// add all pages to tab control.
	for( auto &item : m_pages )
		m_tab_control->AddTabPage( item.get( ) );

	AddControl( m_tab_control );
}

void c_dangerzone_form::misc_tab( ) { }

void c_dangerzone_form::visuals_tab( ) {
	g_menu.set_y_pos( 36 );
	g_menu.set_x_pos( 35 );

	auto item_dist = new c_slider( "Max visual distance", m_tab_control, 0, 5000, &g_vars.dz.max_item_distance, 2500, " units" );
	auto healthshot = new c_checkbox( "Healthshot", m_tab_control, &g_vars.dz.healthshot );
	auto weapon_case = new c_checkbox( "Weapon case", m_tab_control, &g_vars.dz.weapon_case );
	auto tool_case = new c_checkbox( "Tool case", m_tab_control, &g_vars.dz.tool_case );
	auto ammobox = new c_checkbox( "Ammobox", m_tab_control, &g_vars.dz.ammobox );
	auto melee = new c_checkbox( "Melee weapon", m_tab_control, &g_vars.dz.melee_weapon );
	auto breach_charge = new c_checkbox( "Breaching charge", m_tab_control, &g_vars.dz.breach_charge );
	auto airdrop = new c_checkbox( "Airdrop", m_tab_control, &g_vars.dz.airdrop );
	auto armor = new c_checkbox( "Armor", m_tab_control, &g_vars.dz.armor );
	auto tablet = new c_checkbox( "Tablet upgrade", m_tab_control, &g_vars.dz.tablet_upgrade );
	auto drone = new c_checkbox( "Drone", m_tab_control, &g_vars.dz.drone );
	auto turret = new c_checkbox( "Turret", m_tab_control, &g_vars.dz.turret );
	auto cash = new c_checkbox( "Money", m_tab_control, &g_vars.dz.cash );
	auto safe = new c_checkbox( "Safe", m_tab_control, &g_vars.dz.safe );
	auto contract = new c_checkbox( "Contract", m_tab_control, &g_vars.dz.contract );
	auto parachute = new c_checkbox( "Parachute", m_tab_control, &g_vars.dz.parachute );
	auto jammer = new c_checkbox( "Jammer", m_tab_control, &g_vars.dz.jammer );
}

/* GROUPBOX */
c_groupbox::c_groupbox( const AnsiString &text, int x, int y, int w, int h ) {
	GroupBox::SetFont( g_renderer.m_fonts.at( FONT_VERDANA_BOLD_7PX ) );
	Control::SetBounds( x, y, w, h );
	SetText( text );

	// reset y position.
	g_menu.set_y_pos( default_y_pos );
}

/* COLOR PICKER */
void c_colorpicker::init( int x, int y, Control *parent, float *col ) {
	SetLocation( x, y );
	parent->AddControl( this );

	auto timer = new OSHGui::Timer( );
	timer->SetInterval( 50 );
	timer->Start( );
	parent->AddControl( timer );

	timer->GetTickEvent( ) += OSHGui::TickEventHandler( [ this, col ]( Control *sender ) {
		this->SetColor( OSHGui::Drawing::Color::FromARGB( col ) );
	} );

	// click event.
	this->GetColorChangedEvent( ) += OSHGui::ColorChangedEventHandler( [ this, col ]( Control *, const OSHGui::Drawing::Color &color ) {
		col[ 0 ] = color.GetAlpha( ) * 255;
		col[ 1 ] = color.GetRed( ) * 255;
		col[ 2 ] = color.GetGreen( ) * 255;
		col[ 3 ] = color.GetBlue( ) * 255;

		SetColor( GetColor( ) );
	} );
}

c_colorpicker::c_colorpicker( Control *parent, Control *control, float *col ) {
	if( control->GetType( ) == OSHGui::ControlType::ComboBox )
		init( parent->GetWidth( ) - 38, control->GetTop( ) + 6, parent, col );
	else
		init( parent->GetWidth( ) - 38, control->GetTop( ) + 2, parent, col );
}

void c_checkbox::init( const AnsiString &text, int x, int y, Control *parent, bool *cvar ) {
	SetBackColor( g_renderer.get_instance( )->GetPrimaryColor( ) );
	SetFont( g_renderer.m_fonts.at( FONT_VERDANA_7PX ) );
	SetLocation( x, y );
	SetText( text );
	SetChecked( *cvar );

	auto timer = new OSHGui::Timer( );
	timer->SetInterval( 250 );
	timer->Start( );
	parent->AddControl( timer );

	timer->GetTickEvent( ) += OSHGui::TickEventHandler( [ this, cvar ]( Control *sender ) {
		this->SetChecked( *cvar );
	} );

	parent->AddControl( this );

	// click event.
	this->GetClickEvent( ) += OSHGui::ClickEventHandler( [ cvar ]( Control *sender ) {
		*cvar = !( *cvar );
	} );
}

c_checkbox::c_checkbox( const AnsiString &text, int x, int y, Control *parent, bool *cvar ) {
	init( text, x, y, parent, cvar );
}

c_checkbox::c_checkbox( const AnsiString &text, Control *parent, bool *cvar ) {
	init( text, g_menu.get_x_pos( ), g_menu.get_y_pos( ), parent, cvar );
	g_menu.push_y_pos( 18 );
}

/* LABEL */
void c_label::init( const AnsiString &text, int x, int y, Control *parent, const FontPtr &font, int style, OSHColor colour ) {
	SetForeColor( colour );
	SetLocation( x, y );
	SetFont( font );
	SetStyle( style );
	SetText( text );

	parent->AddControl( this );
}

c_label::c_label( const AnsiString &text, int x, int y, Control *parent, const FontPtr &font, int style, OSHColor colour ) {
	init( text, x, y, parent, font, style, colour );
}

c_label::c_label( const AnsiString &text, int x, int y, Control *parent ) {
	init( text, x, y, parent, g_renderer.get_font( FONT_VERDANA_7PX ), 1, OSHColor::FromRGB( 201, 201, 201 ) );
}

c_label::c_label( const AnsiString &text, Control *parent, const FontPtr &font, int style, OSHColor colour ) {
	init( text, g_menu.get_x_pos( ) + 20, g_menu.get_y_pos( ), parent, font, style, colour );
	g_menu.push_y_pos( Control::GetSize( ).Height + 7 );
}

c_label::c_label( const AnsiString &text, Control *parent ) {
	init( text, g_menu.get_x_pos( ) + 20, g_menu.get_y_pos( ), parent, g_renderer.get_font( FONT_VERDANA_7PX ), 1, OSHColor::FromRGB( 201, 201, 201 ) );
	g_menu.push_y_pos( Control::GetSize( ).Height + 7 );
}

/* COMBOBOX */
void c_combo::init( const AnsiString &text, std::vector< AnsiString > items, int x, int y, Control *parent, int max_items, int *cvar ) {
	SetLocation( x, y );
	SetFont( g_renderer.get_font( FONT_VERDANA_7PX ) );
	SetMaxShowItems( max_items );

	auto label = new c_label( text, GetLeft( ), GetTop( ) - 12, this );
	parent->AddControl( label );

	for( auto const &item : items )
		AddItem( item );

	auto timer = new OSHGui::Timer( );
	timer->SetInterval( 250 );
	timer->Start( );

	parent->AddControl( timer );
	parent->AddControl( this );

	// tick event.
	timer->GetTickEvent( ) += OSHGui::TickEventHandler( [ this, cvar ]( Control *sender ) {
		this->SetSelectedIndex( *cvar, false );
	} );

	// click event.
	this->GetSelectedIndexChangedEvent( ) += OSHGui::SelectedIndexChangedEventHandler( [ this, cvar ]( Control *sender ) {
		*cvar = this->GetSelectedIndex( );
	} );
}

c_combo::c_combo( const AnsiString &text, const std::vector< AnsiString > &items, int x, int y, Control *parent, int max_items, int *cvar ) {
	init( text, items, x, y, parent, max_items, cvar );
}

c_combo::c_combo( const AnsiString &text, const std::vector< AnsiString > &items, Control *parent, int max_items, int *cvar, int parent_width ) {
	init( text, items, parent_width / 2 - Control::GetWidth( ) / 2 - 3, g_menu.get_y_pos( ) + 12, parent, max_items, cvar );
	g_menu.push_y_pos( 40 );
}

/* SOUND COMBO */
void c_sound_combo::init( const AnsiString &text, std::vector< AnsiString > items, int x, int y, Control *parent, int max_items, int *cvar ) {
	SetLocation( x, y );
	SetFont( g_renderer.get_font( FONT_VERDANA_7PX ) );
	SetMaxShowItems( max_items );

	auto label = new c_label( text, GetLeft( ), GetTop( ) - 12, this );
	parent->AddControl( label );

	for( auto const &item : items )
		AddItem( item );

	auto timer = new OSHGui::Timer( );
	timer->SetInterval( 250 );
	timer->Start( );

	//parent->AddControl( refresh_button );
	parent->AddControl( timer );
	parent->AddControl( this );

	// tick event.
	timer->GetTickEvent( ) += OSHGui::TickEventHandler( [ this, cvar ]( Control *sender ) {
		this->SetSelectedIndex( *cvar, false );
	} );

	// click event.
	this->GetSelectedIndexChangedEvent( ) += OSHGui::SelectedIndexChangedEventHandler( [ this, cvar ]( Control *sender ) {
		*cvar = this->GetSelectedIndex( );
		g_menu.m_selected_sound_text = this->GetSelectedItem( )->GetItemText( );
	} );
}

c_sound_combo::c_sound_combo( const AnsiString &text, const std::vector< AnsiString > &items, int x, int y, Control *parent, int max_items, int *cvar ) {
	init( text, items, x, y, parent, max_items, cvar );
}

c_sound_combo::c_sound_combo( const AnsiString &text, const std::vector< AnsiString > &items, Control *parent, int max_items, int *cvar, int parent_width ) {
	init( text, items, parent_width / 2 - Control::GetWidth( ) / 2 - 3, g_menu.get_y_pos( ) + 12, parent, max_items, cvar );
	g_menu.push_y_pos( 40 );
}

/* HOTKEY */
void c_hotkey::init( const AnsiString &text, int x, int y, Control *parent, int *cvar ) {
	SetLocation( x, y );
	SetFont( g_renderer.get_font( FONT_VERDANA_7PX ) );

	auto label = new OSHGui::Label( );
	label->SetForeColor( OSHColor::FromARGB( 255, 201, 201, 201 ) );
	label->SetFont( g_renderer.get_font( FONT_VERDANA_7PX ) );
	label->SetLocation( GetLeft( ), GetTop( ) - 13 );
	label->SetStyle( 1 );
	label->SetText( text );
	parent->AddControl( label );

	auto timer = new OSHGui::Timer( );
	timer->SetInterval( 5 );
	timer->Start( );
	parent->AddControl( timer );

	timer->GetTickEvent( ) += OSHGui::TickEventHandler( [ this, cvar ]( Control *sender ) {
		this->SetHotkey( static_cast< OSHGui::Key >( *cvar ) );
	} );

	parent->AddControl( this );

	// click event.
	this->GetHotkeyChangedEvent( ) += OSHGui::HotkeyChangedEventHandler( [ this, cvar ]( Control *sender ) {
		*cvar = static_cast< int >( GetHotkey( ) );
	} );
}

c_hotkey::c_hotkey( const AnsiString &text, Control *parent, int *cvar, int parent_width ) {
	Control::SetSize( 160, 20 );
	init( text, parent_width / 2 - Control::GetWidth( ) / 2 - 3, g_menu.get_y_pos( ) + 12, parent, cvar );
	g_menu.push_y_pos( 40 );
}

/* SLIDER */
void c_slider::init( const AnsiString &text, int x, int y, Control *parent, float min, float max, int *value, int default_value, std::string append_text ) {
	std::once_flag once_flag;
	std::call_once( once_flag, [ & ] {
		*value = default_value;
	} );

	SetFont( g_renderer.get_font( FONT_VERDANA_BOLD_7PX ) );
	SetBackColor( g_renderer.get_instance( )->GetPrimaryColor( ) );
	SetLocation( x, y );

	SetMinimum( min );
	SetMaximum( max );
	SetPrecision( 0 );
	SetText( text );
	SetAppendText( append_text );

	auto timer = new OSHGui::Timer( );
	timer->SetInterval( 5 );
	timer->Start( );

	parent->AddControl( timer );
	parent->AddControl( this );

	// tick event.
	timer->GetTickEvent( ) += OSHGui::TickEventHandler( [ this, value ]( Control *sender ) {
		this->SetValue( *value );
	} );

	// value changed event.
	this->GetValueChangedEvent( ) += OSHGui::ValueChangedEventHandler( [ this, value ]( Control *sender ) {
		*value = this->GetValue( );
	} );
}

void c_slider::init( const AnsiString &text, int x, int y, Control *parent, float min, float max, float *value, int precision, float default_value, std::string append_text ) {
	std::once_flag once_flag;
	std::call_once( once_flag, [ & ] {
		*value = default_value;
	} );

	SetFont( g_renderer.get_font( FONT_VERDANA_BOLD_7PX ) );
	SetBackColor( g_renderer.get_instance( )->GetPrimaryColor( ) );
	SetLocation( x, y );

	SetMinimum( min );
	SetMaximum( max );
	SetPrecision( precision );
	SetText( text );
	SetAppendText( append_text );

	auto timer = new OSHGui::Timer( );
	timer->SetInterval( 5 );
	timer->Start( );

	parent->AddControl( timer );
	parent->AddControl( this );

	// tick event.
	timer->GetTickEvent( ) += OSHGui::TickEventHandler( [ this, value ]( Control *sender ) {
		this->SetValue( *value );
	} );

	// value changed event.
	this->GetValueChangedEvent( ) += OSHGui::ValueChangedEventHandler( [ this, value ]( Control *sender ) {
		*value = this->GetValue( );
	} );
}

c_slider::c_slider( const AnsiString &text, int x, int y, Control *parent, float min, float max, int *value, int default_value, std::string append_text ) {
	init( text, x, y, parent, min, max, value, default_value, append_text );
}

c_slider::c_slider( const AnsiString &text, int x, int y, Control *parent, float min, float max, float *value, int precision, float default_value, std::string append_text ) {
	init( text, x, y, parent, min, max, value, precision, default_value, append_text );
}

c_slider::c_slider( const AnsiString &text, Control *parent, float min, float max, int *value, int default_value, std::string append_text ) {
	int x = parent->GetWidth( ) / 2 - Control::GetWidth( ) / 2;
	int y = g_menu.get_y_pos( ) + 4;

	if( text.empty( ) ) {
		x -= 2;
		y -= 16;
	}

	init( text, x, y, parent, min, max, value, default_value, append_text );

	g_menu.push_y_pos( text.empty( ) ? Control::GetSize( ).Height - 4 : Control::GetSize( ).Height + 10 );
}

c_slider::c_slider( const AnsiString &text, Control *parent, float min, float max, float *value, int precision, float default_value, std::string append_text ) {
	int x = parent->GetWidth( ) / 2 - Control::GetWidth( ) / 2;
	int y = g_menu.get_y_pos( ) + 4;

	if( text.empty( ) ) {
		x -= 2;
		y -= 16;
	}

	init( text, x, y, parent, min, max, value, precision, default_value, append_text );

	g_menu.push_y_pos( text.empty( ) ? Control::GetSize( ).Height - 4 : Control::GetSize( ).Height + 10 );
}

// chai menu
chai_console *console_text;
int current_selection = 0;

void print_to_console( chai_console_text str ) {
	// this is so we can pass chaiscript this function to print to
	console_text->AppendText( str );
	console_text->Invalidate( );
}

void c_chaiscript_form::update_label( ) {
	label_console_pos->SetText( std::to_string( current_selection + 1 ) + " / " + std::to_string( chai_manager::sessions.size( ) ) );
}

void c_chaiscript_form::init_component( ) {
	SetSize( SizeI( 524, 400 ) );
	init_controls( );

	controls::c_label *form_name = new controls::c_label( "ChaiScript", this );
	form_name->SetLocation( OSHGui::Drawing::PointI( 6, -14 ) );
}

void c_chaiscript_form::submit_code( ) {
	console_text->AppendText( text_input->GetText( ) );
	std::string text = text_input->GetText( );
	if( text[ 0 ] == 'l' && text[ 1 ] == 'o' && text[ 2 ] == 'a' && text[ 3 ] == 'd' && text[ 4 ] == ' ' ) {
		// check if first 5 characters spell "load "
		std::string location = text.substr( 5 );
		chai_manager::sessions.at( current_selection ).chai_load( location );
	}
	else {
		chai_manager::sessions.at( current_selection ).chai_loadscript( text );

	}
	text_input->SetText( "" );
	console_text->Invalidate( );
}

void c_chaiscript_form::init_controls( ) {
	// textbox for input to chaiscript
	label_console_pos = new OSHGui::Label( );
	text_input = new OSHGui::TextBox( );
	text_input->SetSize( 438, 21 );
	text_input->SetLocation( 7, 343 );
	text_input->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	text_input->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	text_input->SetText( "" );
	// button to send input to chaiscript
	auto text_input_submit = new OSHGui::Button( );
	text_input_submit->SetSize( 60, 20 );
	text_input_submit->SetLocation( 445, 343 );
	text_input_submit->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	text_input_submit->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	text_input_submit->SetText( "Submit" );

	auto button_input_remove = new OSHGui::Button( );
	button_input_remove->SetSize( 20, 20 );
	button_input_remove->SetLocation( 3, 367 );
	button_input_remove->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	button_input_remove->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	button_input_remove->SetText( "-" );

	auto button_input_add = new OSHGui::Button( );
	button_input_add->SetSize( 20, 20 );
	button_input_add->SetLocation( 26, 367 );
	button_input_add->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	button_input_add->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	button_input_add->SetText( "+" );

	auto button_input_left = new OSHGui::Button( );
	button_input_left->SetSize( 20, 20 );
	button_input_left->SetLocation( 49, 367 );
	button_input_left->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	button_input_left->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	button_input_left->SetText( "<" );

	auto button_input_right = new OSHGui::Button( );
	button_input_right->SetSize( 20, 20 );
	button_input_right->SetLocation( 72, 367 );
	button_input_right->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	button_input_right->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	button_input_right->SetText( ">" );

	label_console_pos->SetLocation( 120, 373 );
	label_console_pos->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	label_console_pos->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	label_console_pos->SetText( "1 / 1" );

	// console to display output from chaiscript
	console_text = new chai_console( );
	console_text->SetSize( 497, 320 );
	console_text->SetLocation( 3, 3 );
	console_text->SetBackColor( OSHGui::Drawing::Color::FromARGB( 255, 27, 27, 34 ) );
	console_text->SetFont( g_renderer.get_instance( )->GetDefaultFont( ) );
	console_text->SetForeColor( OSHGui::Drawing::Color::FromRGB( 255, 255, 255 ) );

	text_input_submit->GetClickEvent( ) += OSHGui::ClickEventHandler( [this]( Control *sender ) {
		c_chaiscript_form::submit_code( );
	} );
	button_input_add->GetClickEvent( ) += OSHGui::ClickEventHandler( [this]( Control *sender ) {
		chai_wrapper base_wrapper;
		base_wrapper.wrapper_name = "Session";
		base_wrapper.chai_init( );
		chai_manager::sessions.push_back( base_wrapper );

		chai_manager::sessions.at( chai_manager::sessions.size( ) - 1 ).text_list = new std::vector< chai_console_text >( );
		update_label( );
	} );
	button_input_remove->GetClickEvent( ) += OSHGui::ClickEventHandler( [this]( Control *sender ) {
		// add later
		update_label( );
	} );
	button_input_right->GetClickEvent( ) += OSHGui::ClickEventHandler( [this]( Control *sender ) {
		console_text->Invalidate( );
		if( current_selection < chai_manager::sessions.size( ) - 1 )
			current_selection++;
		console_text->text_list = chai_manager::sessions.at( current_selection ).text_list;
		update_label( );
	} );
	button_input_left->GetClickEvent( ) += OSHGui::ClickEventHandler( [this]( Control *sender ) {
		console_text->Invalidate( );
		if( current_selection > 0 )
			current_selection--;
		console_text->text_list = chai_manager::sessions.at( current_selection ).text_list;
		update_label( );
	} );

	chai_wrapper base_wrapper;
	base_wrapper.wrapper_name = "Session 1";
	base_wrapper.chai_init( );
	chai_manager::sessions.push_back( base_wrapper );

	chai_manager::sessions.at( 0 ).text_list = new std::vector< chai_console_text >( );

	console_text->text_list = chai_manager::sessions.at( 0 ).text_list;

	chai_manager::print_func = &print_to_console;

	AddControl( text_input );
	AddControl( text_input_submit );
	AddControl( button_input_left );
	AddControl( button_input_right );
	AddControl( label_console_pos );
	AddControl( console_text );
	AddControl( button_input_remove );
	AddControl( button_input_add );
}

