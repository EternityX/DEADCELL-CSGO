#include "../../inc.h"
#include "../../features/misc/misc.h"
#include "../../features/chaiscript/chai_wrapper.hpp"
#include "../../features/ragebot/ragebot.h"
#include "../../features/animations/anim.h"
#include "../../features/anti-aim/antiaim.h"
#include "../../features/nade_pred/nade_pred.h"
#include "../../features/engine_pred/engine_pred.h"
#include "../../features/fakelag/fakelag.h"

bool __fastcall hook::CreateMove( uintptr_t ecx, uintptr_t edx, float flInputSampleTime, c_user_cmd *cmd ) {
	static bool ret = g_hooks.m_clientmode.get_old_method< fn::CreateMove_t >( CREATE_MOVE )( ecx, flInputSampleTime, cmd );

	g_cl.m_local = c_csplayer::get_local( );

	if( !cmd->m_command_number )
		return ret;

	if( ret )
		g_csgo.m_prediction->set_local_viewangles( cmd->m_viewangles );

	i_net_channel_info *channel_info = g_csgo.m_engine->get_net_channel_info( );
	if( channel_info ) {
		float latency = channel_info->get_latency( FLOW_INCOMING ) + channel_info->get_latency( FLOW_OUTGOING );

		g_cl.m_rtt = static_cast< int >( std::round( latency * 1000.f ) );
		g_cl.m_ping = static_cast< int >( std::round( channel_info->get_average_latency( FLOW_OUTGOING ) * 1000.f ) );
	}

	static float framerate;
	framerate = 0.9 * framerate + ( 1.0 - 0.9 ) * g_csgo.m_global_vars->m_absolute_frametime;

	g_cl.m_client_framerate = static_cast< int >( 1.f / framerate );
	g_cl.m_under_tickrate = g_cl.m_client_framerate <= ( 1.0f / g_csgo.m_global_vars->m_interval_per_tick );

	g_cl.m_cmd = cmd;
	
	if (g_vars.misc.chaiscript_enabled) {
		for (auto & session : chai_manager::sessions) {
			if (session.chai_defined_createmove && session.chai_finished_init)
				session.chai_hook_createmove(cmd);
		}
	}

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
	g_engine_pred.start( cmd );
	{
		g_misc.automatic_fire( g_cl.m_local->get_active_weapon( ), cmd );
		//if ( g_vars.misc.autozeus )
		//	g_misc.auto_zeus( cmd );

		g_ragebot.work( cmd );

		//g_ragebot.auto_revolver( g_cl.m_local->get_active_weapon( ), cmd );

		g_fakelag.think( cmd );

		g_antiaim.set_angles( cmd );
	}
	g_engine_pred.end( );

	g_misc.fix_movement( cmd, wish_angle );

	math::normalize_angles( cmd->m_viewangles );
	math::clamp_angles( cmd->m_viewangles );

	if( g_cl.m_local ) {
		if( g_cl.m_sendpacket ) {
			g_cl.m_last_sent_origin = g_cl.m_local->origin( );
			g_antiaim.m_fake = cmd->m_viewangles;
		}
		else {
			g_antiaim.m_real = cmd->m_viewangles;
		}
	}

	g_cl.m_local->get_render_angles( ) = g_antiaim.m_real;

	uintptr_t *frame_pointer;
	__asm mov frame_pointer, ebp;
	*reinterpret_cast< bool* >( *frame_pointer - 0x1C ) = g_cl.m_sendpacket;

	return false;
}
