#pragma once

class i_move_helper {
private:
	virtual void UnknownVirtual( ) = 0;
public:
	virtual void set_host( i_client_entity* host ) = 0;
private:
	virtual void pad00( ) = 0;
	virtual void pad01( ) = 0;
public:
	virtual void process_impacts( ) = 0;
};

class i_prediction {
public:
	void get_local_viewangles( vec3_t &angle ) {
		using original_fn = void( __thiscall *)( void *, vec3_t & );
		util::misc::vfunc< original_fn >( this, 12 )( this, angle );
	}

	void set_local_viewangles( vec3_t &angle ) {
		using original_fn = void( __thiscall *)( void *, vec3_t & );
		util::misc::vfunc< original_fn >( this, 13 )( this, angle );
	}

	void update( int start_frame, bool valid_frame, int inc_ack, int out_cmd )
	{
		util::misc::vfunc< void( __thiscall * )( void *, int, bool, int, int ) >
			( this, 3 )( this, start_frame, valid_frame, inc_ack, out_cmd );
	}

	void check_moving_ground( i_client_entity *player, double frametime )
	{
		using fnCheckMovingGround = void( __thiscall * )( void *, i_client_entity *, double );
		util::misc::vfunc< fnCheckMovingGround >( this, 18 )( this, player, frametime );
	}

	void setup_move( i_client_entity *player, c_user_cmd *ucmd, i_move_helper *helper, void *movedata )
	{
		util::misc::vfunc< void( __thiscall* )( void*, i_client_entity*, c_user_cmd*, i_move_helper*, void* ) >
			( this, 20 )( this, player, ucmd, helper, movedata );
	}

	void finish_move( i_client_entity *player, c_user_cmd *ucmd, void *movedata )
	{
		util::misc::vfunc< void( __thiscall* )( void*, i_client_entity*, c_user_cmd*, void* ) >
			( this, 21 )( this, player, ucmd, movedata );
	}

	char pad00 [ 8 ]; 					// 0x0000
	bool m_in_prediction;				// 0x0008
	char pad01 [ 1 ];					// 0x0009
	bool m_engine_paused;				// 0x000A
	char pad02 [ 13 ];					// 0x000B
	bool m_is_first_time_predicted;		// 0x0018
};
