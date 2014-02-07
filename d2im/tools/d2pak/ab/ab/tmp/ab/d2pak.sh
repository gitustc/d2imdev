#!/bin/bash


# input  :   folder_name, folder_name must be in this folder
#            do not use ./folder_name for /home/..../folder_name
#            only: d2pak.sh folder_name
#            otherwise the string generated will not be of consistency

# output :   folder_name.d2p

# interal:   mkdir ./tmp
#            keep it so we can check



########################################################################
# global const
TMP_FOLDER_PATH="./tmp"
INPUT_FOLDER_PATH=$1
FILE_COUNT_IN_FOLDER=0
COLLISION_PERC=1
########################################################################



########################################################################
# input: seed
#      : tmp/list
# outpt: tmp/hashv_seed
#
# calc_hashv $seed
#
calc_hashv(){
    cp $TMP_FOLDER_PATH/list $TMP_FOLDER_PATH/calc_hash
    sed -i "s@.*@./mpqhash $1 &@" $TMP_FOLDER_PATH/calc_hash
    chmod +x $TMP_FOLDER_PATH/calc_hash
    $TMP_FOLDER_PATH/calc_hash > $TMP_FOLDER_PATH/hashv_$1
    rm $TMP_FOLDER_PATH/calc_hash -f
}
########################################################################



########################################################################
# test_capacity $seed $file_capacity
test_capacity(){
    local coll=`count_collision $1 $2`
    printf "%u" $(($coll*100/$FILE_COUNT_IN_FOLDER))
}
########################################################################




########################################################################
# count_collision $seed $file_capacity
count_collision(){

    local tmp_input=$TMP_FOLDER_PATH/hashv_$1
    local tmp_outpt=$TMP_FOLDER_PATH/outpout

    awk '{printf("%u\n", $0 % '"$2"')}' $tmp_input > $tmp_outpt
    local count=`cat $tmp_outpt | sort | uniq | awk '{print NR}' | tail -1`
    echo $(($FILE_COUNT_IN_FOLDER - $count))

}
########################################################################




########################################################################
# init_tmp res_folder
init_tmp(){
    rm -rf $TMP_FOLDER_PATH
    mkdir $TMP_FOLDER_PATH
    find $1 -type f > $TMP_FOLDER_PATH/list
    FILE_COUNT_IN_FOLDER=`awk '{print NR}' $TMP_FOLDER_PATH/list | tail -1`
}
########################################################################



########################################################################
# check_para $# $1
check_para(){
    if (( $1 < 1 ))
    then
        echo ./d2pak folder_name
        exit 1
    fi

    if [ ! -d $2 ]
    then
        echo  $2 does not exist ...
        exit 1
    fi
}
########################################################################





########################################################################
calc_all_hash(){
    for i in 1 2 3 4 5
    do
        calc_hashv $i
    done
}
########################################################################





########################################################################
find_first_seed(){

    local tmp_capacity=$(($FILE_COUNT_IN_FOLDER * 1))
    while true
    do
        for seed in 1 2 3 4 5
        do
            local tmp_coll_perc=`test_capacity $seed $tmp_capacity`
            # echo $seed $tmp_capacity $tmp_coll_perc
            if (( $tmp_coll_perc <= $COLLISION_PERC ))
            then
                echo $seed $tmp_capacity
                return
            fi
        done
        tmp_capacity=$(($tmp_capacity + 1))
    done
}

check_para $# $1
init_tmp $1
calc_all_hash

find_first_seed

