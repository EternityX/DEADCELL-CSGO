#pragma once

#pragma warning( disable : 4307 ) // warning C4307: '*': integral constant overflow
#pragma warning( disable : 4244 ) // warning C4244: '=': conversion possible loss of data

using ulong_t = unsigned long;

#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS

#include <windows.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <cctype>
#include <random>
#include <array>
#include <utility>
#include <intrin.h>
#include <chrono>
#include <iostream>

// imgui.
#include <d3d9.h>
#include <imgui.h>
#include <imgui_internal.h>
#include "imgui/fontawesome5.h"
#include <examples/imgui_impl_dx9.h>
#include <examples/imgui_impl_win32.h>

#include "hash/hash.hpp"
#include "pe/types.hpp"
#include "pe/module/module.hpp"
#include "pe/pe.hpp"
#include "util/misc/misc.hpp"
#include "util/pattern/pattern.hpp"
#include "math/math.hpp"
#include "console/console.hpp"
#include "input/input.hpp"

// renderer.
#include "imgui/renderer.hpp"