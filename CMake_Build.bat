@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

echo  ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
echo.
echo    .d8888b.  888                       d8b                .d8888b.  8888888888      8888888b.  8888888888 888     888 
echo   d88P  Y88b 888                       Y8P               d88P  Y88b 888             888   Y88b 888        888     888 
echo   Y88b.      888                                         Y88b.      888             888    888 888        888     888 
echo    "Y888b.   888  888 888  888 888d888 888 88888b.d88b.   "Y888b.   8888888         888    888 8888888    Y88b   d88P 
echo       "Y88b. 888 .88P 888  888 888P"   888 888  888  88b     "Y88b. 888             888    888 888         Y88b d88P  
echo         "888 888888K  888  888 888     888 888  888  888       "888 888             888    888 888          Y88o88P   
echo   Y88b  d88P 888  88b Y88b 888 888     888 888  888  888 Y88b  d88P 888             888  .d88P 888           Y888P    
echo    "Y8888P"  888  888  "Y88888 888     888 888  888  888  "Y8888P"  8888888888      8888888P"  8888888888     Y8P     
echo                            888                                                                                        
echo                       Y8b d88P                                                                                        
echo                        "Y88P"                                                                                         
echo                                                                                                                 v1.0.3 
echo  ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
echo.

:check_git
:: =================================
:: Check for Git
:: =================================
where git >nul 2>&1
if %ERRORLEVEL%==0 (
    for /f "tokens=* delims=" %%A in ('git --version') do (
        echo  Git found:           %%A
        goto :check_cmake
    )
) else (
    echo  Git not found. Please download and install Git from:
    echo      https://git-scm.com/downloads/win
    pause
    exit /b 1
)

:check_cmake
:: =================================
:: Check for CMake
:: =================================
where cmake >nul 2>&1
if %ERRORLEVEL%==0 (
    for /f "tokens=* delims=" %%A in ('cmake --version') do (
        echo  CMake found:         %%A
        goto :check_cmakelists
    )
) else (
    echo  CMake not found. Please download and install CMake from:
    echo      https://cmake.org/download/
    pause
    exit /b 1
)

:check_cmakelists
:: =================================
:: Check for CMakeLists.txt
:: =================================
if exist "CMakeLists.txt" (
    set "foundTag=0"
    set "projectName="
    :: Read the file line by line
    for /f "usebackq tokens=* delims=" %%A in ("CMakeLists.txt") do (
        set "line=%%A"

        if !foundTag! == 1 (
            set "projectName=!line: =!"
            set "foundTag=0"
        )

        echo !line! | findstr /c:"# Must match folder name" >nul
        if !errorlevel! == 0 (
            set "foundTag=1"
        )
    )

    :: Output result
    if defined projectName (
        echo  Project found:       !projectName!
    ) else (
        echo  Project found:       None [WARNING] Unknown CMakeLists.txt, aborting.
        pause
        exit /b 1
    )
) else (
    echo  Project found:       None
)

:check_configuration
:: =================================
:: Check for previous configuration
:: =================================
if exist "build\vs2022-ng\CMakeCache.txt" (
    set CONFIG_PRESET=vs2022-ng
    goto :ask_regeneration
) else if exist "build\vs2022-po3\CMakeCache.txt" (
    set CONFIG_PRESET=vs2022-po3
    goto :ask_regeneration
)
:: =================================
:: Fallback: check CMakeLists.txt
:: =================================
if exist "CMakeLists.txt" (

    for /f "tokens=*" %%A in ('findstr /C:"set(COMMONLIB_VARIANT" CMakeLists.txt') do (
        set "line=%%A"
        setlocal EnableDelayedExpansion

        rem Strip the prefix
        set "line=!line:set(COMMONLIB_VARIANT =!"
        rem Strip parentheses and quotes
        set "line=!line:)=!"
        set "line=!line:"=!"
        rem Remove leading/trailing spaces
        for /f "tokens=* delims= " %%P in ("!line!") do (
            endlocal
            set "CONFIG_PRESET=%%P"
        )
    )
)

if defined CONFIG_PRESET (
    echo  Configuration found: %CONFIG_PRESET%
    goto :build_configuration
)

