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


class chai_wrapper {
public:
	chaiscript::ChaiScript* chai;
	std::string wrapper_name = "unnamed session";
	std::function<void(chai_console_text)> print_func;
	std::vector<chai_console_text> *text_list;
	bool chai_finished_init;

	bool chai_defined_createmove;
	bool chai_defined_endscene;

	void chai_additional_parse(const std::string & str);
	void chai_load(const std::string &str);
	void chai_loadscript(const std::string &str);
	void chai_hook_createmove(c_user_cmd* cmd);
	void chai_hook_endscene(c_renderer* r);
	void chai_init();
};

namespace chai_manager {
	extern std::vector<chai_wrapper> sessions;
	extern std::function<void(chai_console_text)> print_func;
	void chai_error(const std::string &str);
	void chai_print(const std::string &str);
	extern std::vector<std::string> chai_aa_pitch;
	extern std::vector<std::string> chai_aa_yaw;
};
