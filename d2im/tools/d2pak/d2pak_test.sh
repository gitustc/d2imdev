#!/bin/bash

# d2pak_test test.d2p




INPUT_FILE=$1
ORIGINAL_FOLDER_PATH=${INPUT_FILE/.d2p/}
TMP_FOLDER_PATH=${ORIGINAL_FOLDER_PATH}_tmp
TEST_FOLDER_PATH=${ORIGINAL_FOLDER_PATH}_test


#######################################################################################
# check_para $# $1
check_para(){
    if (( $1 < 1 ))
    then
        echo ./d2pak_test pakage.d2p
        exit 1
    fi

    if [ ! -e $2 ]
    then
        echo  $2 does not exist ...
        exit 1
    fi

    if [ ! -e $TMP_FOLDER_PATH/list ]
    then
        echo $TMP_FOLDER_PATH/list is missing ...
        exit 1
    fi
}

#######################################################################################
init_test(){
    rm -rf $TEST_FOLDER_PATH
    mkdir -p $TEST_FOLDER_PATH/$ORIGINAL_FOLDER_PATH
}

extract_all_files(){
    echo extract files ...
    cat $TMP_FOLDER_PATH/list | while read line ; do
    local tmp_folder=`echo "$line" | sed -r "s/\/[^/]+$//"` 
    if [ ! -d $TEST_FOLDER_PATH/"$tmp_folder" ]
    then
        mkdir -p $TEST_FOLDER_PATH/"$tmp_folder"
    fi
    ./d2pextr $INPUT_FILE "$line" $TEST_FOLDER_PATH/"$line" ; done
}


compare_all_files(){
    echo start to compare ...
    cat $TMP_FOLDER_PATH/list | while read line ; do
    diff "$line" $TEST_FOLDER_PATH/"$line" > /dev/null
    local tmp_state=`echo $?`
    if (( $tmp_state != 0 ))
    then
        echo error occurs ...
        echo 1. $line
        echo 2. $TEST_FOLDER_PATH/$line
        return 1
    fi ; done

    echo no error ...

}

#######################################################################################
# main entry
check_para $# $1
init_test
extract_all_files
compare_all_files