echo  Configuration found: None
goto :ask_configuration

:ask_regeneration
:: =================================
:: Select configuration regeneration
:: =================================
echo  Configuration found: %CONFIG_PRESET%
echo.
choice /M "Do you want to regenerate the configuration files? "
echo ----------------------------------------------------------
if errorlevel 2 (
    echo Skipping regeneration...
    goto :ask_build
) else if errorlevel 1 (
    echo Regenerating configuration files...
    goto :build_configuration
)

:ask_configuration
:: =================================
:: Select configuration preset
:: =================================
echo.
echo Select CommonLibSSE variant:
echo  1. powerof3
echo  2. alandtse
echo.

set /p variant_choice="Enter choice [1-2]: "

if "%variant_choice%"=="1" (
    set CONFIG_PRESET=vs2022-po3
) else if "%variant_choice%"=="2" (
    set CONFIG_PRESET=vs2022-ng
    set SKYRIM_SUPPORT_AE=ON
    goto :create_project
) else (
    echo Invalid choice. Defaulting to powerof3.
    set CONFIG_PRESET=vs2022-po3
)

echo.
echo Select SkyrimSE version:
echo  1. Latest
echo  2. 1.5.97
echo.

set /p skyrim_latest="Enter choice [1-2]: "

if "%skyrim_latest%"=="1" (
    set SKYRIM_SUPPORT_AE=ON
) else if "%skyrim_latest%"=="2" (
    set SKYRIM_SUPPORT_AE=OFF
) else (
    echo Invalid choice. Defaulting to Latest.
    set SKYRIM_SUPPORT_AE=ON
)

:create_project
echo.
echo  Creating a new project using %CONFIG_PRESET%
echo ----------------------------------------------------------
:ask_project_name
set /p PROJECT_NAME=Enter project name (no spaces): 
if "%PROJECT_NAME%"=="" (
    echo  Error: Project name cannot be empty.
    goto :ask_project_name
)

:ask_project_author
set /p PROJECT_AUTHOR=Enter author name: 
if "%PROJECT_AUTHOR%"=="" (
    echo  Error: Project author cannot be empty.
    goto :ask_project_author
)

echo.
echo  The following you can hit enter to set default values
echo -------------------------------------------------------
:ask_project_description
set /p PROJECT_DESCRIPTION=Enter description: 
if "%PROJECT_DESCRIPTION%"=="" (
    set PROJECT_DESCRIPTION=%PROJECT_NAME%
)

:ask_project_version
set /p PROJECT_VERSION=Enter version (e.g. 1.0): 
if "%PROJECT_VERSION%"=="" (
    set PROJECT_VERSION=1.0
)

:ask_project_year
set /p PROJECT_YEAR=Enter year (e.g. (C) 2024-2025): 
if "%PROJECT_YEAR%"=="" (
    set "PROJECT_YEAR=(C) 2025"
)

:ask_project_license
set /p PROJECT_LICENSE=Enter license (e.g. MIT): 
if "%PROJECT_LICENSE%"=="" (
    set PROJECT_LICENSE=MIT
)

:: =================================
:: Project templates
:: =================================
echo.
echo Select a template example for '%PROJECT_NAME%':
echo  1. Bare  (essentials to get up and running)
echo  2. Basic (console hook and event registration)
echo  3. Menu  (console hook and imgui menu)
echo.

set /p project_template="Enter choice [1-3]: "

if "%project_template%"=="1" (
    goto :bare_project
) else if "%project_template%"=="2" (
    goto :basic_project
) else if "%project_template%"=="3" (
    goto :menu_project
) else (
    echo Invalid choice. Defaulting to bare project.
    goto :bare_project
)

:bare_project
:: =================================
:: Bare Project
:: =================================
:: Main project folder
mkdir %PROJECT_NAME% >nul 2>&1

