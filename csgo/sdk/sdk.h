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
#include "interfaces\iclientmodeshared.h"
#include "interfaces\icliententity.h"
#include "interfaces\icliententitylist.h"
#include "interfaces\ivmodelinfo.h"
#include "interfaces\iclientrenderable.h"
#include "interfaces\cinput.h"
#include "interfaces\icvar.h"
#include "interfaces\cglobalvars.h"
#include "interfaces\chandle.h"
#include "interfaces\imaterialvar.h"
#include "interfaces\imaterial.h"
#include "interfaces\itexture.h"
#include "interfaces\imaterialsystem.h"
#include "interfaces\ivrenderview.h"
#include "interfaces\ivmodelrender.h"
#include "interfaces\isurface.h"
#include "interfaces\ivdebugoverlay.h"
#include "interfaces\ipanel.h"
#include "interfaces\inetchannel.h"
#include "interfaces\istudiorender.h"
#include "interfaces\ienginetrace.h"
#include "interfaces\igamemovement.h"
#include "interfaces\iprediction.h"
#include "interfaces\igameevent.h"
#include "interfaces\imatrendercontext.h"
#include "interfaces\iviewrenderbeams.h"
#include "interfaces\iweaponinfo.h"
#include "interfaces\iweaponsystem.h"
#include "interfaces\iefx.h"
#include "interfaces\cglowobjectmanager.h"
#include "interfaces\ienginesounds.h"
#include "interfaces\cphysicssurface.h"
#include "interfaces\ilocalize.h"
#include "interfaces\steam.h"
#include "interfaces\icommandline.h"

#include "..\sdk\utils\interface_manager.h"
#include "csgo\csgo.h"

#include "utils\netvars.h"

#include "custom classes\entity.h"

#include "custom classes\keyvalues.h"

#include "features\client.h"
