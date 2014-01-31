#!/bin/bash


# input  :   folder_name, folder_name must be in this folder
#            do not use ./folder_name for /home/..../folder_name
#            only: d2pak.sh folder_name
#            otherwise the string generated will not be of consistency

# output :   folder_name.d2p

# interal:   mkdir ./tmp
#            keep it so we can check



# global const

TMP_FOLDER_PATH="./tmp"

INPUT_FOLDER_PATH=$1

if [[ $# < 1 ]]
then
    echo ./d2pak folder_name
    exit 1
fi

if [ ! -d $1 ]
then
    echo  $1 does not exist ...
    exit 1
fi

# step one
# generate the file name list in tmp

rm -rf $TMP_FOLDER_PATH
mkdir $TMP_FOLDER_PATH
find $1 -type f > $TMP_FOLDER_PATH/list



# setp two
# calculate the hash value of it

sed -i 's@.*@./mpqhash 1 &@' $TMP_FOLDER_PATH/list
chmod +x $TMP_FOLDER_PATH/list
$TMP_FOLDER_PATH/list > $TMP_FOLDER_PATH/hashv


