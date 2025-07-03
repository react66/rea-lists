#!/bin/bash
make 
if [ $? -eq 0 ]
then
 echo "SUCCESS! **********************"
else
 read -p "ERROR! Press key to exit: " y
 exit 1
fi
sleep 2
