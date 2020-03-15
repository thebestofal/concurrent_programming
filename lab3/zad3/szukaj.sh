#!/bin/bash

for i in $2/*
do
	if [ -f $i ] && [[ $i =~ .*/$1 ]]
	then
		echo $i
	elif [ -d $i ]
	then
		./szukaj.sh $1 $i
	
	fi
done
