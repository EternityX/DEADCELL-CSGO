#pragma once

enum
{
	TE_BEAMPOINTS = 0x00,		// beam effect between two points
	TE_SPRITE = 0x01,	// additive sprite, plays 1 cycle
	TE_BEAMDISK = 0x02,	// disk that expands to max radius over lifetime
	TE_BEAMCYLINDER = 0x03,		// cylinder that expands to max radius over lifetime
	TE_BEAMFOLLOW = 0x04,		// create a line of decaying beam segments until entity stops moving
	TE_BEAMRING = 0x05,		// connect a beam ring to two entities
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08,		// Fades according to viewpoint
	TE_BEAMTESLA = 0x09,
};

enum {
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100,
	// Only calculate our noise once
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400,
	// Attachment indices represent hitbox indices instead when this is set.
	FBEAM_STARTVISIBLE = 0x00000800,
	// Has this client actually seen this beam's start entity yet?
	FBEAM_ENDVISIBLE = 0x00001000,
	// Has this client actually seen this beam's end entity yet?
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000,
	// When drawing a beam with a halo, don't ignore the segments and endwidth
	FBEAM_REVERSED = 0x00010000,
	NUM_BEAM_FLAGS = 17 // KEEP THIS UPDATED!
};

struct beam_trail_t {
	// NOTE:  Don't add user defined fields except after these four fields.
	beam_trail_t *next;
	float die;
	vec3_t org;
	vec3_t vel;
};

#define NOISE_DIVISIONS		128
#define MAX_BEAM_ENTS		10

typedef int pixelvis_handle_t;

class beam_t
{
public:
	beam_t();
	// Methods of i_client_renderable
	virtual const vec3_t&	get_render_origin(void) = 0;
	virtual const vec3_t&	get_render_angles(void) = 0;
	virtual const matrix3x4_t &renderable_to_world_transform() = 0;
	virtual void			get_render_bounds(vec3_t& mins, vec3_t& maxs) = 0;
	virtual bool			should_draw(void) = 0;
	virtual bool			is_transparent(void) = 0;
	virtual int				draw_model(int flags) = 0;
	virtual void			compute_fx_blend() = 0;
	virtual int				get_fx_blend() = 0;
	// Resets the beam state
	void			reset();
	// Method to computing the bounding box
	void			compute_bounds();
	// Bounding box...
	vec3_t			m_mins;
	vec3_t			m_maxs;
	pixelvis_handle_t *m_query_handle_halo;
	float			m_halo_proxy_size;
	// Data is below..
	// Next beam in list
	beam_t*			m_next;
	// Type of beam
	int				m_type;
	int				m_flags;
	// Control points for the beam
	int				m_num_attachments;
	vec3_t			m_attachment[MAX_BEAM_ENTS];
	vec3_t			m_delta;
	// 0 .. 1 over lifetime of beam
	float			m_t;
	float			m_freq;
	// Time when beam should die
	float			m_die;
	float			m_width;
	float			m_end_width;
	float			m_fade_length;
	float			m_amplitude;
	float			m_life;
	// Color
	float			r, g, b;
	float			m_brightness;
	// Speed
	float			m_speed;
	// Animation
	float			m_frame_rate;
	float			m_frame;
	int				m_segments;
	// Attachment entities for the beam
	HANDLE			m_entity[MAX_BEAM_ENTS];
	int				m_attachment_index[MAX_BEAM_ENTS];
	// Model info
	int				m_model_index;
	int				m_halo_index;
	float			m_halo_scale;
	int				m_frame_count;
	float			m_rg_noise[NOISE_DIVISIONS + 1];
	// Popcorn trail for beam follows to use
	beam_trail_t*	m_trail;
	// for TE_BEAMRINGPOINT
	float			m_start_radius;
	float			m_end_radius;
	// for FBEAM_ONLYNOISEONCE
	bool			m_calculated_noise;
	float			m_hdr_color_scale;
};

struct beam_info_t {
	int			m_type;

	// Entities
	i_client_entity* m_start_ent;
	int			m_start_attachment;
	i_client_entity* m_end_ent;
	int			m_end_attachment;

	// Points
	vec3_t		m_start;
	vec3_t		m_end;

	int			m_model_index;
	const char	*m_model_name;

