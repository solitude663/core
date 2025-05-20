@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

set IncludeDir=
set LibDir=..\external\libs

set Libs=
rem user32.lib gdi32.lib winmm.lib shell32.lib kernel32.lib winmm.lib msvcrtd.lib

set ExtraWarnings=
rem -wd4146 -wd4245 -wd4458 -wd4459 -wd4244 -wd4458 -wd4245 -wd4324 -wd4310 -wd4065 -wd4702

set CompilerFlags=-nologo -MTd -Od -WX -W4 %ExtraWarnings% -wd4996 -wd4505 -wd4267 -wd4456 -wd4201 -wd4100 -wd4189 -wd4101 -wd4624 -wd4127 -wd4002 -std:c++17 -FAsc -Zi -GR- -EHa

set LinkerFlags=-INCREMENTAL:no

set Macros=
rem -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS -D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

cl %Macros% %CompilerFlags% /Fmtest.map %Macros% ..\src\test.cpp -Fetest %IncludeDir% /link %LinkerFlags% /LIBPATH:%LibDir% %Libs%

popd

