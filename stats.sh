#!/bin/bash
echo "Packet statistics" | tee log
echo "\t"
echo queue input count drop output | tee log
while :
do
	echo -n "queue1 \t"
	echo -n "$(cat /click/aaqm/total1) \t" | tee -a log
	echo -n "$(cat /click/aaqm/count1) \t" | tee -a log
	echo -n "$(cat /click/aaqm/drop1) \t" | tee -a log
	echo "$(cat /click/after0/count)" | tee -a log
	echo -n "queue2 \t"
	echo -n "$(cat /click/aaqm/total2) \t" | tee -a log
	echo -n "$(cat /click/aaqm/count2) \t" | tee -a log
	echo -n "$(cat /click/aaqm/drop2) \t" | tee -a log
	echo "$(cat /click/after1/count)" | tee -a log
	echo -n "queue3 \t"
	echo -n "$(cat /click/aaqm/total3) \t" | tee -a log
	echo -n "$(cat /click/aaqm/count3) \t" | tee -a log
	echo -n "$(cat /click/aaqm/drop3) \t" | tee -a log
	echo "$(cat /click/after2/count)" | tee -a log
	echo -n "queue4 \t"
	echo -n "$(cat /click/aaqm/total4) \t" | tee -a log
	echo -n "$(cat /click/aaqm/count4) \t" | tee -a log
	echo -n "$(cat /click/aaqm/drop4) \t" | tee -a log
	echo "$(cat /click/after3/count)" | tee -a log
	echo -n "queue5 \t"
	echo -n "$(cat /click/aaqm/total5) \t" | tee -a log
	echo -n "$(cat /click/aaqm/count5) \t" | tee -a log
	echo -n "$(cat /click/aaqm/drop5) \t" | tee -a log
	echo "$(cat /click/after4/count)" | tee -a log
	echo " Output rate: $(cat /click/tee_counter/rate) pkts/sec" | tee -a log
	echo " Output rate: $(cat /click/tee_counter/bit_rate) bits/sec" | tee -a log
	echo "-----------------" | tee -a log
	sleep 1
done


