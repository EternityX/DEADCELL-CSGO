#include "engine_pred.h"
#include "../misc/misc.h"

c_engine_pred g_engine_pred;

int c_engine_pred::post_think( C_BasePlayer *player ) const {
	static auto PostThinkVPhysics = pattern::find< bool( __thiscall*)( C_BaseEntity * ) >( g_csgo.m_client_dll, "55 8B EC 83 E4 F8 81 EC ?? ?? ?? ?? 53 8B D9" );
	static auto SimulatePlayerSimulatedEntities = pattern::find< void(__thiscall*)( C_BaseEntity * ) >( g_csgo.m_client_dll, "56 8B F1 57 8B BE ?? ?? ?? ?? 83 EF 01" );

	util::misc::vfunc< void( __thiscall *)( void * ) >( g_csgo.m_modelcache, 33 )( g_csgo.m_modelcache );
	if( player->alive( ) ) {

		util::misc::vfunc< void( __thiscall *)( void * ) >( player, 334 )( player );

		if( player->flags( ) & FL_ONGROUND )
			*reinterpret_cast< uintptr_t * >( uintptr_t( player ) + 0x3014 ) = 0;

		if( *reinterpret_cast< int * >( uintptr_t( player ) + 0x28BC ) == -1 )
			util::misc::vfunc< void( __thiscall *)( void *, int ) >( player, 214 )( player, 0 );

		util::misc::vfunc< void( __thiscall *)( void * ) >( player, 215 )( player );

		PostThinkVPhysics( player );
	}
	SimulatePlayerSimulatedEntities( player );

	return util::misc::vfunc< int( __thiscall *)( void * ) >( g_csgo.m_modelcache, 34 )( g_csgo.m_modelcache );
}

void c_engine_pred::pre_start( ) {
	if( !g_csgo.m_engine->IsInGame( ) )
		return;

	if( g_csgo.m_clientstate->m_nDeltaTick > 0 ) {
		g_csgo.m_prediction->Update( g_csgo.m_clientstate->m_nDeltaTick, g_csgo.m_clientstate->m_nDeltaTick > 0,
		                             g_csgo.m_clientstate->m_nLastAcknowledgedCommand, g_csgo.m_clientstate->m_nLastOutgoingCommand + g_csgo.m_clientstate->m_nChokedCommands );
	}
}

void c_engine_pred::start( CUserCmd *ucmd ) {
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

		*reinterpret_cast< CUserCmd ** >( uint32_t( player ) + 0x3334 ) = ucmd; // m_pCurrentCommand
		*reinterpret_cast< CUserCmd ** >( uint32_t( player ) + 0x3288 ) = ucmd; // unk01
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
	const bool old_in_prediction = g_csgo.m_prediction->m_bInPrediction;
	const bool old_first_prediction = g_csgo.m_prediction->m_bIsFirstTimePredicted;

	//	set globals correctly
	g_csgo.m_global_vars->m_cur_time = player->tickbase( ) * g_csgo.m_global_vars->m_interval_per_tick;
	g_csgo.m_global_vars->m_frametime = g_csgo.m_prediction->m_bEnginePaused ? 0 : g_csgo.m_global_vars->m_interval_per_tick;
	g_csgo.m_global_vars->m_tickcount = player->tickbase( );

	//	setup prediction
	g_csgo.m_prediction->m_bIsFirstTimePredicted = false;
	g_csgo.m_prediction->m_bInPrediction = true;

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
	g_csgo.m_prediction->CheckMovingGround( player, g_csgo.m_global_vars->m_frame_count );

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
	g_csgo.m_movehelper->SetHost( player );

	//	setup input
	g_csgo.m_prediction->SetupMove( player, ucmd, g_csgo.m_movehelper, m_movedata );

	//	run movement
	g_csgo.m_game_movement->ProcessMovement( player, m_movedata );

	//	finish prediction
	g_csgo.m_prediction->FinishMove( player, ucmd, m_movedata );

	//	invoke impact functions
	g_csgo.m_movehelper->ProcessImpacts( );

	//	CPrediction::RunPostThink
	{
		post_think( player );
	}

	//	restore tickbase
	g_cl.m_local->tickbase( ) = old_tickbase;

	//	restore prediction
	g_csgo.m_prediction->m_bIsFirstTimePredicted = old_first_prediction;
	g_csgo.m_prediction->m_bInPrediction = old_in_prediction;

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

	g_csgo.m_game_movement->Reset( );
}
