@ECHO OFF
SETLOCAL EnableDelayedExpansion

IF NOT EXIST "build_config.ini" (
    ECHO Error: build_config.ini not found.
    GOTO :EOF
)

FOR /F "tokens=1* delims==" %%L IN ('type build_config.ini ^| find "="') DO (
    SET "%%L=%%M"
)

IF NOT DEFINED VSINSTALLDIR (
    IF DEFINED VSDEV_PATH (
         CALL SET "VSDEV_CMD=!VSDEV_PATH!"
    )

    IF NOT EXIST "!VSDEV_CMD!" (
         ECHO Error: vsdevcmd.bat not found at "!VSDEV_CMD!"
         GOTO :EOF
    ) ELSE (
         CALL "!VSDEV_CMD!" -arch=x64
    )
)

SET "METAMOD_PATH=%BASE_PATH%\metamod"
SET "SOURCE2_PATH=%BASE_PATH%\Source2"
SET "PLUGIN_PATH=%BASE_PATH%\%OUTPUT_NAME%"

(
    ECHO /nologo
    ECHO /O1
    ECHO /GL
    ECHO /std:c++17
    ECHO /MT
    ECHO /W4
    ECHO /EHsc
    ECHO /D_CRT_SECURE_NO_WARNINGS
    ECHO /DMETA_IS_SOURCE2
    ECHO /DMETA_NO_HL2SDK
    ECHO /I"%METAMOD_PATH%"
    ECHO /I"%METAMOD_PATH%\core"
    ECHO /I"%METAMOD_PATH%\core\sourcehook"
    ECHO /I"%SOURCE2_PATH%"
    ECHO /I"%PLUGIN_PATH%"
    ECHO /I"%PLUGIN_PATH%\xbyak"
    ECHO /c
    ECHO "%METAMOD_PATH%\core\metamod_oslink.cpp"
    ECHO "%PLUGIN_PATH%\utils\detour_manager.cpp"
    ECHO "%PLUGIN_PATH%\%PLUGIN_NAME%.cpp"
) > compile.rsp

%RC_CMD% /nologo /fo version.res metadata\version.rc
IF ERRORLEVEL 1 (
    ECHO Failed to compile the resource file.
    goto :EOF
)

%CL_CMD% @compile.rsp
IF ERRORLEVEL 1 (
    ECHO Compilation failed.
    goto :EOF
)

%LINK_CMD% %LINKER_PARAM% metamod_oslink.obj %PLUGIN_NAME%.obj detour_manager.obj version.res /OUT:%OUTPUT_NAME%.dll
IF ERRORLEVEL 1 (
    ECHO Linking failed.
    goto :EOF
)

CALL metadata\version_increment.bat

if EXIST %OUTPUT_NAME%.exp del %OUTPUT_NAME%.exp
if EXIST %OUTPUT_NAME%.lib del %OUTPUT_NAME%.lib
if EXIST *.obj del *.obj
if EXIST version.res del version.res
if EXIST compile.rsp del compile.rsp

ECHO Build completed successfully.
ENDLOCAL
