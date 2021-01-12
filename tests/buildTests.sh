#!/usr/bin/env bash
printf "catch.hpp exists? "
if [ -a catch.hpp ]; then
	printf "Yes.\n"
else
	printf "No.\n"
	wget https://github.com/catchorg/Catch2/releases/download/v2.13.0/catch.hpp
fi
pushd ..
	printf "libBMP.a exists? "
	if [ -a ./bin/libBMP.a ]; then
		printf "Yes.\n"
	else
		printf "No.\n"
		make	
	fi
popd
make test
