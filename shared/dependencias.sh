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
	sudo make install
	cd ..
	
	cd $cwd
	cd Debug
	make clean
	make all
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
