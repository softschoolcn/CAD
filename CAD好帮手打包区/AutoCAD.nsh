!include "x64.nsh"
!include "WordFunc.nsh"
!include "StrFunc.nsh"
!include "nsProcess.nsh"
${StrRep}
Var /GLOBAL ACAD_REG_VERSION
Var /GLOBAL ACAD_REG_PRODUCT
Var /GLOBAL RegGlodCADToolsRegKey
var /GLOBAL RegGlodCADToolsSubSupportPath
var /GLOBAL Glb_AcadLocation
Var /GLOBAL Glb_AcadRelease
Function InstallAutoCAD
;;打包sys16目录
  SetOutPath "$INSTDIR\sys16.0"
  File "打包文件\sys16.0\*.dll"
  File "打包文件\sys16.0\*.rx"
  File "打包文件\sys16.0\*.arx"
  File "打包文件\sys16.0\*.ini"
;;打包sys16目录
  SetOutPath "$INSTDIR\sys16"
  File "打包文件\sys16\*.dll"
  File "打包文件\sys16\*.rx"
  File "打包文件\sys16\*.arx"
  File "打包文件\sys16\*.ini"
  ;;打包sys17目录
  SetOutPath "$INSTDIR\sys17"
  File "打包文件\sys17\*.dll"
  File "打包文件\sys17\*.rx"
  File "打包文件\sys17\*.arx"
  File "打包文件\sys17\*.ini"
  ;;打包sys18目录
  SetOutPath "$INSTDIR\sys18"
  File "打包文件\sys18\*.dll"
  File "打包文件\sys18\*.rx"
  File "打包文件\sys18\*.arx"
  File "打包文件\sys18\*.ini"

  ;;打包sys19目录
  SetOutPath "$INSTDIR\sys19"
  File "打包文件\sys19\*.dll"
  File "打包文件\sys19\*.rx"
  File "打包文件\sys19\*.arx"
  File "打包文件\sys19\*.ini"
  
  ;;打包sys20目录
  SetOutPath "$INSTDIR\sys20"
  ;;File "打包文件\sys20\*.dll"
  File "打包文件\sys20\*.rx"
  File "打包文件\sys20\*.arx"
  File "打包文件\sys20\*.ini"
  ;;打包sys18(X64)目录
  SetOutPath "$INSTDIR\sys18(X64)"
  File "打包文件\sys18(X64)\*.dll"
  File "打包文件\sys18(X64)\*.rx"
  File "打包文件\sys18(X64)\*.arx"
  File "打包文件\sys18(X64)\*.ini"

  SetOutPath "$INSTDIR\sys19(X64)"
  File "打包文件\sys19(X64)\*.dll"
  File "打包文件\sys19(X64)\*.rx"
  File "打包文件\sys19(X64)\*.arx"
  File "打包文件\sys19(X64)\*.ini"
  
  ;;打包sys20(X64)目录
  SetOutPath "$INSTDIR\sys20(X64)"
  ;;File "打包文件\sys20(X64)\*.dll"
  File "打包文件\sys20(X64)\*.rx"
  File "打包文件\sys20(X64)\*.arx"
  File "打包文件\sys20(X64)\*.ini"
  Call .RegGlodCADToolsToAllCAD
  ${If} ${RunningX64}
  Call .RegGlodCADToolsToAllCADX64
  ${Endif}
FunctionEnd
Function Un.UnstallAutoCAD
;;删除sys16.0目录
  Delete "$INSTDIR\sys16.0\*.arx"
  Delete "$INSTDIR\sys16.0\*.dll"
  Delete "$INSTDIR\sys16.0\*.ini"
  Delete "$INSTDIR\sys16.0\*.rx"
  RMDir "$INSTDIR\sys16.0"
