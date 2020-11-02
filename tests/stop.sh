#!/bin/bash

## command ##
cmd="./dobby stop task_name_a"
$cmd
## get status ##
status=$?

if [ $status -eq 0 ]; then
    echo "Stopping a task: PASSED"
else
    echo "Stopping a task: FAILED"
fi
