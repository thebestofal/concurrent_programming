#!/bin/bash
while true
do
	wynik=$(< wyniki.txt)
	if [ $wynik ]
	then
		echo $wynik
		> wyniki.txt
	else
		read x
		echo $x > dane.txt
	fi
done
