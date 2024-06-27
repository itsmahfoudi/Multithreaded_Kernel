#/bin/bash
export PREFIX="$HOME/opt/cross/"
export TARGET=i686-elf 
export PATH="$PREFIX/bin:$PATH"
mkdir build build/memory build/idt
mkdir bin
make all

