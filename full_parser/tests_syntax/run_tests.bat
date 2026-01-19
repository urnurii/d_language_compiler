@echo off
setlocal enabledelayedexpansion

REM Путь к MSYS2
set MSYS2_PATH=C:\msys64

REM Проверяем наличие MSYS2
if not exist "%MSYS2_PATH%\mingw64\bin" (
    echo Error: MSYS2 not found at %MSYS2_PATH%
    pause
    exit /b 1
)

REM Добавляем MSYS2 в PATH
set PATH=%MSYS2_PATH%\mingw64\bin;%MSYS2_PATH%\usr\bin;%PATH%

set COMPILER=.\compiler.exe
set GRAPHVIZ_BIN=%MSYS2_PATH%\mingw64\bin

REM Проверяем наличие graphviz в MSYS2
if not exist "%GRAPHVIZ_BIN%\dot.exe" (
    echo Error: Graphviz not found in MSYS2
    echo Install it with: pacman -S mingw-w64-x86_64-graphviz
    pause
    exit /b 1
)

REM Создаём папку для результатов
if not exist "test_results" mkdir test_results

echo ========================================
echo Starting test suite
echo ========================================
echo.

set PASSED=0
set FAILED=0

REM Ищем все тестовые файлы
for %%F in (test_*.d) do (
    set TEST_NAME=%%~nF
    echo Processing !TEST_NAME!...
    
    REM Компилируем и генерируем AST
    %COMPILER% "%%F" > "test_results\!TEST_NAME!.log" 2>&1
    
    REM Проверяем наличие ast.dot
    if exist "ast.dot" (
        REM Переименовываем и перемещаем dot файл
        move ast.dot "test_results\!TEST_NAME!.dot" >nul
        
        REM Генерируем SVG из dot файла
        "%GRAPHVIZ_BIN%\dot.exe" -Tsvg "test_results\!TEST_NAME!.dot" -o "test_results\!TEST_NAME!.svg"
        
        echo  [PASS] !TEST_NAME!
        set /a PASSED+=1
    ) else (
        echo  [FAIL] !TEST_NAME!
        set /a FAILED+=1
    )
)

echo.
echo ========================================
echo Passed: !PASSED!
echo Failed: !FAILED!
echo ========================================
echo.

pause
