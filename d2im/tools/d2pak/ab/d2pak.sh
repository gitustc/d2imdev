#!/bin/bash

# input  :   /file/folder/path/folder_name
# output :   folder_name.d2p
# interal:   mkdir ./tmp


# global const

TMP_FOLDER_PATH="./tmp"

INPUT_FOLDER_PATH=$1


# step one
# generate the file name list in tmp

rm -rf $TMP_FOLDER_PATH
mkdir $TMP_FOLDER_PATH

find $1 -type f > $TMP_FOLDER_PATH/list
