#!/bin/sh
# Test script to show the differences of BM versions

YABASIC='yabasic'
BASMAT='../basmat' # basmat program should be compiled
BASMAT2BAS='BM2sim.yab'
BASMAT3BAS='BM3sim.yab'
BASMAT4BAS='BM4sim.yab'
SEQUENCE='test.txt' # test sequences

cat $SEQUENCE | while read seq
do
  arg="-v 1 -o 1 -s 30 \"$seq\""
  # $BASMAT $arg | head -n 3 > v1.txt
  arg="-v 2 -o 1 -s 30 \"$seq\""
  # $BASMAT $arg | grep -v "Not" | grep -v "seq" | head -n 3 > v2.txt
  arg="-v 2.3 -o 1 -s 30 \"$seq\""
  $BASMAT $arg | grep -v "Not" | grep -v "seq" | head -n 3 > v2.3.txt
  arg="-v 3 -o 1 -s 30 \"$seq\""
  # $BASMAT $arg | grep -v "Not" | grep -v "seq" | head -n 3 > v3.txt
  # echo \"$seq\" | $YABASIC $BASMAT2BAS | head -n 3 > v2b.txt
  # echo \"$seq\" | $YABASIC $BASMAT3BAS | head -n 3 > v3b.txt
  echo \"$seq\" | $YABASIC $BASMAT4BAS | head -n 3 > v4b.txt

#  DIFF=$(diff v1.txt v2.txt)$(diff v1.txt v3.txt)$(diff v2.txt v3.txt)
#  DIFF=$(diff v2.txt v3.txt) # See only the difference of BM2 and BM3
#  DIFF=$(diff v2.txt v2b.txt) # See only the difference of BM2 and BM2 basic
#  DIFF=$(diff v3.txt v3b.txt) # See only the difference of BM3 and BM3 basic
#  DIFF=$(diff v2.txt v4b.txt) # See only the difference of BM2 and BM4 basic
  DIFF=$(diff v2.3.txt v4b.txt) # See only the difference of BM2.3 and BM4 basic
  if [ "$DIFF" != "" ]
  then
    # echo "BM1"
    # cat v1.txt
    # echo "BM2"
    # cat v2.txt
    # echo "BM2B"
    # cat v2b.txt
    echo "BM2.3"
    cat v2.3.txt
    # echo "BM3"
    # cat v3.txt
    # echo "BM3B"
    # cat v3b.txt
    echo "BM4B"
    cat v4b.txt
  fi
  rm v*.txt
done
