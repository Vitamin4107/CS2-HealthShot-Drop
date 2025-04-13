@echo off
setlocal enabledelayedexpansion

set "versionFile=%~dp0version.h"
set "tmpFile=%~dp0version.tmp"

if not exist "%versionFile%" (
    echo File "%versionFile%" not found.
    exit /b 1
)

type nul > "%tmpFile%"

for /f "usebackq delims=" %%L in ("%versionFile%") do (
    set "line=%%L"
    echo !line! | findstr /b /c:"#define PLUGIN_VER_BUILD" >nul
    if !errorlevel! EQU 0 (
        for /f "tokens=3" %%A in ("!line!") do (
            set "currentBuild=%%A"
        )
        set /a newBuild=!currentBuild!+1
        echo #define PLUGIN_VER_BUILD !newBuild!>>"%tmpFile%"
    ) else (
        echo !line!>>"%tmpFile%"
    )
)

move /y "%tmpFile%" "%versionFile%" >nul

echo Version updated successfully.
endlocal