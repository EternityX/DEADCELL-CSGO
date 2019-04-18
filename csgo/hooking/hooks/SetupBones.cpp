#include "../../inc.h"
#include "../../features/animations/anim.h"

// credits: good friend of mine.. ( how to do it is already on uc though but code still can exist and it's just the pvs / accumulate layers skip and nothing more )
bool __fastcall animations::SetupBones( uintptr_t ecx, uintptr_t edx, matrix3x4_t *bone_to_world_out, int max_bones, int bone_bask, float current_time ) {
	auto e = reinterpret_cast < c_csplayer * >( ecx - 4 ); // do NOT change this

	fn::SetupBones_t original = nullptr;
	try {
		original = g_anim.m_track[ e->get_index( ) ].m_renderable_vmt->get_old_method< fn::SetupBones_t >( hook::idx::SETUP_BONES );
	}
	catch ( std::out_of_range &ex ) {
		UNREFERENCED_PARAMETER( ex );
		_RPT1( _CRT_WARN, "Out of range track access inside setupbones.", ex.what( ) );
		return false;
	}

	if ( e && e->is_player( ) ) {
		int restore_magic = 0;
		float restore_frametime = 0.f;

		/*
			- C_BasePlayer::SkipAnimationFrame, fix for matrix being 1 tick too late?
			- IDA signature if anyone wants to take a look themselves : 57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3 
		*/

		int &last_animation_framecount = *reinterpret_cast< int* >( uintptr_t( e ) + 0xA68 );
		int &magic = *reinterpret_cast< int* >( uintptr_t( e ) + 0x64 );

		restore_frametime = g_csgo.m_global_vars->m_frametime;
		restore_magic = magic;

		if ( !( magic % 3 ) ) {
			magic = 9;
		}

		/*
			- C_BasePlayer::SkipAnimationFrame
			- if ( frametime < 0.0033333334 )
		*/

		if ( g_csgo.m_global_vars->m_frametime > 0.0033333334f ) {
			g_csgo.m_global_vars->m_frametime = 0.003333333f;
		}

		/* 
			- C_BasePlayer::SkipAnimationFrame
			- Pseudo code :
				last_animation_framecount = *(player + 0xA68);
				framecount = g_Globals->m_framecount;
				if ( last_animation_framecount ) {
					result = abs(framecount - last_animation_framecount);
					if ( result < 2 )
			
		*/
		last_animation_framecount = g_csgo.m_global_vars->m_frame_count;

		int &occlusion_flag = *reinterpret_cast< int* >( uintptr_t( e ) + 0xA24 );
		int &prev_bone_mask = *reinterpret_cast< int* >( uintptr_t( e ) + 0x269C );

		static auto offs_bone_mask = g_netvars.get_offset( "DT_BaseAnimating", "m_nForceBone" ) + 0x20;
		*reinterpret_cast< int* >( uintptr_t( e ) + offs_bone_mask ) = 0;
		prev_bone_mask = 0;

		/*
			- C_CSPlayer::AccumulateLayers, skip the function
			- Pseudo code :
				result = (*(*g_Engine + 372))();
				if ( result || !(*(v5 + 0xA28) & 0xA) )
			- IDA signature : 55 8B EC 57 8B F9 8B 0D ? ? ? ? 8B 01 8B 80
		*/
		occlusion_flag = 0xA;

		e->invalidate_bone_cache( );

		bool result = original( ecx, bone_to_world_out, max_bones, bone_bask, current_time );

		magic = restore_magic;
		g_csgo.m_global_vars->m_frametime = restore_frametime;

		return result;
	}

	return original( ecx, bone_to_world_out, max_bones, bone_bask, current_time );
}