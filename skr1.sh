#!/bin/bash


set -euo pipefail


if [ $# -eq 0 ]; then
    echo "Użycie: $0 A [B]"
    exit 1
fi


if [ $# -eq 1 ]; then
    A=1
    B=$1
else
    A=$1
    B=$2
fi


if [ "$B" -lt "$A" ]; then
    exit 0
fi


drukuj_komorke() {
    printf "%4s" "$1"
}


drukuj_komorke ""  
for i in $(seq "$A" "$B"); do
    drukuj_komorke "$i"
done
echo


for i in $(seq "$A" "$B"); do
    drukuj_komorke "$i"  
    for j in $(seq "$A" "$B"); do
        wynik=$((i * j))
        drukuj_komorke "$wynik"
    done
    echo
done

