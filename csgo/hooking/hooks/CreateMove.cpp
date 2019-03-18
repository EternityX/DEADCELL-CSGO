#include "../../inc.hpp"
#include "../../features/misc/misc.h"
#include "../../features/ragebot/ragebot.h"
#include "../../features/animations/anim.h"
#include "../../features/anti-aim/antiaim.h"
#include "../../features/nade_pred/nade_pred.h"
#include "../../features/engine_pred/engine_pred.h"

bool __fastcall hook::CreateMove( uintptr_t ecx, uintptr_t edx, float flInputSampleTime, CUserCmd *cmd ) {
	static bool ret = g_hooks.m_clientmode.get_old_method< fn::CreateMove_t >( 24 )( ecx, flInputSampleTime, cmd );

	g_cl.m_sendpacket = true;

	g_cl.m_local = C_CSPlayer::get_local( );

	/*if ( g_misc.halftime_fix( cmd ) )
		return true;*/

	if( !cmd->m_command_number )
		return ret;

	INetChannelInfo *channel_info = g_csgo.m_engine->GetNetChannelInfo( );
	if( channel_info ) {
		float latency = channel_info->GetLatency( FLOW_INCOMING ) + channel_info->GetLatency( FLOW_OUTGOING );

		g_cl.m_rtt = static_cast< int >( std::round( latency * 1000.f ) );
		g_cl.m_ping = static_cast< int >( std::round( channel_info->GetAverageLatency( FLOW_OUTGOING ) * 1000.f ) );
	}

	static float framerate;
	framerate = 0.9 * framerate + ( 1.0 - 0.9 ) * g_csgo.m_global_vars->m_absolute_frametime;
	g_cl.m_under_server_tick_rate = static_cast< int >( 1.f / framerate <= 65 ) ? true : false;

	g_cl.m_cmd = cmd;

	vec3_t wish_angle = cmd->m_viewangles;

	if( g_vars.misc.fast_duck )
		cmd->m_buttons |= IN_BULLRUSH;

	if( g_vars.misc.bhop )
		g_misc.bunnyhop( cmd );

	if( g_vars.misc.air_strafe )
		g_misc.strafe( cmd );

	g_misc.unpredicted_vel = g_cl.m_local->velocity( );

	g_nadepred.trace( cmd );

	g_engine_pred.pre_start( );
	g_engine_pred.start( cmd ) ;

	g_misc.automatic_fire( g_cl.m_local->get_active_weapon( ), cmd );

	if( g_vars.misc.autozeus )
		g_misc.auto_zeus( cmd );

	g_ragebot.work( cmd );

	g_antiaim.set_angles( cmd );

	g_engine_pred.end( );

	g_misc.fix_movement( cmd, wish_angle );

	math::normalize_angles( cmd->m_viewangles );
	math::clamp_angles( cmd->m_viewangles );

	if( g_cl.m_sendpacket && g_cl.m_local )
		g_cl.m_last_sent_origin = g_cl.m_local->origin( );

	g_antiaim.m_real = cmd->m_viewangles;

	uintptr_t* framePointer;
	__asm mov framePointer, ebp;
	*reinterpret_cast< bool* > ( *framePointer - 0x1C ) = g_cl.m_sendpacket;

	return false;
}
