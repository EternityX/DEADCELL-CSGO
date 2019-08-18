#pragma once

enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class chl_client {
public:
	client_class *get_all_classes() {
		return util::misc::vfunc< client_class *(__thiscall *)( decltype( this ) ) >( this, 8 )( this );
	}
};

typedef struct player_info_s {
	int64_t __pad0;
	int xuid_low;
	int xuid_high;
	char m_player_name[ 256 ];
	int m_user_id;
	char m_steam_id[ 33 ];
	unsigned int m_steam3_id;
	char m_friends_name[ 256 ];
	bool m_is_fake_player;
	bool m_is_hltv;
	unsigned int m_custom_files[ 4 ];
	unsigned char m_files_downloaded;
} player_info_t;

typedef struct InputContextHandle_t__ *input_context_handle_t;
struct client_textmessage_t;
class model_t;
class surf_info;
class i_material;
class c_sentence;
class c_audio_source;
class audio_state_t;
class i_spatial_query;
class i_material_system;
class c_phys_collide;
class i_achievement_mngr;
class i_net_channel_info;
class isp_shared_memory;
class c_gamestats_data;
class key_values;
class c_steam_api_context;
struct frustum_t;
class pfn_demo_custom_data_callback;

class i_engine_client {
public:
	virtual int get_intersecting_surfaces( const model_t *model, const vec3_t &vCenter, const float radius,
	                                     const bool bOnlyVisibleSurfaces, surf_info *pInfos, const int nMaxInfos ) = 0;
	virtual vec3_t get_light_for_point( const vec3_t &pos, bool bClamp ) = 0;
	virtual i_material *trace_line_material_and_lighting( const vec3_t &start, const vec3_t &end, vec3_t &diffuseLightColor,
	                                                 vec3_t &baseColor ) = 0;
	virtual const char *parse_file( const char *data, char *token, int maxlen ) = 0;
	virtual bool copy_file( const char *source, const char *destination ) = 0;
	virtual void get_screen_size( int &width, int &height ) = 0;
	virtual void server_cmd( const char *szCmdString, bool bReliable = true ) = 0;
	virtual void client_cmd( const char *szCmdString ) = 0;
	virtual bool get_player_info( int ent_num, player_info_t *pinfo ) = 0;
	virtual int get_player_for_user_id( int userID ) = 0;
	virtual client_textmessage_t *text_message_get( const char *pName ) = 0; // 10
	virtual bool con_is_visible( void ) = 0;
	virtual int get_local_player( void ) = 0;
	virtual const model_t *load_model( const char *pName, bool bProp = false ) = 0;
	virtual float get_last_time_stamp( void ) = 0;
	virtual c_sentence *get_sentence( c_audio_source *pAudioSource ) = 0; // 15
	virtual float get_sentence_length( c_audio_source *pAudioSource ) = 0;
	virtual bool is_streaming( c_audio_source *pAudioSource ) const = 0;
	virtual void get_viewangles( vec3_t &va ) = 0;
	virtual void set_viewangles( vec3_t &va ) = 0;
	virtual int get_max_clients( void ) = 0; // 20
	virtual const char *key_lookup_binding( const char *pBinding ) = 0;
	virtual const char *key_binding_for_key( int &code ) = 0;
	virtual void key_set_binding( int, char const * ) = 0;
	virtual void start_key_trap_mode( void ) = 0;
	virtual bool check_done_key_trapping( int &code ) = 0;
	virtual bool is_in_game( void ) = 0;
	virtual bool is_connected( void ) = 0;
	virtual bool is_drawing_loading_image( void ) = 0;
	virtual void hide_loading_plaque( void ) = 0;
	virtual void con_printf( int pos, const char *fmt, ... ) = 0; // 30
	virtual void con_nx_printf( const struct con_nprint_s *info, const char *fmt, ... ) = 0;
	virtual int is_box_visible( const vec3_t &mins, const vec3_t &maxs ) = 0;
	virtual int is_box_in_view_cluster( const vec3_t &mins, const vec3_t &maxs ) = 0;
	virtual bool cull_box( const vec3_t &mins, const vec3_t &maxs ) = 0;
	virtual void sound_extra_update( void ) = 0;
	virtual const char *get_game_directory( void ) = 0;
	virtual const matrix3x4_t &world_to_screen_matrix() = 0;
	virtual const matrix3x4_t &world_to_view_matrix() = 0;
	virtual int game_lump_version( int lumpId ) const = 0;
	virtual int game_lump_size( int lumpId ) const = 0; // 40
	virtual bool load_game_lump( int lumpId, void *pBuffer, int size ) = 0;
	virtual int level_leaf_count() const = 0;
	virtual i_spatial_query *get_bsp_tree_query() = 0;
	virtual void linear_to_gamma( float *linear, float *gamma ) = 0;
	virtual float light_style_value( int style ) = 0; // 45
	virtual void compute_dynamic_lighting( const vec3_t &pt, const vec3_t *pNormal, vec3_t &color ) = 0;
	virtual void get_ambient_light_color( vec3_t &color ) = 0;
	virtual int get_dx_support_level() = 0;
	virtual bool supports_hdr() = 0;
	virtual void mat_stub( i_material_system *pMatSys ) = 0; // 50
	virtual void get_chapter_name( char *pchBuff, int iMaxLength ) = 0;
	virtual char const *get_level_name( void ) = 0;
	virtual char const *get_level_name_short( void ) = 0;
	virtual char const *get_map_group_name( void ) = 0;
	virtual struct i_voice_tweak_s *get_voice_tweak_api( void ) = 0;
	virtual void set_voice_caster_id( unsigned int someint ) = 0; // 56
	virtual void enginestats_begin_frame( void ) = 0;
	virtual void enginestats_end_frame( void ) = 0;
	virtual void fire_events() = 0;
	virtual int get_leaves_area( unsigned short *pLeaves, int nLeaves ) = 0;
	virtual bool does_box_touch_area_frustum( const vec3_t &mins, const vec3_t &maxs, int iArea ) = 0; // 60
	virtual int get_frustum_list( frustum_t **pList, int listMax ) = 0;
	virtual bool should_use_area_frustum( int i ) = 0;
	virtual void set_audio_state( const audio_state_t &state ) = 0;
	virtual int sentence_group_pick( int groupIndex, char *name, int nameBufLen ) = 0;
	virtual int sentence_group_pick_sequential( int groupIndex, char *name, int nameBufLen, int sentenceIndex, int reset ) =
	0;
	virtual int sentence_index_from_name( const char *pSentenceName ) = 0;
	virtual const char *sentence_name_from_index( int sentenceIndex ) = 0;
	virtual int sentence_group_index_from_name( const char *pGroupName ) = 0;
	virtual const char *sentence_group_name_from_index( int groupIndex ) = 0;
	virtual float sentence_length( int sentenceIndex ) = 0;
	virtual void compute_lighting( const vec3_t &pt, const vec3_t *pNormal, bool bClamp, vec3_t &color,
	                              vec3_t *pBoxColors = nullptr ) = 0;
	virtual void activate_occluder( int nOccluderIndex, bool bActive ) = 0;
	virtual bool is_occluded( const vec3_t &vecAbsMins, const vec3_t &vecAbsMaxs ) = 0; // 74
	virtual int get_occlusion_view_id( void ) = 0;
	virtual void *save_alloc_memory( size_t num, size_t size ) = 0;
	virtual void save_free_memory( void *pSaveMem ) = 0;
	virtual i_net_channel_info *get_net_channel_info( void ) = 0;
	virtual void debug_draw_phys_collide( const c_phys_collide *pCollide, i_material *pMaterial, const matrix3x4_t &transform,
	                                   const Color &color ) = 0; //79
	virtual void check_point( const char *pName ) = 0; // 80
	virtual void draw_portals() = 0;
	virtual bool is_playing_demo( void ) = 0;
	virtual bool is_recording_demo( void ) = 0;
	virtual bool is_playing_time_demo( void ) = 0;
	virtual int get_demo_recording_tick( void ) = 0;
	virtual int get_demo_playback_tick( void ) = 0;
	virtual int get_demo_playback_start_tick( void ) = 0;
	virtual float get_demo_playback_time_scale( void ) = 0;
	virtual int get_demo_playback_total_ticks( void ) = 0;
	virtual bool is_paused( void ) = 0; // 90
	virtual float get_timescale( void ) const = 0;
	virtual bool is_taking_screenshot( void ) = 0;
	virtual bool is_hltv( void ) = 0;
	virtual bool is_level_main_menu_background( void ) = 0;
	virtual void get_main_menu_background_name( char *dest, int destlen ) = 0;
	virtual void set_occlusion_parameters( const int /*OcclusionParams_t*/ &params ) = 0; // 96
	virtual void get_ui_language( char *dest, int destlen ) = 0;
	virtual int is_skybox_visible_from_point( const vec3_t &vecPoint ) = 0;
	virtual const char *get_map_entities_string() = 0;
	virtual bool is_in_edit_mode( void ) = 0; // 100
	virtual float get_screen_aspect_ratio( int viewportWidth, int viewportHeight ) = 0;
	virtual bool removed_steam_refresh_login( const char *password, bool isSecure ) = 0; // 100
	virtual bool removed_steam_process_call( bool &finished ) = 0;
	virtual unsigned int get_engine_build_number() = 0; // engines build
	virtual const char *get_product_version_string() = 0; // mods version number (steam.inf)
	virtual void grab_pre_color_corrected_frame( int x, int y, int width, int height ) = 0;
	virtual bool is_hammer_running() const = 0;
	virtual void execute_client_cmd( const char *szCmdString ) = 0; //108
	virtual bool map_has_hdr_lighting( void ) = 0;
	virtual bool map_has_light_map_alpha_data( void ) = 0;
	virtual int get_app_id() = 0;
	virtual vec3_t get_light_for_point_fast( const vec3_t &pos, bool bClamp ) = 0;
	virtual void client_cmd_unrestricted1( char const *, int, bool );
	virtual void client_cmd_unrestricted( const char *szCmdString, const char *newFlag = nullptr ) = 0;
	// 114, new flag, quick testing shows setting 0 seems to work, haven't looked into it.
	virtual void set_restrict_server_commands( bool bRestrict ) = 0;
	virtual void set_restrict_client_commands( bool bRestrict ) = 0;
	virtual void set_overlay_bind_proxy( int iOverlayID, void *pBindProxy ) = 0;
	virtual bool copy_frame_buffer_to_material( const char *pMaterialName ) = 0;
	virtual void read_configuration( const int iController, const bool readDefault ) = 0;
	virtual void set_achievement_mgr( i_achievement_mngr *pAchievementMgr ) = 0;
	virtual i_achievement_mngr *get_achievement_mgr() = 0;
	virtual bool map_load_failed( void ) = 0;
	virtual void set_map_load_failed( bool bState ) = 0;
	virtual bool is_low_violence() = 0;
	virtual const char *get_most_recent_save_game( void ) = 0;
	virtual void set_most_recent_save_game( const char *lpszFilename ) = 0;
	virtual void start_xbox_exiting_process() = 0;
	virtual bool is_save_in_progress() = 0;
	virtual bool is_auto_save_dangerous_in_progress( void ) = 0;
	virtual unsigned int on_storage_device_attached( int iController ) = 0;
	virtual void on_storage_device_detached( int iController ) = 0;
	virtual char * const get_save_dir_name( void ) = 0;
	virtual void write_screenshot( const char *pFilename ) = 0;
	virtual void reset_demo_interpolation( void ) = 0;
	virtual int get_active_split_screen_player_slot() = 0;
	virtual int set_active_split_screen_player_slot( int slot ) = 0;
	virtual bool set_local_player_is_resolvable( char const *pchContext, int nLine, bool bResolvable ) = 0;
	virtual bool is_local_player_resolvable() = 0;
	virtual int get_split_screen_player( int nSlot ) = 0;
	virtual bool is_split_screen_active() = 0;
	virtual bool is_valid_split_screen_slot( int nSlot ) = 0;
	virtual int first_valid_split_screen_slot() = 0; // -1 == invalid
	virtual int next_valid_split_screen_slot( int nPreviousSlot ) = 0; // -1 == invalid
	virtual isp_shared_memory *get_single_player_shared_memory_space( const char *szName, int ent_num = ( 1 << 11 ) ) = 0;
	virtual void compute_lighting_cube( const vec3_t &pt, bool bClamp, vec3_t *pBoxColors ) = 0;
	virtual void register_demo_custom_data_callback( const char *szCallbackSaveID, pfn_demo_custom_data_callback pCallback ) = 0;
	virtual void record_demo_custom_data( pfn_demo_custom_data_callback pCallback, const void *pData, size_t iDataLength ) = 0;
	virtual void set_pitch_scale( float flPitchScale ) = 0;
	virtual float get_pitch_scale( void ) = 0;
	virtual bool load_filmmaker() = 0;
	virtual void unload_filmmaker() = 0;
	virtual void set_leaf_flag( int nLeafIndex, int nFlagBits ) = 0;
	virtual void recalculate_bsp_leaf_flags( void ) = 0;
	virtual bool dsp_get_current_das_room_new( void ) = 0;
	virtual bool dsp_get_current_das_room_changed( void ) = 0;
	virtual bool dsp_get_current_das_room_sky_above( void ) = 0;
	virtual float dsp_get_current_das_room_sky_percent( void ) = 0;
	virtual void set_mix_group_of_current_mixer( const char *szgroupname, const char *szparam, float val, int setMixerType ) =
	0;
	virtual int get_mix_layer_index( const char *szmixlayername ) = 0;
	virtual void set_mix_layer_level( int index, float level ) = 0;
	virtual int get_mix_group_index( char const *groupname ) = 0;
	virtual void set_mix_layer_trigger_factor( int i1, int i2, float fl ) = 0;
	virtual void set_mix_layer_trigger_factor( char const *char1, char const *char2, float fl ) = 0;
	virtual bool is_creating_reslist() = 0;
	virtual bool is_creating_xbox_reslist() = 0;
	virtual void set_timescale( float flTimescale ) = 0;
	virtual void set_gamestats_data( c_gamestats_data *pGamestatsData ) = 0;
	virtual c_gamestats_data *get_gamestats_data() = 0;
	virtual const char *key_lookup_binding_ex( const char *pBinding, int iUserId = -1, int iStartCount = 0,
	                                         int iAllowJoystick = -1 ) = 0;
	virtual int key_code_for_binding( char const *, int, int, int ) = 0;
	virtual void update_lights( void ) = 0;
	virtual int get_bug_submission_count() const = 0;
	virtual void clear_bug_submission_count() = 0;
	virtual bool does_level_contain_water() const = 0;
	virtual float get_server_simulation_frame_time() const = 0;
	virtual void solid_moved( class i_client_entity *pSolidEnt, class i_collideable *pSolidCollide,
	                         const vec3_t *pPrevAbsOrigin, bool accurateBboxTriggerChecks ) = 0;
	virtual void trigger_moved( class i_client_entity *pTriggerEnt, bool accurateBboxTriggerChecks ) = 0;
	virtual void compute_leaves_connected( const vec3_t &vecOrigin, int nCount, const int *pLeafIndices, bool *pIsConnected )
	= 0;
	virtual bool is_in_commentary_mode( void ) = 0;
	virtual void set_blur_fade( float amount ) = 0;
	virtual bool is_transitioning_to_load() = 0;
	virtual void search_paths_changed_after_install() = 0;
	virtual void configure_system_level( int nCPULevel, int nGPULevel ) = 0;
	virtual void set_connection_password( char const *pchCurrentPW ) = 0;
	virtual c_steam_api_context *get_steam_api_context() = 0;
	virtual void submit_stat_record( char const *szMapName, unsigned int uiBlobVersion, unsigned int uiBlobSize,
	                               const void *pvBlob ) = 0;
	virtual void server_cmd_key_values( key_values *pKeyValues ) = 0; // 203
	virtual void sphere_paint_surface( const model_t *model, const vec3_t &location, unsigned char chr, float fl1,
	                                 float fl2 ) = 0;
	virtual bool has_paintmap( void ) = 0;
	virtual void enable_paintmap_render() = 0;
	//virtual void                TracePaintSurface( const model_t *model, const vec3_t& position, float radius, CUtlvec3_t<Color>& surfColors ) = 0;
	virtual void sphere_trace_paint_surface( const model_t *model, const vec3_t &position, const vec3_t &vec2, float radius,
	                                      /*CUtlvec3_t<unsigned char, CUtlMemory<unsigned char, int>>*/
	                                      int &utilVecShit ) = 0;
	virtual void remove_all_paint() = 0;
	virtual void paint_all_surfaces( unsigned char uchr ) = 0;
	virtual void remove_paint( const model_t *model ) = 0;
	virtual bool is_active_app() = 0;
	virtual bool is_client_local_to_active_server() = 0;
	virtual void tick_progress_bar() = 0;
	virtual input_context_handle_t get_input_context( int /*EngineInputContextId_t*/ id ) = 0;
	virtual void get_startup_image( char *filename, int size ) = 0;
	virtual bool is_using_local_network_backdoor( void ) = 0;
	virtual void save_game( const char *, bool, char *, int, char *, int ) = 0;
	virtual void get_generic_memory_stats( /* GenericMemoryStat_t */ void ** ) = 0;
	virtual bool game_has_shutdown_and_flushed_memory( void ) = 0;
	virtual int get_last_acknowledged_command( void ) = 0;
	virtual void finish_container_writes( int i ) = 0;
	virtual void finish_async_save( void ) = 0;
	virtual int get_server_tick( void ) = 0;
	virtual const char *get_mod_directory( void ) = 0;
	virtual bool audio_language_changed( void ) = 0;
	virtual bool is_auto_save_in_progress( void ) = 0;
	virtual void start_loading_screen_for_command( const char *command ) = 0;
	virtual void start_loading_screen_for_key_values( key_values *values ) = 0;
	virtual void sos_set_opvar_float( const char *, float ) = 0;
	virtual void sos_get_opvar_float( const char *, float & ) = 0;
	virtual bool is_subscribed_map( const char *, bool ) = 0;
	virtual bool is_featured_map( const char *, bool ) = 0;
	virtual void get_demo_playback_parameters( void ) = 0;
	virtual int get_client_version( void ) = 0;
	virtual bool is_demo_skipping( void ) = 0;
	virtual void set_demo_important_event_data( const key_values *values ) = 0;
	virtual void clear_events( void ) = 0;
	virtual int get_safe_zone_x_min( void ) = 0;
	virtual bool is_voice_recording( void ) = 0;
	virtual void force_voice_record_on( void ) = 0;
	virtual bool is_replay( void ) = 0;