;;删除sys16目录
  Delete "$INSTDIR\sys16\*.arx"
  Delete "$INSTDIR\sys16\*.dll"
  Delete "$INSTDIR\sys16\*.ini"
  Delete "$INSTDIR\sys16\*.rx"
  RMDir "$INSTDIR\sys16"
  ;;删除sys17目录
  Delete "$INSTDIR\sys17\*.arx"
  Delete "$INSTDIR\sys17\*.dll"
  Delete "$INSTDIR\sys17\*.ini"
  Delete "$INSTDIR\sys17\*.rx"
  RMDir "$INSTDIR\sys17"
  ;;删除sys18目录
  Delete "$INSTDIR\sys18\*.arx"
  Delete "$INSTDIR\sys18\*.rx"
  Delete "$INSTDIR\sys18\*.dll"
  Delete "$INSTDIR\sys18\*.ini"
  RMDir "$INSTDIR\sys18"
  ;;删除sys19目录
  Delete "$INSTDIR\sys19\*.arx"
  Delete "$INSTDIR\sys19\*.rx"
  Delete "$INSTDIR\sys19\*.dll"
  Delete "$INSTDIR\sys19\*.exe"
  Delete "$INSTDIR\sys19\*.ini"
  RMDir "$INSTDIR\sys19"
  ;;删除sys20目录
  Delete "$INSTDIR\sys20\*.arx"
  Delete "$INSTDIR\sys20\*.rx"
  Delete "$INSTDIR\sys20\*.dll"
  Delete "$INSTDIR\sys20\*.exe"
  Delete "$INSTDIR\sys20\*.ini"
  RMDir "$INSTDIR\sys20"
  ;;删除sys18(X64)目录
  Delete "$INSTDIR\sys18(X64)\*.arx"
  Delete "$INSTDIR\sys18(X64)\*.rx"
  Delete "$INSTDIR\sys18(X64)\*.dll"
  Delete "$INSTDIR\sys19(X64)\*.exe"
  Delete "$INSTDIR\sys19(X64)\*.ini"
  RMDir "$INSTDIR\sys18(X64)"
  ;;删除sys19(X64)目录
  Delete "$INSTDIR\sys19(X64)\*.arx"
  Delete "$INSTDIR\sys19(X64)\*.rx"
  Delete "$INSTDIR\sys19(X64)\*.dll"
  Delete "$INSTDIR\sys19(X64)\*.ini"
  RMDir "$INSTDIR\sys19(X64)"
  
  ;;删除sys20(X64)目录
  Delete "$INSTDIR\sys20(X64)\*.arx"
  Delete "$INSTDIR\sys20(X64)\*.rx"
  Delete "$INSTDIR\sys20(X64)\*.dll"
  Delete "$INSTDIR\sys20(X64)\*.exe"
  Delete "$INSTDIR\sys20(X64)\*.ini"
  RMDir "$INSTDIR\sys20(X64)"
  
  ;;卸载所有的自动加载
	Call Un.RegGlodCADToolsToAllCAD
	${If} ${RunningX64}
  Call Un.RegGlodCADToolsToAllCADx64
  ${Endif}
