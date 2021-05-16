
cd ../..
SET PROJNAME=MITest
SET BUILD_CONFIG=Debug
SET BUILD_PLAT=x64

"C:\Program Files\CMake\bin\cmake" -S . -B build
cd build

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %BUILD_PLAT%

SET COMPILE_LOG_DIR=CompileLogs
IF NOT EXIST %COMPILE_LOG_DIR% (
	mkdir %COMPILE_LOG_DIR%
)

msbuild %PROJNAME%.sln -m -p:Configuration=%BUILD_CONFIG% -p:Platform=%BUILD_PLAT% -fl1 -flp1:logfile=%COMPILE_LOG_DIR%/%PROJNAME%Errors.txt;errorsonly -fl2 -flp2:logfile=%COMPILE_LOG_DIR%/%PROJNAME%Warnings.txt;warningsonly
cd %BUILD_CONFIG%

::Test stuff should be here
setlocal enabledelayedexpansion
for %%a in ("BasicTypes") do (
     set TEST=%%~a
     !TEST!.exe
)
endlocal
PAUSE