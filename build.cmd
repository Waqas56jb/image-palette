@echo off

set PATH=C:\Work\auto2\msys64\mingw64\bin;%PATH%

cd zlib
gcc -Ofast -DNDEBUG -c *.c
ar rcs libz.a adler32.o crc32.o deflate.o inffast.o inflate.o inftrees.o trees.o zutil.o
cd..

g++ main.cpp lodepng.cpp zlib/libz.a -Ofast -Os -s -static -Wl,-flto,--gc-sections -flto-partition=none -o example.exe
pause