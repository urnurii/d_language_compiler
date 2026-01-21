@echo off
setlocal
for %%F in (*.d) do (
    echo ==== %%F ====
    ..\..\compiler.exe "%%F"
)
endlocal
