#pragma once

class vars {
public:
	struct rage {
		int key;
		bool enabled;
		int target_selection;
		float pointscale = 60.f;
		bool prefer_bodyaim;
		bool bodyaim_lethal;
		int hitchance;
		int min_dmg;
		bool auto_fire;
		bool silent;
		bool autostop;
		int autoscope;
		bool head;
		bool pelvis;
		bool legs;
		bool arms;
		bool teammate;
		bool hitbox_head;
		bool hitbox_pelvis;
		bool hitbox_legs;
		bool hitbox_arms;
		int primary_hitbox;
		bool dynamic_hitbox;
	} rage;
	struct visuals {
		int activation_type;
		int activation_key;
		bool bbox;
		float box_color[ 4 ] = { 220, 255, 0, 0 };
		bool name;
		float name_color[ 4 ] = { 220, 255, 255, 255 };
		bool weapon;
		float weapon_color[ 4 ] = { 220, 255, 255, 255 };
		bool healthbar;
		bool ammo_bar;
		float ammo_bar_color[ 4 ] = { 220, 79, 130, 255 };
		bool money;
		float money_color[ 4 ] = { 200, 149, 184, 6 };
		bool flags;
		float flag_color[ 4 ] = { 200, 255, 255, 255 };
		bool flash_bar;
		float flash_bar_color[ 4 ] = { 220, 255, 255, 255 };
		bool glow;
		float glow_color[4] = { 155, 206, 115, 136 };
		int filter;
		bool dropped_weapons;
		bool dropped_ammo;
		float dropped_ammo_color[ 4 ] = { 220, 79, 130, 255 };
		bool chicken;
		bool projectiles;
		bool objectives;
		bool dormancy_fade;
		bool grenade_pred;
		bool spread_circle;
		bool hitmarker;
		bool impact;
		float impact_color[ 4 ] = { 255, 255, 0, 0 };
		bool radar;
		float spread_circle_color[ 4 ] = { 70, 113, 59, 198 };
		struct chams{
			bool teammates;
			bool enabled;
			bool twopass;
			int type;
			float alpha;
			float vis_color[ 4 ] = { 220, 255, 0, 0 };
			float hid_color[ 4 ] = { 220, 255, 255, 255 };
			bool local;
			float local_col[ 4 ] = { 220, 206, 115, 136 };
		} chams;
		struct misc {
			bool no_flash;
			bool fog;
			bool remove_scope;
			bool remove_blur;
			float scope_color[ 4 ] = { 220, 0, 0, 0 };
			bool remove_smoke;
		} misc;
		struct extra {
			bool speclist;
			bool antiaim_direction;
			float antiaim_direction_color[ 4 ] = { 178, 113, 59, 198 };
			bool misc_flags;
			bool points;
		} extra;
		struct effects {
			int camera_fov = 90;
			int weapon_fov = 64;
		} effects;
	} visuals;
	struct misc {
		struct fakelag {
			bool enabled;
			int type;
			int amount;
			int lagjump;
		} fakelag;
		bool bhop;
		bool air_strafe;
		bool thirdperson;
		bool thirdperson_dead;
		bool thirdperson_grenade;
		int thirdperson_key;
		bool autozeus;
		bool log_damage;
		bool log_purchases;
		int nightmode;
		int prop_transparency;
		bool fast_duck;
		bool dangerzone_menu;
		int hitmarker_sound;
		bool clienthitboxes;
		float duration;
	} misc;
	struct antiaim{
		bool enabled;
		int yaw;
		float jitter;
		int fake_yaw;
		int pitch;
		int left_override_key = 37;
		int right_override_key = 39;
		int backwards_override_key = 40;
		int reset_override_key = 38;
	} antiaim;
	struct dangerzone{
		int max_item_distance;
		bool healthshot;
		bool drone;
		bool turret;
		bool cash;
		bool safe;
		bool weapon_case;
		bool tool_case;
		bool tablet_upgrade;
		bool airdrop;
		bool armor;
		bool contract;
		bool parachute;
		bool ammobox;
		bool jammer;
		bool melee_weapon;
		bool breach_charge;
	} dz;
};

extern vars g_vars;