	D3DMATRIX &world_to_screen_matrix_d3d() {
		ulong_t function_ptr = ( *reinterpret_cast< ulong_t * * >( this ) )[ 37 ];
		ulong_t render_ptr = *reinterpret_cast< ulong_t* >( function_ptr + 1 );
		ulong_t view_matrix = *reinterpret_cast< ulong_t* >( render_ptr + 0xDC ) + 0x3DC;

		return reinterpret_cast< D3DMATRIX& >( *reinterpret_cast< ulong_t* >( view_matrix ) );
	}
};

struct c_view_setup {
	PAD( 0x10 )
	int m_x;
	int m_x_old;
	int m_y;
	int m_y_old;
	int m_width;
	int m_width_old;
	int m_height;
	int m_height_old;
	PAD( 0x80 )
	float m_fov;
	float m_fov_viewmodel;
	vec3_t m_origin;
	vec3_t m_angles;
	float m_znear;
	float m_zfar;
	float m_znear_viewmodel;
	float m_zfar_viewmodel;
	float m_aspect_ratio;
	float m_near_blur_depth;
	float m_near_focus_depth;
	float m_far_focus_depth;
	float m_far_blur_depth;
	float m_near_blur_radius;
	float m_far_blur_radius;
	float m_dof_quality;
	int m_motionblur_mode;
	PAD( 0x44 )
	int m_edge_blur;
};
