#!/bin/bash
echo "Podaj liczbe:"
	read x
	echo $x > dane.txt
while true
do
	wynik=$(< wyniki.txt)
	if [ $wynik ]
	then
		echo $wynik
		> wyniki.txt
		break
	fi
done


