#!/bin/bash

## command ##
cmd="dobby --version"
$cmd
## get status ##
status=$?

if [ $status -eq 0 ]; then
    echo "Printing version with --version: PASSED"
else
    echo "Printing version with --version: FAILED"
    exit 1
fi

## command ##
cmd="dobby -v"
$cmd
## get status ##
status=$?

if [ $status -eq 0 ]; then
    echo "Printing version with -v: PASSED"
else
    echo "Printing version with -v: FAILED"
    exit 1
fi
