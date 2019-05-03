#pragma once

struct c_animstate {

	char pad_0x0000[0x18]; //0x0000
	float anim_update_timer; //0x0018 
	char pad_0x001C[0xC]; //0x001C
	float started_moving_time; //0x0028 
	float last_move_time; //0x002C 
	char pad_0x0030[0x10]; //0x0030
	float last_lby_time; //0x0040 
	char pad_0x0044[0x8]; //0x0044
	float run_amount; //0x004C 
	char pad_0x0050[0x10]; //0x0050
	void *entity; //0x0060 
	__int32 active_weapon; //0x0064 
	__int32 last_active_weapon; //0x0068 
	float last_client_side_animation_update_time; //0x006C 
	__int32 last_client_side_animation_update_framecount; //0x0070 
	float eye_timer; //0x0074 
	float eye_angles_y; //0x0078 
	float eye_angles_x; //0x007C 
	float goal_feet_yaw; //0x0080 
	float current_feet_yaw; //0x0084 
	float torso_yaw; //0x0088 
	float last_move_yaw; //0x008C 
	float lean_amount; //0x0090 
	char pad_0x0094[0x4]; //0x0094
	float feet_cycle; //0x0098 
	float feet_yaw_rate; //0x009C 
	char pad_0x00A0[0x4]; //0x00A0
	float duck_amount; //0x00A4 
	float landing_duck_amount; //0x00A8 
	char pad_0x00AC[0x4]; //0x00AC
	vec3_t current_origin;
	vec3_t last_origin;
	float velocity_x; //0x00C8 
	float velocity_y; //0x00CC 
	char pad_0x00D0[0x10]; //0x00D0
	float move_direction_1; //0x00E0 
	float move_direction_2; //0x00E4 
	char pad_0x00E8[0x4]; //0x00E8
	float m_velocity; //0x00EC 
	float jump_fall_velocity; //0x00F0 
	float clamped_velocity; //0x00F4 
	float feet_speed_forwards_or_sideways; //0x00F8 
	float feet_speed_unknown_forwards_or_sideways; //0x00FC 
	float last_time_started_moving; //0x0100 
	float last_time_stopped_moving; //0x0104 
	bool on_ground; //0x0108 
	bool hit_in_ground_animation; //0x010C 
	char pad_0x0110[0x4]; //0x0110
	float last_origin_z; //0x0114 
	float head_from_ground_distance_standing; //0x0118 
	float stop_to_full_running_fraction; //0x011C 
	char pad_0x0120[0x14]; //0x0120
	__int32 is_not_moving; //0x0134 
	char pad_0x0138[0x20]; //0x0138
	float last_anim_update_time; //0x0158 
	float moving_direction_x; //0x015C 
	float moving_direction_y; //0x0160 
	float moving_direction_z; //0x0164 
	char pad_0x0168[0x44]; //0x0168
	__int32 started_moving; //0x01AC 
	char pad_0x01B0[0x8]; //0x01B0
	float lean_yaw; //0x01B8 
	char pad_0x01BC[0x8]; //0x01BC
	float poses_speed; //0x01C4 
	char pad_0x01C8[0x8]; //0x01C8
	float ladder_speed; //0x01D0 
	char pad_0x01D4[0x8]; //0x01D4
	float ladder_yaw; //0x01DC 
	char pad_0x01E0[0x8]; //0x01E0
	float some_pose; //0x01E8 
	char pad_0x01EC[0x14]; //0x01EC
	float body_yaw; //0x0200 
	char pad_0x0204[0x8]; //0x0204
	float body_pitch; //0x020C 
	char pad_0x0210[0x8]; //0x0210
	float death_yaw; //0x0218 
	char pad_0x021C[0x8]; //0x021C
	float stand; //0x0224 
	char pad_0x0228[0x8]; //0x0228
	float jump_fall; //0x0230 
	char pad_0x0234[0x8]; //0x0234
	float aim_blend_stand_idle; //0x023C 
	char pad_0x0240[0x8]; //0x0240
	float aim_blend_crouch_idle; //0x0248 
	char pad_0x024C[0x8]; //0x024C
	float strafe_yaw; //0x0254 
	char pad_0x0258[0x8]; //0x0258
	float aim_blend_stand_walk; //0x0260 
	char pad_0x0264[0x8]; //0x0264
	float aim_blend_stand_run; //0x026C 
	char pad_0x0270[0x8]; //0x0270
	float aim_blend_crouch_walk; //0x0278 
	char pad_0x027C[0x8]; //0x027C
	float move_blend_walk; //0x0284 
	char pad_0x0288[0x8]; //0x0288
	float move_blend_run; //0x0290 
	char pad_0x0294[0x8]; //0x0294
	float move_blend_crouch; //0x029C 
	char pad_0x02A0[0x4]; //0x02A0
	float speed; //0x02A4 
	__int32 moving_in_any_direction; //0x02A8 
	float acceleration; //0x02AC 
	char pad_0x02B0[0x74]; //0x02B0
	float crouch_height; //0x0324 
	__int32 is_full_crouched; //0x0328 
	char pad_0x032C[0x4]; //0x032C
	float velocity_subtract_x; //0x0330 
	float velocity_subtract_y; //0x0334 
	float velocity_subtract_z; //0x0338 
	float standing_head_height; //0x033C 
	char pad_0x0340[0x4]; //0x0340


	void update( float y, float x ){
		static auto update = pattern::find( "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54" );

		_asm {
			mov ecx, this
			movss xmm1, y
			movss xmm2, x
			call update
		}
	}
}; //Size=0x0344
