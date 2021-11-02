#!/usr/bin/env bash

INPUT=(input*)
GOLDEN=(output*)
CASES=${#INPUT[@]}

HOMEWORK=$(pwd)
HOMEWORK=${HOMEWORK##*/}

for i in "${!INPUT[@]}"; do
	./a.out < ${INPUT[$i]} | diff -w - ${GOLDEN[$i]}
	
	if [[ $? -eq 0 ]]; then
		echo "${HOMEWORK} testcase $((i+1))/${CASES} ... passed"
	else
		echo "${HOMEWORK} testcase $((i+1))/${CASES} ... failed"
		exit 1
	fi
done
