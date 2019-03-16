#pragma once

enum ClientFrameStage_t {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class CHLClient {
public:
	ClientClass *GetAllClasses() {
		return util::misc::vfunc< ClientClass *(__thiscall *)( decltype( this ) ) >( this, 8 )( this );
	}
};

typedef struct player_info_s {
	int64_t __pad0;
	int xuid_low;
	int xuid_high;
	char m_szPlayerName[ 256 ];
	int m_nUserID;
	char m_szSteamID[ 33 ];
	unsigned int m_nSteam3ID;
	char m_szFriendsName[ 256 ];
	bool m_bIsFakePlayer;
	bool m_bIsHLTV;
	unsigned int m_dwCustomFiles[ 4 ];
	unsigned char m_FilesDownloaded;
} player_info_t;

typedef struct InputContextHandle_t__ *InputContextHandle_t;
struct client_textmessage_t;
class model_t;
class SurfInfo;
class IMaterial;
class CSentence;
class CAudioSource;
class AudioState_t;
class ISpatialQuery;
class IMaterialSystem;
class CPhysCollide;
class IAchievementMgr;
class INetChannelInfo;
class ISPSharedMemory;
class CGamestatsData;
class KeyValues;
class CSteamAPIContext;
struct Frustum_t;
class pfnDemoCustomDataCallback;

class IEngineClient {
public:
	virtual int GetIntersectingSurfaces( const model_t *model, const vec3_t &vCenter, const float radius,
	                                     const bool bOnlyVisibleSurfaces, SurfInfo *pInfos, const int nMaxInfos ) = 0;
	virtual vec3_t GetLightForPoint( const vec3_t &pos, bool bClamp ) = 0;
	virtual IMaterial *TraceLineMaterialAndLighting( const vec3_t &start, const vec3_t &end, vec3_t &diffuseLightColor,
	                                                 vec3_t &baseColor ) = 0;
	virtual const char *ParseFile( const char *data, char *token, int maxlen ) = 0;
	virtual bool CopyFile( const char *source, const char *destination ) = 0;
	virtual void GetScreenSize( int &width, int &height ) = 0;
	virtual void ServerCmd( const char *szCmdString, bool bReliable = true ) = 0;
	virtual void ClientCmd( const char *szCmdString ) = 0;
	virtual bool GetPlayerInfo( int ent_num, player_info_t *pinfo ) = 0;
	virtual int GetPlayerForUserID( int userID ) = 0;
	virtual client_textmessage_t *TextMessageGet( const char *pName ) = 0; // 10
	virtual bool Con_IsVisible( void ) = 0;
	virtual int GetLocalPlayer( void ) = 0;
	virtual const model_t *LoadModel( const char *pName, bool bProp = false ) = 0;
	virtual float GetLastTimeStamp( void ) = 0;
	virtual CSentence *GetSentence( CAudioSource *pAudioSource ) = 0; // 15
	virtual float GetSentenceLength( CAudioSource *pAudioSource ) = 0;
	virtual bool IsStreaming( CAudioSource *pAudioSource ) const = 0;
	virtual void GetViewAngles( vec3_t &va ) = 0;
	virtual void SetViewAngles( vec3_t &va ) = 0;
	virtual int GetMaxClients( void ) = 0; // 20
	virtual const char *Key_LookupBinding( const char *pBinding ) = 0;
	virtual const char *Key_BindingForKey( int &code ) = 0;
	virtual void Key_SetBinding( int, char const * ) = 0;
	virtual void StartKeyTrapMode( void ) = 0;
	virtual bool CheckDoneKeyTrapping( int &code ) = 0;
	virtual bool IsInGame( void ) = 0;
	virtual bool IsConnected( void ) = 0;
	virtual bool IsDrawingLoadingImage( void ) = 0;
	virtual void HideLoadingPlaque( void ) = 0;
	virtual void Con_NPrintf( int pos, const char *fmt, ... ) = 0; // 30
	virtual void Con_NXPrintf( const struct con_nprint_s *info, const char *fmt, ... ) = 0;
	virtual int IsBoxVisible( const vec3_t &mins, const vec3_t &maxs ) = 0;
	virtual int IsBoxInViewCluster( const vec3_t &mins, const vec3_t &maxs ) = 0;
	virtual bool CullBox( const vec3_t &mins, const vec3_t &maxs ) = 0;
	virtual void Sound_ExtraUpdate( void ) = 0;
	virtual const char *GetGameDirectory( void ) = 0;
	virtual const matrix3x4_t &WorldToScreenMatrix() = 0;
	virtual const matrix3x4_t &WorldToViewMatrix() = 0;
	virtual int GameLumpVersion( int lumpId ) const = 0;
	virtual int GameLumpSize( int lumpId ) const = 0; // 40
	virtual bool LoadGameLump( int lumpId, void *pBuffer, int size ) = 0;
	virtual int LevelLeafCount() const = 0;
	virtual ISpatialQuery *GetBSPTreeQuery() = 0;
	virtual void LinearToGamma( float *linear, float *gamma ) = 0;
	virtual float LightStyleValue( int style ) = 0; // 45
	virtual void ComputeDynamicLighting( const vec3_t &pt, const vec3_t *pNormal, vec3_t &color ) = 0;
	virtual void GetAmbientLightColor( vec3_t &color ) = 0;
	virtual int GetDXSupportLevel() = 0;
	virtual bool SupportsHDR() = 0;
	virtual void Mat_Stub( IMaterialSystem *pMatSys ) = 0; // 50
	virtual void GetChapterName( char *pchBuff, int iMaxLength ) = 0;
	virtual char const *GetLevelName( void ) = 0;
	virtual char const *GetLevelNameShort( void ) = 0;
	virtual char const *GetMapGroupName( void ) = 0;
	virtual struct IVoiceTweak_s *GetVoiceTweakAPI( void ) = 0;
	virtual void SetVoiceCasterID( unsigned int someint ) = 0; // 56
	virtual void EngineStats_BeginFrame( void ) = 0;
	virtual void EngineStats_EndFrame( void ) = 0;
	virtual void FireEvents() = 0;
	virtual int GetLeavesArea( unsigned short *pLeaves, int nLeaves ) = 0;
	virtual bool DoesBoxTouchAreaFrustum( const vec3_t &mins, const vec3_t &maxs, int iArea ) = 0; // 60
	virtual int GetFrustumList( Frustum_t **pList, int listMax ) = 0;
	virtual bool ShouldUseAreaFrustum( int i ) = 0;
	virtual void SetAudioState( const AudioState_t &state ) = 0;
	virtual int SentenceGroupPick( int groupIndex, char *name, int nameBufLen ) = 0;
	virtual int SentenceGroupPickSequential( int groupIndex, char *name, int nameBufLen, int sentenceIndex, int reset ) =
	0;
	virtual int SentenceIndexFromName( const char *pSentenceName ) = 0;
	virtual const char *SentenceNameFromIndex( int sentenceIndex ) = 0;
	virtual int SentenceGroupIndexFromName( const char *pGroupName ) = 0;
	virtual const char *SentenceGroupNameFromIndex( int groupIndex ) = 0;
	virtual float SentenceLength( int sentenceIndex ) = 0;
	virtual void ComputeLighting( const vec3_t &pt, const vec3_t *pNormal, bool bClamp, vec3_t &color,
	                              vec3_t *pBoxColors = nullptr ) = 0;
	virtual void ActivateOccluder( int nOccluderIndex, bool bActive ) = 0;
	virtual bool IsOccluded( const vec3_t &vecAbsMins, const vec3_t &vecAbsMaxs ) = 0; // 74
	virtual int GetOcclusionViewId( void ) = 0;
	virtual void *SaveAllocMemory( size_t num, size_t size ) = 0;
	virtual void SaveFreeMemory( void *pSaveMem ) = 0;
	virtual INetChannelInfo *GetNetChannelInfo( void ) = 0;
	virtual void DebugDrawPhysCollide( const CPhysCollide *pCollide, IMaterial *pMaterial, const matrix3x4_t &transform,
	                                   const Color &color ) = 0; //79
	virtual void CheckPoint( const char *pName ) = 0; // 80
	virtual void DrawPortals() = 0;
	virtual bool IsPlayingDemo( void ) = 0;
	virtual bool IsRecordingDemo( void ) = 0;
	virtual bool IsPlayingTimeDemo( void ) = 0;
	virtual int GetDemoRecordingTick( void ) = 0;
	virtual int GetDemoPlaybackTick( void ) = 0;
	virtual int GetDemoPlaybackStartTick( void ) = 0;
	virtual float GetDemoPlaybackTimeScale( void ) = 0;
	virtual int GetDemoPlaybackTotalTicks( void ) = 0;
	virtual bool IsPaused( void ) = 0; // 90
	virtual float GetTimescale( void ) const = 0;
	virtual bool IsTakingScreenshot( void ) = 0;
	virtual bool IsHLTV( void ) = 0;
	virtual bool IsLevelMainMenuBackground( void ) = 0;
	virtual void GetMainMenuBackgroundName( char *dest, int destlen ) = 0;
	virtual void SetOcclusionParameters( const int /*OcclusionParams_t*/ &params ) = 0; // 96
	virtual void GetUILanguage( char *dest, int destlen ) = 0;
	virtual int IsSkyboxVisibleFromPoint( const vec3_t &vecPoint ) = 0;
	virtual const char *GetMapEntitiesString() = 0;
	virtual bool IsInEditMode( void ) = 0; // 100
	virtual float GetScreenAspectRatio( int viewportWidth, int viewportHeight ) = 0;
	virtual bool REMOVED_SteamRefreshLogin( const char *password, bool isSecure ) = 0; // 100
	virtual bool REMOVED_SteamProcessCall( bool &finished ) = 0;
	virtual unsigned int GetEngineBuildNumber() = 0; // engines build
	virtual const char *GetProductVersionString() = 0; // mods version number (steam.inf)
	virtual void GrabPreColorCorrectedFrame( int x, int y, int width, int height ) = 0;
	virtual bool IsHammerRunning() const = 0;
	virtual void ExecuteClientCmd( const char *szCmdString ) = 0; //108
	virtual bool MapHasHDRLighting( void ) = 0;
	virtual bool MapHasLightMapAlphaData( void ) = 0;
	virtual int GetAppID() = 0;
	virtual vec3_t GetLightForPointFast( const vec3_t &pos, bool bClamp ) = 0;
	virtual void ClientCmd_Unrestricted1( char const *, int, bool );
	virtual void ClientCmd_Unrestricted( const char *szCmdString, const char *newFlag = nullptr ) = 0;
	// 114, new flag, quick testing shows setting 0 seems to work, haven't looked into it.
	virtual void SetRestrictServerCommands( bool bRestrict ) = 0;
	virtual void SetRestrictClientCommands( bool bRestrict ) = 0;
	virtual void SetOverlayBindProxy( int iOverlayID, void *pBindProxy ) = 0;
	virtual bool CopyFrameBufferToMaterial( const char *pMaterialName ) = 0;
	virtual void ReadConfiguration( const int iController, const bool readDefault ) = 0;
	virtual void SetAchievementMgr( IAchievementMgr *pAchievementMgr ) = 0;
	virtual IAchievementMgr *GetAchievementMgr() = 0;
	virtual bool MapLoadFailed( void ) = 0;
	virtual void SetMapLoadFailed( bool bState ) = 0;
	virtual bool IsLowViolence() = 0;
	virtual const char *GetMostRecentSaveGame( void ) = 0;
	virtual void SetMostRecentSaveGame( const char *lpszFilename ) = 0;
	virtual void StartXboxExitingProcess() = 0;
	virtual bool IsSaveInProgress() = 0;
	virtual bool IsAutoSaveDangerousInProgress( void ) = 0;
	virtual unsigned int OnStorageDeviceAttached( int iController ) = 0;
	virtual void OnStorageDeviceDetached( int iController ) = 0;
	virtual char * const GetSaveDirName( void ) = 0;
	virtual void WriteScreenshot( const char *pFilename ) = 0;
	virtual void ResetDemoInterpolation( void ) = 0;
	virtual int GetActiveSplitScreenPlayerSlot() = 0;
	virtual int SetActiveSplitScreenPlayerSlot( int slot ) = 0;
	virtual bool SetLocalPlayerIsResolvable( char const *pchContext, int nLine, bool bResolvable ) = 0;
	virtual bool IsLocalPlayerResolvable() = 0;
	virtual int GetSplitScreenPlayer( int nSlot ) = 0;
	virtual bool IsSplitScreenActive() = 0;
	virtual bool IsValidSplitScreenSlot( int nSlot ) = 0;
	virtual int FirstValidSplitScreenSlot() = 0; // -1 == invalid
	virtual int NextValidSplitScreenSlot( int nPreviousSlot ) = 0; // -1 == invalid
	virtual ISPSharedMemory *GetSinglePlayerSharedMemorySpace( const char *szName, int ent_num = ( 1 << 11 ) ) = 0;
	virtual void ComputeLightingCube( const vec3_t &pt, bool bClamp, vec3_t *pBoxColors ) = 0;
	virtual void RegisterDemoCustomDataCallback( const char *szCallbackSaveID, pfnDemoCustomDataCallback pCallback ) = 0;
	virtual void RecordDemoCustomData( pfnDemoCustomDataCallback pCallback, const void *pData, size_t iDataLength ) = 0;
	virtual void SetPitchScale( float flPitchScale ) = 0;
	virtual float GetPitchScale( void ) = 0;
	virtual bool LoadFilmmaker() = 0;
	virtual void UnloadFilmmaker() = 0;
	virtual void SetLeafFlag( int nLeafIndex, int nFlagBits ) = 0;
	virtual void RecalculateBSPLeafFlags( void ) = 0;
	virtual bool DSPGetCurrentDASRoomNew( void ) = 0;
	virtual bool DSPGetCurrentDASRoomChanged( void ) = 0;
	virtual bool DSPGetCurrentDASRoomSkyAbove( void ) = 0;
	virtual float DSPGetCurrentDASRoomSkyPercent( void ) = 0;
	virtual void SetMixGroupOfCurrentMixer( const char *szgroupname, const char *szparam, float val, int setMixerType ) =
	0;
	virtual int GetMixLayerIndex( const char *szmixlayername ) = 0;
	virtual void SetMixLayerLevel( int index, float level ) = 0;
	virtual int GetMixGroupIndex( char const *groupname ) = 0;
	virtual void SetMixLayerTriggerFactor( int i1, int i2, float fl ) = 0;
	virtual void SetMixLayerTriggerFactor( char const *char1, char const *char2, float fl ) = 0;
	virtual bool IsCreatingReslist() = 0;
	virtual bool IsCreatingXboxReslist() = 0;
	virtual void SetTimescale( float flTimescale ) = 0;
	virtual void SetGamestatsData( CGamestatsData *pGamestatsData ) = 0;
	virtual CGamestatsData *GetGamestatsData() = 0;
	virtual void GetMouseDelta( int &dx, int &dy, bool b ) = 0; // unknown
	virtual const char *Key_LookupBindingEx( const char *pBinding, int iUserId = -1, int iStartCount = 0,
	                                         int iAllowJoystick = -1 ) = 0;
	virtual int Key_CodeForBinding( char const *, int, int, int ) = 0;
	virtual void UpdateDAndELights( void ) = 0;
	virtual int GetBugSubmissionCount() const = 0;
	virtual void ClearBugSubmissionCount() = 0;
	virtual bool DoesLevelContainWater() const = 0;
	virtual float GetServerSimulationFrameTime() const = 0;
	virtual void SolidMoved( class IClientEntity *pSolidEnt, class ICollideable *pSolidCollide,
	                         const vec3_t *pPrevAbsOrigin, bool accurateBboxTriggerChecks ) = 0;
	virtual void TriggerMoved( class IClientEntity *pTriggerEnt, bool accurateBboxTriggerChecks ) = 0;
	virtual void ComputeLeavesConnected( const vec3_t &vecOrigin, int nCount, const int *pLeafIndices, bool *pIsConnected )
	= 0;
	virtual bool IsInCommentaryMode( void ) = 0;
	virtual void SetBlurFade( float amount ) = 0;
	virtual bool IsTransitioningToLoad() = 0;
	virtual void SearchPathsChangedAfterInstall() = 0;
	virtual void ConfigureSystemLevel( int nCPULevel, int nGPULevel ) = 0;
	virtual void SetConnectionPassword( char const *pchCurrentPW ) = 0;
	virtual CSteamAPIContext *GetSteamAPIContext() = 0;
	virtual void SubmitStatRecord( char const *szMapName, unsigned int uiBlobVersion, unsigned int uiBlobSize,
	                               const void *pvBlob ) = 0;
	virtual void ServerCmdKeyValues( KeyValues *pKeyValues ) = 0; // 203
	virtual void SpherePaintSurface( const model_t *model, const vec3_t &location, unsigned char chr, float fl1,
	                                 float fl2 ) = 0;
	virtual bool HasPaintmap( void ) = 0;
	virtual void EnablePaintmapRender() = 0;
	//virtual void                TracePaintSurface( const model_t *model, const vec3_t& position, float radius, CUtlvec3_t<Color>& surfColors ) = 0;
	virtual void SphereTracePaintSurface( const model_t *model, const vec3_t &position, const vec3_t &vec2, float radius,
	                                      /*CUtlvec3_t<unsigned char, CUtlMemory<unsigned char, int>>*/
	                                      int &utilVecShit ) = 0;
	virtual void RemoveAllPaint() = 0;
	virtual void PaintAllSurfaces( unsigned char uchr ) = 0;
	virtual void RemovePaint( const model_t *model ) = 0;
	virtual bool IsActiveApp() = 0;
	virtual bool IsClientLocalToActiveServer() = 0;
	virtual void TickProgressBar() = 0;
	virtual InputContextHandle_t GetInputContext( int /*EngineInputContextId_t*/ id ) = 0;
	virtual void GetStartupImage( char *filename, int size ) = 0;
	virtual bool IsUsingLocalNetworkBackdoor( void ) = 0;
	virtual void SaveGame( const char *, bool, char *, int, char *, int ) = 0;
	virtual void GetGenericMemoryStats( /* GenericMemoryStat_t */ void ** ) = 0;
	virtual bool GameHasShutdownAndFlushedMemory( void ) = 0;
	virtual int GetLastAcknowledgedCommand( void ) = 0;
	virtual void FinishContainerWrites( int i ) = 0;
	virtual void FinishAsyncSave( void ) = 0;
	virtual int GetServerTick( void ) = 0;
	virtual const char *GetModDirectory( void ) = 0;
	virtual bool AudioLanguageChanged( void ) = 0;
	virtual bool IsAutoSaveInProgress( void ) = 0;
	virtual void StartLoadingScreenForCommand( const char *command ) = 0;
	virtual void StartLoadingScreenForKeyValues( KeyValues *values ) = 0;
	virtual void SOSSetOpvarFloat( const char *, float ) = 0;
	virtual void SOSGetOpvarFloat( const char *, float & ) = 0;
	virtual bool IsSubscribedMap( const char *, bool ) = 0;
	virtual bool IsFeaturedMap( const char *, bool ) = 0;
	virtual void GetDemoPlaybackParameters( void ) = 0;
	virtual int GetClientVersion( void ) = 0;
	virtual bool IsDemoSkipping( void ) = 0;
	virtual void SetDemoImportantEventData( const KeyValues *values ) = 0;
	virtual void ClearEvents( void ) = 0;
	virtual int GetSafeZoneXMin( void ) = 0;
	virtual bool IsVoiceRecording( void ) = 0;
	virtual void ForceVoiceRecordOn( void ) = 0;
	virtual bool IsReplay( void ) = 0;

	D3DMATRIX &world_to_screen_matrix_d3d() {
		ulong_t function_ptr = ( *reinterpret_cast< ulong_t ** >(this) )[ 37 ];
		ulong_t render_ptr = *reinterpret_cast< ulong_t * >(function_ptr + 1);
		ulong_t view_matrix = *reinterpret_cast< ulong_t * >(render_ptr + 0xDC) + 0x3DC;

		return reinterpret_cast< D3DMATRIX & >(*reinterpret_cast< ulong_t * >(view_matrix));
	}
};

struct CViewSetup {
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