:: Subfolders
mkdir "%PROJECT_NAME%\include" >nul 2>&1
mkdir "%PROJECT_NAME%\include\api" >nul 2>&1
mkdir "%PROJECT_NAME%\include\util" >nul 2>&1
mkdir "%PROJECT_NAME%\src" >nul 2>&1
mkdir "%PROJECT_NAME%\src\api" >nul 2>&1
mkdir "%PROJECT_NAME%\resource" >nul 2>&1
mkdir "%PROJECT_NAME%\pch" >nul 2>&1

:: Copy specific files manually to target subfolders
copy /Y "cmake\templates\bare\include\plugin.h" "%PROJECT_NAME%\include\" >nul
copy /Y "cmake\templates\bare\include\api\skse_api.h" "%PROJECT_NAME%\include\api\" >nul
copy /Y "cmake\templates\bare\include\api\versionlibdb.h" "%PROJECT_NAME%\include\api\" >nul
copy /Y "cmake\templates\bare\include\util\LogUtil.h" "%PROJECT_NAME%\include\util\" >nul
copy /Y "cmake\templates\bare\include\util\StringUtil.h" "%PROJECT_NAME%\include\util\" >nul
copy /Y "cmake\templates\bare\pch\PCH.cpp" "%PROJECT_NAME%\pch\" >nul
copy /Y "cmake\templates\bare\pch\PCH.h" "%PROJECT_NAME%\pch\" >nul
copy /Y "cmake\templates\bare\resource\plugin.rc" "%PROJECT_NAME%\resource\" >nul
copy /Y "cmake\templates\bare\src\dllmain.cpp" "%PROJECT_NAME%\src\" >nul
copy /Y "cmake\templates\bare\src\plugin.cpp" "%PROJECT_NAME%\src\" >nul
copy /Y "cmake\templates\bare\src\api\skse_api.cpp" "%PROJECT_NAME%\src\api\" >nul
copy /Y "cmake\templates\.clang-format" "%PROJECT_NAME%\" >nul

set TEMPLATE_NAME=Bare
set INSTALL_IMGUI=OFF
goto :write_cmakelists

:basic_project
:: =================================
:: Basic Project
:: =================================
:: Main project folder
mkdir %PROJECT_NAME% >nul 2>&1

:: Subfolders
mkdir "%PROJECT_NAME%\include" >nul 2>&1
mkdir "%PROJECT_NAME%\include\api" >nul 2>&1
mkdir "%PROJECT_NAME%\include\console" >nul 2>&1
mkdir "%PROJECT_NAME%\include\game" >nul 2>&1
mkdir "%PROJECT_NAME%\include\hook" >nul 2>&1
mkdir "%PROJECT_NAME%\include\util" >nul 2>&1
mkdir "%PROJECT_NAME%\src" >nul 2>&1
mkdir "%PROJECT_NAME%\src\api" >nul 2>&1
mkdir "%PROJECT_NAME%\src\console" >nul 2>&1
mkdir "%PROJECT_NAME%\src\game" >nul 2>&1
mkdir "%PROJECT_NAME%\src\hook" >nul 2>&1
mkdir "%PROJECT_NAME%\resource" >nul 2>&1
mkdir "%PROJECT_NAME%\pch" >nul 2>&1

