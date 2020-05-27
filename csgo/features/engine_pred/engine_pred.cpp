#include "engine_pred.h"
#include "../misc/misc.h"

c_engine_pred g_engine_pred;

int c_engine_pred::post_think( c_base_player *player ) const {
	static auto PostThinkVPhysics = pattern::find< bool( __thiscall*)( c_base_entity * ) >( g_csgo.m_client_dll, "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB ? ? ? ? ? 75 50" );
	static auto SimulatePlayerSimulatedEntities = pattern::find< void(__thiscall*)( c_base_entity * ) >( g_csgo.m_client_dll, "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72 90 8B 86" );

	util::misc::vfunc< void( __thiscall *)( void * ) >( g_csgo.m_modelcache, 33 )( g_csgo.m_modelcache );
	if( player->alive( ) ) {

		util::misc::vfunc< void( __thiscall *)( void * ) >( player, 339 )( player );

		if( player->flags( ) & FL_ONGROUND )
			*reinterpret_cast< uintptr_t * >( uintptr_t( player ) + 0x3014 ) = 0;

		if( *reinterpret_cast< int * >( uintptr_t( player ) + 0x28BC ) == -1 )
			util::misc::vfunc< void( __thiscall *)( void *, int ) >( player, 218 )( player, 0 );

		util::misc::vfunc< void( __thiscall *)( void * ) >( player, 219 )( player );

		PostThinkVPhysics( player );
	}
	SimulatePlayerSimulatedEntities( player );

	return util::misc::vfunc< int( __thiscall *)( void * ) >( g_csgo.m_modelcache, 34 )( g_csgo.m_modelcache );
}

void c_engine_pred::pre_start( ) {
	if( !g_csgo.m_engine->is_in_game( ) )
		return;

	if( g_csgo.m_clientstate->m_delta_tick > 0 ) {
		g_csgo.m_prediction->update( g_csgo.m_clientstate->m_delta_tick, g_csgo.m_clientstate->m_delta_tick > 0,
		                             g_csgo.m_clientstate->m_last_command_ack, g_csgo.m_clientstate->m_last_outgoing_command + g_csgo.m_clientstate->m_choked_commands );
	}
}

