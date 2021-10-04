:loop
del *walied*
SimpleNtSyscallFuzzer.exe /maxcount 5000
goto loop
taskkill /im calc.exe /f