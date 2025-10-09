@echo off
setlocal enabledelayedexpansion

set files=test1.d test2.d test3.d test4.d test5.d test6.d test7.d test8.d test9.d test10.d test11.d test12.d test13.d test14.d test15.d test16.d test17.d test18.d

set counter=1
for %%f in (%files%) do (
    LexerTest.exe %%f > result!counter!.txt
    set /a counter+=1
)

echo The end!
pause