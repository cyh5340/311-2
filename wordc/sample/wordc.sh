#!/bin/sh
STARTTIME=`date +%s.%N`

rm -f "$2" "$3"
cat "$1" | tr "[:upper:]" "[:lower:]" | tr -cd "[[:alnum:]\n ]" | tr ' ' '\n'| tr -d '\[\]\_' | sort | uniq -c | gawk '/[0-9]+[" "]+[a-z0-9]+/ {print $2", "$1}' >> "$2"

ENDTIME=`date +%s.%N`

TIMEDIFF=`echo "$ENDTIME - $STARTTIME" | bc | gawk -F"." '{print $1"."substr($2,1,6)}'`
printf "File: $1\nTimer: $TIMEDIFF" >> "$3"
