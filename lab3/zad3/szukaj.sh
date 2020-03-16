#!/bin/bash
declare -i counter
counter=0
children=""
for i in $2/*
do
	if [ -f $i ] && [[ $i =~ .*/$1 ]]
	then
		echo $i
		((counter=counter+1))
	elif [ -d $i ]
	then
		./szukaj.sh $1 $i "null" &
		children+=($!)
	fi
done

for i in ${children[@]} #all values of array
do
	if [ $i ]
	then
		wait $i
		counter=$((counter+$?))
	fi
done

if [ -z $3 ]
then
	if [ $counter -eq 0 ]
	 then
		echo "Nie znaleziono"
	fi
fi

