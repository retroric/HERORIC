@ECHO OFF


::
:: Initial check.
:: Verify if the SDK is correctly configured
::
rem IF "%OSDK%"=="" GOTO ErCfg
IF "%OSDK%"=="" SET OSDK=V:\EMULATION\ORIC\OSDK\OSDK_1_5

::
:: Set the build paremeters
::
CALL config.bat


::
:: Launch the compilation of files
::
CALL %OSDK%\bin\make.bat %OSDKFILE%
GOTO End


::
:: Outputs an error message
::
:ErCfg
ECHO == ERROR ==
ECHO The Oric SDK was not configured properly
ECHO You should have a OSDK environment variable setted to the location of the SDK
IF "%OSDKBRIEF%"=="" PAUSE
GOTO End


:End
