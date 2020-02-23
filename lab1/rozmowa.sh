#!/bin/bash
while true
do
	echo "0 - odczyt komunikatu"
	echo "1 - zapis komunikatu"
	echo "2 - koniec pracy"
	read option
	if [ $option -eq 0 ]
	then
		cat $1
	elif [ $option -eq 1 ]
	then
		read comment
		echo $comment >> $1
	elif [ $option -eq 2 ]
	then
		exit 0
	else
		echo "Podałeś złą opcję!"
	fi
done
