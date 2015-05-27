#!/bin/bash
# ...
#echo "Starter script " >> time.txt;
echo "Starting" > dmp_i/time.c.txt;
echo "Starting" > dmp_i/time.sfm.txt;
echo "Starting" > dmp_i/time.tre.txt;
cd Implementation/src
i="1";
while [ $i -lt 23 ];
do
echo $i >> ../../dmp_i/time.c.txt;
echo $i >> ../../dmp_i/time.sfm.txt;
echo $i >> ../../dmp_i/time.tre.txt;
d="0";
while [ $d -lt 10 ];
do
echo $d $i;
/usr/bin/time -o ../../dmp_i/time.c.txt -a -f "\t%e" ./insert.out ../data/chr$i.fa > ../../dmp_i/dump.$i.$d.txt;
/usr/bin/time -o ../../dmp_i/time.sfm.txt -a -f "\t%e" ../../sfm/scan_for_matches ../../pat1 < ../data/chr$i.fa > ../../dmp_i/sfm.$i.$d.txt;
/usr/bin/time -o ../../dmp_i/time.tre.txt -a -f "\t%e" tre-agrep --regexp="(TGCAAGCGTTAAT){+1}" ../data/chr$i.fa --line-number >  ../../dmp_i/tredump.$i.$d.txt;
d=$[$d+1];
printf "."
done
i=$[$i+1];
done
