#!/bin/bash

# run this script from within the *same* directory.
#
# note that
#  * the xml files are created on-the-fly from the "template" xml ones, so edit the template xml files if you want to change the tests
#    current substitutions are:
#     - replace '$CWD' with the current working directory
#     - For testing the lite mode, replace '<Cycle' with corresponding cycle config for proof lite mode
#  * the script kills all proofserv.exe processes running with the same user on the same machine at the beginning,
#    so DO NOT run the script while running another proof lite analysis on the same machine

# configurew which tests to run; here is the configuration for running all tests:
tests="0 1 2 3 4"
modes="local lite"

[ -x ./run-test.sh ] || { echo "Error: run this script from its own directory!"; exit 1; }


function run_checked () {
  echo Running "$* ..."
  $*  > /dev/null 2>&1
  code=$?
  if [ $code -gt 0 ]; then
      echo "Error executing $*: result code was $code";
      exit 1;
  fi
}

function killproof () {
   uid=`id -u`
   echo -n "Looking for proofserv.exe..."
   pgrep -u $uid proofserv.exe > /dev/null
   if [ $? -gt 0 ]; then
      echo "none were running";
      return
   fi
   echo -n " found; killing ..."
   pkill -u $uid proofserv.exe
   sleep 1
   pgrep -u $uid proofserv.exe > /dev/null
   if [ $? -gt 0 ]; then
      echo "all killed";
      return
   fi
   echo -n " still running, killing again ..."
   pkill -u $uid -9 proofserv.exe
   sleep 1
   pgrep -u $uid proofserv.exe > /dev/null
   if [ $? -gt 0 ]; then
      echo "all killed";
      return
   fi
   echo "proof server not killable, exiting"
   exit 1;
}


for m in $modes; do
  echo "Testing mode=${m}"
  if [ $m = "lite" ]; then
     killproof;
  fi
  
  for n in $tests; do
    if [ -f prepare-in${n}.py ]; then
        run_checked ./prepare-in${n}.py
    fi
    
    # pre-sframe: prepare xml config by copying the template and doing some substitutions:
    cp TestModule${n}-template.xml TestModule${n}-${m}.xml
    sed -i "s|\$CWD|${PWD}|" TestModule${n}-${m}.xml
    if [ $m = "lite" ]; then 
       sed -i 's|<Cycle |<Cycle RunMode="PROOF" ProofServer="lite://" |' TestModule${n}-${m}.xml
    fi
    
    rm -rf out${n}
    mkdir out${n}
    run_checked sframe_main TestModule${n}-${m}.xml
    # post-sframe: move output for non-local stuff
    
    if [ -f check-out${n}.py ]; then
        run_checked ./check-out${n}.py
    fi
  done
done

# if we reach that, everything went fine:
echo All Tests passed
