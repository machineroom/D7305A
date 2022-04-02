REM Auxilliary DOS batch file to install the INMOS occam 2 Toolset
REM 
REM Copyright (C) INMOS Ltd, 1990, 1991, 1992, 1993

REM %1 = source, now in d7305a destination directory

COPY %1:\disk1.exe

ECHO Insert disk 2
PAUSE

VOL %1: > NUL

COPY %1:\disk2.exe

ECHO Insert disk 3
PAUSE

vol %1: > NUL

COPY %1:\disk3.exe

ECHO Unpacking the archive
disk1 -d
del disk1.exe
disk2 -d
del disk2.exe
disk3 -d
del disk3.exe

ECHO Installation complete
ECHO You may delete install2.bat



