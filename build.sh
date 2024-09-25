#!/bin/bash
export PREFIX="$HOME/opt/cross/"
export TARGET=i686-elf 
export PATH="$PREFIX/bin:$PATH"
mkdir build build/memory build/idt build/io build/memory/heap
mkdir bin
make all

