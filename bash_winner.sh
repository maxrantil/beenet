#!/bin/bash

i=0

while [ $i -le 99 ];
do
    ./match.sh max_beenet max_beenet_improved_movement
    gzcat *.log.g* | grep Winner >> score.txt
    rm *.log.g*
    ((i++))
done

echo "The team you looking for won "
cat score.txt | grep improved | wc -l
echo " times out of "
echo $i

echo "The other team you were not looking for won "
cat score.txt | grep Max  | wc -l
echo " times out of "
echo $i
rm score.txt