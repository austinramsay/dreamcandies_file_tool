#!/bin/bash

/bin/rm test_results
touch test_results

for i in 00
do
	make test$i
	echo starting test $i ... >> test_results
	./tests/test$i &>> test_results
	echo >> test_results
	echo test$i complete.
done
