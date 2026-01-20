#pragma once
// 定义导出宏
#ifdef GGRIDCTRL_MODULE 
#define GRIDCTRL_EXT __declspec(dllexport)
#pragma message("导出GGridCtrl.lib……")
#else 
#define GRIDCTRL_EXT __declspec(dllimport)
#pragma message("导入GGridCtrl.lib……")
#endif
#include "CellRange.h"
#include "GridCtrl.h"
#include "GridCellDateTime.h"
#include "GridCellCheck.h"
#include "GridCellCombo.h"
#include "GridCellNumeric.h"
#include "GridCellColor.h"