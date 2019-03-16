#pragma once

class CInput {
public:
	void *pvftable; //0x00
	PAD(0x8)
	bool m_fTrackIRAvailable; //0x04
	bool m_fMouseInitialized; //0x05
	bool m_fMouseActive; //0x06
	bool m_fJoystickAdvancedInit; //0x07
	PAD(0x2C) //0x08
	void *m_pKeys; //0x34
	PAD(0x64) //0x38
	int pad_0x41;
	int pad_0x42;
	bool m_fCameraInterceptingMouse; //0x9C
	bool m_fCameraInThirdPerson; //0x9D
	bool m_fCameraMovingWithMouse; //0x9E
	vec3_t m_vecCameraOffset; //0xA0
	bool m_fCameraDistanceMove; //0xAC
	int m_nCameraOldX; //0xB0
	int m_nCameraOldY; //0xB4
	int m_nCameraX; //0xB8
	int m_nCameraY; //0xBC
	bool m_CameraIsOrthographic; //0xC0
	vec3_t m_angPreviousViewAngles; //0xC4
	vec3_t m_angPreviousViewAnglesTilt; //0xD0
	float m_flLastForwardMove; //0xDC
	int m_nClearInputState; //0xE0
};

enum ButtonFlags_t {
	IN_ATTACK = ( 1 << 0 ),
	IN_JUMP = ( 1 << 1 ),
	IN_DUCK = ( 1 << 2 ),
	IN_FORWARD = ( 1 << 3 ),
	IN_BACK = ( 1 << 4 ),
	IN_USE = ( 1 << 5 ),
	IN_CANCEL = ( 1 << 6 ),
	IN_LEFT = ( 1 << 7 ),
	IN_RIGHT = ( 1 << 8 ),
	IN_MOVELEFT = ( 1 << 9 ),
	IN_MOVERIGHT = ( 1 << 10 ),
	IN_ATTACK2 = ( 1 << 11 ),
	IN_RUN = ( 1 << 12 ),
	IN_RELOAD = ( 1 << 13 ),
	IN_ALT1 = ( 1 << 14 ),
	IN_ALT2 = ( 1 << 15 ),
	IN_SCORE = ( 1 << 16 ),
	IN_SPEED = ( 1 << 17 ),
	IN_WALK = ( 1 << 18 ),
	IN_ZOOM = ( 1 << 19 ),
	IN_WEAPON1 = ( 1 << 20 ),
	IN_WEAPON2 = ( 1 << 21 ),
	IN_BULLRUSH = ( 1 << 22 ),
	IN_GRENADE1 = ( 1 << 23 ),
	IN_GRENADE2 = ( 1 << 24 ),
	IN_ATTACK3 = ( 1 << 25 ),
};

class CUserCmd {
public:
	virtual ~CUserCmd() = default;

	int m_command_number;
	int m_tick_count;
	vec3_t m_viewangles;
	vec3_t m_aimdirection;
	float m_forwardmove;
	float m_sidemove;
	float m_upmove;
	int m_buttons;
	uint8_t m_impulse;
	int m_weaponselect;
	int m_weaponsubtype;
	int m_random_seed;
	short m_mousedx;
	short m_mousedy;
	bool m_hasbeenpredicted;
	// vec3_t  m_headangles;
	// vec3_t  m_headoffset;
};