:: Copy specific files manually to target subfolders
copy /Y "cmake\templates\basic\include\plugin.h" "%PROJECT_NAME%\include\" >nul
copy /Y "cmake\templates\basic\include\api\skse_api.h" "%PROJECT_NAME%\include\api\" >nul
copy /Y "cmake\templates\basic\include\api\versionlibdb.h" "%PROJECT_NAME%\include\api\" >nul
copy /Y "cmake\templates\basic\include\console\ConsoleCommands.h" "%PROJECT_NAME%\include\console\" >nul
copy /Y "cmake\templates\basic\include\game\GameEvents.h" "%PROJECT_NAME%\include\game\" >nul
copy /Y "cmake\templates\basic\include\game\GameHelper.h" "%PROJECT_NAME%\include\game\" >nul
copy /Y "cmake\templates\basic\include\hook\FunctionHook.h" "%PROJECT_NAME%\include\hook\" >nul
copy /Y "cmake\templates\basic\include\hook\InputHook.h" "%PROJECT_NAME%\include\hook\" >nul
copy /Y "cmake\templates\basic\include\hook\MainHook.h" "%PROJECT_NAME%\include\hook\" >nul
copy /Y "cmake\templates\basic\include\util\LogUtil.h" "%PROJECT_NAME%\include\util\" >nul
copy /Y "cmake\templates\basic\include\util\StringUtil.h" "%PROJECT_NAME%\include\util\" >nul
copy /Y "cmake\templates\basic\pch\PCH.cpp" "%PROJECT_NAME%\pch\" >nul
copy /Y "cmake\templates\basic\pch\PCH.h" "%PROJECT_NAME%\pch\" >nul
copy /Y "cmake\templates\basic\resource\plugin.rc" "%PROJECT_NAME%\resource\" >nul
copy /Y "cmake\templates\basic\src\dllmain.cpp" "%PROJECT_NAME%\src\" >nul
copy /Y "cmake\templates\basic\src\plugin.cpp" "%PROJECT_NAME%\src\" >nul
copy /Y "cmake\templates\basic\src\api\skse_api.cpp" "%PROJECT_NAME%\src\api\" >nul
copy /Y "cmake\templates\basic\src\console\ConsoleCommands.cpp" "%PROJECT_NAME%\src\console\" >nul
copy /Y "cmake\templates\basic\src\game\GameEvents.cpp" "%PROJECT_NAME%\src\game\" >nul
copy /Y "cmake\templates\basic\src\hook\FunctionHook.cpp" "%PROJECT_NAME%\src\hook\" >nul
copy /Y "cmake\templates\basic\src\hook\InputHook.cpp" "%PROJECT_NAME%\src\hook\" >nul
copy /Y "cmake\templates\basic\src\hook\MainHook.cpp" "%PROJECT_NAME%\src\hook\" >nul
copy /Y "cmake\templates\.clang-format" "%PROJECT_NAME%\" >nul

set TEMPLATE_NAME=Basic
set INSTALL_IMGUI=OFF
goto :write_cmakelists

:menu_project
:: =================================
:: Menu Project
:: =================================
:: Main project folder
mkdir %PROJECT_NAME% >nul 2>&1

:: Subfolders
mkdir "%PROJECT_NAME%\include" >nul 2>&1
mkdir "%PROJECT_NAME%\include\api" >nul 2>&1
mkdir "%PROJECT_NAME%\include\console" >nul 2>&1
mkdir "%PROJECT_NAME%\include\hook" >nul 2>&1
mkdir "%PROJECT_NAME%\include\input" >nul 2>&1
mkdir "%PROJECT_NAME%\include\menu" >nul 2>&1
mkdir "%PROJECT_NAME%\include\menu\fonts" >nul 2>&1
mkdir "%PROJECT_NAME%\include\menu\menus" >nul 2>&1
mkdir "%PROJECT_NAME%\include\util" >nul 2>&1
mkdir "%PROJECT_NAME%\src" >nul 2>&1
mkdir "%PROJECT_NAME%\src\api" >nul 2>&1
mkdir "%PROJECT_NAME%\src\console" >nul 2>&1
mkdir "%PROJECT_NAME%\src\hook" >nul 2>&1
mkdir "%PROJECT_NAME%\src\input" >nul 2>&1
mkdir "%PROJECT_NAME%\src\menu" >nul 2>&1
mkdir "%PROJECT_NAME%\src\menu\menus" >nul 2>&1
mkdir "%PROJECT_NAME%\resource" >nul 2>&1
mkdir "%PROJECT_NAME%\pch" >nul 2>&1

