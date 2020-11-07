#!/bin/bash

source "tests/no_args.sh"
source "tests/version.sh"
source "tests/start.sh"
source "tests/stop.sh"
source "tests/list.sh"

echo "=============================="
if [ $status -eq 0 ]; then
    echo "ALL TESTS ARE PASSED"
else
    echo "SOME TESTS ARE FAILED"
fi

exit $status
