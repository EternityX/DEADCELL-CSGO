﻿#pragma once

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40
#define   CONTENTS_OPAQUE               0x80
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800
#define   CONTENTS_TEAM2                0x1000
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000

#define   CONTENTS_MONSTER              0x2000000
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001
#define   SURF_SKY2D                    0x0002
#define   SURF_SKY                      0x0004
#define   SURF_WARP                     0x0008
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020
#define   SURF_TRIGGER                  0x0040
#define   SURF_NODRAW                   0x0080

#define   SURF_HINT                     0x0100

#define   SURF_SKIP                     0x0200
#define   SURF_NOLIGHT                  0x0400
#define   SURF_BUMPLIGHT                0x0800
#define   SURF_NOSHADOWS                0x1000
#define   SURF_NODECALS                 0x2000
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000
#define   SURF_HITBOX                   0x8000

// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

#define TEXTURE_GROUP_LIGHTMAP						( "Lightmaps" )
#define TEXTURE_GROUP_WORLD							( "World textures" )
#define TEXTURE_GROUP_MODEL							( "Model textures" )
#define TEXTURE_GROUP_VGUI							( "VGUI textures" )
#define TEXTURE_GROUP_PARTICLE						( "Particle textures" )
#define TEXTURE_GROUP_DECAL							( "Decal textures" )
#define TEXTURE_GROUP_SKYBOX						( "SkyBox textures" )
#define TEXTURE_GROUP_CLIENT_EFFECTS				( "ClientEffect textures" )
#define TEXTURE_GROUP_OTHER							( "Other textures" )
#define TEXTURE_GROUP_PRECACHED						( "Precached" )
#define TEXTURE_GROUP_CUBE_MAP						( "CubeMap textures" )
#define TEXTURE_GROUP_RENDER_TARGET					( "RenderTargets" )
#define TEXTURE_GROUP_UNACCOUNTED					( "Unaccounted textures" )
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			( "Static Indices" )
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		( "Displacement Verts" )
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	( "Lighting Verts" )
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	( "World Verts" )
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	( "Model Verts" )
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	( "Other Verts" )
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			( "Dynamic Indices" )
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			( "Dynamic Verts" )
#define TEXTURE_GROUP_DEPTH_BUFFER					( "DepthBuffer" )
#define TEXTURE_GROUP_VIEW_MODEL					( "ViewModel" )
#define TEXTURE_GROUP_PIXEL_SHADERS					( "Pixel Shaders" )
#define TEXTURE_GROUP_VERTEX_SHADERS				( "Vertex Shaders" )
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			( "RenderTarget Surfaces" )
#define TEXTURE_GROUP_MORPH_TARGETS					( "Morph Targets" )

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)( dt ) / g_csgo.m_global_vars->m_interval_per_tick ) )
#define TICKS_TO_TIME( dt )		( g_csgo.m_global_vars->m_interval_per_tick * (float)( dt ) )

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

#define Assert( _exp ) ((void)0)

class model_t;
struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct mstudiobodyparts_t;
struct mstudiotexture_t;
struct virtualmodel_t;
class Quaternion;

class RadianEuler
{
public:
	RadianEuler( void )							{ }
	RadianEuler( float X, float Y, float Z )		{ x = X; y = Y; z = Z; }
	RadianEuler( Quaternion const &q );	// evil auto type promotion!!!
	RadianEuler( Angle_t const &angles );	// evil auto type promotion!!!

											// Initialization
	inline void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f )	{ x = ix; y = iy; z = iz; }

	//	conversion to qangle
	Angle_t ToQAngle( void ) const;
	bool IsValid() const;
	void Invalidate();

	inline float *Base() { return &x; }
	inline const float *Base() const { return &x; }

	// array access...
	float operator[]( int i ) const;
	float& operator[]( int i );

	float x, y, z;
};

class Quaternion				// same data-layout as engine's vec4_t,
{								//		which is a float[4]
public:
	inline Quaternion( void )	{}
	inline Quaternion( float ix, float iy, float iz, float iw ) : x( ix ), y( iy ), z( iz ), w( iw ) { }
	inline Quaternion( RadianEuler const &angle );	// evil auto type promotion!!!

	inline void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f )	{ x = ix; y = iy; z = iz; w = iw; }

	bool IsValid() const;
	void Invalidate();

	bool operator==( const Quaternion &src ) const;
	bool operator!=( const Quaternion &src ) const;

	float* Base() { return ( float* )this; }
	const float* Base() const { return ( float* )this; }

	// array access...
	float operator[]( int i ) const;
	float& operator[]( int i );

	float x, y, z, w;
};

class __declspec( align( 16 ) ) QuaternionAligned : public Quaternion
{
public:
	inline QuaternionAligned( void ) {};
	inline QuaternionAligned( float X, float Y, float Z, float W )
	{
		Init( X, Y, Z, W );
	}
public:
	explicit QuaternionAligned( const Quaternion &vOther )
	{
		Init( vOther.x, vOther.y, vOther.z, vOther.w );
	}

