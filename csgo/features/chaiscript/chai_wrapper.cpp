#include "chai_wrapper.hpp"
chaiscript::ChaiScript* chai;
bool chai_finished_init = false;
bool chai_defined_createmove = true;
bool chai_defined_endscene = true;
std::vector<chai_console_text> text_output;
std::vector<chai_console_text> *text_list = new std::vector<chai_console_text>();

void chai_wrapper::chai_additional_parse(const std::string &str) {
	if (str.find("create_move(cmd)") != std::string::npos) { chai_defined_createmove = true;  }
	if (str.find("end_scene(r)") != std::string::npos) { chai_defined_endscene = true;  }
	// mayb find a better method of this?
}
	
void chai_wrapper::chai_load(const std::string &str) {
	std::fstream file(str.c_str(), std::fstream::in);
	std::string content = "";
	std::ifstream input_file = std::ifstream(str);
	if (input_file.good()) {
		try {
			std::getline(input_file, content, '\0');
			chai_loadscript(content);
			chai_manager::chai_print(content);
			input_file.close();
		}
		catch (const std::exception &ex) {
			input_file.close();
			chai_manager::chai_error("Failed to open file");
		}
	}
	else {
		chai_manager::chai_error("Failed to open file");
	}
}
void chai_wrapper::chai_loadscript(const std::string &str) {
	try {
		chai->eval(str);
	}
	catch (const double) {}
	catch (int) {}
	catch (float) {}
	catch (const std::string &) {}
	catch (const std::exception &e) {
		chai_manager::chai_error(e.what());
	}
	chai_additional_parse(str);
}

void chai_wrapper::chai_hook_createmove(c_user_cmd* cmd) {
	try {
		auto func = chai->eval<std::function<void(c_user_cmd *)>>("create_move");
		func(cmd);
	}
	catch (std::exception &e) {
		chai_manager::chai_error(e.what());
		chai_defined_createmove = false;
	}
}

void chai_wrapper::chai_hook_endscene(c_renderer* r) {
	try {
		chai_wrapper_renderer renderer = chai_wrapper_renderer(r);
		auto func = chai->eval<std::function<void(chai_wrapper_renderer *)>>("end_scene");
		func(&renderer);
	}
	catch (std::exception &e) {
		chai_manager::chai_error(e.what());
		chai_defined_endscene = false;
	}
}
	

