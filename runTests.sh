#!/bin/bash

# Automatically compile and run all tests in src/test/ numberOfTets times
# Keep track of number of failed tests

failedTests=0
numberOfTests=10

for testName in src/test/*.cpp; do
   # echo $testName
   ./compile.sh $testName
   echo "Running $testName $numberOfTests times..."
   for ((i=0; i<=numberOfTests; i++)); do
      ./a.out

      if [ $? -ne 0 ]
      then
         failedTests++
      fi
   done
   echo
done

echo "Number of tests failed: $failedTests"
