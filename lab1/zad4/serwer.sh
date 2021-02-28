#!/bin/bash
while true
do
	x=$(< dane.txt)
	if [ $x ]
	then
		wielomian=$(( $x*$x*$x+2 ))
		echo $wielomian > wyniki.txt
		> dane.txt
	fi
done

