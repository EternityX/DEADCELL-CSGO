#pragma once

enum {
	TE_BEAMPOINTS = 0x00,
	// beam effect between two points
	TE_SPRITE = 0x01,
	// additive sprite, plays 1 cycle
	TE_BEAMDISK = 0x02,
	// disk that expands to max radius over lifetime
	TE_BEAMCYLINDER = 0x03,
	// cylinder that expands to max radius over lifetime
	TE_BEAMFOLLOW = 0x04,
	// create a line of decaying beam segments until entity stops moving
	TE_BEAMRING = 0x05,
	// connect a beam ring to two entities
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08,
	// Fades according to viewpoint
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

struct BeamTrail_t {
	// NOTE:  Don't add user defined fields except after these four fields.
	BeamTrail_t *next;
	float die;
	vec3_t org;
	vec3_t vel;
};

#define NOISE_DIVISIONS		128
#define MAX_BEAM_ENTS		10

typedef int pixelvis_handle_t;

class Beam_t {
public:
	Beam_t();

	// Methods of IClientRenderable
	virtual const vec3_t &GetRenderOrigin( void ) = 0;
	virtual const vec3_t &GetRenderAngles( void ) = 0;
	virtual const matrix3x4_t &RenderableToWorldTransform() = 0;
	virtual void GetRenderBounds( vec3_t &mins, vec3_t &maxs ) = 0;
	virtual bool ShouldDraw( void ) = 0;
	virtual bool IsTransparent( void ) = 0;
	virtual int DrawModel( int flags ) = 0;
	virtual void ComputeFxBlend() = 0;
	virtual int GetFxBlend() = 0;

	// Resets the beam state
	void Reset();

	// Method to computing the bounding box
	void ComputeBounds();

	// Bounding box...
	vec3_t m_Mins;
	vec3_t m_Maxs;
	pixelvis_handle_t *m_queryHandleHalo;
	float m_haloProxySize;

	// Data is below..

	// Next beam in list
	Beam_t *next;

	// Type of beam
	int type;
	int flags;

	// Control points for the beam
	int numAttachments;
	vec3_t attachment[ MAX_BEAM_ENTS ];
	vec3_t delta;

	// 0 .. 1 over lifetime of beam
	float t;
	float freq;

	// Time when beam should die
	float die;
	float width;
	float endWidth;
	float fadeLength;
	float amplitude;
	float life;

	// Color
	float r, g, b;
	float brightness;

	// Speed
	float speed;

	// Animation
	float frameRate;
	float frame;
	int segments;

	// Attachment entities for the beam
	HANDLE entity[ MAX_BEAM_ENTS ];
	int attachmentIndex[ MAX_BEAM_ENTS ];

	// Model info
	int modelIndex;
	int haloIndex;

	float haloScale;
	int frameCount;

	float rgNoise[ NOISE_DIVISIONS + 1 ];

	// Popcorn trail for beam follows to use
	BeamTrail_t *trail;

	// for TE_BEAMRINGPOINT
	float start_radius;
	float end_radius;

	// for FBEAM_ONLYNOISEONCE
	bool m_bCalculatedNoise;

	float m_flHDRColorScale;
};

struct BeamInfo_t {
	//Beam
	int m_nType;
	C_BaseEntity *m_pStartEnt;
	int m_nStartAttachment;
	C_BaseEntity *m_pEndEnt;
	int m_nEndAttachment;
	vec3_t m_vecStart;
	vec3_t m_vecEnd;
	int m_nModelIndex;
	const char *m_pszModelName;
	int m_nHaloIndex;
	const char *m_pszHaloName;
	float m_flHaloScale;
	float m_flLife;
	float m_flWidth;
	float m_flEndWidth;
	float m_flFadeLength;
	float m_flAmplitude;
	float m_flBrightness;
	float m_flSpeed;
	int m_nStartFrame;
	float m_flFrameRate;
	float m_flRed;
	float m_flGreen;
	float m_flBlue;
	bool m_bRenderable;
	int m_nSegments;
	int m_nFlags;
	// Rings
	vec3_t m_vecCenter;
	float m_flStartRadius;
	float m_flEndRadius;

