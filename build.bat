@ECHO OFF
SETLOCAL EnableDelayedExpansion

IF NOT EXIST "build_config.ini" (
    echo Error: build_config.ini not found.
    goto :EOF
)

FOR /F "tokens=1* delims==" %%L IN ('type build_config.ini ^| find "="') DO (
    SET "%%L=%%M"
)

SET "METAMOD_PATH=%BASE_PATH%\metamod"
SET "SOURCE2_PATH=%BASE_PATH%\Source2"
SET "PLUGIN_PATH=%BASE_PATH%\%OUTPUT_NAME%"

if not exist "%PLUGIN_PATH%\xbyak\.git" (
    echo Cloning xbyak submodule from GitHub...
    %GIT_CMD% clone -b %XBYAK_BRANCH% %XBYAK_REPO% "%PLUGIN_PATH%\xbyak"
    IF ERRORLEVEL 1 (
       echo Failed to clone xbyak repository.
       goto :EOF
    )
) else (
    echo Updating xbyak submodule...
    pushd "%PLUGIN_PATH%\xbyak"
    %GIT_CMD% pull
    IF ERRORLEVEL 1 (
         echo Failed to update xbyak repository.
         popd
         goto :EOF
    )
    popd
)

(
    echo /nologo
    echo /O1
    echo /GL
    echo /std:c++17
    echo /MT
    echo /W4
    echo /EHsc
    echo /D_CRT_SECURE_NO_WARNINGS
    echo /DMETA_IS_SOURCE2
    echo /DMETA_NO_HL2SDK
    echo /I"%METAMOD_PATH%"
    echo /I"%METAMOD_PATH%\core"
    echo /I"%METAMOD_PATH%\core\sourcehook"
    echo /I"%SOURCE2_PATH%"
    echo /I"%PLUGIN_PATH%"
    echo /I"%PLUGIN_PATH%\xbyak"
    echo /c
    echo "%METAMOD_PATH%\core\metamod_oslink.cpp"
    echo "%PLUGIN_PATH%\utils\detour_manager.cpp"
    echo "%PLUGIN_PATH%\%PLUGIN_NAME%.cpp"
) > compile.rsp

%RC_CMD% /nologo /fo version.res metadata\version.rc
IF ERRORLEVEL 1 (
    echo Failed to compile the resource file.
    goto :EOF
)

%CL_CMD% @compile.rsp
IF ERRORLEVEL 1 (
    echo Compilation failed.
    goto :EOF
)

%LINK_CMD% %LINKER_PARAM% metamod_oslink.obj %PLUGIN_NAME%.obj detour_manager.obj version.res /OUT:%OUTPUT_NAME%.dll
IF ERRORLEVEL 1 (
    echo Linking failed.
    goto :EOF
)

CALL metadata\version_increment.bat

if EXIST %OUTPUT_NAME%.exp del %OUTPUT_NAME%.exp
if EXIST %OUTPUT_NAME%.lib del %OUTPUT_NAME%.lib
if EXIST *.obj del *.obj
if EXIST version.res del version.res
if EXIST compile.rsp del compile.rsp

echo Build completed successfully.
ENDLOCAL