:: Copy specific files manually to target subfolders
copy /Y "cmake\templates\menu\include\plugin.h" "%PROJECT_NAME%\include\" >nul
copy /Y "cmake\templates\menu\include\api\skse_api.h" "%PROJECT_NAME%\include\api\" >nul
copy /Y "cmake\templates\menu\include\api\versionlibdb.h" "%PROJECT_NAME%\include\api\" >nul
copy /Y "cmake\templates\menu\include\console\ConsoleCommands.h" "%PROJECT_NAME%\include\console\" >nul
copy /Y "cmake\templates\menu\include\hook\GraphicsHook.h" "%PROJECT_NAME%\include\hook\" >nul
copy /Y "cmake\templates\menu\include\hook\InputHook.h" "%PROJECT_NAME%\include\hook\" >nul
copy /Y "cmake\templates\menu\include\hook\MainHook.h" "%PROJECT_NAME%\include\hook\" >nul
copy /Y "cmake\templates\menu\include\input\InputManager.h" "%PROJECT_NAME%\include\input\" >nul
copy /Y "cmake\templates\menu\include\menu\fonts\fa-solid-900.h" "%PROJECT_NAME%\include\menu\fonts\" >nul
copy /Y "cmake\templates\menu\include\menu\fonts\IconsFontAwesome6Menu.h" "%PROJECT_NAME%\include\menu\fonts\" >nul
copy /Y "cmake\templates\menu\include\menu\menus\PlayerMenu.h" "%PROJECT_NAME%\include\menu\menus\" >nul
copy /Y "cmake\templates\menu\include\menu\ConsoleMenu.h" "%PROJECT_NAME%\include\menu\" >nul
copy /Y "cmake\templates\menu\include\menu\IMenu.h" "%PROJECT_NAME%\include\menu\" >nul
copy /Y "cmake\templates\menu\include\menu\ImguiHelper.h" "%PROJECT_NAME%\include\menu\" >nul
copy /Y "cmake\templates\menu\include\menu\MainMenu.h" "%PROJECT_NAME%\include\menu\" >nul
copy /Y "cmake\templates\menu\include\menu\Menus.h" "%PROJECT_NAME%\include\menu\" >nul
copy /Y "cmake\templates\menu\include\menu\MenuManager.h" "%PROJECT_NAME%\include\menu\" >nul
copy /Y "cmake\templates\menu\include\util\KeyboardUtil.h" "%PROJECT_NAME%\include\util\" >nul
copy /Y "cmake\templates\menu\include\util\LogUtil.h" "%PROJECT_NAME%\include\util\" >nul
copy /Y "cmake\templates\menu\include\util\StringUtil.h" "%PROJECT_NAME%\include\util\" >nul
copy /Y "cmake\templates\menu\pch\PCH.cpp" "%PROJECT_NAME%\pch\" >nul
copy /Y "cmake\templates\menu\pch\PCH.h" "%PROJECT_NAME%\pch\" >nul
copy /Y "cmake\templates\menu\resource\plugin.rc" "%PROJECT_NAME%\resource\" >nul
copy /Y "cmake\templates\menu\src\dllmain.cpp" "%PROJECT_NAME%\src\" >nul
copy /Y "cmake\templates\menu\src\plugin.cpp" "%PROJECT_NAME%\src\" >nul
copy /Y "cmake\templates\menu\src\api\skse_api.cpp" "%PROJECT_NAME%\src\api\" >nul
copy /Y "cmake\templates\menu\src\console\ConsoleCommands.cpp" "%PROJECT_NAME%\src\console\" >nul
copy /Y "cmake\templates\menu\src\hook\GraphicsHook.cpp" "%PROJECT_NAME%\src\hook\" >nul
copy /Y "cmake\templates\menu\src\hook\InputHook.cpp" "%PROJECT_NAME%\src\hook\" >nul
copy /Y "cmake\templates\menu\src\hook\MainHook.cpp" "%PROJECT_NAME%\src\hook\" >nul
copy /Y "cmake\templates\menu\src\input\InputManager.cpp" "%PROJECT_NAME%\src\input\" >nul
copy /Y "cmake\templates\menu\src\menu\menus\PlayerMenu.cpp" "%PROJECT_NAME%\src\menu\menus\" >nul
copy /Y "cmake\templates\menu\src\menu\ConsoleMenu.cpp" "%PROJECT_NAME%\src\menu\" >nul
copy /Y "cmake\templates\menu\src\menu\IMenu.cpp" "%PROJECT_NAME%\src\menu\" >nul
copy /Y "cmake\templates\menu\src\menu\ImguiHelper.cpp" "%PROJECT_NAME%\src\menu\" >nul
copy /Y "cmake\templates\menu\src\menu\MainMenu.cpp" "%PROJECT_NAME%\src\menu\" >nul
copy /Y "cmake\templates\menu\src\menu\MenuManager.cpp" "%PROJECT_NAME%\src\menu\" >nul
copy /Y "cmake\templates\.clang-format" "%PROJECT_NAME%\" >nul

