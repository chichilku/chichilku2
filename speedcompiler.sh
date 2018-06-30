#!/bin/bash
#chichilku speedcompiler by ChillerDragon
#copyright ChillerDragon 2018

while :
do
	clear
	echo "=== chichilku speed compiler ==="
	echo "written by ChillerDragon"
	echo "executing compile.sh and chichilku binary: "
	./compile.sh; ./chichilku
	read -n1 -r -p "Press q to quit and anything else to rebuild..." key

	if [ "$key" = 'q' ]; then
		exit;
	fi
done
