#!/usr/bin/bash

i=0

while [ $i -le 9 ];
do
    ./match.sh example-agent max_beenet
    zcat example-agent-max_beenet* | grep Winner >> score.txt
    rm *.log.g*
    ((i++))
done

echo "The team you looking for won "
cat score.txt | grep Max | wc -l
echo " times out of "
echo $i

echo "The other team you were not looking for won "
cat score.txt | grep example_agent  | wc -l
echo " times out of "
echo $i
rm score.txt