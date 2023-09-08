@echo off
@REM in/00xx.txtを読み込んでout/00xx.txtに出力する
setlocal enabledelayedexpansion
 
for /l %%a in (0,1,99) do (
  set num=000%%a
  set num=!num:~-4,4!
  call AHC023-4.exe /c "in\!num!.txt" "out\!num!.txt"
 )
 
endlocal

pause