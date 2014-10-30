#!/bin/bash
_MODEL1=$1
_MODEL2=$2
_MODEL3=$3
_FRAME=$4
_MAXLEN=$5
for((i=0; i<=${_FRAME}; i=i+1))
do
	len=$((${_MAXLEN}-$(expr length $i)))
    p=''
    for (( j=1; j<=${len}; j=j+1 ))
    do
		p=$(echo "${p}0")
    done
	pp=$(echo "$i" | sed s/^/${p}/)

echo	./Effective_Stress.exe ${_MODEL1}_${pp}.vpf ${_MODEL2}_${pp}.csv ${_MODEL3}_${i}000.dat 10 240 10 40 10 220
done