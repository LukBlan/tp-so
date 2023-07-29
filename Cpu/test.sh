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
	./Debug/Cpu  cpubase.config
fi

if [ "$1" == "MEMORIA" ]; then 
	./Debug/Cpu  cpu-memoria.config
fi

if [ "$1" == "ERROR" ]; then 
	./Debug/Cpu  cpuerror.config
fi

if [ "$1" == "FS" ]; then 
	./Debug/Cpu  cpu-fileSystem.config
fi

if [ "$1" == "DEADLOCK" ]; then 
	./Debug/Cpu  cpudeadlock.config
fi


