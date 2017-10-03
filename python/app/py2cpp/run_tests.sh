#!/bin/bash

`fpsmake --env`

if [[ -z ${FPS_ROOT_DIR} ]] ; then 
  echo ""
  echo "ERROR: Environment variable FPS_ROOT_DIR not defined"
  echo ""
  exit -1 
fi

for fname in `ls *.py` ; do
  ./${fname}
  if [[ $? -ne 0 ]] ; then 
    echo ""
    echo "ERROR: Test failure in '${fname}' - Aborting run"
    echo ""
    exit -1 
  fi
done 

  
