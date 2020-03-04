#!/bin/bash
declare -i silnia_n silnia_k silnia_nk
read n
read k

./silnia.sh $n &
n_pid=$!

./silnia.sh $k &
k_pid=$!

./silnia.sh $(($n-$k)) &
silnia_nk=$!

wait $n_pid
silnia_n=$?

wait $k_pid
silnia_k=$?

wait $nk_pid
silnia_nk=$?

#./silnia.sh k &
#wait $!
#silnia_k=$?

#./silnia.sh $((n-k)) &
#wait $!
#silnia_nk=$?

echo $silnia_n
echo $silnia_k
echo $silnia_nk

exit 0