	QuaternionAligned& operator=( const Quaternion &vOther )
	{
		Init( vOther.x, vOther.y, vOther.z, vOther.w );
		return *this;
	}
};

struct mstudiobbox_t {
	int bone_index;
	int m_iGroup;
	vec3_t bb_min;
	vec3_t bb_max;
	int m_iHitboxNameIndex;
	int m_iPad01 [ 3 ];
	float m_flRadius;
	int m_iPad02 [ 4 ];

	const char *pszHitboxName() const {
		if( m_iHitboxNameIndex == 0 )
			return "";

		return ( reinterpret_cast< const char * >(this) + m_iHitboxNameIndex );
	}
};

struct mstudiohitboxset_t {
	int nameindex;

	const char *get_name() const {
		return ( (char *)this ) + nameindex;
	}

	int numhitboxes;
	int hitboxindex;

	mstudiobbox_t *pHitbox( int i ) const {
		return reinterpret_cast< mstudiobbox_t * >(( (unsigned char *)this ) + hitboxindex) + i;
	};
};

struct mstudiobone_t {
	int					sznameindex;
	inline char * const pszName( void ) const { return ( ( char * )this ) + sznameindex; }
	int		 			parent;		// parent bone
	int					bonecontroller[ 6 ];	// bone controller index, -1 == none

												// default values
	vec3_t				pos;
	Quaternion			quat;
	RadianEuler			rot;
	// compression scale
	vec3_t				posscale;
	vec3_t				rotscale;

	matrix3x4_t			poseToBone;
	Quaternion			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void *pProcedure() const { if( procindex == 0 ) return NULL; else return  ( void * )( ( ( byte * )this ) + procindex ); };
	int					surfacepropidx;	// index into string tablefor property name
	inline char * const pszSurfaceProp( void ) const { return ( ( char * )this ) + surfacepropidx; }
	inline int			GetSurfaceProp( void ) const { return surfacepropLookup; }

	int					contents;		// See BSPFlags.h for the contents flags
	int					surfacepropLookup;	// this index must be cached by the loader, not saved in the file
	int					unused[ 7 ];		// remove as appropriate
};

struct mstudiobonecontroller_t
{
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					inputfield;	// 0-3 user set controller, 4 mouth
	int					unused[ 8 ];
};

struct studiohdr_t {
	int					id;
	int					version;

	long				checksum;		// this has to be the same in the phy and vtx files to load!

	inline const char *	pszName( void ) const { return name; }
	char				name[ 64 ];

	int					length;

	vec3_t				eyeposition;	// ideal eye position

	vec3_t				illumposition;	// illumination center

	vec3_t				hull_min;		// ideal movement hull size
	vec3_t				hull_max;

