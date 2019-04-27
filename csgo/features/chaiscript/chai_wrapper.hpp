#pragma once
#include "../../inc.h"
#include "../vars.h"
#include "chai_console.hpp"
#include "include/chaiscript.hpp"
#include "wrappers/chai_wrapper_entlist.hpp"
#include "wrappers/chai_wrapper_player.hpp"
#include "wrappers/chai_wrapper_misc.hpp"
#include "wrappers/chai_wrapper_math.hpp"
#include "wrappers/chai_wrapper_renderer.hpp"
#include <fstream>


namespace chai_wrapper {
	extern chaiscript::ChaiScript* chai;
	extern std::function<void(chai_console_text)> print_func;
	extern bool chai_finished_init;

	extern bool chai_defined_createmove;
	extern bool chai_defined_endscene;

	extern void chai_print( const std::string &str );
	extern void chai_load( const std::string &str );
	extern void chai_loadscript( const std::string &str );
	extern void chai_hook_createmove( c_user_cmd* cmd );
	extern void chai_hook_endscene( c_renderer* r );
	extern void chai_init( std::function<void(chai_console_text)> print );
};
