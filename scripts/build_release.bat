@echo off

set QT_DIR=D:\Qt\Qt5.4.2\5.4\msvc2013_opengl
set SRC_DIR=%cd%
set BUILD_DIR=%cd%\build

if not exist %QT_DIR% exit
if not exist %SRC_DIR% exit
if not exist %BUILD_DIR% md %BUILD_DIR%

cd build

call "C:\Program Files (x86)\Microsoft Visual Studio\2013_opengl\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

%QT_DIR%\bin\qmake.exe %SRC_DIR%\qtest.pro -spec win32-msvc  "CONFIG+=release"
if exist %BUILD_DIR%\release\qtest.exe del %BUILD_DIR%\release\qtest.exe
nmake Release
if not exist %BUILD_DIR%\release\Qt5Core.dll (
  %QT_DIR%\bin\windeployqt.exe %BUILD_DIR%\release\qtest.exe
)