#!/bin/bash

## command ##
cmd="dobby"
$cmd
## get status ##
status=$?

if [ $status -eq 1 ]; then
    echo "Running without arguments: PASSED"
else
    echo "Running without arguments: FAILED"
    exit 1
fi
