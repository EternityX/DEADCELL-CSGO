#pragma once
#include "../../inc.hpp"

class c_fakelag {
private:
	enum type : int {
		MAXIMUM = 0,
		ADAPTIVE,
	};
public:
	int m_choked; // replace this later, m_nChokedCommands in clientstate is always 0 so this will work for now
				  // cba to fix the clientstate mango
	void think( CUserCmd* cmd );
};

extern c_fakelag g_fakelag;