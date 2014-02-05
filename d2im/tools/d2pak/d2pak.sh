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
FILE_COUNT_IN_FOLDER=0



#define functions here


########################################################################
# input: seed
#      : tmp/list
# outpt: tmp/hashv_seed

calc_hashv(){
    cp $TMP_FOLDER_PATH/list $TMP_FOLDER_PATH/calc_hash
    sed -i "s@.*@./mpqhash $1 &@" $TMP_FOLDER_PATH/calc_hash
    chmod +x $TMP_FOLDER_PATH/calc_hash
    $TMP_FOLDER_PATH/calc_hash > $TMP_FOLDER_PATH/hashv_$1
    rm $TMP_FOLDER_PATH/calc_hash -f
}
########################################################################




########################################################################
# test capacity
test_capacity(){
    
}
########################################################################






########################################################################
# test parameters
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
########################################################################



########################################################################
# step one
# generate the file name list in tmp
rm -rf $TMP_FOLDER_PATH
mkdir $TMP_FOLDER_PATH
find $1 -type f > $TMP_FOLDER_PATH/list
FILE_COUNT_IN_FOLDER=`awk '{print NR}' $TMP_FOLDER_PATH/list | tail -1`
########################################################################



########################################################################
# setp two
# calculate the hash value of it
for seed in 1
do
    calc_hashv $seed
    awk '{printf("%u\n", $0 % 10)}' $TMP_FOLDER_PATH/hashv_$seed
done
########################################################################
