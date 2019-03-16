#pragma once

class CBoneAccessor {
public:
	inline matrix3x4_t *GetBoneArrayForWrite( void ) const {
		return m_pBones;
	}

	inline void SetBoneArrayForWrite( matrix3x4_t *bonearray ) {
		for( int bones = 0; bones < 128; bones++ )
			m_pBones[ bones ] = bonearray[ bones ];
	}

	matrix3x4_t *m_pBones;

	int m_ReadableBones; // Which bones can be read.
	int m_WritableBones; // Which bones can be written.
};

enum LifeState
{
	ALIVE = 0,
	DYING,
	DEAD,
	RESPAWNABLE,
	DISCARDBODY,
};

class C_BaseEntity : public IClientEntity {
public:
	NETVAR( int, team, "DT_BaseEntity", "m_iTeamNum" )
	NETVAR( int, survival_team, "DT_BaseEntity", "m_nSurvivalTeam" )
	NETVAR( vec3_t, origin, "DT_BaseEntity", "m_vecOrigin" )
	NETVAR( int, model_index, "DT_BaseEntity", "m_nModelIndex" )
	NETVAR( bool, spotted, "DT_BaseEntity", "m_bSpotted" )
	NETVAR( float, simtime, "DT_BaseEntity", "m_flSimulationTime" )
	NETVAR( vec3_t, mins, "DT_BaseEntity", "m_vecMins" )
	NETVAR( vec3_t, maxs, "DT_BaseEntity", "m_vecMaxs" )
	NETVAR( CBaseHandle, owner, "DT_BaseEntity", "m_hOwnerEntity" );
	NETVAR( bool, bomb_ticking, "DT_PlantedC4", "m_bBombTicking" )
	NETVAR( bool, bomb_defused, "DT_PlantedC4", "m_bBombDefused" )
	NETVAR( float, blow_time, "DT_PlantedC4", "m_flC4Blow" )
	NETVAR( float, defuse_countdown, "DT_PlantedC4", "m_flDefuseCountDown" )
	NETVAR( int, dronegun_health, "DT_Dronegun", "m_iHealth" )
	NETVAR( bool, dronegun_has_target, "DT_Dronegun", "m_bHasTarget" )
	NETVAR( vec3_t, dronegun_target_pos, "DT_Dronegun", "m_vecAttentionTarget" )

	VFUNC( 10, abs_origin( ), vec3_t&( __thiscall* )( void* ) )( )
	VFUNC( 75, set_model_index( int index ), void( __thiscall* )( void*, int ) )( index )

	float &old_simtime( ) {
		const static auto offset = g_netvars.get_offset( "DT_BaseEntity", "m_flSimulationTime" ) + 4;
		return *reinterpret_cast< float * >( this + offset );
	};

	int &get_take_damage( ) {
		return *reinterpret_cast< int * >( uintptr_t( this ) + 0x280 );
	}

	bool is_player( ) {
		return util::misc::vfunc< bool( __thiscall *)( void * ) >( this, 151 )( this );
	}

	bool is_weapon( ) {
		return util::misc::vfunc< bool(__thiscall*)( void * ) >( this, 162 )( this );
	}

	bool is_valid_world( bool check_dormant ) {
		bool ret = true;

		if( check_dormant && this->IsDormant( ) )
			ret = false;

		return ret;
	}

	float get_defuse_length( ) {
		float return_value = defuse_countdown( ) - g_csgo.m_global_vars->m_cur_time;
		return return_value < 0 ? 0.f : return_value;
	}

	IClientRenderable *renderable( ) {
		return reinterpret_cast< IClientRenderable* >( reinterpret_cast< uintptr_t >( this ) + 0x4 );
	}

	matrix3x4_t &coord_frame( ) {
		const static auto m_CollisionGroup = g_netvars.get_offset( "DT_BaseEntity", "m_CollisionGroup" );

		auto m_rgflCoordinateFrame = m_CollisionGroup - 0x30;

		return *reinterpret_cast< matrix3x4_t * >( reinterpret_cast< uintptr_t >( this ) + m_rgflCoordinateFrame );
	}
};

class C_BaseCombatCharacter : public C_BaseEntity {
public:
	
	NETVAR( float, next_attack, "DT_BaseCombatCharacter", "m_flNextAttack" )
};

