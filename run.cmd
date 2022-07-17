@echo off
chcp 65001
mode 40,30
cls

echo [building...]
call :build
echo [starting...]
call :start
exit /b 0

:start (
  : call lisch /type:select /format:table "Open" "Close" "" "Exit"
  : call vecho /end:pointer [4]Hola: 
  : echo ola

  call buffer
)


:build (
  : g++ lisch\bs-compatibility.cpp -o lisch -std=c++0x
  : g++ vecho\main.cpp -o vecho -std=c++0x
  g++ buffer.cpp -o buffer -std=c++0x

  exit /b 0
)
