; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "CAD好帮手"
#define MyAppVersion "1.1.0"
#define MyAppPublisher "CAD工具之家"
#define MyAppURL "http://www.cadgj.com/"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{96A8EB6E-F5C4-4CA7-BE73-C3B992AA42EA}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputBaseFilename=HaoBangShou V{#MyAppVersion}
SetupIconFile=打包文件\GoodAssistant.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Files]
Source:"打包文件\*.exe";DestDir:"{app}";Flags:ignoreversion;
Source:"打包文件\*.xml";DestDir:"{app}";Flags:ignoreversion;
Source:"打包文件\*.txt";DestDir:"{app}";Flags:ignoreversion;
Source:"打包文件\sys17\*.arx";DestDir:"{app}\sys17";Flags:ignoreversion;
Source:"打包文件\sys17\*.ini";DestDir:"{app}\sys17";Flags:ignoreversion;
Source:"打包文件\sys18\*.arx";DestDir:"{app}\sys18";Flags:ignoreversion;
Source:"打包文件\sys18\*.ini";DestDir:"{app}\sys18";Flags:ignoreversion;
Source:"打包文件\sys18(X64)\*.arx";DestDir:"{app}\sys18(X64)";Flags:ignoreversion;
Source:"打包文件\sys18(X64)\*.ini";DestDir:"{app}\sys18(X64)";Flags:ignoreversion;
Source:"打包文件\sys19\*.arx";DestDir:"{app}\sys19";Flags:ignoreversion;
Source:"打包文件\sys19\*.ini";DestDir:"{app}\sys19";Flags:ignoreversion;
Source:"打包文件\sys19(X64)\*.arx";DestDir:"{app}\sys19(X64)";Flags:ignoreversion;
Source:"打包文件\sys19(X64)\*.ini";DestDir:"{app}\sys19(X64)";Flags:ignoreversion;
Source:"打包文件\sys20\*.arx";DestDir:"{app}\sys20";Flags:ignoreversion;
Source:"打包文件\sys20\*.ini";DestDir:"{app}\sys20";Flags:ignoreversion;
Source:"打包文件\sys20(X64)\*.arx";DestDir:"{app}\sys20(X64)";Flags:ignoreversion;
Source:"打包文件\sys20(X64)\*.ini";DestDir:"{app}\sys20(X64)";Flags:ignoreversion;
Source:"打包文件\sys22(X64)\*.arx";DestDir:"{app}\sys22(X64)";Flags:ignoreversion;
Source:"打包文件\config\*.ini";DestDir:"{app}\config";Flags:ignoreversion;
Source:"打包文件\config\*.xml";DestDir:"{app}\config";Flags:ignoreversion;
Source:"打包文件\Plotters\*.pc3";DestDir:"{app}\Plotters";Flags:ignoreversion;
Source:"打包文件\bin\*.dll";DestDir:"{app}\bin";Flags:ignoreversion;
Source:"打包文件\bin\*.exe";DestDir:"{app}\bin";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.exe";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.dll";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.ATB";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.ATM";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.ps";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.dir";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.GS";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.OS2";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.OSF";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.pfb";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.plist";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.scale";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.SGI";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.sh";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.Sol";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.Ult";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.VMS";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.xbm";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\*.xpm";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\COPYING";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\Fontmap";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\pphs";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
Source:"打包文件\3rd\gs\xlatmap";DestDir:"{app}\3rd\gs";Flags:ignoreversion;
[Dirs]
Name:"{app}\UserConfig"

[Run]
Filename: "{app}\InstallCADPlugin.exe"; Parameters: "/i"
[UninstallRun]
Filename: "{app}\InstallCADPlugin.exe"; Parameters: "/u"

