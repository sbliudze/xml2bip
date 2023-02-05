#! /bin/bash


MODEL_FILES="`ls *.model`"
TIMOD_FILES="`ls *.timod`"

for file in ${MODEL_FILES}
  do
      # clean up
      rm ${file}
  done

for timod_file in ${TIMOD_FILES}
do
    rm ${timod_file}
done
