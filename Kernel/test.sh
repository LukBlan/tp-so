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

if [ "$1" == "BASE" -a "$2" == "FIFO" ]; then 
	./Debug/Kernel  kernelbase.config
fi

if [ "$1" == "BASE" -a "$2" == "HRRN" ]; then  
	./Debug/Kernel  kernelbaseHRRN.config
fi

if [ "$1" == "MEMORIA" ]; then 
	./Debug/Kernel  kernel-memoria.config
fi

if [ "$1" == "ERROR" ]; then 
	./Debug/Kernel  kernelerror.config
fi

if [ "$1" == "FS" ]; then 
	./Debug/Kernel  kernel-fileSystem.config
fi

if [ "$1" == "DEADLOCK" ]; then 
	./Debug/Kernel  kerneldeadlock.config
fi