void c_engine_pred::start( c_user_cmd *ucmd ) {
	if( !ucmd || !g_csgo.m_movehelper || !g_cl.m_local )
		return;

	auto player = g_cl.m_local;
	if( !player )
		return;

	if( !m_movedata )
		m_movedata = malloc( 182 );

	if( !m_prediction_player || !m_prediction_seed ) {
		m_prediction_seed = c_vmt::get_method( g_csgo.m_prediction, 19 ).at( 0x30 );
		m_prediction_player = c_vmt::get_method( g_csgo.m_prediction, 19 ).at( 0x3E );
	}

	//	CPrediction::StartCommand
	{
		*reinterpret_cast< int * >( m_prediction_seed ) = ucmd ? ucmd->m_random_seed : -1;
		*reinterpret_cast< int * >( m_prediction_player ) = reinterpret_cast< int >( player );

		*reinterpret_cast< c_user_cmd ** >( uint32_t( player ) + 0x3334 ) = ucmd; // m_pCurrentCommand
		*reinterpret_cast< c_user_cmd ** >( uint32_t( player ) + 0x3288 ) = ucmd; // unk01
	}

	//	backup player variables
	m_old_player.flags = player->flags( );
	m_old_player.velocity = player->velocity( );

	//	backup globals
	m_old_globals.curtime = g_csgo.m_global_vars->m_cur_time;
	m_old_globals.frametime = g_csgo.m_global_vars->m_frametime;
	m_old_globals.tickcount = g_csgo.m_global_vars->m_tickcount;

	//	backup tick base
	const int old_tickbase = g_cl.m_local->tickbase( );

	//	backup prediction variables
	const bool old_in_prediction = g_csgo.m_prediction->m_in_prediction;
	const bool old_first_prediction = g_csgo.m_prediction->m_is_first_time_predicted;

	//	set globals correctly
	g_csgo.m_global_vars->m_cur_time = player->tickbase( ) * g_csgo.m_global_vars->m_interval_per_tick;
	g_csgo.m_global_vars->m_frametime = g_csgo.m_prediction->m_engine_paused ? 0 : g_csgo.m_global_vars->m_interval_per_tick;
	g_csgo.m_global_vars->m_tickcount = player->tickbase( );

	//	setup prediction
	g_csgo.m_prediction->m_is_first_time_predicted = false;
	g_csgo.m_prediction->m_in_prediction = true;

	if( ucmd->m_impulse )
		*reinterpret_cast< uint32_t * >( uint32_t( player ) + 0x31FC ) = ucmd->m_impulse;

	//	CBasePlayer::UpdateButtonState
	{
		ucmd->m_buttons |= *reinterpret_cast< uint32_t * >( uint32_t( player ) + 0x3330 );

		const int v16 = ucmd->m_buttons;
		int *unk02 = reinterpret_cast< int * >( uint32_t( player ) + 0x31F8 );
		const int v17 = v16 ^ *unk02;

		*reinterpret_cast< int * >( uint32_t( player ) + 0x31EC ) = *unk02;
		*reinterpret_cast< int * >( uint32_t( player ) + 0x31F8 ) = v16;
		*reinterpret_cast< int * >( uint32_t( player ) + 0x31F0 ) = v16 & v17;
		*reinterpret_cast< int * >( uint32_t( player ) + 0x31F4 ) = v17 & ~v16;
	}

	//	check if player is standing on moving ground
	g_csgo.m_prediction->check_moving_ground( player, g_csgo.m_global_vars->m_frametime );

	//	copy from command to player
	player->set_local_viewangles( ucmd->m_viewangles );

	//	CPrediction::RunPreThink
	{
		//	THINK_FIRE_ALL_FUNCTIONS
		if( player->physics_run_think( 0 ) ) {
			player->pre_think( );
		}
	}

	//	CPrediction::RunThink
	{
		const auto next_think = reinterpret_cast< int * >( uint32_t( player ) + 0xFC );
		if( *next_think > 0 && *next_think <= player->tickbase( ) ) {
			//	TICK_NEVER_THINK
			*next_think = -1;

			player->think( );
		}
	}

	//	set host
	g_csgo.m_movehelper->set_host( player );

	//	setup input
	g_csgo.m_prediction->setup_move( player, ucmd, g_csgo.m_movehelper, m_movedata );

	//	run movement
	g_csgo.m_game_movement->process_movement( player, m_movedata );

	//	finish prediction
	g_csgo.m_prediction->finish_move( player, ucmd, m_movedata );

	//	invoke impact functions
	g_csgo.m_movehelper->process_impacts( );

	//	CPrediction::RunPostThink
	{
		post_think( player );
	}

	//	restore tickbase
	g_cl.m_local->tickbase( ) = old_tickbase;

	//	restore prediction
	g_csgo.m_prediction->m_is_first_time_predicted = old_first_prediction;
	g_csgo.m_prediction->m_in_prediction = old_in_prediction;
}

void c_engine_pred::end( ) const {
	auto player = g_cl.m_local;
	if( !player || !g_csgo.m_movehelper )
		return;

	g_csgo.m_global_vars->m_cur_time = m_old_globals.curtime;
	g_csgo.m_global_vars->m_frametime = m_old_globals.frametime;
	g_csgo.m_global_vars->m_tickcount = m_old_globals.tickcount;

	//	CPrediction::FinishCommand
	{
		*reinterpret_cast< uint32_t * >( reinterpret_cast< uint32_t >( player ) + 0x3334 ) = 0;

		*reinterpret_cast< int * >( m_prediction_seed ) = static_cast< uint32_t >( -1 );
		*reinterpret_cast< int * >( m_prediction_player ) = 0;
	}

	g_csgo.m_game_movement->reset( );
}
