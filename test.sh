#!/bin/bash
cd ./build
cmake ..
make
for num in {1..30}
do
    echo "${num}-th test begins"

    ../bin/main

    if ! diff ~/rdt/input.txt ~/rdt/output.txt &> /dev/null
    then
        echo "Error: The contents of input.txt and output.txt are different."
        exit 1
    fi

    echo "${num}-th test ends"

done

echo "30 tests passed"

