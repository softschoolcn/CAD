set Solution=CADCommTools
set OutFile=Output.txt
DEL "%OutFile%"

@echo "开始编译->%Solution%16.0.sln"
"C:\Program Files\Microsoft Visual Studio .NET\Common7\IDE\devenv.exe"   "%Solution%16.0.sln" /Build "Release" /out "%OutFile%"
@echo "完成编译->%Solution%16.0.sln"

@echo "开始编译->%Solution%16.sln"
"C:\Program Files\Microsoft Visual Studio .NET\Common7\IDE\devenv.exe"   "%Solution%16.sln" /Build "Release" /out "%OutFile%"
@echo "完成编译->%Solution%16.sln"


@echo "开始编译->%Solution%17.sln"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" "%Solution%17.sln" /build "Release|Win32" /out "%OutFile%"
@echo "完成编译->%Solution%17.sln"

@echo "开始编译->%Solution%18.sln"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" "%Solution%18.sln" /build "Release|Win32" /out "%OutFile%"
@echo "完成编译->%Solution%18.sln"

@echo "开始编译->%Solution%19.sln"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe" "%Solution%19.sln" /build "Release|Win32" /out "%OutFile%"
@echo "完成编译->%Solution%19.sln"


@echo "开始编译->%Solution%18(X64).sln"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" "%Solution%18(X64).sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%18(X64).sln"

@echo "开始编译->%Solution%19(X64).sln"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe" "%Solution%19(X64).sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%19(X64).sln"

pause