class C_BasePlayer : public C_BaseCombatCharacter {
public:
	NETVAR( int, health, "DT_BasePlayer", "m_iHealth" );
	NETVAR( int, flags, "DT_BasePlayer", "m_fFlags" );
	NETVAR( int, tickbase, "DT_BasePlayer", "m_nTickBase" );
	NETVAR( int, lifestate, "DT_BasePlayer", "m_lifeState" );
	NETVAR( vec3_t, velocity, "DT_BasePlayer", "m_vecVelocity[0]" );
	NETVAR( vec3_t, view_offset, "DT_BasePlayer", "m_vecViewOffset[0]" );
	NETVAR( vec3_t, punch_angle, "DT_BasePlayer", "m_aimPunchAngle" );
	NETVAR( CBaseHandle, weapon_handle, "DT_BasePlayer", "m_hActiveWeapon" )
	NETVAR( CBaseHandle, viewmodel_handle, "DT_BasePlayer", "m_hViewModel[0]" )

	OFFSET( int, impulse, 0x31FC )

	bool alive( ){
		return health( ) > 0;
	}

	void set_local_viewangles( vec3_t &angle ) {
		//  (*(*player + 1468))(player, &cmd->viewangles);
		using fnSetLocalViewAngles = void( __thiscall * )( void *, vec3_t & );
		util::misc::vfunc< fnSetLocalViewAngles >( this, 367 )( this, angle );
	}

	bool physics_run_think( int unk01 ) {
		// if ( C_BaseEntity::PhysicsRunThink((_DWORD *)player, 0) )
		static auto impl_PhysicsRunThink = reinterpret_cast< bool( __thiscall * )( void *, int ) >(
			pattern::find( g_csgo.m_client_dll, "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16" )
			);

		return impl_PhysicsRunThink( this, unk01 );
	}

	void think( ) {
		util::misc::vfunc< void( __thiscall * )( void * ) >( this, 138 )( this );
	}

	void pre_think( ) {
		util::misc::vfunc< void( __thiscall * )( void * ) >( this, 312 )( this );
	}

	void post_think( ) {
		util::misc::vfunc< void( __thiscall * )( void * ) >( this, 313 )( this );
	}

	void select_item( const char *name, int a2 ){
		util::misc::vfunc< void( __thiscall * )( void *, const char *, int ) >( this, 323 )( this, name, a2 );
	}
};


class C_BaseCSGrenade : public C_BaseEntity {
public:
	NETVAR( bool, pin_pulled, "DT_BaseCSGrenade", "m_bPinPulled" );
	NETVAR( float, throw_time, "DT_BaseCSGrenade", "m_fThrowTime" );
	NETVAR( float, throw_strength, "DT_BaseCSGrenade", "m_flThrowStrength" );
};

