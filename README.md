# bt2bip

## Prerequisites tinyxml2
- git clone https://github.com/leethomason/tinyxml2.git
- cd tinyxml2
- mkdir build
- cmake ..
- make -j
- sudo make install -j

## Building
- git clone https://github.com/789wpw/xml2bip.git
- cd xml2bip
- mkdir build 
- cd build
- cmake ..
- make -j

## Running model transformation
- ./xml2bip -o ../test/mytree.bip -i ../test/mytree.xml
 where -o output-file-path  -i input-file-path


## Running model checking
- enter folder scripts
- run gene_models.bash to translate bip to smv file (make sure the bip model is copied to this folder)
  e.g., ./gene_models.bash
- run add_property.py to add property to the smv file,
  e.g., python add_property.py
- run nuXmv_ic3.bash to verify the smv file,
  e.g., ./nuXmv_ic3.bash
- run clean_up.bash to remove intermediate files (optional)