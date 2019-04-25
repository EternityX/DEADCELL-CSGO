#include "chai_wrapper.hpp"
namespace chai_wrapper {
	chaiscript::ChaiScript* chai;
	std::function<void(chai_console_text)> print_func;
	bool chai_finished_init = false;
	bool chai_defined_createmove = false;

	void chai_print(const std::string &str) {
		// quick hack to make printing to chai_console work
		chai_console_text cstxt;
		cstxt.back_color = OSHGui::Drawing::Color::FromARGB(50, 0, 0, 0);
		cstxt.side_color = OSHGui::Drawing::Color::FromARGB(255, 237, 150, 166);
		cstxt.text = str;
		chai_wrapper::print_func(cstxt);
	}

	void chai_additional_parse(const std::string &str) {
		if (str.find("create_move(cmd)") != std::string::npos) { chai_defined_createmove = true; chai_print("defined crmove"); }
	}

	void chai_load(const std::string &str) {
		std::fstream file(str.c_str(), std::fstream::in);
		std::string content = "";
		if (file.is_open()) {
			std::getline(file, content, '\0');
			chai_loadscript(content);
		}
		else {
			chai_console_text cstxt;
			cstxt.back_color = OSHGui::Drawing::Color::FromARGB(50, 0, 0, 0);
			cstxt.side_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
			cstxt.fore_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
			cstxt.text = "Failed to open file";
			chai_wrapper::print_func(cstxt);
		}
	}
	void chai_loadscript(const std::string &str) {
		try {
			chai->eval(str);
		}
		catch (const double) {}
		catch (int) {}
		catch (float) {}
		catch (const std::string &) {}
		catch (const std::exception &e) {
			chai_console_text cstxt;
			cstxt.back_color = OSHGui::Drawing::Color::FromARGB(50, 0, 0, 0);
			cstxt.side_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
			cstxt.fore_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
			cstxt.text = e.what();
			chai_wrapper::print_func(cstxt);
		}
		chai_additional_parse(str);
	}
	void chai_hook_createmove(c_user_cmd* cmd) {
		try { 
			chai_wrapper_ucmd wrapper = chai_wrapper_ucmd(cmd);
			auto func = chai->eval<std::function<void(chai_wrapper_ucmd)>>("create_move");
			func(wrapper); 
		}
		catch (std::exception &e) {
			chai_console_text cstxt;
			cstxt.back_color = OSHGui::Drawing::Color::FromARGB(50, 0, 0, 0);
			cstxt.side_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
			cstxt.fore_color = OSHGui::Drawing::Color::FromARGB(255, 255, 0, 0);
			cstxt.text = e.what();
			chai_wrapper::print_func(cstxt);
		}
	}
	
	void chai_init(std::function<void(chai_console_text)> print) {
		using namespace chaiscript;
		
		chai = new chaiscript::ChaiScript();
		chai_defined_createmove = false;

		print_func = print;

		// basic functions
		chai->add(chaiscript::fun(&chai_print), "print");
		chai->add(chaiscript::fun(&chai_load), "load");

		// c_user_cmd wrapper
		chai->add(user_type<chai_wrapper_ucmd>(), "usercmd");
		chai->add(fun(&chai_wrapper_ucmd::set_forward_move), "set_forward_move");
		chai->add(fun(&chai_wrapper_ucmd::set_side_move), "set_side_move");
		chai->add(fun(&chai_wrapper_ucmd::set_up_move), "set_up_move");
		chai->add(fun(&chai_wrapper_ucmd::set_buttons), "set_buttons");
		chai->add(fun(&chai_wrapper_ucmd::set_viewangles), "set_viewangles");
		chai->add(fun(&chai_wrapper_ucmd::get_buttons), "get_buttons");
		chai->add(fun(&chai_wrapper_ucmd::get_tick_count), "get_tick_count");
		chai->add(fun(&chai_wrapper_ucmd::get_viewangles), "get_viewangles");
		chai->add(fun(&chai_wrapper_ucmd::get_forward_move), "get_forward_move");
		chai->add(fun(&chai_wrapper_ucmd::get_side_move), "get_side_move");
		chai->add(fun(&chai_wrapper_ucmd::get_up_move), "get_up_move");

		// entlist
		chai->add(user_type<chai_wrapper_entlist>(), "entitylist");
		chai->add(fun(&chai_wrapper_entlist::get_max_clients), "get_entity_amount");
		chai->add(fun(&chai_wrapper_entlist::get_local_player), "get_local_player");
		chai->add(fun(&chai_wrapper_entlist::get_player), "get_player");

		// player
		chai->add(user_type<chai_wrapper_player>(), "player");
		chai->add(fun(&chai_wrapper_player::get_eye_pos), "get_eye_pos");
		chai->add(fun(&chai_wrapper_player::get_health), "get_health");
		chai->add(fun(&chai_wrapper_player::get_team), "get_team");
		chai->add(fun(&chai_wrapper_player::get_origin), "get_origin");
		chai->add(fun(&chai_wrapper_player::is_alive), "is_alive");
		chai->add(fun(&chai_wrapper_player::is_valid), "is_valid");
		
		// misc
		chai->add(user_type<chai_wrapper_misc>(), "misc");
		chai->add(fun(&chai_wrapper_misc::key_pressed), "key_pressed"); // if this doesn't work for you, use the actual key byte code in the chaiscript file ("0x20", not "20" for VK_SPACE)
		chai->add(fun(&return_misc_wrap), "get_misc");
		chai->add(fun(&return_entlist_wrap), "get_entlist");


		// vector3
		chai->add(user_type<vec3_t>(), "vec3_t");
		chai->add(fun(&vec3_t::x), "x");
		chai->add(fun(&vec3_t::y), "y");
		chai->add(fun(&vec3_t::z), "z");

		// math (from chaiscript_extras)
		chai->add(fun(&chai_cos), "cos");
		chai->add(fun(&chai_sin), "sin");
		chai->add(fun(&chai_tan), "tan");

		chai_finished_init = true;
	}
}