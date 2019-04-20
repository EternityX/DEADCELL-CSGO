#pragma once

#pragma warning( disable : 4307 ) // warning C4307: '*': integral constant overflow
#pragma warning( disable : 4244 ) // warning C4244: '=': conversion possible loss of data
#pragma warning( disable : 4996 ) // stupid json error when you try and stream a file to json so we can use it

#define NOMINMAX

#include <windows.h>
#include <winuser.h>
#include <intrin.h>
#include <cstdint>
#include <string>
#include <stdio.h>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <random>
#include <thread>
#include <chrono>
#include <deque>

#include "sdk/address.h"
#include "sdk/stack.h"

// framework include.
#include "../deadcell/inc.hpp"

using namespace renderer;
extern c_renderer g_renderer;

// used for compile-time FNV-1a 32bit hashes.
#define CT_HASH32( str )                                \
    [ & ]( ) {                                          \
        constexpr hash32_t out = util::hash::fnv1a_32( str ); \
                                                        \
        return out;                                     \
    }( )

// used for compile-time FNV-1a 32bit hashes when above macro cant be used for constexpr variables.
#define CT_CONSTHASH32( str ) util::hash::fnv1a_32( str )

#define DEFINE_VFUNC(...) ( this, __VA_ARGS__ ); }
#define VFUNC( index, func, sig ) auto func { return util::misc::vfunc< sig >( this, index ) DEFINE_VFUNC

//sdk 
#include "features/vars.h"

#include "sdk/sdk.h"

#include "hooking/manager.h"
#include "menu/menu.h"
#include "menu/ui.h"

#include "configs/config.h"

#include "features/notifications/notifications.h"
#include "features/events/events.h"