set TEMPLATE_NAME=Menu
set INSTALL_IMGUI=ON
goto :write_cmakelists

:write_cmakelists
:: =================================
:: Write CMakeLists.txt
:: =================================
echo.
echo  Creating project '%PROJECT_NAME%' with %TEMPLATE_NAME% template
echo -------------------------------------------------------
set CMakeLists_Line1=# Mimimum CMake version
set CMakeLists_Line2=cmake_minimum_required(VERSION 3.24.0)
set CMakeLists_Line3=# =========================
set CMakeLists_Line4=# Project Configuration
set CMakeLists_Line5=# =========================
set CMakeLists_Line6=project(
set CMakeLists_Line7=    # Must match folder name
set CMakeLists_Line8=    %PROJECT_NAME%
set CMakeLists_Line9=    DESCRIPTION "%PROJECT_DESCRIPTION%"
set CMakeLists_Line10=    VERSION     %PROJECT_VERSION%
set CMakeLists_Line11=    LANGUAGES   CXX
set CMakeLists_Line12=)
set CMakeLists_Line13=# =========================
set CMakeLists_Line14=# Metadata / Licensing
set CMakeLists_Line15=# =========================
set CMakeLists_Line16=set(PROJECT_AUTHOR  "%PROJECT_AUTHOR%")
set CMakeLists_Line17=set(PROJECT_YEAR    "%PROJECT_YEAR%")
set CMakeLists_Line18=set(PROJECT_LICENSE "%PROJECT_LICENSE%")
set CMakeLists_Line19=# =========================
set CMakeLists_Line20=# CMake Module Path
set CMakeLists_Line21=# =========================
set CMakeLists_Line22=list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set CMakeLists_Line23=# =========================
set CMakeLists_Line24=# Project Modules
set CMakeLists_Line25=# =========================
set CMakeLists_Line26=include(utility_config)
set CMakeLists_Line27=include(project_config)
set CMakeLists_Line28=# =========================
set CMakeLists_Line29=# CommonLibSSE Variant
set CMakeLists_Line30=# =========================
set CMakeLists_Line31=set(COMMONLIB_VARIANT "%CONFIG_PRESET%")
set CMakeLists_Line32=option(SKYRIM_SUPPORT_AE "Enables support for Skyrim AE" %SKYRIM_SUPPORT_AE%)
set CMakeLists_Line33=option(INSTALL_IMGUI "Installs imgui framework" %INSTALL_IMGUI%)
set CMakeLists_Line34=# =========================
set CMakeLists_Line35=# Submodules / Dependencies
set CMakeLists_Line36=# =========================
set CMakeLists_Line37=include(submodule_config)

