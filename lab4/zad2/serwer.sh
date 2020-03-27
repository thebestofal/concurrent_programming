#!/bin/bash
serwer=serwerfifo

if [[ $1 ]]; then
	if [[ ! -p $1 ]]; then
		echo "Nie działa kolejka fifo klienta"
	else
		x=$(($2*$2))
		echo $x > $1
	fi
else
  	trap "echo Caught SIGUSR1; exit 1;" SIGUSR1;
	trap "rm -f $serwer" EXIT
	trap "" SIGTERM
	trap "" SIGHUP 
	trap "" SIGCHLD

	if [[ ! -p $serwer ]]; then
		mkfifo $serwer
	fi

	while true
	do
		if read liczba < $serwer; then
			$0 $liczba &
		fi
	done
fi
