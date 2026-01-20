#pragma once
#ifdef CADGLOBALTOOLS_MODULE 
#define GLOBALTOOLS_EXT __declspec(dllexport)
#pragma message("导出CADGlobalTools.lib……")
#else 
#define GLOBALTOOLS_EXT __declspec(dllimport)
#pragma message("导入CADGlobalTools.lib……")
#endif
#include "GCADUtils.h"
#include "GStringHelper.h"
#include "GAcadError.h"
#include "IdsTrans.h"
#include "GLog.h"
#include "GStringMatch.h"
#include "GUserDataRecord.h"
#include "GMTextFun.h"
#include "EnumEntity.h"

#include "CoordinateD.h"
#include "BoxD.h"
#include "LineD.h"
#include "GlobalSystemValue.h"
#include "GZoom.h"
#include "GFileTools.h"
#include "GSameColorHelper.h"
#include "GTempIntVarHelper.h"
#include "GSelectHelper.h"
#include "GLayerUtils.h"
#include "GSortentsTableTools.h"