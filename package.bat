@echo off
setlocal

set BUILD_DIR=build
set PACKAGE_DIR=Desolate-Package
set CONFIG=Release

echo Building %CONFIG%...
cmake --build %BUILD_DIR% --config %CONFIG% >nul
if %errorlevel% neq 0 (
    echo Build failed!
    exit /b 1
)

echo Creating portable package...
if exist %PACKAGE_DIR% rmdir /s /q %PACKAGE_DIR%
mkdir %PACKAGE_DIR%

copy %BUILD_DIR%\bin\%CONFIG%\main.exe %PACKAGE_DIR%\ >nul
xcopy resources %PACKAGE_DIR%\resources\ /e /i /q >nul

echo.
echo Package created in %PACKAGE_DIR%/
dir %PACKAGE_DIR%\main.exe
echo.
echo To distribute: zip the %PACKAGE_DIR% folder.
