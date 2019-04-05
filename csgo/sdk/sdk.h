#pragma once

// Pad macro
#ifndef CONCAT_IMPL
#define CONCAT_IMPL(x, y) x##y
#ifndef MACRO_CONCAT
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#ifndef PAD
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];
#endif
#endif
#endif

// Custom Classes
#include "custom classes\color.h"
#include "custom classes\vector.h"
#include "custom classes\angle.h"
#include "custom classes\matrix.h"
#include "custom classes\animstate.h"

#include "utils\c_vmt.h"

#include "valve utils\cutlvector.h"

// Interfaces
#include "interfaces\dt_recv.h"
#include "interfaces\client_class.h"
#include "interfaces\def.h"
#include "interfaces\clientdll.h"
#include "interfaces\i_client_mode_shared.h"
#include "interfaces\i_client_entity.h"
#include "interfaces\i_client_entity_list.h"
#include "interfaces\iv_model_info.h"
#include "interfaces\i_client_renderable.h"
#include "interfaces\c_input.h"
#include "interfaces\i_cvar.h"
#include "interfaces\c_global_vars.h"
#include "interfaces\c_handle.h"
#include "interfaces\i_material_var.h"
#include "interfaces\i_material.h"
#include "interfaces\i_texture.h"
#include "interfaces\i_material_system.h"
#include "interfaces\iv_render_view.h"
#include "interfaces\iv_model_render.h"
#include "interfaces\i_surface.h"
#include "interfaces\iv_debug_overlay.h"
#include "interfaces\i_panel.h"
#include "interfaces\i_netchannel.h"
#include "interfaces\i_studio_render.h"
#include "interfaces\i_engine_trace.h"
#include "interfaces\i_game_movement.h"
#include "interfaces\i_prediction.h"
#include "interfaces\i_game_event.h"
#include "interfaces\i_matrendercontext.h"
#include "interfaces\i_view_render_beams.h"
#include "interfaces\i_weaponinfo.h"
#include "interfaces\i_weapon_system.h"
#include "interfaces\i_efx.h"
#include "interfaces\c_glow_object_mngr.h"
#include "interfaces\ienginesounds.h"
#include "interfaces\c_physics_surface.h"
#include "interfaces\i_localize.h"
#include "interfaces\steam.h"
#include "interfaces\i_command_line.h"

#include "..\sdk\utils\interface_manager.h"
#include "csgo\csgo.h"

#include "utils\netvars.h"

#include "custom classes\entity.h"

#include "custom classes\keyvalues.h"

#include "features\client.h"
