#!/bin/bash
echo $$
echo $1 
sleep 3
counter=$1
((counter++))
if [ $counter -lt 6 ]
then
./procesy.sh $counter &./procesy.sh $counter 
fi
