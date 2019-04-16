#pragma once
#include "../../inc.h"

class c_fakelag {
private:
	enum type : int {
		MAXIMUM = 0,
		ADAPTIVE,
	};
public:
	int m_choked; // replace this later, m_nChokedCommands in clientstate is always 0 so this will work for now
				  // cba to fix the clientstate mango
	void think( c_user_cmd* cmd );
};

extern c_fakelag g_fakelag;