void chai_wrapper::chai_init() {
	using namespace chaiscript;

	chai = new chaiscript::ChaiScript();
	chai_defined_createmove = false;

	//print_func = print;

	// basic functions
	chai->add(user_type<chai_wrapper>(), "main");
	chai->add(fun(&chai_manager::chai_print), "print");
	chai->add(fun(&chai_wrapper::chai_load), "load");

	// c_user_cmd wrapper
	chai->add(user_type<c_user_cmd>(), "usercmd");
	chai->add(fun(&c_user_cmd::m_forwardmove), "m_command_number");
	chai->add(fun(&c_user_cmd::m_tick_count), "m_tick_count");
	chai->add(fun(&c_user_cmd::m_viewangles), "m_viewangles");
	chai->add(fun(&c_user_cmd::m_aimdirection), "m_aimdirection");
	chai->add(fun(&c_user_cmd::m_forwardmove), "m_forwardmove");
	chai->add(fun(&c_user_cmd::m_sidemove), "m_sidemove");
	chai->add(fun(&c_user_cmd::m_upmove), "m_upmove");
	chai->add(fun(&c_user_cmd::m_buttons), "m_buttons");
	chai->add(fun(&c_user_cmd::m_impulse), "m_impulse");
	chai->add(fun(&c_user_cmd::m_weaponselect), "m_weaponselect");
	chai->add(fun(&c_user_cmd::m_weaponsubtype), "m_weaponsubtype");
	chai->add(fun(&c_user_cmd::m_random_seed), "m_random_seed");
	chai->add(fun(&c_user_cmd::m_mousedx), "m_mousedx");
	chai->add(fun(&c_user_cmd::m_mousedy), "m_mousedy");
	chai->add(fun(&c_user_cmd::m_hasbeenpredicted), "m_hasbeenpredicted");

	// c_renderer wrapper
	chai->add(user_type<chai_wrapper_renderer>(), "renderer");
	chai->add(fun(&chai_wrapper_renderer::ansi_text), "ansi_text");
	chai->add(fun(&chai_wrapper_renderer::rect), "rect");
	chai->add(fun(&chai_wrapper_renderer::circle), "circle");
	chai->add(fun(&chai_wrapper_renderer::filled_rect), "filled_rect");
	chai->add(fun(&chai_wrapper_renderer::line), "line");

	// entlist
	chai->add(user_type<chai_wrapper_entlist>(), "entitylist");
	chai->add(fun(&chai_wrapper_entlist::get_max_clients), "get_entity_amount"); // fix
	chai->add(fun(&chai_wrapper_entlist::get_local_player), "get_local_player");
	chai->add(fun(&chai_wrapper_entlist::get_player), "get_player");
	chai->add(fun(&chai_wrapper_entlist::is_valid), "is_valid");

	chai->add(user_type<chai_wrapper_player>(), "player");
	chai->add(fun(&chai_wrapper_player::get_eye_pos), "get_eye_pos");
	chai->add(fun(&chai_wrapper_player::get_health), "get_health");
	chai->add(fun(&chai_wrapper_player::get_team), "get_team");
	chai->add(fun(&chai_wrapper_player::get_origin), "get_origin");
	chai->add(fun(&chai_wrapper_player::is_alive), "is_alive");
	chai->add(fun(&chai_wrapper_player::is_valid), "is_valid");
	chai->add(fun(&chai_wrapper_player::get_name), "get_name");
	chai->add(fun(&chai_wrapper_player::get_id), "get_id");
	chai->add(fun(&chai_wrapper_player::is_same_as), "is_same_as");
	chai->add(fun(&chai_wrapper_player::player_check), "player_check");
	chai->add(fun(&chai_wrapper_player::get_index), "get_index");


	// misc
	chai->add(user_type<chai_wrapper_misc>(), "misc");
	chai->add(fun(&chai_wrapper_misc::key_pressed), "key_pressed"); // if this doesn't work for you, use the actual key byte code in the chaiscript file ("0x20", not "20" for VK_SPACE)
	chai->add(fun(&chai_wrapper_misc::is_visible), "is_visible");
	chai->add(fun(&return_misc_wrap), "get_misc");
	chai->add(fun(&return_entlist_wrap), "get_entlist");
	chai->add(fun(&return_color), "get_color");
	chai->add(fun(&chai_world_to_screen), "world_to_screen");

	chai->add(user_type<vec3_t>(), "vec3_t");
	chai->add(fun(&vec3_t::x), "x");
	chai->add(fun(&vec3_t::y), "y");
	chai->add(fun(&vec3_t::z), "z");

	chai->add(fun(&vec3_t::distance), "distance");

	chai->add(user_type<OSHGui::Drawing::Color>(), "color");
	chai->add(fun(&OSHGui::Drawing::Color::GetRed), "get_red");
	chai->add(fun(&OSHGui::Drawing::Color::GetGreen), "get_green");
	chai->add(fun(&OSHGui::Drawing::Color::GetBlue), "get_blue");
	chai->add(fun(&OSHGui::Drawing::Color::FromRGB), "from_rgb");
	chai->add(constructor<OSHGui::Drawing::Color(float, float, float)>(), "color");
	chai->add(constructor<OSHGui::Drawing::Color(float, float, float, float)>(), "color");
	chai->add(fun(&chai_cos), "cos");
	chai->add(fun(&chai_sin), "sin");
	chai->add(fun(&chai_tan), "tan");

	chai_finished_init = true;
}

namespace chai_manager {
	std::function<void(chai_console_text)> print_func;
	std::vector<chai_wrapper> sessions;
	std::vector<std::string> chai_aa_pitch;
	std::vector<std::string> chai_aa_yaw;

	void chai_print(const std::string &str) {
		// quick hack to make printing to chai_console work
		chai_console_text cstxt;
		cstxt.back_color = OSHGui::Drawing::Color::FromARGB(50, 0, 0, 0);
		cstxt.side_color = OSHGui::Drawing::Color::FromARGB(255, 237, 150, 166);
		cstxt.text = str;
		chai_manager::print_func(cstxt);
	}
	void chai_error(const std::string &str) {
		chai_console_text cstxt;
		cstxt.back_color = OSHGui::Drawing::Color::FromARGB(50, 0, 0, 0);
		cstxt.side_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
		cstxt.fore_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
		cstxt.text = str;
		chai_manager::print_func(cstxt);
	}
};