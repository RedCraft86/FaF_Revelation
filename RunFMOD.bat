@ECHO OFF
if "%~1"=="" (
    echo [ERROR] FMOD project name not provided.
    exit /b 1
)
START "" "%~dp0\FMODProject\%~1_FMOD.fspro"