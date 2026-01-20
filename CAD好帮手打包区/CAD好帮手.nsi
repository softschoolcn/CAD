; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

; 安装程序初始定义常量
!define PRODUCT_NAME "CAD好帮手"
!define PRODUCT_VERSION "1.0.9"
!define PRODUCT_PUBLISHER "www.cadgj.com, Inc."
!define PRODUCT_WEB_SITE "http://www.cadgj.com"
!define PRODUCT_ROOT_KEY "SOFTWARE\GoodAssistant"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\AppMainExe.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
Var /GLOBAL SYS_UPDATE
Var /GLOBAL SYS_Directory
;;定义一个全局变量，用于保存当前正在处理的CAD产品 如:ACAD-4001:804
;;////////////////////////根据不同项目需要做调整的部分///////////////////////
;;////////////////////////根据不同项目需要做调整的部分///////////////////////
SetCompressor lzma

; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"
!include "logiclib.nsh"
!include "WinMessages.nsh"
!include "FileFunc.nsh"
!include "x64.nsh"
!include "AutoCAD.nsh"
!insertmacro GetParameters
!insertmacro GetOptions
;;{UnStrRep}
;;!include "WordFunc.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON "打包文件\GoodAssistant.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 许可协议页面
;!insertmacro MUI_PAGE_LICENSE "c:\path\to\licence\YourSoftwareLicence.txt"
; 安装目录选择页面
Page directory skipDirectory
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_SHOWREADME
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION Info
!define MUI_FINISHPAGE_SHOWREADME_TEXT "访问CAD工具之家(cadgj.com)"
; 安装完成页面
!insertmacro MUI_PAGE_FINISH
Function Info
ExecShell "open" "http://www.cadgj.com"
Functionend

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "SimpChinese"

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "HaoBangShou ${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "CAD好帮手(http://www.cadgj.com)"
Section "MainSection" SEC01
StrCmp $SYS_Directory "" lb_AfterCheckOldVersion 0
  IfFileExists "$SYS_Directory\*.*" 0 lb_AfterCheckOldVersion
	StrCpy $R1 0
  ${While} $R1 < 5
  Sleep 1000
    IntOp $R1 $R1 + 1
    IfFileExists "$SYS_Directory\AutoUpdate\Update.ini" lb_UninstNotFinish lb_UninstFinish
    lb_UninstFinish:
    StrCpy $R1 1000
    lb_UninstNotFinish:
    DetailPrint "等待旧版本程序卸载完成,请稍后..."
  ${EndWhile}
	lb_AfterCheckOldVersion:


	
	SetOutPath "$INSTDIR"
	File "打包文件\*.ico"
	File "打包文件\*.txt"
  ;;打包自动更新
  SetOutPath "$INSTDIR\AutoUpdate"
  ;;Update.exe会实现自我更新
	SetOverwrite try
	File "打包文件\AutoUpdate\Update.exe"
	SetOverwrite ifdiff
	File "打包文件\AutoUpdate\Update.ini"
	File "打包文件\AutoUpdate\*.dll"
	File "打包文件\AutoUpdate\UpdateConfig.ini"
	File "打包文件\AutoUpdate\UpdateURL.ini"
	SetOverwrite off
	File "打包文件\AutoUpdate\UpdateLog.ini"
	SetOverwrite on
  ;;打包config目录
  SetOutPath "$INSTDIR\Config"
  File "打包文件\config\*.ini"
  File "打包文件\config\*.xml"
  
  SetOutPath "$INSTDIR\Plotters"
  File "打包文件\Plotters\*.pc3"
  
  SetOutPath "$INSTDIR\UserConfig"
  SetOutPath "$INSTDIR\log"
  File "打包文件\log\*.ini"

	SetOutPath "$INSTDIR\bin"
  File "打包文件\bin\*.exe"
  File "打包文件\bin\*.dll"
  SetOutPath "$INSTDIR\Doc"
  File "打包文件\Doc\*.doc"
  File "打包文件\Doc\*.pdf"
  
  SetOutPath "$INSTDIR\3rd"
  SetOutPath "$INSTDIR\3rd\gs"
  File "打包文件\3rd\gs\*.exe"
  File "打包文件\3rd\gs\*.dll"
  File "打包文件\3rd\gs\*.ATB"
  File "打包文件\3rd\gs\*.ATM"
  File "打包文件\3rd\gs\*.ps"
  File "打包文件\3rd\gs\*.dir"
  File "打包文件\3rd\gs\*.GS"
  File "打包文件\3rd\gs\*.OS2"
  File "打包文件\3rd\gs\*.OSF"
  File "打包文件\3rd\gs\*.pfb"
  File "打包文件\3rd\gs\*.plist"
  File "打包文件\3rd\gs\*.scale"
  File "打包文件\3rd\gs\*.SGI"
  File "打包文件\3rd\gs\*.sh"
  File "打包文件\3rd\gs\*.Sol"
  File "打包文件\3rd\gs\*.Ult"
  File "打包文件\3rd\gs\*.VMS"
  File "打包文件\3rd\gs\*.xbm"
  File "打包文件\3rd\gs\*.xpm"
  File "打包文件\3rd\gs\COPYING"
  File "打包文件\3rd\gs\Fontmap"
  File "打包文件\3rd\gs\pphs"
  File "打包文件\3rd\gs\xlatmap"
  
  ;;SetOutPath "$INSTDIR\qrcode_data"
  ;;File "打包文件\qrcode_data\*.dat"
  
  Call InstallAutoCAD
  
  ;写注册表
  WriteRegStr HKLM "${PRODUCT_ROOT_KEY}" "AppLocation" "$INSTDIR"
  WriteRegStr HKLM "${PRODUCT_ROOT_KEY}" "Version" ${PRODUCT_VERSION}
  ${If} ${RunningX64}
  SetRegView 64
  WriteRegStr HKLM "${PRODUCT_ROOT_KEY}" "AppLocation" "$INSTDIR"
  WriteRegStr HKLM "${PRODUCT_ROOT_KEY}" "Version" ${PRODUCT_VERSION}
  SetRegView 32
  ${Endif}
  
SectionEnd
Section -AdditionalIcons
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\检查最新版本.lnk" "$INSTDIR\AutoUpdate\Update.exe" "/UserCheck"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\关于CAD好帮手.lnk" "$INSTDIR\Doc\CAD好帮手.pdf"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\卸载${PRODUCT_NAME}.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\AppMainExe.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\AppMainExe.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/
Section Uninstall
;;删除根目录
Delete "$INSTDIR\*.ini"
  Delete "$INSTDIR\*.txt"
  Delete "$INSTDIR\*.xml"
  Delete "$INSTDIR\*.exe"
  Delete "$INSTDIR\*.dll"
  ;;删除config目录
  Delete "$INSTDIR\Config\*.mns"
  Delete "$INSTDIR\Config\*.mnr"
  Delete "$INSTDIR\Config\*.cui"
  Delete "$INSTDIR\Config\*.cuix"
  Delete "$INSTDIR\Config\*.xml"
  Delete "$INSTDIR\Config\*.ini"
  RMDir "$INSTDIR\Config\"
  
  Delete "$INSTDIR\Plotters\*.pc3"
  RMDir "$INSTDIR\Plotters\"
  ;;删除log目录
  Delete "$INSTDIR\log\*.ini"
  RMDir "$INSTDIR\log\"
  ;;删除bin目录
  Delete "$INSTDIR\bin\*.dll"
  Delete "$INSTDIR\bin\*.exe"
  RMDir "$INSTDIR\bin"
  ;;删除Doc目录
  Delete "$INSTDIR\Doc\*.doc"
  Delete "$INSTDIR\Doc\*.pdf"
  RMDir "$INSTDIR\Doc"
  
  ;;删除3rd\gs目录
  Delete "$INSTDIR\3rd\gs\*.*"
  RMDir "$INSTDIR\3rd\gs"
  Delete "$INSTDIR\3rd\*.*"
  RMDir "$INSTDIR\3rd"
  
  ;;删除Doc目录
  Delete "$INSTDIR\qrcode_data\*.dat"
  RMDir "$INSTDIR\qrcode_data"
  
  
  Call Un.UnstallAutoCAD

  Delete "$SMPROGRAMS\${PRODUCT_NAME}\卸载${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\关于CAD好帮手.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\检查最新版本.lnk"

  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
  SetOutPath $TEMP
  
  ;;删除AutoUpdate目录
  Delete "$INSTDIR\AutoUpdate\*.ini"
  Delete "$INSTDIR\AutoUpdate\*.dll"
  Delete "$INSTDIR\AutoUpdate\*.exe"
  RMDir "$INSTDIR\AutoUpdate"
  Delete "$INSTDIR\*.ico"
	RMDir "$INSTDIR"
  
  DeleteRegKey HKLM "${PRODUCT_ROOT_KEY}"
  ${If} ${RunningX64}
  SetRegView 64
  DeleteRegKey HKLM "${PRODUCT_ROOT_KEY}"
  SetRegView 32
  ${Endif}

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function .onInit
System::Call 'kernel32::CreateMutexA(i 0, i 0, t "GoodAssistantInstallMutex") i .r1 ?e'
 Pop $R0

 StrCmp $R0 0 +3
   MessageBox MB_OK|MB_ICONEXCLAMATION "安装程序已经在运行。"
   Abort
   



${GetParameters} $R0
;;不能使用/D这个是系统使用的，你无法获取到
  ClearErrors
  ${GetOptions} $R0 /UPDATE= $SYS_UPDATE
  ReadRegStr $SYS_Directory HKLM "${PRODUCT_ROOT_KEY}" "AppLocation"
  
  ;;检查是否已经安装了老版本
  StrCmp $SYS_UPDATE "TRUE" lb_AfterCheckOldVersion 0
  StrCmp $SYS_Directory "" lb_AfterCheckOldVersion 0
  IfFileExists "$SYS_Directory\*.*" 0 lb_AfterCheckOldVersion
  MessageBox MB_YESNO "安装程序检测到本机安装有其它版本的${PRODUCT_NAME}，是否卸载?。$\r$\n点击“是”卸载原有版本并安装新版本，点击“否”退出。" IDNO lb_Exit
  IfFileExists "$SYS_Directory\uninst.exe" 0 lb_AfterCheckOldVersion
  ;;MessageBox MB_OK "$SYS_Directory\uninst.exe"
	ExecWait '"$SYS_Directory\uninst.exe" /S'
	;;IfFileExists "$SYS_Directory\uninst.exe" 0 +2
	;;卸载失败会用户取消卸载直接返回
	;;Abort
lb_AfterCheckOldVersion:
  Call CheckConflictProcess
  return
  lb_Exit:
Abort
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除 $(^Name) ，及其所有的组件？" /SD IDYES IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从您的计算机移除。" /SD IDOK
FunctionEnd

Function skipDirectory
StrCmp $SYS_UPDATE "TRUE" 0 lb_Directory_end
StrCmp $SYS_Directory "" lb_Directory_end 0
IfFileExists "$SYS_Directory\*.*" 0 lb_Directory_end
StrCpy $INSTDIR $SYS_Directory
Abort
lb_Directory_end:
FunctionEnd

;;检测冲突的进程
Function CheckConflictProcess
Call CheckAcadProcess
FunctionEnd