	vec3_t				view_bbmin;		// clipping bounding box
	vec3_t				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;
	inline mstudiobone_t *pBone( int i ) const { Assert( i >= 0 && i < numbones ); return ( mstudiobone_t * )( ( ( byte * )this ) + boneindex ) + i; };
	int					RemapSeqBone( int iSequence, int iLocalBone ) const;	// maps local sequence bone to global bone
	int					RemapAnimBone( int iAnim, int iLocalBone ) const;		// maps local animations bone to global bone

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;
	inline mstudiobonecontroller_t *pBonecontroller( int i ) const { Assert( i >= 0 && i < numbonecontrollers ); return ( mstudiobonecontroller_t * )( ( ( byte * )this ) + bonecontrollerindex ) + i; };

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t	*pHitboxSet( int i ) const
	{
		Assert( i >= 0 && i < numhitboxsets );
		return ( mstudiohitboxset_t * )( ( ( byte * )this ) + hitboxsetindex ) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox( int i, int set ) const
	{
		mstudiohitboxset_t const *s = pHitboxSet( set );
		if( !s )
			return NULL;

		return s->pHitbox( i );
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount( int set ) const
	{
		mstudiohitboxset_t const *s = pHitboxSet( set );
		if( !s )
			return 0;

		return s->numhitboxes;
	};

	// file local animations? and sequences
	//private:
	int					numlocalanim;			// animations/poses
	int					localanimindex;		// animation descriptions
	inline mstudioanimdesc_t *pLocalAnimdesc( int i ) const { return NULL; };

	int					numlocalseq;				// sequences
	int					localseqindex;
	inline mstudioseqdesc_t *pLocalSeqdesc( int i ) const { return NULL; };

	//public:
	bool				SequencesAvailable() const
	{
		return true;
	}

	int					GetNumSeq() const;
	mstudioanimdesc_t	&pAnimdesc( int i ) const;
	mstudioseqdesc_t	&pSeqdesc( int i ) const;
	int					iRelativeAnim( int baseseq, int relanim ) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq( int baseseq, int relseq ) const;		// maps seq local seq reference to global seq index

																			//private:
	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;
	//public:
	int					GetSequenceActivity( int iSequence );
	void				SetSequenceActivity( int iSequence, int iActivity );
	int					GetActivityListVersion( void );
	void				SetActivityListVersion( int version ) const;
	int					GetEventListVersion( void );
	void				SetEventListVersion( int version );

	// raw textures
	int					numtextures;
	int					textureindex;
	inline mstudiotexture_t *pTexture( int i ) const { return NULL; };


	// raw textures search paths
	int					numcdtextures;
	int					cdtextureindex;
	inline char			*pCdtexture( int i ) const { return ( ( ( char * )this ) + *( ( int * )( ( ( byte * )this ) + cdtextureindex ) + i ) ); };

	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short		*pSkinref( int i ) const { return ( short * )( ( ( byte * )this ) + skinindex ) + i; };

	int					numbodyparts;
	int					bodypartindex;
	inline mstudiobodyparts_t	*pBodypart( int i ) const { return NULL; };
};

struct CIncomingSequence {
	CIncomingSequence::CIncomingSequence( int instate, int outstate, int seqnr, float time ) {
		inreliablestate = instate;
		outreliablestate = outstate;
		sequencenr = seqnr;
		curtime = time;
	}

	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};

enum class TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

enum OverrideType_t {
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

enum LightType_t {
	MATERIAL_LIGHT_DISABLE = 0,
	MATERIAL_LIGHT_POINT,
	MATERIAL_LIGHT_DIRECTIONAL,
	MATERIAL_LIGHT_SPOT,
};

struct LightDesc_t {
	LightType_t m_Type; //< MATERIAL_LIGHT_xxx
	vec3_t m_Color; //< color+intensity 
	vec3_t m_Position; //< light source center position
	vec3_t m_Direction; //< for SPOT, direction it is pointing
	float m_Range; //< distance range for light.0=infinite
	float m_Falloff; //< angular falloff exponent for spot lights
	float m_Attenuation0; //< constant distance falloff term
	float m_Attenuation1; //< linear term of falloff
	float m_Attenuation2; //< quadatic term of falloff
	float m_Theta; //< inner cone angle. no angular falloff 
	//< within this cone
	float m_Phi; //< outer cone angle

	// the values below are derived from the above settings for optimizations
	// These aren't used by DX8. . used for software lighting.
	float m_ThetaDot;
	float m_PhiDot;
	unsigned int m_Flags;
protected:
	float OneOver_ThetaDot_Minus_PhiDot;
	float m_RangeSquared;
public:
};

struct DrawModelInfo_t {
	studiohdr_t *m_pStudioHdr;
	void *m_pHardwareData;
	void *m_Decals;
	int m_Skin;
	int m_Body;
	int m_HitboxSet;
	DWORD *m_pClientEntity;
	int m_Lod;
	void *m_pColorMeshes;
	bool m_bStaticLighting;
	vec3_t m_vecAmbientCube[6]; // ambient, and lights that aren't in locallight[]
	int m_nLocalLightCount;
	LightDesc_t m_LocalLightDescs[4];
};

struct cplane_t {
	vec3_t m_normal;
	float m_dist;
	BYTE m_type;
	BYTE m_sign_bits;
	BYTE m_pad[ 2 ];
};

struct csurface_t {
	const char *m_name;
	short m_surface_props;
	unsigned short m_flags;
};

struct SurfacePhysicsParams_t {
	float friction;
	float elasticity;
	float density;
	float thickness;
	float dampening;
};

struct SurfaceAudioParams_t {
	float reflectivity; // like elasticity, but how much sound should be reflected by this surface
	float hardnessFactor; // like elasticity, but only affects impact sound choices
	float roughnessFactor; // like friction, but only affects scrape sound choices   
	float roughThreshold; // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float hardThreshold; // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float hardVelocityThreshold; // collision velocity > this causes "hard" impacts, < this causes "soft" impacts   
	float highPitchOcclusion;
	//a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float midPitchOcclusion;
	float lowPitchOcclusion;
};

struct SurfaceSoundNames_t {
	unsigned short walkstepleft;
	unsigned short walkstepright;
	unsigned short runstepleft;
	unsigned short runstepright;
	unsigned short impactsoft;
	unsigned short impacthard;
	unsigned short scrapesmooth;
	unsigned short scraperough;
	unsigned short bulletimpact;
	unsigned short rolling;
	unsigned short breaksound;
	unsigned short strainsound;
};

struct SurfaceSoundHandles_t {
	short walkstepleft;
	short walkstepright;
	short runstepleft;
	short runstepright;
	short impactsoft;
	short impacthard;
	short scrapesmooth;
	short scraperough;
	short bulletimpact;
	short rolling;
	short breaksound;
	short strainsound;
};

struct SurfaceGameProps_t {
	float maxspeedfactor;
	float jumpfactor;
	float penetrationmodifier;
	float damagemodifier;
	uint16_t material;
	uint8_t climbable;
};

struct SurfaceData_t {
	SurfacePhysicsParams_t physics;
	SurfaceAudioParams_t audio;
	SurfaceSoundNames_t sounds;
	SurfaceGameProps_t game;
};

enum TextureCharacters {
	CHAR_TEX_ANTLION = 'A',
	CHAR_TEX_BLOODYFLESH = 'B',
	CHAR_TEX_CONCRETE = 'C',
	CHAR_TEX_DIRT = 'D',
	CHAR_TEX_EGGSHELL = 'E',
	CHAR_TEX_FLESH = 'F',
	CHAR_TEX_GRATE = 'G',
	CHAR_TEX_ALIENFLESH = 'H',
	CHAR_TEX_CLIP = 'I',
	CHAR_TEX_PLASTIC = 'L',
	CHAR_TEX_METAL = 'M',
	CHAR_TEX_SAND = 'N',
	CHAR_TEX_FOLIAGE = 'O',
	CHAR_TEX_COMPUTER = 'P',
	CHAR_TEX_SLOSH = 'S',
	CHAR_TEX_TILE = 'T',
	CHAR_TEX_CARDBOARD = 'U',
	CHAR_TEX_VENT = 'V',
	CHAR_TEX_WOOD = 'W',
	CHAR_TEX_GLASS = 'Y',
	CHAR_TEX_WARPSHIELD = 'Z',
};

//-----------------------------------------------------------------------------
// DrawModel flags
//-----------------------------------------------------------------------------
enum {
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,

	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,

	STUDIORENDER_DRAW_ACCURATETIME = 0x10,
	// Use accurate timing when drawing the model.
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,

	STUDIORENDER_DRAW_WIREFRAME = 0x80,

	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,

	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,

	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,

	STUDIORENDER_GENERATE_STATS = 0x8000,
};

enum Flag {
	FL_ONGROUND = ( 1 << 0 ),
	FL_DUCKING = ( 1 << 1 ),
	FL_WATERJUMP = ( 1 << 2 ),
	FL_ONTRAIN = ( 1 << 3 ),
	FL_INRAIN = ( 1 << 4 ),
	FL_FROZEN = ( 1 << 5 ),
	FL_ATCONTROLS = ( 1 << 6 ),
	FL_CLIENT = ( 1 << 7 ),
	FL_FAKECLIENT = ( 1 << 8 ),
};

enum player_hitbox : int {
	head = 0,
	neck,
	pelvis,
	stomach,
	thorax,
	l_chest,
	u_chest,
	r_thigh,
	l_thigh,
	r_calf,
	l_calf,
	r_foot,
	l_foot,
	r_hand,
	l_hand,
	r_upperarm,
	r_forearm,
	l_upperarm,
	l_forearm,
	max_hitbox,
};

enum CSWeaponType : int {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum Weapon {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG558,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034
};

enum ShaderStencilFunc_t {
	SHADER_STENCILFUNC_NEVER = 0,
	SHADER_STENCILFUNC_LESS,
	SHADER_STENCILFUNC_EQUAL,
	SHADER_STENCILFUNC_LEQUAL,
	SHADER_STENCILFUNC_GREATER,
	SHADER_STENCILFUNC_NOTEQUAL,
	SHADER_STENCILFUNC_GEQUAL,
	SHADER_STENCILFUNC_ALWAYS
};

enum ShaderStencilOp_t {
	SHADER_STENCILOP_KEEP = 0,
	SHADER_STENCILOP_ZERO,
	SHADER_STENCILOP_SET_TO_REFERENCE,
	SHADER_STENCILOP_INCREMENT_CLAMP,
	SHADER_STENCILOP_DECREMENT_CLAMP,
	SHADER_STENCILOP_INVERT,
	SHADER_STENCILOP_INCREMENT_WRAP,
	SHADER_STENCILOP_DECREMENT_WRAP,
};

enum DrawModelFlags_t {
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	// Not a studio flag, but used to flag model as a non-sorting brush model
	STUDIO_TRANSPARENCY = 0x80000000,
	// Not a studio flag, but used to flag model as using shadow depth material override
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SKIP_DECALS = 0x10000000,
};

enum MoveTypes : uint8_t {
	MOVETYPE_NONE = 0,
	// never moves
	MOVETYPE_ISOMETRIC,
	// For players -- in TF2 commander view, etc.
	MOVETYPE_WALK,
	// player only - moving on the ground
	MOVETYPE_STEP,
	// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY,
	// No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY,
	// flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS,
	// uses VPHYSICS for simulation
	MOVETYPE_PUSH,
	// no clip to world, push and crush
	MOVETYPE_NOCLIP,
	// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER,
	// Used by players only when going onto a ladder
	MOVETYPE_OBSERVER,
	// Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM,
	// Allows the entity to describe its own physics
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	// should always be defined as the last item in the list
	MOVETYPE_MAX_BITS = 4
};

enum Hitgroups {
	HITGROUP_INVALID = -1,
	HITGROUP_GENERIC,
	HITGROUP_HEAD,
	HITGROUP_CHEST,
	HITGROUP_STOMACH,
	HITGROUP_LEFTARM,
	HITGROUP_RIGHTARM,
	HITGROUP_LEFTLEG,
	HITGROUP_RIGHTLEG,
	HITGROUP_GEAR = 10
};

enum ClassID {
	CAI_BaseNPC = 0,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBRC4Target,
	CBreachCharge,
	CBreachChargeProjectile,
	CBreakableProp,
	CBreakableSurface,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDangerZone,
	CDangerZoneController,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDrone,
	CDronegun,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvGasCanister,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFists,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CGrassBurn,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoMapRegion,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemCash,
	CItemDogtags,
	CKnife,
	CKnifeGG,
	CLightGlow,
	CMaterialModifyControl,
	CMelee,
	CMolotovGrenade,
	CMolotovProjectile,
	CMovieDisplay,
	CParadropChopper,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysicsPropMultiplayer,
	CPhysMagnet,
	CPhysPropAmmoBox,
	CPhysPropLootCrate,
	CPhysPropRadarJammer,
	CPhysPropWeaponUpgrade,
	CPlantedC4,
	CPlasma,
	CPlayerResource,
	CPointCamera,
	CPointCommentaryNode,
	CPointWorldText,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CProp_Hallucination,
	CPropCounter,
	CPropDoorRotating,
	CPropJeep,
	CPropVehicleDriveable,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRopeKeyframe,
	CSCAR17,
	CSceneEntity,
	CSensorGrenade,
	CSensorGrenadeProjectile,
	CShadowControl,
	CSlideshowDisplay,
	CSmokeGrenade,
	CSmokeGrenadeProjectile,
	CSmokeStack,
	CSpatialEntity,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CStatueProp,
	CSteamJet,
	CSun,
	CSunlightShadowControl,
	CSurvivalSpawnChopper,
	CTablet,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFireBullets,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlantBomb,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTERadioIcon,
	CTEShatterSurface,
	CTEShowLine,
	CTesla,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CTriggerPlayerMovement,
	CTriggerSoundOperator,
	CVGuiScreen,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAug,
	CWeaponAWP,
	CWeaponBaseItem,
	CWeaponBizon,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponCycler,
	CWeaponElite,
	CWeaponFamas,
	CWeaponFiveSeven,
	CWeaponG3SG1,
	CWeaponGalil,
	CWeaponGalilAR,
	CWeaponGlock,
	CWeaponHKP2000,
	CWeaponM249,
	CWeaponM3,
	CWeaponM4A1,
	CWeaponMAC10,
	CWeaponMag7,
	CWeaponMP5Navy,
	CWeaponMP7,
	CWeaponMP9,
	CWeaponNegev,
	CWeaponNOVA,
	CWeaponP228,
	CWeaponP250,
	CWeaponP90,
	CWeaponSawedoff,
	CWeaponSCAR20,
	CWeaponScout,
	CWeaponSG550,
	CWeaponSG552,
	CWeaponSG556,
	CWeaponSSG08,
	CWeaponTaser,
	CWeaponTec9,
	CWeaponTMP,
	CWeaponUMP45,
	CWeaponUSP,
	CWeaponXM1014,
	CWorld,
	CWorldVguiText,
	DustTrail,
	MovieExplosion,
	ParticleSmokeGrenade,
	RocketTrail,
	SmokeTrail,
	SporeExplosion,
	SporeTrail
};

enum MaterialFlags_t {
	MATERIAL_VAR_DEBUG = 1,
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = 2,
	MATERIAL_VAR_NO_DRAW = 4,
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = 8,
	MATERIAL_VAR_VERTEXCOLOR = 16,
	MATERIAL_VAR_VERTEXALPHA = 32,
	MATERIAL_VAR_SELFILLUM = 64,
	MATERIAL_VAR_ADDITIVE = 128,
	MATERIAL_VAR_ALPHATEST = 256,
	// MATERIAL_VAR_UNUSED = 512,
	MATERIAL_VAR_ZNEARER = 1024,
	MATERIAL_VAR_MODEL = 2048,
	MATERIAL_VAR_FLAT = 4096,
	MATERIAL_VAR_NOCULL = 8192,
	MATERIAL_VAR_NOFOG = 16384,
	MATERIAL_VAR_IGNOREZ = 32768,
	MATERIAL_VAR_DECAL = 65536,
	MATERIAL_VAR_ENVMAPSPHERE = 131072,
	// OBSOLETE
	// MATERIAL_VAR_UNUSED = 262144,
	MATERIAL_VAR_ENVMAPCAMERASPACE = 524288,
	// OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = 1048576,
	MATERIAL_VAR_TRANSLUCENT = 2097152,
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = 4194304,
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = 8388608,
	// OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = 16777216,
	MATERIAL_VAR_ENVMAPMODE = 33554432,
	// OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = 67108864,
	MATERIAL_VAR_HALFLAMBERT = 134217728,
	MATERIAL_VAR_WIREFRAME = 268435456,
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = 536870912,
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = 1073741824,
	// MATERIAL_VAR_VERTEXFOG = -2147483648,
};

enum ECstrike15UserMessages {
	CS_UM_VGUIMenu = 1,
	CS_UM_Geiger = 2,
	CS_UM_Train = 3,
	CS_UM_HudText = 4,
	CS_UM_SayText = 5,
	CS_UM_SayText2 = 6,
	CS_UM_TextMsg = 7,
	CS_UM_HudMsg = 8,
	CS_UM_ResetHud = 9,
	CS_UM_GameTitle = 10,
	CS_UM_Shake = 12,
	CS_UM_Fade = 13,
	// fade HUD in/out
	CS_UM_Rumble = 14,
	CS_UM_CloseCaption = 15,
	CS_UM_CloseCaptionDirect = 16,
	CS_UM_SendAudio = 17,
	CS_UM_RawAudio = 18,
	CS_UM_VoiceMask = 19,
	CS_UM_RequestState = 20,
	CS_UM_Damage = 21,
	CS_UM_RadioText = 22,
	CS_UM_HintText = 23,
	CS_UM_KeyHintText = 24,
	CS_UM_ProcessSpottedEntityUpdate = 25,
	CS_UM_ReloadEffect = 26,
	CS_UM_AdjustMoney = 27,
	CS_UM_UpdateTeamMoney = 28,
	CS_UM_StopSpectatorMode = 29,
	CS_UM_KillCam = 30,
	CS_UM_DesiredTimescale = 31,
	CS_UM_CurrentTimescale = 32,
	CS_UM_AchievementEvent = 33,
	CS_UM_MatchEndConditions = 34,
	CS_UM_DisconnectToLobby = 35,
	CS_UM_DisplayInventory = 37,
	CS_UM_WarmupHasEnded = 38,
	CS_UM_ClientInfo = 39,
	CS_UM_CallVoteFailed = 45,
	CS_UM_VoteStart = 46,
	CS_UM_VotePass = 47,
	CS_UM_VoteFailed = 48,
	CS_UM_VoteSetup = 49,
	CS_UM_SendLastKillerDamageToClient = 51,
	CS_UM_ItemPickup = 53,
	CS_UM_ShowMenu = 54,
	// show hud menu
	CS_UM_BarTime = 55,
	// For the C4 progress bar.
	CS_UM_AmmoDenied = 56,
	CS_UM_MarkAchievement = 57,
	CS_UM_ItemDrop = 59,
	CS_UM_GlowPropTurnOff = 60,
};

struct ModelRenderInfo_t {
	vec3_t m_origin;
	vec3_t m_angles;
	char pad_0x0001[0x4];
	void *m_renderable;
	const void *m_model;
	const matrix3x4_t *m_model_to_world;
	const matrix3x4_t *m_lighting_offset;
	const vec3_t *m_lighting_origin;
	int m_flags;
	int m_entity_index;
	int m_skin;
	int m_body;
	int m_hitboxset;
	unsigned short m_instance;
};

class CFlashLightEffect {
public:
	bool m_bIsOn; //0x0000 
	char pad_0x0001[0x3]; //0x0001
	int m_nEntIndex; //0x0004 
	WORD m_FlashLightHandle; //0x0008 
	char pad_0x000A[0x2]; //0x000A
	float m_flMuzzleFlashBrightness; //0x000C 
	float m_flFov; //0x0010 
	float m_flFarZ; //0x0014 
	float m_flLinearAtten; //0x0018 
	bool m_bCastsShadows; //0x001C 
	char pad_0x001D[0x3]; //0x001D
	float m_flCurrentPullBackDist; //0x0020 
	DWORD m_MuzzleFlashTexture; //0x0024 
	DWORD m_FlashLightTexture; //0x0028 
	char m_szTextureName[64]; //0x1559888 
}; //Size=0x006C

class AnimationLayer_t {
public:
	PAD( 20 )
	uint32_t m_nOrder;
	uint32_t m_nSequence;
	float m_flPrevCycle;
	float m_flWeight;
	float m_flWeightDeltaRate;
	float m_flPlaybackRate;
	float m_flCycle;
	void *m_pOwner;
	char pad_0038[ 4 ];
};

struct MaterialVideoMode_t {
	int m_Width;
	int m_Height;
	int m_Format;
	int m_RefreshRate;
};

struct MaterialSystem_Config_t {
	MaterialVideoMode_t m_VideoMode;
	float m_fMonitorGamma;
	float m_fGammaTVRangeMin;
	float m_fGammaTVRangeMax;
	float m_fGammaTVExponent;
	bool m_bGammaTVEnabled;
	bool m_bTripleBuffered;
	int m_nAASamples;
	int m_nForceAnisotropicLevel;
	int m_nSkipMipLevels;
	int m_nDxSupportLevel;
	int m_nFlags;
	bool m_bEditMode;
	char m_nProxiesTestMode;
	bool m_bCompressedTextures;
	bool m_bFilterLightmaps;
	bool m_bFilterTextures;
	bool m_bReverseDepth;
	bool m_bBufferPrimitives;
	bool m_bDrawFlat;
	bool m_bMeasureFillRate;
	bool m_bVisualizeFillRate;
	bool m_bNoTransparency;
	bool m_bSoftwareLighting;
	bool m_bAllowCheats;
	char m_nShowMipLevels;
	bool m_bShowLowResImage;
	bool m_bShowNormalMap;
	bool m_bMipMapTextures;
	char m_nFullbright;
	bool m_bFastNoBump;
	bool m_bSuppressRendering;
	bool m_bDrawGray;
	bool m_bShowSpecular;
	bool m_bShowDiffuse;
	int m_nWindowedSizeLimitWidth;
	int m_nWindowedSizeLimitHeight;
	int m_nAAQuality;
	bool m_bShadowDepthTexture;
	bool m_bMotionBlur;
	bool m_bSupportFlashlight;
	bool m_bPaintEnabled;
	PAD( 12 )
};

class INetChannel;
class CClockDriftMgr
{
public:
	float m_ClockOffsets[ 17 ];   //0x0000
	uint32_t m_iCurClockOffset; //0x0044
	uint32_t m_nServerTick;     //0x0048
	uint32_t m_nClientTick;     //0x004C
}; //Size: 0x0050

class C_CSGameRulesProxy
{
public:
	bool freeze_period( ) {
		return *reinterpret_cast< bool *>( uintptr_t( this ) + 0x20 );
	}
};

class ClientState {
public:
public:
	void ForceFullUpdate() {
		*reinterpret_cast<int*>(uintptr_t(this) + 0x174) = -1;
	};

	char pad_0000[156];             //0x0000
	INetChannel* m_NetChannel;          //0x009C
	uint32_t m_nChallengeNr;        //0x00A0
	char pad_00A4[100];             //0x00A4
	uint32_t m_nSignonState;        //0x0108
	char pad_010C[8];               //0x010C
	float m_flNextCmdTime;          //0x0114
	uint32_t m_nServerCount;        //0x0118
	uint32_t m_nCurrentSequence;    //0x011C
	char pad_0120[8];               //0x0120
	CClockDriftMgr m_ClockDriftMgr; //0x0128
	uint32_t m_nDeltaTick;          //0x0178
	bool m_bPaused;                 //0x017C
	char pad_017D[3];               //0x017D
	uint32_t m_nViewEntity;         //0x0180
	uint32_t m_nPlayerSlot;         //0x0184
	char m_szLevelName[260];        //0x0188
	char m_szLevelNameShort[80];    //0x028C
	char m_szGroupName[80];         //0x02DC
	char pad_032С[92];				//0x032С
	uint32_t m_nMaxClients;         //0x0388
	char pad_0314[18820];           //0x0314
	float m_flLastServerTickTime;   //0x4C98
	bool insimulation;              //0x4C9C
	char pad_4C9D[3];               //0x4C9D
	uint32_t oldtickcount;          //0x4CA0
	float m_tickRemainder;          //0x4CA4
	float m_frameTime;              //0x4CA8
	uint32_t m_nLastOutgoingCommand;//0x4CAC
	uint32_t m_nChokedCommands;     //0x4CB0
	uint32_t m_nLastCommandAck;     //0x4CB4
	uint32_t m_nCommandAck;			//0x4CB8
	uint32_t m_nSoundSequence;      //0x4CBC
	char pad_4CC0[80];              //0x4CC0
	vec3_t viewangles;              //0x4D10
};

class IMemAlloc
{
public:
	// Release versions
	virtual void *Alloc(size_t nSize) = 0;
	virtual void *Realloc(void *pMem, size_t nSize) = 0;
	virtual void  Free(void *pMem) = 0;
	virtual void *Expand_NoLongerSupported(void *pMem, size_t nSize) = 0;

	// Debug versions
	virtual void *Alloc(size_t nSize, const char *pFileName, int nLine) = 0;
	virtual void *Realloc(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;
	virtual void  Free(void *pMem, const char *pFileName, int nLine) = 0;
	virtual void *Expand_NoLongerSupported(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;

	// Returns size of a particular allocation
	virtual size_t GetSize(void *pMem) = 0;

	// Force file + line information for an allocation
	virtual void PushAllocDbgInfo(const char *pFileName, int nLine) = 0;
	virtual void PopAllocDbgInfo() = 0;

	virtual long CrtSetBreakAlloc(long lNewBreakAlloc) = 0;
	virtual	int CrtSetReportMode(int nReportType, int nReportMode) = 0;
	virtual int CrtIsValidHeapPointer(const void *pMem) = 0;
	virtual int CrtIsValidPointer(const void *pMem, unsigned int size, int access) = 0;
	virtual int CrtCheckMemory(void) = 0;
	virtual int CrtSetDbgFlag(int nNewFlag) = 0;
	virtual void CrtMemCheckpoint(_CrtMemState *pState) = 0;
	virtual int heapchk() = 0;

	virtual void DumpStats() = 0;
	virtual void DumpStatsFileBase(char const *pchFileBase) = 0;
};

typedef unsigned short MDLHandle_t;

enum
{
	MDLHANDLE_INVALID = (MDLHandle_t)~0
};

enum MDLCacheDataType_t
{
	// Callbacks to get called when data is loaded or unloaded for these:
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,

	// Callbacks NOT called when data is loaded or unloaded for these:
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK,
};

class IMDLCacheNotify
{
public:
	// Called right after the data is loaded
	virtual void OnDataLoaded(MDLCacheDataType_t type, MDLHandle_t handle) = 0;

	// Called right before the data is unloaded
	virtual void OnDataUnloaded(MDLCacheDataType_t type, MDLHandle_t handle) = 0;
};

enum MDLCacheFlush_t
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,

	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;
struct studiohwdata_t;

class IMDLCache
{
public:
	// Used to install callbacks for when data is loaded + unloaded
	// Returns the prior notify
	virtual void SetCacheNotify(IMDLCacheNotify *pNotify) = 0;

	// NOTE: This assumes the "GAME" path if you don't use
	// the UNC method of specifying files. This will also increment
	// the reference count of the MDL
	virtual MDLHandle_t FindMDL(const char *pMDLRelativePath) = 0;

	// Reference counting
	virtual int AddRef(MDLHandle_t handle) = 0;
	virtual int Release(MDLHandle_t handle) = 0;
	virtual int GetRef(MDLHandle_t handle) = 0;

	// Gets at the various data associated with a MDL
	virtual studiohdr_t *GetStudioHdr(MDLHandle_t handle) = 0;
	virtual studiohwdata_t *GetHardwareData(MDLHandle_t handle) = 0;
	virtual vcollide_t *GetVCollide(MDLHandle_t handle) = 0;
	virtual unsigned char *GetAnimBlock(MDLHandle_t handle, int nBlock) = 0;
	virtual virtualmodel_t *GetVirtualModel(MDLHandle_t handle) = 0;
	virtual int GetAutoplayList(MDLHandle_t handle, unsigned short **pOut) = 0;
	virtual vertexFileHeader_t *GetVertexData(MDLHandle_t handle) = 0;

	// Brings all data associated with an MDL into memory
	virtual void TouchAllData(MDLHandle_t handle) = 0;

	// Gets/sets user data associated with the MDL
	virtual void SetUserData(MDLHandle_t handle, void* pData) = 0;
	virtual void *GetUserData(MDLHandle_t handle) = 0;

	// Is this MDL using the error model?
	virtual bool IsErrorModel(MDLHandle_t handle) = 0;

	// Flushes the cache, force a full discard
	virtual void Flush(MDLCacheFlush_t nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Flushes a particular model out of memory
	virtual void Flush(MDLHandle_t handle, int nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Returns the name of the model (its relative path)
	virtual const char *GetModelName(MDLHandle_t handle) = 0;

	// faster access when you already have the studiohdr
	virtual virtualmodel_t *GetVirtualModelFast(const studiohdr_t *pStudioHdr, MDLHandle_t handle) = 0;

	// all cache entries that subsequently allocated or successfully checked
	// are considered "locked" and will not be freed when additional memory is needed
	virtual void BeginLock() = 0;

	// reset all protected blocks to normal
	virtual void EndLock() = 0;

	// returns a pointer to a counter that is incremented every time the cache has been out of the locked state (EVIL)
	virtual int *GetFrameUnlockCounterPtrOLD() = 0;

	// Finish all pending async operations
	virtual void FinishPendingLoads() = 0;

	virtual vcollide_t *GetVCollideEx(MDLHandle_t handle, bool synchronousLoad = true) = 0;
	virtual bool GetVCollideSize(MDLHandle_t handle, int *pVCollideSize) = 0;

	virtual bool GetAsyncLoad(MDLCacheDataType_t type) = 0;
	virtual bool SetAsyncLoad(MDLCacheDataType_t type, bool bAsync) = 0;

	virtual void BeginMapLoad() = 0;
	virtual void EndMapLoad() = 0;
	virtual void MarkAsLoaded(MDLHandle_t handle) = 0;

	virtual void InitPreloadData(bool rebuild) = 0;
	virtual void ShutdownPreloadData() = 0;

	virtual bool IsDataLoaded(MDLHandle_t handle, MDLCacheDataType_t type) = 0;

	virtual int *GetFrameUnlockCounterPtr(MDLCacheDataType_t type) = 0;

	virtual studiohdr_t *LockStudioHdr(MDLHandle_t handle) = 0;
	virtual void UnlockStudioHdr(MDLHandle_t handle) = 0;

	virtual bool PreloadModel(MDLHandle_t handle) = 0;

	// Hammer uses this. If a model has an error loading in GetStudioHdr, then it is flagged
	// as an error model and any further attempts to load it will just get the error model.
	// That is, until you call this function. Then it will load the correct model.
	virtual void ResetErrorModelStatus(MDLHandle_t handle) = 0;

	virtual void MarkFrame() = 0;
};