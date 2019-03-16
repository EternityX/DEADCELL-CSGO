#pragma once

class CMoveData {
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	int m_nPlayerHandle; // edict index on server, client entity handle on client=
	int m_nImpulseCommand; // Impulse command issued.
	vec3_t m_vecViewAngles; // Command view angles (local space)
	vec3_t m_vecAbsViewAngles; // Command view angles (world space)
	int m_nButtons; // Attack buttons.
	int m_nOldButtons; // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	vec3_t m_vecVelocity; // edict::velocity        // Current movement direction.
	vec3_t m_vecAngles; // edict::angles
	vec3_t m_vecOldAngles;
	float m_outStepHeight; // how much you climbed this move
	vec3_t m_outWishVel; // This is where you tried 
	vec3_t m_outJumpVel; // This is your jump velocity
	vec3_t m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	float m_flUnknown[5];
	vec3_t m_vecAbsOrigin;
};

class IGameMovement {
public:
public:
	virtual			~IGameMovement( void )
	{ }

	virtual void	ProcessMovement( IClientEntity *pPlayer, void *pMove ) = 0;
	virtual void	Reset( void ) = 0;
	virtual void	StartTrackPredictionErrors( IClientEntity *pPlayer ) = 0;
	virtual void	FinishTrackPredictionErrors( IClientEntity *pPlayer ) = 0;
	virtual void	DiffPrint( char const *fmt, ... ) = 0;

	virtual vec3_t const&	GetPlayerMins( bool ducked ) const = 0;
	virtual vec3_t const&	GetPlayerMaxs( bool ducked ) const = 0;
	virtual vec3_t const&   GetPlayerViewOffset( bool ducked ) const = 0;

	virtual bool			IsMovingPlayerStuck( void ) const = 0;
	virtual IClientEntity*	GetMovingPlayer( void ) const = 0;
	virtual void			UnblockPusher( IClientEntity* pPlayer, IClientEntity *pPusher ) = 0;

	virtual void SetupMovementBounds( void *pMove ) = 0;
};
