#pragma once

class c_move_data {
public:
	bool m_first_run_of_functions : 1;
	bool m_game_code_moved_player : 1;
	int m_player_handle; // edict index on server, client entity handle on client=
	int m_impulse_command; // Impulse command issued.
	vec3_t m_viewangles; // Command view angles (local space)
	vec3_t m_abs_viewangles; // Command view angles (world space)
	int m_buttons; // Attack buttons.
	int m_old_buttons; // From host_client->oldbuttons;
	float m_forward_move;
	float m_side_move;
	float m_up_move;
	float m_max_speed;
	float m_client_max_speed;
	vec3_t m_velocity; // edict::velocity        // Current movement direction.
	vec3_t m_angles; // edict::angles
	vec3_t m_old_angles;
	float m_out_step_height; // how much you climbed this move
	vec3_t m_out_wish_vel; // This is where you tried 
	vec3_t m_out_jump_vel; // This is your jump velocity
	vec3_t m_constraint_center;
	float m_constraint_radius;
	float m_constraint_width;
	float m_constraint_speed_factor;
	float m_unknown[5];
	vec3_t m_abs_origin;
};

class i_game_movement {
public:
public:
	virtual			~i_game_movement( void )
	{ }

	virtual void	process_movement( i_client_entity *pPlayer, void *pMove ) = 0;
	virtual void	reset( void ) = 0;
	virtual void	start_track_prediction_errors( i_client_entity *pPlayer ) = 0;
	virtual void	finish_track_prediction_errors( i_client_entity *pPlayer ) = 0;
	virtual void	diff_print( char const *fmt, ... ) = 0;

	virtual vec3_t const&	get_player_mins( bool ducked ) const = 0;
	virtual vec3_t const&	get_player_maxs( bool ducked ) const = 0;
	virtual vec3_t const&   get_player_view_offset( bool ducked ) const = 0;

	virtual bool			is_moving_player_stuck( void ) const = 0;
	virtual i_client_entity*	get_moving_player( void ) const = 0;
	virtual void			unblock_pusher( i_client_entity* pPlayer, i_client_entity *pPusher ) = 0;

	virtual void setup_movement_bounds( void *pMove ) = 0;
};
