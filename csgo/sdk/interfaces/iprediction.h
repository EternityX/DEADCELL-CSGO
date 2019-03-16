#pragma once

class IMoveHelper {
private:
	virtual void UnknownVirtual( ) = 0;
public:
	virtual void SetHost( IClientEntity* host ) = 0;
private:
	virtual void pad00( ) = 0;
	virtual void pad01( ) = 0;
public:
	virtual void ProcessImpacts( ) = 0;
};

class IPrediction {
public:
	void GetLocalViewAngles( vec3_t &angle ) {
		using original_fn = void( __thiscall *)( void *, vec3_t & );
		util::misc::vfunc< original_fn >( this, 12 )( this, angle );
	}

	void SetLocalViewangles( vec3_t &angle ) {
		using original_fn = void( __thiscall *)( void *, vec3_t & );
		util::misc::vfunc< original_fn >( this, 13 )( this, angle );
	}

	void Update( int start_frame, bool valid_frame, int inc_ack, int out_cmd )
	{
		util::misc::vfunc< void( __thiscall * )( void *, int, bool, int, int ) >
			( this, 3 )( this, start_frame, valid_frame, inc_ack, out_cmd );
	}

	void CheckMovingGround( IClientEntity *player, double frametime )
	{
		using fnCheckMovingGround = void( __thiscall * )( void *, IClientEntity *, double );
		util::misc::vfunc< fnCheckMovingGround >( this, 18 )( this, player, frametime );
	}

	void SetupMove( IClientEntity *player, CUserCmd *ucmd, IMoveHelper *helper, void *movedata )
	{
		util::misc::vfunc< void( __thiscall* )( void*, IClientEntity*, CUserCmd*, IMoveHelper*, void* ) >
			( this, 20 )( this, player, ucmd, helper, movedata );
	}

	void FinishMove( IClientEntity *player, CUserCmd *ucmd, void *movedata )
	{
		util::misc::vfunc< void( __thiscall* )( void*, IClientEntity*, CUserCmd*, void* ) >
			( this, 21 )( this, player, ucmd, movedata );
	}

	char pad00 [ 8 ]; 					// 0x0000
	bool m_bInPrediction;				// 0x0008
	char pad01 [ 1 ];					// 0x0009
	bool m_bEnginePaused;				// 0x000A
	char pad02 [ 13 ];					// 0x000B
	bool m_bIsFirstTimePredicted;		// 0x0018
};