FunctionEnd
Function .RegGlodCADToolsXX
;;只有安装位置和版本号有效才是有效的版本
	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\GGridCtrl.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\AGGridCtrl" "DESCRIPTION" "CADCommTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\AGGridCtrl" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\GGridCtrl.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\AGGridCtrl" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADGlobalTools.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\ACADGlobalTools" "DESCRIPTION" "CADGlobalTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\ACADGlobalTools" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADGlobalTools.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\ACADGlobalTools" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADConfigTools.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\BCADConfigTools" "DESCRIPTION" "CADConfigTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\BCADConfigTools" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADConfigTools.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\BCADConfigTools" "LOADCTRLS" 0x2
	${EndIf}

  ${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADCommTools.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\CCADCommTools" "DESCRIPTION" "CADCommTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\CCADCommTools" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADCommTools.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\CCADCommTools" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADTextTools.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADTextTools" "DESCRIPTION" "CADTextTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADTextTools" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADTextTools.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\DCADTextTools" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADBlockTools.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADBlockTools" "DESCRIPTION" "CADBlockTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADBlockTools" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADBlockTools.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\DCADBlockTools" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADLayerTools.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADLayerTools" "DESCRIPTION" "CADLayerTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADLayerTools" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADLayerTools.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\DCADLayerTools" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADScreenMenu.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADScreenMenu" "DESCRIPTION" "CADScreenMenu"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADScreenMenu" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADScreenMenu.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\DCADScreenMenu" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\BatchPlot.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DBatchPlot" "DESCRIPTION" "BatchPlot"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DBatchPlot" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\BatchPlot.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\DBatchPlot" "LOADCTRLS" 0x2
	${EndIf}

	;;防止重复加载
	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\DwgCompareEx.arx"
	DeleteRegKey HKLM "$RegGlodCADToolsRegKey\boitboy_DwgCompare"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DwgCompareEx" "DESCRIPTION" "DwgCompareEx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DwgCompareEx" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\DwgCompareEx.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\DwgCompareEx" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\RegionSerach.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\RegionSerach" "DESCRIPTION" "RegionSerach"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\RegionSerach" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\RegionSerach.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\RegionSerach" "LOADCTRLS" 0x2
	${EndIf}

	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\RemoveEduStamp.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\RemoveEduStamp" "DESCRIPTION" "CADCommTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\RemoveEduStamp" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\RemoveEduStamp.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\RemoveEduStamp" "LOADCTRLS" 0x2
	${EndIf}




	${If} ${FileExists} "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADDimTools.arx"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADDimTools" "DESCRIPTION" "CADDimTools"
	WriteRegStr HKLM "$RegGlodCADToolsRegKey\DCADDimTools" "LOADER" "$INSTDIR\$RegGlodCADToolsSubSupportPath\CADDimTools.arx"
	WriteRegDWORD HKLM "$RegGlodCADToolsRegKey\DCADDimTools" "LOADCTRLS" 0x2
	${EndIf}
FunctionEnd
;;注册金曲CAD插件自动启动
Function .RegGlodCADTools
StrCpy $RegGlodCADToolsRegKey "SOFTWARE\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Applications"
StrCpy $RegGlodCADToolsSubSupportPath "sys16"
${Select} $ACAD_REG_VERSION
    ${Case} "R16.0"
    StrCpy $RegGlodCADToolsSubSupportPath "sys16.0"
    ${Case2} "R16.1" "R16.2"
    StrCpy $RegGlodCADToolsSubSupportPath "sys16"
    ${Case3} "R17.0" "R17.1" "R17.2"
    StrCpy $RegGlodCADToolsSubSupportPath "sys17"
    ${Case3} "R18.0" "R18.1" "R18.2"
    StrCpy $RegGlodCADToolsSubSupportPath "sys18"
    ${Case3} "R19.0" "R19.1" "R19.2"
    StrCpy $RegGlodCADToolsSubSupportPath "sys19"
    ${Case2} "R20.0" "R20.1"
    StrCpy $RegGlodCADToolsSubSupportPath "sys20"
    ${CaseElse}
    Goto done
  ${EndSelect}
	ReadRegStr $Glb_AcadLocation HKLM "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT" "AcadLocation"
	StrCmp $Glb_AcadLocation "" done
	ReadRegStr $Glb_AcadRelease HKLM "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT" "Release"
	StrCmp $Glb_AcadRelease "" done
	;;只有安装位置和版本号有效才是有效的版本
	Call .RegGlodCADToolsXX
	
	
	${If} $ACAD_REG_VERSION = "R19.1"
	${OrIf} $ACAD_REG_VERSION = "R20.0"
	${OrIf} $ACAD_REG_VERSION = "R20.1"
	StrCpy $R0 0
		addTrustedPathLoop:
			EnumRegKey $R1 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles" $R0
			StrCmp $R1 "" done
			ReadRegStr $R2 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$R1\Variables" "TRUSTEDPATHS"
			${WordAdd} $R2 ";" "+$INSTDIR\$RegGlodCADToolsSubSupportPath" $R3
			WriteRegStr HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$R1\Variables" "TRUSTEDPATHS" $R3
			IntOp $R0 $R0 + 1
			Goto addTrustedPathLoop
	${EndIf}
			;;设置打印机
			ReadRegStr $5 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles" ""
  StrCmp $5 "" done
  ReadRegStr $7 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$5\General" "PrinterConfigDir"
SetOverwrite try
StrCmp $7 "" done
${StrRep} "$7" "$7" "%USERPROFILE%" "$PROFILE"

CopyFiles /SILENT /FILESONLY "$INSTDIR\Plotters\DXBTrans.cadgj.com.pc3" "$7\DXBTrans.cadgj.com.pc3"
CopyFiles /SILENT /FILESONLY "$INSTDIR\Plotters\EPSTrans.cadgj.com.pc3" "$7\EPSTrans.cadgj.com.pc3"
done:
FunctionEnd


;;注册金曲CAD插件自动启动
Function .RegGlodCADToolsX64
StrCpy $RegGlodCADToolsRegKey "SOFTWARE\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Applications"
StrCpy $RegGlodCADToolsSubSupportPath "sys16"
${Select} $ACAD_REG_VERSION
    ${Case3} "R18.0" "R18.1" "R18.2"
    StrCpy $RegGlodCADToolsSubSupportPath "sys18(X64)"
    ${Case3} "R19.0" "R19.1" "R19.2"
    StrCpy $RegGlodCADToolsSubSupportPath "sys19(X64)"
    ${Case2} "R20.0" "R20.1"
    StrCpy $RegGlodCADToolsSubSupportPath "sys20(X64)"
    ${CaseElse}
    Goto done
  ${EndSelect}
	ReadRegStr $Glb_AcadLocation HKLM "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT" "AcadLocation"
	StrCmp $Glb_AcadLocation "" done
	ReadRegStr $Glb_AcadRelease HKLM "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT" "Release"
	StrCmp $Glb_AcadRelease "" done
	;;只有安装位置和版本号有效才是有效的版本
	Call .RegGlodCADToolsXX





${If} $ACAD_REG_VERSION = "R19.1"
	${OrIf} $ACAD_REG_VERSION = "R20.0"
	${OrIf} $ACAD_REG_VERSION = "R20.1"
	StrCpy $R0 0
		addTrustedPathLoop:
			EnumRegKey $R1 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles" $R0
			StrCmp $R1 "" done
			ReadRegStr $R2 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$R1\Variables" "TRUSTEDPATHS"
			${WordAdd} $R2 ";" "+$INSTDIR\$RegGlodCADToolsSubSupportPath" $R3
			WriteRegStr HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$R1\Variables" "TRUSTEDPATHS" $R3
			IntOp $R0 $R0 + 1
			Goto addTrustedPathLoop
	${EndIf}
			
			;;设置打印机
			ReadRegStr $5 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles" ""
  StrCmp $5 "" done
  ReadRegStr $7 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$5\General" "PrinterConfigDir"
SetOverwrite try
StrCmp $7 "" done
${StrRep} "$7" "$7" "%USERPROFILE%" "$PROFILE"

CopyFiles /SILENT /FILESONLY "$INSTDIR\Plotters\DXBTrans.cadgj.com.pc3" "$7\DXBTrans.cadgj.com.pc3"
CopyFiles /SILENT /FILESONLY "$INSTDIR\Plotters\EPSTrans.cadgj.com.pc3" "$7\EPSTrans.cadgj.com.pc3"
			
done:
FunctionEnd

;;将金曲CAD工具包安装到某个CAD版本中
Function .RegGlodCADToolsToCADProduct
StrCpy $2 0
loop1:
  EnumRegKey $3 HKLM "SOFTWARE\Autodesk\AutoCAD\$ACAD_REG_VERSION" $2
  StrCmp $3 "" done1
  IntOp $2 $2 + 1
  StrCpy $ACAD_REG_PRODUCT $3
  Call .RegGlodCADTools
	goto loop1
  ;;MessageBox MB_YESNO|MB_ICONQUESTION "$1$\n$\n更多?" IDYES loop
done1:
FunctionEnd

;;将金曲CAD工具包安装到某个CAD版本中
Function .RegGlodCADToolsToCADProductX64
StrCpy $2 0
loop1:
  EnumRegKey $3 HKLM "SOFTWARE\Autodesk\AutoCAD\$ACAD_REG_VERSION" $2
  StrCmp $3 "" done1
  IntOp $2 $2 + 1
  StrCpy $ACAD_REG_PRODUCT $3
  Call .RegGlodCADToolsX64
	goto loop1
  ;;MessageBox MB_YESNO|MB_ICONQUESTION "$1$\n$\n更多?" IDYES loop
done1:
FunctionEnd

;;将金曲CAD工具包安装到所有的CAD版本中
Function .RegGlodCADToolsToAllCAD
StrCpy $0 0
loop:
  EnumRegKey $1 HKLM "SOFTWARE\Autodesk\AutoCAD" $0
  StrCmp $1 "" done
  IntOp $0 $0 + 1
  StrCpy $ACAD_REG_VERSION $1
  Call .RegGlodCADToolsToCADProduct
  Goto loop
done:
FunctionEnd

;;将金曲CAD工具包安装到所有的CAD版本中
Function .RegGlodCADToolsToAllCADX64
SetRegView 64
StrCpy $0 0
loop:
  EnumRegKey $1 HKLM "SOFTWARE\Autodesk\AutoCAD" $0
  StrCmp $1 "" done
  IntOp $0 $0 + 1
  StrCpy $ACAD_REG_VERSION $1
  Call .RegGlodCADToolsToCADProductX64
  Goto loop
done:
SetRegView 32
FunctionEnd

;;从指定的CAD产品中卸载金曲CAD智能工具包
Function Un.RegGlodCADTools
StrCpy $RegGlodCADToolsRegKey "SOFTWARE\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Applications"
;;卸载
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\AGGridCtrl"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\ACADGlobalTools"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\BCADConfigTools"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\CCADCommTools"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DwgCompare"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DwgCompareEx"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DCADScreenMenu"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DwgTrans"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\RegionSerach"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\RemoveEduStamp"

DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DCADTextTools"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DCADLayerTools"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DCADBlockTools"
DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DBatchPlot"

DeleteRegKey HKLM "$RegGlodCADToolsRegKey\DCADDimTools"

;cad2014删除信任路径
	StrCmp $ACAD_REG_VERSION "R19.1" 0 done
		StrCpy $R0 0
		addTrustedPathLoop:
			EnumRegKey $R1 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles" $R0
			StrCmp $R1 "" done
			ReadRegStr $R2 HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$R1\Variables" "TRUSTEDPATHS"
			${WordAdd} $R2 ";" "-$INSTDIR\$RegGlodCADToolsSubSupportPath" $R3
			WriteRegStr HKCU "Software\Autodesk\AutoCAD\$ACAD_REG_VERSION\$ACAD_REG_PRODUCT\Profiles\$R1\Variables" "TRUSTEDPATHS" $R3
			IntOp $R0 $R0 + 1
			Goto addTrustedPathLoop
	;;路径被删除后再次打开CAD会自动卸载
	done:
FunctionEnd
;;将金曲CAD工具包从某个CAD版本中卸载
Function Un.RegGlodCADToolsToCADProduct
StrCpy $2 0
loop1:
  EnumRegKey $3 HKLM "SOFTWARE\Autodesk\AutoCAD\$ACAD_REG_VERSION" $2
  StrCmp $3 "" done1
  IntOp $2 $2 + 1
  StrCpy $ACAD_REG_PRODUCT $3
  Call Un.RegGlodCADTools
	goto loop1
  ;;MessageBox MB_YESNO|MB_ICONQUESTION "$1$\n$\n更多?" IDYES loop
done1:
FunctionEnd

;;将金曲CAD工具包从所有的CAD版本中卸载
Function Un.RegGlodCADToolsToAllCAD
StrCpy $0 0
loop:
  EnumRegKey $1 HKLM "SOFTWARE\Autodesk\AutoCAD" $0
  StrCmp $1 "" done
  IntOp $0 $0 + 1
  StrCpy $ACAD_REG_VERSION $1
  Call Un.RegGlodCADToolsToCADProduct
  Goto loop
done:
FunctionEnd

Function Un.RegGlodCADToolsToAllCADx64
StrCpy $0 0
loop:
	SetRegView 64
	EnumRegKey $1 HKLM "SOFTWARE\Autodesk\AutoCAD" $0
	StrCmp $1 "" done
	IntOp $0 $0 + 1
	StrCpy $ACAD_REG_VERSION $1
	Call Un.RegGlodCADToolsToCADProduct
	Goto loop
	SetRegView 32
done:
FunctionEnd

;;检测ACAD进程
Function CheckAcadProcess
CheckProc:
${nsProcess::FindProcess} "acad.exe" $R0
StrCmp $R0 0 0 Done
MessageBox MB_RETRYCANCEl "安装程序检测到 AutoCAD 正在运行，请退出程序后重试。$\r$\n点击“重试”立即重新检测，点击“取消”退出。" IDCANCEL Exit
Goto CheckProc
Exit:
Abort
Done:
Pop $R0
FunctionEnd