> CMakeLists.txt echo !CMakeLists_Line1!
>> CMakeLists.txt echo !CMakeLists_Line2!
>> CMakeLists.txt echo.
>> CMakeLists.txt echo !CMakeLists_Line3!
>> CMakeLists.txt echo !CMakeLists_Line4!
>> CMakeLists.txt echo !CMakeLists_Line5!
>> CMakeLists.txt echo !CMakeLists_Line6!
>> CMakeLists.txt echo !CMakeLists_Line7!
>> CMakeLists.txt echo !CMakeLists_Line8!
>> CMakeLists.txt echo !CMakeLists_Line9!
>> CMakeLists.txt echo !CMakeLists_Line10!
>> CMakeLists.txt echo !CMakeLists_Line11!
>> CMakeLists.txt echo !CMakeLists_Line12!
>> CMakeLists.txt echo.
>> CMakeLists.txt echo !CMakeLists_Line13!
>> CMakeLists.txt echo !CMakeLists_Line14!
>> CMakeLists.txt echo !CMakeLists_Line15!
>> CMakeLists.txt echo !CMakeLists_Line16!
>> CMakeLists.txt echo !CMakeLists_Line17!
>> CMakeLists.txt echo !CMakeLists_Line18!
>> CMakeLists.txt echo.
>> CMakeLists.txt echo !CMakeLists_Line19!
>> CMakeLists.txt echo !CMakeLists_Line20!
>> CMakeLists.txt echo !CMakeLists_Line21!
>> CMakeLists.txt echo !CMakeLists_Line22!
>> CMakeLists.txt echo.
>> CMakeLists.txt echo !CMakeLists_Line23!
>> CMakeLists.txt echo !CMakeLists_Line24!
>> CMakeLists.txt echo !CMakeLists_Line25!
>> CMakeLists.txt echo !CMakeLists_Line26!
>> CMakeLists.txt echo !CMakeLists_Line27!
>> CMakeLists.txt echo.
>> CMakeLists.txt echo !CMakeLists_Line28!
>> CMakeLists.txt echo !CMakeLists_Line29!
>> CMakeLists.txt echo !CMakeLists_Line30!
>> CMakeLists.txt echo !CMakeLists_Line31!
>> CMakeLists.txt echo !CMakeLists_Line32!
>> CMakeLists.txt echo !CMakeLists_Line33!
>> CMakeLists.txt echo.
>> CMakeLists.txt echo !CMakeLists_Line34!
>> CMakeLists.txt echo !CMakeLists_Line35!
>> CMakeLists.txt echo !CMakeLists_Line36!
>> CMakeLists.txt echo !CMakeLists_Line37!

:: =================================
:: Write .gitignore
:: =================================
set GitIgnore_Line1=# Ignore build artifacts and binary outputs
set GitIgnore_Line2=/.bin/
set GitIgnore_Line3=/.lib/
set GitIgnore_Line4=/build/
set GitIgnore_Line5=# Ignore extern directory inside %PROJECT_NAME%
set GitIgnore_Line6=/%PROJECT_NAME%/extern/

> .gitignore echo !GitIgnore_Line1!
>> .gitignore echo !GitIgnore_Line2!
>> .gitignore echo !GitIgnore_Line3!
>> .gitignore echo !GitIgnore_Line4!
>> .gitignore echo.
>> .gitignore echo !GitIgnore_Line5!
>> .gitignore echo !GitIgnore_Line6!

:build_configuration
:: =================================
:: Build configuration
:: =================================
echo.
echo [INFO] Configuring with preset: %CONFIG_PRESET%...
cmake --preset %CONFIG_PRESET%
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed with preset %CONFIG_PRESET%.
    pause
    exit /b 1
)
echo.
echo [SUCCESS] Configuration complete.

:ask_build
:: =================================
:: Select build type
:: =================================
echo.
echo Select build type for %CONFIG_PRESET%:
echo  1. No build
echo  2. RelWithDebInfo
echo  3. Debug
echo.
set /p build_choice="Enter build option [1-3]: "

if "%build_choice%"=="1" (
    exit /b 0
)

if "%CONFIG_PRESET%"=="vs2022-ng" (
    if "%build_choice%"=="2" (
        set BUILD_PRESET=vs2022-ng-rel
    ) else if "%build_choice%"=="3" (
        set BUILD_PRESET=vs2022-ng-debug
    ) else (
        echo Invalid choice. Defaulting to No build.
        pause
        exit /b 0
    )
) else if "%CONFIG_PRESET%"=="vs2022-po3" (
    if "%build_choice%"=="2" (
        set BUILD_PRESET=vs2022-po3-rel
    ) else if "%build_choice%"=="3" (
        set BUILD_PRESET=vs2022-po3-debug
    ) else (
        echo Invalid choice. Defaulting to No build.
        pause
        exit /b 0
    )
)

:: =================================
:: Clean build
:: =================================
choice /M "Clean build? "
if errorlevel 2 (
    set CLEAN=
) else if errorlevel 1 (
    set CLEAN=--clean-first
)

:: =================================
:: Run build
:: =================================
echo.
echo [INFO] Building with preset: %BUILD_PRESET%
cmake --build --preset %BUILD_PRESET% %CLEAN%
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Build completed successfully.
pause
exit /b 0
