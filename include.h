#pragma once

#include "resolve.hpp"

#include "unity/u_component.hpp"
#include "unity/u_camera.hpp"
#include "unity/u_transform.hpp"
#include "unity/u_time.hpp"
#include "unity/u_renderer.hpp"
#include "unity/u_material.hpp"
#include "unity/u_shader.hpp"
#include "unity/u_behaviour.hpp"
#include "unity/u_render_settings.hpp"
#include "unity/u_light.hpp"
#include "unity/u_object.hpp"
#include "unity/u_mesh.hpp"

#include "so2/s_player.hpp"
#include "so2/s_player_controller.hpp"
#include "so2/s_player_controls.hpp"

namespace sdk {

inline void reset_all() {
    sdk::unity::comp_reset();
    sdk::unity::cam_reset();
    sdk::unity::tr_reset();
    sdk::unity::time_reset();
    sdk::unity::re_reset();
    sdk::unity::mat_reset();
    sdk::unity::sh_reset();
    sdk::unity::beh_reset();
    sdk::unity::rset_reset();
    sdk::unity::light_reset();
    sdk::unity::obj_reset();
    sdk::unity::mesh_reset();
    sdk::so2::pctrl_reset();
    sdk::so2::ctrls_reset();
    sdk::class_cache_reset();
}

}
