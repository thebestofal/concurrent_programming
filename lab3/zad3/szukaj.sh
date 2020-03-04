#!/bin/bash

#find $2 $1
for i in $2/*
do
	if [ -f $i && $i -eq $1 ]
	then
		echo $i
	fi
done