	BeamInfo_t() {
		m_nType = TE_BEAMPOINTS;
		m_nSegments = -1;
		m_pszModelName = nullptr;
		m_pszHaloName = nullptr;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};

class C_Beam;

class IViewRenderBeams {
	// Construction
public:
	IViewRenderBeams( void );
	virtual ~IViewRenderBeams( void ) = 0;

	// Implement IViewRenderBeams
public:
	virtual void InitBeams( void ) = 0;
	virtual void ShutdownBeams( void ) = 0;
	virtual void ClearBeams( void ) = 0;

	// Updates the state of the temp ent beams
	virtual void UpdateTempEntBeams() = 0;

	virtual void DrawBeam( Beam_t *pbeam ) = 0;
	virtual void DrawBeam( C_Beam *pbeam, ITraceFilter *pEntityBeamTraceFilter = NULL ) = 0;

	virtual void KillDeadBeams( C_BaseEntity *pDeadEntity ) = 0;

	virtual void CreateBeamEnts( int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
	                             float life, float width, float endWidth, float fadeLength, float amplitude,
	                             float brightness, float speed, int startFrame,
	                             float framerate, float r, float g, float b, int type = -1 ) = 0;
	virtual Beam_t *CreateBeamEnts( BeamInfo_t &beamInfo ) = 0;

	virtual void CreateBeamEntPoint( int nStartEntity, const vec3_t *pStart, int nEndEntity, const vec3_t *pEnd,
	                                 int modelIndex, int haloIndex, float haloScale,
	                                 float life, float width, float endWidth, float fadeLength, float amplitude,
	                                 float brightness, float speed, int startFrame,
	                                 float framerate, float r, float g, float b ) = 0;
	virtual Beam_t *CreateBeamEntPoint( BeamInfo_t &beamInfo ) = 0;

	virtual void CreateBeamPoints( vec3_t &start, vec3_t &end, int modelIndex, int haloIndex, float haloScale,
	                               float life, float width, float endWidth, float fadeLength, float amplitude,
	                               float brightness, float speed, int startFrame,
	                               float framerate, float r, float g, float b ) = 0;
	virtual Beam_t *CreateBeamPoints( BeamInfo_t &beamInfo ) = 0;

	virtual void CreateBeamRing( int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
	                             float life, float width, float endWidth, float fadeLength, float amplitude,
	                             float brightness, float speed, int startFrame,
	                             float framerate, float r, float g, float b, int flags ) = 0;
	virtual Beam_t *CreateBeamRing( BeamInfo_t &beamInfo ) = 0;

	virtual void CreateBeamRingPoint( const vec3_t &center, float start_radius, float end_radius, int modelIndex,
	                                  int haloIndex, float haloScale,
	                                  float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
	                                  float brightness, float speed, int startFrame,
	                                  float framerate, float r, float g, float b, int flags ) = 0;
	virtual Beam_t *CreateBeamRingPoint( BeamInfo_t &beamInfo ) = 0;

	virtual void CreateBeamCirclePoints( int type, vec3_t &start, vec3_t &end,
	                                     int modelIndex, int haloIndex, float haloScale, float life, float width,
	                                     float endWidth, float fadeLength, float amplitude, float brightness, float speed,
	                                     int startFrame, float framerate, float r, float g, float b ) = 0;
	virtual Beam_t *CreateBeamCirclePoints( BeamInfo_t &beamInfo ) = 0;

	virtual void CreateBeamFollow( int startEnt, int modelIndex, int haloIndex, float haloScale,
	                               float life, float width, float endWidth, float fadeLength, float r, float g, float b,
	                               float brightness ) = 0;
	virtual Beam_t *CreateBeamFollow( BeamInfo_t &beamInfo ) = 0;
};
