@echo off
setlocal
title Memento GUI Setup

set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo Visual Studio was not found
    echo Install Visual Studio Community with Desktop development with C++
    pause
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSROOT=%%i"

if not defined VSROOT (
    echo The Visual Studio C++ workload was not found
    echo Open Visual Studio Installer and add Desktop development with C++
    pause
    exit /b 1
)

call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul

set "CMAKE=%VSROOT%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

if not exist "%CMAKE%" (
    echo CMake was not found in the Visual Studio installation
    echo Add CMake tools for Windows through Visual Studio Installer
    pause
    exit /b 1
)

if not exist "%ROOT%\Data\movies.csv" (
    echo Data\movies.csv was not found
    pause
    exit /b 1
)

if not exist "%ROOT%\Data\ratings.csv" (
    echo Data\ratings.csv was not found
    pause
    exit /b 1
)

echo Configuring Memento
"%CMAKE%" -S "%ROOT%" -B "%ROOT%\build-gui" -G "NMake Makefiles"
if errorlevel 1 goto :build_error

echo Building MementoGUI
"%CMAKE%" --build "%ROOT%\build-gui" --target MementoGUI
if errorlevel 1 goto :build_error

if /i "%~1"=="--build-only" exit /b 0

echo Starting MementoGUI
pushd "%ROOT%"
start "" "%ROOT%\build-gui\MementoGUI.exe"
popd
exit /b 0

:build_error
echo.
echo The GUI could not be built
echo Review the first error above or see the README troubleshooting section
pause
exit /b 1
