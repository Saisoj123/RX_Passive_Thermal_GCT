@echo off
REM Build and upload script for RX Servant ESP32
REM Usage: build_rx.bat [1|2|3|4|debug]

echo ========================================
echo  RX Passive Thermal GCT Build Script
echo ========================================

if "%1"=="1" (
    echo Building for GCT Device 1...
    pio run -e rx-gct1
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Build successful! Upload now? [Y/N]
        set /p confirm=
        if /i "%confirm%"=="Y" (
            echo.
            echo Uploading to ESP32...
            pio run -e rx-gct1 --target upload
            echo Upload command completed with exit code: %ERRORLEVEL%
            if %ERRORLEVEL% EQU 0 (
                echo Upload successful!
            ) else (
                echo Upload FAILED! Check ESP32 connection and COM port.
            )
        ) else (
            echo Upload skipped.
        )
    ) else (
        echo.
        echo Build FAILED! Error code: %ERRORLEVEL%
        goto show_help
    )
) else if "%1"=="2" (
    echo Building for GCT Device 2...
    pio run -e rx-gct2
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Build successful! Upload now? [Y/N]
        set /p confirm=
        if /i "%confirm%"=="Y" (
            echo.
            echo Uploading to ESP32...
            pio run -e rx-gct2 --target upload
            echo Upload command completed with exit code: %ERRORLEVEL%
            if %ERRORLEVEL% EQU 0 (
                echo Upload successful!
            ) else (
                echo Upload FAILED! Check ESP32 connection and COM port.
            )
        ) else (
            echo Upload skipped.
        )
    ) else (
        echo.
        echo Build FAILED! Error code: %ERRORLEVEL%
        goto show_help
    )
) else if "%1"=="3" (
    echo Building for GCT Device 3...
    pio run -e rx-gct3
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Build successful! Upload now? [Y/N]
        set /p confirm=
        if /i "%confirm%"=="Y" (
            echo.
            echo Uploading to ESP32...
            pio run -e rx-gct3 --target upload
            echo Upload command completed with exit code: %ERRORLEVEL%
            if %ERRORLEVEL% EQU 0 (
                echo Upload successful!
            ) else (
                echo Upload FAILED! Check ESP32 connection and COM port.
            )
        ) else (
            echo Upload skipped.
        )
    ) else (
        echo.
        echo Build FAILED! Error code: %ERRORLEVEL%
        goto show_help
    )
) else if "%1"=="4" (
    echo Building for GCT Device 4...
    pio run -e rx-gct4
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Build successful! Upload now? [Y/N]
        set /p confirm=
        if /i "%confirm%"=="Y" (
            echo.
            echo Uploading to ESP32...
            pio run -e rx-gct4 --target upload
            echo Upload command completed with exit code: %ERRORLEVEL%
            if %ERRORLEVEL% EQU 0 (
                echo Upload successful!
            ) else (
                echo Upload FAILED! Check ESP32 connection and COM port.
            )
        ) else (
            echo Upload skipped.
        )
    ) else (
        echo.
        echo Build FAILED! Error code: %ERRORLEVEL%
        goto show_help
    )
) else if "%1"=="debug" (
    echo Building DEBUG version...
    pio run -e rx-servant-debug
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Build successful! Upload now? [Y/N]
        set /p confirm=
        if /i "%confirm%"=="Y" (
            echo.
            echo Uploading to ESP32...
            pio run -e rx-servant-debug --target upload
            echo Upload command completed with exit code: %ERRORLEVEL%
            if %ERRORLEVEL% EQU 0 (
                echo Upload successful!
            ) else (
                echo Upload FAILED! Check ESP32 connection and COM port.
            )
        ) else (
            echo Upload skipped.
        )
    ) else (
        echo.
        echo Build FAILED! Error code: %ERRORLEVEL%
        goto show_help
    )
) else (
    echo Usage: build_rx.bat [1^|2^|3^|4^|debug]
    echo.
    echo   1-4    : Build for specific GCT device ID
    echo   debug  : Build debug version
    echo.
    echo Examples:
    echo   build_rx.bat 1      - Build for GCT Device 1
    echo   build_rx.bat debug  - Build debug version
    goto end
)

goto end

:show_help
echo.
echo Common issues:
echo - PlatformIO not installed: Install PlatformIO Core or VS Code extension
echo - ESP32 not connected: Check USB connection
echo - Wrong COM port: Check device manager

echo.
echo Done!
:end
pause
