set BakDir=±¸·Ý\HaoBangShouCode%date:~0,10%
rd "%BakDir%\" /s /q
md "%BakDir%\"
copy "*.sln" "%BakDir%\"
copy "*.cmd" "%BakDir%\"
copy "*.exe" "%BakDir%\"
copy "*.xml" "%BakDir%\"

set ProjectName=BatchPlot
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"


set ProjectName=CADCommTools
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"
md "%BakDir%\%ProjectName%\Align"
copy "%ProjectName%\Align\*.bmp" "%BakDir%\%ProjectName%\Align"
md "%BakDir%\%ProjectName%\AverPos"
copy "%ProjectName%\AverPos\*.bmp" "%BakDir%\%ProjectName%\AverPos"

set ProjectName=CADGlobalTools
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"

set ProjectName=CADTextTools
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"

set ProjectName=CADBlockTools
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"

set ProjectName=CADLayerTools
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"

set ProjectName=DwgCompareEx
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"
md "%BakDir%\%ProjectName%\res"
copy "%ProjectName%\res\*.bmp" "%BakDir%\%ProjectName%\res"

set ProjectName=GGridCtrl
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"

set ProjectName=RegionSerach
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"

set ProjectName=RemoveEduStamp
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"


set ProjectName=CADConfigTools
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cur" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"

set ProjectName=CADDimTools
md "%BakDir%\%ProjectName%"
copy "%ProjectName%\*.vcproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.vcxproj.filters" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.cpp" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.h" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.def" "%BakDir%\%ProjectName%\"
copy "%ProjectName%\*.rc" "%BakDir%\%ProjectName%\"