class C_BaseCombatWeapon : public C_BaseCSGrenade {
public:
	NETVAR( short, item_index, "DT_WeaponBaseItem", "m_iItemDefinitionIndex" );
	NETVAR( int, clip, "DT_WeaponBaseItem", "m_iClip1" );
	NETVAR( int, viewmodel_index, "DT_BaseCombatWeapon", "m_iViewModelIndex" );
	NETVAR( CBaseHandle, worldmodel_handle, "DT_BaseCombatWeapon", "m_hWeaponWorldModel" );
	NETVAR( int, dropped_index, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex" );
	NETVAR( float, ready_time, "DT_WeaponBaseItem", "m_flPostponeFireReadyTime" );
	NETVAR( float, next_attack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack" );
	NETVAR( float, next_sec_attack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack" );

	OFFSET( bool, in_reload, 0x3285 )

	bool is_grenade( ) {
		return this->item_index( ) >= 43 && this->item_index( ) <= 48;
	}

	bool is_being_thrown( ) {
		if ( this->is_grenade( ) ) {
			if ( !this->pin_pulled( ) ) {
				float throwtime = this->throw_time( );
				if ( ( throwtime > 0 ) && ( throwtime < g_csgo.m_global_vars->m_cur_time ) )
					return true;
			}
		}

		return false;
	}

	bool is_knife( int i ){
		return ( i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND ) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
	}

	bool has_scope( ) {
		int weapon_index = this->item_index();
		return weapon_index == WEAPON_G3SG1
			|| weapon_index == WEAPON_SCAR20
			|| weapon_index == WEAPON_AWP
			|| weapon_index == WEAPON_AUG
			|| weapon_index == WEAPON_SG558
			|| weapon_index == WEAPON_SSG08;
	}

	bool has_sniper_scope( ) {
		int weapon_index = this->item_index();
		return weapon_index == WEAPON_G3SG1
			|| weapon_index == WEAPON_SCAR20
			|| weapon_index == WEAPON_AWP
			|| weapon_index == WEAPON_SSG08;
	}

	const WeaponInfo_t *get_weapon_info( ) {
		return g_csgo.m_weapon_system->GetWpnData( this->item_index( ) );
	}

	float spread( ) {
		return util::misc::vfunc< float( __thiscall *)( void * ) >( this, 440 )( this );
	}

	float inaccuracy( ) {
		return util::misc::vfunc< float( __thiscall *)( void * ) >( this, 471 )( this );
	}

	void update_accuracy( ) {
		return util::misc::vfunc< void( __thiscall *)( void * ) >( this, 472 )( this );
	}
};

class C_BaseAttributableItem : public C_BaseCombatWeapon
{
public:
	NETVAR( int, account_id, "DT_BaseAttributableItem","m_iAccountID" )
		NETVAR( short, def_index, "DT_BaseAttributableItem","m_iItemDefinitionIndex" )
		NETVAR( int, id_high, "DT_BaseAttributableItem","m_iItemIDHigh" )
		NETVAR( int, quality, "DT_BaseAttributableItem","m_iEntityQuality" )
		NETVAR( unsigned, paintkit, "DT_BaseAttributableItem","m_nFallbackPaintKit" )
		NETVAR( unsigned, seed, "DT_BaseAttributableItem","m_nFallbackSeed" )
		NETVAR( float, wear, "DT_BaseAttributableItem","m_flFallbackWear" )
		NETVAR( unsigned, stattrack, "DT_BaseAttributableItem","m_nFallbackStatTrak" )
};


class C_BaseAnimating : public C_BasePlayer {
public:
	NETVAR( CUtlVector< float >, poses, "DT_BaseAnimating", "m_flPoseParameter" );
	NETVAR( bool, client_side_anims, "DT_BaseAnimating", "m_bClientSideAnimation" );
	NETVAR( int, hitbox_set, "DT_BaseAnimating", "m_nHitboxSet" );

	VFUNC( 219, update_anims(), void( __thiscall* )( void* ) )( )
};

class C_CSPlayer : public C_BaseAnimating {
public:
	NETVAR( bool, has_defuser, "DT_CSPlayer", "m_bHasDefuser" );
	NETVAR( bool, is_scoped, "DT_CSPlayer", "m_bIsScoped" );
	NETVAR( bool, is_defusing, "DT_CSPlayer", "m_bIsDefusing" );
	NETVAR( bool, helmet, "DT_CSPlayer", "m_bHasHelmet" );
	NETVAR( bool, rescuing, "DT_CSPlayer", "m_bIsRescuing" );
	NETVAR( bool, grabbing_hostage, "DT_CSPlayer", "m_bIsGrabbingHostage" );
	NETVAR( bool, has_heavy_armor, "DT_CSPlayer", "m_bHasHeavyArmor" );
	NETVAR( bool, immune, "DT_CSPlayer", "m_bGunGameImmunity" );

	NETVAR( float, lby_t, "DT_CSPlayer", "m_flLowerBodyYawTarget" );
	NETVAR( float, flash_duration, "DT_CSPlayer", "m_flFlashDuration" );
	NETVAR( float, flash_alpha, "DT_CSPlayer", "m_flFlashMaxAlpha" );

	NETVAR( int, observer_mode, "DT_CSPlayer", "m_iObserverMode" );
	NETVAR( int, money, "DT_CSPlayer", "m_iAccount" );
	NETVAR( int, armor, "DT_CSPlayer", "m_ArmorValue" );
	NETVAR( vec3_t, angles, "DT_CSPlayer", "m_angEyeAngles" )
		

	NETVAR( CBaseHandle, observer_handle, "DT_CSPlayer", "m_hObserverTarget" );
	NETVAR( CBaseHandle, ground_ent, "DT_CSPlayer", "m_hGroundEntity" );
	NETVAR( CBaseHandle *, my_weapons, "DT_CSPlayer", "m_hMyWeapons" )
	NETVAR( CBaseHandle *, my_wearables, "DT_CSPlayer", "m_hMyWearables" )

	NETVAR( int, shots_fired, "DT_CSPlayer", "m_iShotsFired" )

	OFFSET( float, spawn_time, 0xA350 )
	OFFSET( CCSGOPlayerAnimState *, animstate, 0x3900 )
	OFFSET( vec3_t, v_angle, 0x31C8 )
	OFFSET( CStudioHdr*, model_ptr, 0x294C + sizeof( void* ) )
	OFFSET( int, get_move_type, 0x25C )
	OFFSET( CBoneAccessor *, bone_accessor, 0x26A8 + sizeof( void* ) )

	template< typename T >
	T &get( uintptr_t offset ) {
		return *reinterpret_cast< T* >( reinterpret_cast< uintptr_t >( this ) + offset );
	}

	CUtlVector< AnimationLayer_t >& animoverlays( ) {
		return *reinterpret_cast< CUtlVector< AnimationLayer_t >* >( reinterpret_cast< uintptr_t >( this ) + 0x2980 );
	}

	static C_CSPlayer *get_local( ) {
		return g_csgo.m_entity_list->Get< C_CSPlayer >( g_csgo.m_engine->GetLocalPlayer( ) );
	}

	CBaseHandle *weapons( ){
		static auto offset = g_netvars.get_offset( "DT_CSPlayer", "m_hMyWeapons" );
		return reinterpret_cast< CBaseHandle * >( uintptr_t( this ) + offset );
	}

	void set_abs_origin( vec3_t &o ) {
		static const auto	ptr_instruction	= pattern::find(
			g_csgo.m_client_dll,
			"55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"
			);

		reinterpret_cast< void( __thiscall * )( void *, const vec3_t & ) >( ptr_instruction )( this, o );
	}

	void set_abs_angles( vec3_t &a ) {
		static const auto ptr_instruction = pattern::find(
			g_csgo.m_client_dll,
			"55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"
			);

		reinterpret_cast< void( __thiscall *)( void *, const vec3_t & ) >( ptr_instruction )( this, a );
	}

	float get_flashed( ) {
		static auto offset = g_netvars.get_offset( "DT_CSPlayer", "m_flFlashDuration" );
		return *reinterpret_cast< float * >( uintptr_t( this ) + ( offset - 12 ) );
	}

	player_info_t get_info( ) {
		player_info_t info;
		g_csgo.m_engine->GetPlayerInfo( this->GetIndex( ), &info );
		return info;
	}

	C_BaseCombatWeapon *get_active_weapon( ) {
		return (C_BaseCombatWeapon *)g_csgo.m_entity_list->GetClientEntityFromHandle( weapon_handle( ) );
	}

	vec3_t eye_pos( ) {
		vec3_t vec{ };

		util::misc::vfunc< void( __thiscall *)( void *, vec3_t & ) >
			( this, 279 )( this, vec );

		return vec;
	}

	int activity( int sequence ) {
		auto *hdr = g_csgo.m_model_info->GetStudioModel( this->GetModel( ) );

		if( !hdr ) {
			return -1;
		}
		using func_t = int( __fastcall *)( void *, void *, int );
		static auto m_function = ( func_t )( pattern::find(
			g_csgo.m_client_dll,
			"55 8B EC 53 8B 5D 08 56 8B F1 83"
		) );

		return m_function( this, hdr, sequence );
	}

	bool is_valid_player( bool check_protected, bool check_dormant ) {
		bool ret = false;

		if( this->alive( ) )
			ret = true;

		if( check_dormant && this->IsDormant( ) )
			ret = false;

		if( check_protected && this->immune( ) )
			ret = false;

		return ret;
	}

	bool is_visible( C_CSPlayer *entity, const vec3_t &start, const vec3_t &end, int mask, C_BaseEntity *ignore ) {
		if( !entity || !ignore )
			return false;

		Ray_t ray;
		CTraceFilter filter;
		filter.m_skip = ignore;
		CGameTrace trace;

		ray.init( start, end );

		g_csgo.m_engine_trace->TraceRay( ray, mask, &filter, &trace );

		return trace.fraction == 1 || trace.hit_entity == entity;
	}

	bool can_shoot( C_BaseCombatWeapon* weapon ) {
		const auto weapontype = weapon->get_weapon_info( )->type;
		if ( weapontype == WEAPONTYPE_KNIFE || weapontype == WEAPONTYPE_C4 || weapon->is_grenade( ) )
			return false;

		if( weapon->clip( ) == 0 )
			return false;

		if( weapon->in_reload( ) )
			return false;

		if( weapon->next_attack( ) > g_csgo.m_global_vars->m_cur_time )
			return false;

		if( this->next_attack( ) > g_csgo.m_global_vars->m_cur_time )
			return false;

		return true;
	}
};
