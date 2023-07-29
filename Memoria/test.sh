#!/usr/bin/env bash

if [ "$1" == "" ]; then
	echo "Error: No se ingresaron comandos"
	exit 1
fi

cwd=$PWD

cd Debug
make clean
make all

cd ..

export LD_LIBRARY_PATH=/home/utnso/Desktop/tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-/shared/Debug

if [ "$1" == "BASE" ]; then 
	./Debug/Memoria  memoriabase.config
fi
if [ "$1" == "MEMORIA" -a "$2" == "FIRST" ]; then 
	./Debug/Memoria  memoria-memoria-first.config
fi

if [ "$1" == "MEMORIA" -a "$2" == "WORST" ]; then 
	./Debug/Memoria  memoria-memoria-worst.config
fi

if [ "$1" == "MEMORIA" -a "$2" == "BEST" ]; then 
	./Debug/Memoria  memoria-memoria-best.config
fi

if [ "$1" == "ERROR" ]; then 
	./Debug/Memoria  memoriaerror.config
fi

if [ "$1" == "FS" ]; then 
	./Debug/Memoria  memoria-fileSystem.config
fi

if [ "$1" == "DEADLOCK" ]; then 
	./Debug/Memoria  memoriadeadlock.config
fi


