#!/bin/bash

## command ##
cmd="./dobby list"
$cmd
## get status ##
status=$?

if [ $status -eq 0 ]; then
    echo "Listing active tasks: PASSED"
else
    echo "Listing active tasks: FAILED"
fi

## command ##
cmd="./dobby list --all"
$cmd
## get status ##
status=$?

if [ $status -eq 0 ]; then
    echo "Listing all tasks: PASSED"
else
    echo "Listing all tasks: FAILED"
fi
