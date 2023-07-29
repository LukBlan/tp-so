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
	./Debug/FileSystem  fileSystem.config
fi

if [ "$1" == "FILESYSTEM" ]; then 
	./Debug/FileSystem  fileSystem-fileSystem.config
fi

if [ "$1" == "ERROR" ]; then 
	./Debug/FileSystem  filesystemerror.config
fi

if [ "$1" == "FS" ]; then 
	echo "FS"
fi

if [ "$1" == "DEADLOCK" ]; then 
	./Debug/FileSystem  filesydeadlock.config
fi