	int			m_halo_index;
	const char	*m_halo_name;
	float		m_halo_scale;

	float		m_life;
	float		m_width;
	float		m_end_width;
	float		m_fade_length;
	float		m_amplitude;

	float		m_brightness;
	float		m_speed;

	int			m_start_frame;
	float		m_frame_rate;

	float		m_red;
	float		m_green;
	float		m_blue;

	bool		m_renderable;

	int			m_num_segments;

	int			m_flags;

	// Rings
	vec3_t		m_center;
	float		m_start_radius;
	float		m_end_radius;

	beam_info_t() {
		m_type = TE_BEAMPOINTS;
		m_num_segments = -1;
		m_model_name = NULL;
		m_halo_name = NULL;
		m_model_index = -1;
		m_halo_index = -1;
		m_renderable = true;
		m_flags = 0;
	}
};

class c_beam;

class i_view_render_beams {
	// Construction
public:
	i_view_render_beams( void );
	virtual ~i_view_render_beams( void ) = 0;

	// Implement IViewRenderBeams
public:
	virtual void init_beams( void ) = 0;
	virtual void shutdown_beams( void ) = 0;
	virtual void clear_beams( void ) = 0;

	// Updates the state of the temp ent beams
	virtual void update_temp_ent_beams() = 0;

	virtual void draw_beam( beam_t *pbeam ) = 0;
	virtual void draw_beam( c_beam *pbeam, i_trace_filter *pEntityBeamTraceFilter = NULL ) = 0;

	virtual void kill_dead_beams( c_base_entity *pDeadEntity ) = 0;

	virtual void create_beam_ents( int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
	                             float life, float width, float endWidth, float fadeLength, float amplitude,
	                             float brightness, float speed, int startFrame,
	                             float framerate, float r, float g, float b, int type = -1 ) = 0;
	virtual beam_t *create_beam_ents( beam_info_t &beamInfo ) = 0;

	virtual void create_beam_ent_point( int nStartEntity, const vec3_t *pStart, int nEndEntity, const vec3_t *pEnd,
	                                 int modelIndex, int haloIndex, float haloScale,
	                                 float life, float width, float endWidth, float fadeLength, float amplitude,
	                                 float brightness, float speed, int startFrame,
	                                 float framerate, float r, float g, float b ) = 0;
	virtual beam_t *create_beam_ent_point( beam_info_t &beamInfo ) = 0;

	virtual void create_beam_points( vec3_t &start, vec3_t &end, int modelIndex, int haloIndex, float haloScale,
	                               float life, float width, float endWidth, float fadeLength, float amplitude,
	                               float brightness, float speed, int startFrame,
	                               float framerate, float r, float g, float b ) = 0;
	virtual beam_t *create_beam_points( beam_info_t &beamInfo ) = 0;

	virtual void create_beam_ring( int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
	                             float life, float width, float endWidth, float fadeLength, float amplitude,
	                             float brightness, float speed, int startFrame,
	                             float framerate, float r, float g, float b, int flags ) = 0;
	virtual beam_t *create_beam_ring( beam_info_t &beamInfo ) = 0;

	virtual void create_beam_ring_point( const vec3_t &center, float start_radius, float end_radius, int modelIndex,
	                                  int haloIndex, float haloScale,
	                                  float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
	                                  float brightness, float speed, int startFrame,
	                                  float framerate, float r, float g, float b, int flags ) = 0;
	virtual beam_t *create_beam_ring_point( beam_info_t &beamInfo ) = 0;

	virtual void create_beam_circle_points( int type, vec3_t &start, vec3_t &end,
	                                     int modelIndex, int haloIndex, float haloScale, float life, float width,
	                                     float endWidth, float fadeLength, float amplitude, float brightness, float speed,
	                                     int startFrame, float framerate, float r, float g, float b ) = 0;
	virtual beam_t *create_beam_circle_points( beam_info_t &beamInfo ) = 0;

	virtual void create_beam_follow( int startEnt, int modelIndex, int haloIndex, float haloScale,
	                               float life, float width, float endWidth, float fadeLength, float r, float g, float b,
	                               float brightness ) = 0;
	virtual beam_t *create_beam_follow( beam_info_t &beamInfo ) = 0;
};
