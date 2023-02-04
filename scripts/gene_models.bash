#! /bin/bash


#echo "list of BIP models:"
BIP_MODELS="`ls *.bip`"
echo BIP_MODELS: ${BIP_MODELS}

for file in ${BIP_MODELS}
  do
      smv_file="${file%%.*}.smv"
      echo processing BIP mdoel: ${smv_file}
      ./BIP2NuSMV.jar ${file} ${smv_file}

      # run nuxmv 

      # clean up
      # rm ${smv_file}
  done
