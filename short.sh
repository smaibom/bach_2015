#!/bin/bash
# ...
#echo "Starter script " >> time.txt;
echo "Starting" > dmp/time.c.txt;
echo "Starting" > dmp/time.sfm.txt;
echo "Starting" > dmp/time.tre.txt;
cd Implementation/src
i="1";
while [ $i -lt 23 ];
do
echo $i >> ../../dmp/time.c.txt;
echo $i >> ../../dmp/time.sfm.txt;
echo $i >> ../../dmp/time.tre.txt;
d="0";
while [ $d -lt 10 ];
do
echo $d $i;
/usr/bin/time -o ../../dmp/time.c.txt -a -f "\t%e" ./short.out ../data/chr$i.fa > ../../dmp/dump.$i.$d.txt;
/usr/bin/time -o ../../dmp/time.sfm.txt -a -f "\t%e" ../../sfm/scan_for_matches ../../pat2 < ../data/chr$i.fa > ../../dmp/sfm.$i.$d.txt;
/usr/bin/time -o ../../dmp/time.tre.txt -a -f "\t%e" tre-agrep --regexp="(TGCAAGCGTTAAT){+2}" ../data/chr$i.fa --line-number >  ../../dmp/tredump.$i.$d.txt;
d=$[$d+1];
printf "."
done
i=$[$i+1];
done
