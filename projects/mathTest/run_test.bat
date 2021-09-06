@echo off
cmake -S %cd% -B %cd%/build
cmake --build %cd%/build --target math-lib
%cd%/bin/math-lib.exe
pause