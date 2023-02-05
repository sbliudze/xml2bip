#! /bin/bash


#echo "list of SMV models:"
SMV_MODELS="`ls *3.smv`"
echo SMV_MODELS: ${SMV_MODELS}

for file in ${SMV_MODELS}
  do
      smv_file="${file%%.*}.smv"
      echo verifying SMV mdoel: ${smv_file}

      # run nuxmv 
      exec ./nuXmv -source ./nuXmv_ic3.cmd ${smv_file}

      # clean up
      # rm ${smv_file}
  done
