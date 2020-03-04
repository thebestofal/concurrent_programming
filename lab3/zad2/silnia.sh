#!/bin/bash
declare -i wynik n
n=$1
wynik=$1
while [ $n -gt 1 ]
do
	wynik=$((wynik*$((n-1))))
	((n=n-1))
done
echo "wynik z silni:" $wynik
exit $wynik
