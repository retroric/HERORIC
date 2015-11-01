@ECHO OFF

::
:: Set the build paremeters:
:: =========================
::
:: Target address for generated code
::
SET OSDKADDR=$800
::
:: Project name (and name of .TAP file)
::
SET OSDKNAME=HERO
::
:: List of input files 
:: (".c" or ".s" extension will be found automagically)
::
SET OSDKFILE=hero
