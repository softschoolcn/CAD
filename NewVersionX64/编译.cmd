set Solution=CADCommTools
set OutFile=Output.txt
DEL "%OutFile%"

@echo "开始编译->%Solution%17.sln"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" "%Solution%17.sln" /build "Release|Win32" /out "%OutFile%"
@echo "完成编译->%Solution%17.sln"

@echo "开始编译->%Solution%18.sln"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" "%Solution%18.sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%18.sln"

@echo "开始编译->%Solution%19.sln"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe" "%Solution%19.sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%19.sln"

@echo "开始编译->%Solution%20.sln"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.exe" "%Solution%20.sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%20.sln"

@echo "开始编译->%Solution%21.sln"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe" "%Solution%21.sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%21.sln"

@echo "开始编译->%Solution%22.sln"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe" "%Solution%22.sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%22.sln"

@echo "开始编译->%Solution%23.sln"
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe" "%Solution%23.sln" /build "Release|x64" /out "%OutFile%"
@echo "完成编译->%Solution%23.sln"

pause