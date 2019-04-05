#pragma once
#include <iostream>

enum egc_result_t
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,           // There is no message in the queue
	k_EGCResultBufferTooSmall = 2,      // The buffer is too small for the requested message
	k_EGCResultNotLoggedOn = 3,         // The client is not logged onto Steam
	k_EGCResultInvalidMessage = 4,      // Something was wrong with the message being sent with SendMessage
};

class i_steam_game_coordinator
{
public:
	virtual egc_result_t gc_send_message(int unMsgType, const void *pubData, int cubData) = 0;
	virtual bool is_message_available(int *pcubMsgSize) = 0;
	virtual egc_result_t gc_retrieve_message(int *punMsgType, void *pubDest, int cubDest, int *pcubMsgSize) = 0;

};

class c_steam_id
{
public:
	c_steam_id()
	{
		m_steamid.m_comp.m_account_id = 0;
		m_steamid.m_comp.m_account_type = 0;
		m_steamid.m_comp.m_universe = 0;
		m_steamid.m_comp.m_account_instance = 0;
	}
	uint32_t get_account_id() const { return m_steamid.m_comp.m_account_id; }

private:
	union steam_id_t
	{
		struct steam_id_component_t
		{
			uint32_t			m_account_id : 32;			// unique account identifier
			unsigned int		m_account_instance : 20;	// dynamic instance ID (used for multiseat type accounts only)
			unsigned int		m_account_type : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			int					m_universe : 8;	// universe this account belongs to
		} m_comp;

		uint64_t all64_bits;
	} m_steamid;
};

class i_steam_user
{
public:
	virtual uint32_t get_steam_user() = 0;
	virtual bool logged_on() = 0;
	virtual c_steam_id get_steam_id() = 0;
};

class i_steam_client
{
public:
	i_steam_user *get_steam_user(void* hSteamUser, void* hSteamPipe, const char *pchVersion)
	{
		typedef i_steam_user*(__stdcall* func)(void*, void*, const char *);
		return util::misc::vfunc<func>(this, 5)(hSteamUser, hSteamPipe, pchVersion);
	}

	i_steam_game_coordinator* get_steam_generic_interface(void* hSteamUser, void* hSteamPipe, const char *pchVersion)
	{
		typedef i_steam_game_coordinator*(__stdcall* func)(void*, void*, const char *);
		return util::misc::vfunc<func>(this, 12)(hSteamUser, hSteamPipe, pchVersion);
	}
};