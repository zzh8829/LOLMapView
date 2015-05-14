@echo off
call :treeProcess
goto :eof

:treeProcess
rem Do whatever you want here over the files of this subdir, for example:

for %%f in (*.luaobj) do (
    set lua=%%f.lua
    echo %lua%
    
)
for /D %%d in (*) do (
    cd %%d
    call :treeProcess
    cd ..
)
exit /b