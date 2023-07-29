#!/usr/bin/env bash

cwd=$PWD

if [ "$1" == "" ]; then
	echo "Error de ejecucion: ./deploy.sh <install | uninstall>"
	exit 1
fi

if [ "$1" == "install" ]; then
	cd ~
	mkdir so-libraries
	cd so-libraries
	git clone https://github.com/sisoputnfrba/so-commons-library.git
	cd so-commons-library
	make debug
	sudo make install
	
	
	cd $cwd
	cd ./../Memoria
	chmod u+x test.sh
	cd ./../FileSystem
	chmod u+x test.sh
	cd ./../Kernel
	chmod u+x test.sh
	cd ./../Cpu
	chmod u+x test.sh
	cd ./../Consola
	chmod u+x test.sh
		
	cd $cwd
	sudo cp -r ./include/* /usr/local/include
	cd $cwd
	cd Debug
	make clean
	make all
	sudo cp libshared.so /usr/local/lib/
fi

if [ "$1" == "uninstall" ]; then
	cd Debug
	make clean
	cd ..
	cd ..
	cd ..
	sudo rm -r tp-2023-1c-Pneumonoultramicroscopicsilicovolcanoconiosis-
	cd ~
	sudo rm -r so-libraries
fi
