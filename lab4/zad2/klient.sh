#!/bin/bash
serwer=serwerfifo
klient="klientfifo"

trap "rm -f $klient" EXIT

if [[ -z $1 ]]
then
	echo "nie podano liczby"
fi

if [[ ! -p $klient ]]
then
    mkfifo $klient
fi

if [[ ! -p $serwer ]]
then
    echo "Serwer nie włączony, lub nie działa kolejka fifo serwera"
    exit 1
fi

echo $klient $1 > $serwer
read wynik < $klient
echo "kwadrat podanej liczby: "$wynik

