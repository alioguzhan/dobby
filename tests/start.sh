#!/bin/bash

## command ##
cmd="dobby start task_name_a"
$cmd
## get status ##
status=$?

if [ $status -eq 0 ]; then
    echo "Starting a task: PASSED"
else
    echo "Starting a task: FAILED"
    exit 1
fi
