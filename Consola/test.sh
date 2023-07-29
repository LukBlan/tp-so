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
    ./Debug/Consola consola.config BASE_1
    ./Debug/Consola consola.config BASE_2
    ./Debug/Consola consola.config BASE_2
fi

if [ "$1" == "MEMORIA"]; then 
    ./Debug/Consola consola.config MEMORIA_1
    ./Debug/Consola consola.config MEMORIA_2
    ./Debug/Consola consola.config MEMORIA_3
fi

if [ "$1" == "ERROR" -a "$2" == "1" ]; then 
    ./Debug/Consola consola.config ERROR_1
fi

if [ "$1" == "ERROR" -a "$2" == "2" ]; then 
    ./Debug/Consola consola.config ERROR_2
fi

if [ "$1" == "ERROR" -a "$2" == "3" ]; then 
    ./Debug/Consola consola.config ERROR_3
fi

if [ "$1" == "ERROR" -a "$2" == "4" ]; then 
    ./Debug/Consola consola.config ERROR_4
fi

if [ "$1" == "FS" -a "$2" == "1" ]; then 
    ./Debug/Consola consola.config FS_1
    ./Debug/Consola consola.config FS_2
fi

if [ "$1" == "FS" -a "$2" == "2"  ]; then 
    ./Debug/Consola consola.config FS_3
    ./Debug/Consola consola.config FS_3
    ./Debug/Consola consola.config FS_3
    ./Debug/Consola consola.config FS_3
fi

if [ "$1" == "DEADLOCK" -a "$2" == "1"]; then 
    ./Debug/Consola consola.config DEADLOCK_1
    ./Debug/Consola consola.config DEADLOCK_2
    ./Debug/Consola consola.config DEADLOCK_3
fi

if [ "$1" == "DEADLOCK" -a "$2" == "2"]; then 
    ./Debug/Consola consola.config DEADLOCK_4
fi
