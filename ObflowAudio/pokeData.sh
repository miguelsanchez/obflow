#!/bin/bash
cd /opt/oblong/greenhouse/bin

while true; do
protein="/Users/iblopez/Library/Developer/CoreSimulator/Devices/695ACE88-8946-4085-ABAD-50373619C38B/data/Containers/Data/Application/B4BC7E2B-E09E-4FC5-8008-AE61D12B2E6D/Documents/protein"
if [ -f "$protein" ]
then
	value=$(<$protein)
	printf "$value"
	printf "\n===========\n"
	poke tcp://eixample.local/hackathon $protein 
	rm -f $protein
# else
	# echo "no data"
fi
done