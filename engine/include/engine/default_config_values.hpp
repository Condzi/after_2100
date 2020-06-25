#pragma once

#include <engine/cstring.hpp>
#include <engine/typedefs.hpp>

namespace con
{
// The values we default to if reading from file failes.
// !!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!
// !!									 !!
// !! Please, keep it up to date with    !!
// !! the original local.variables file! !!
// !!									 !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
compile_constant DEFAULT_CONFIG_CSTRING = CString{
R"(
# Should be in settings configuration file instead?
# Used only at window creation.
:/display
    debug_context_version   4.4
    release_context_version 3.0

    vsync true
    frame_cap 120
    fullscreen false
    window_size 1280 720


:/gameplay
    # Physic updates per second
    ups 60

# Probably should be cut off from public game builds?
:/debug
    dev_console false
    gl_debug    true
)" };
}