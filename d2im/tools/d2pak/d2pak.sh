#!/bin/bash


# input  :   folder_name, folder_name must be in this folder
#            do not use ./folder_name for /home/..../folder_name
#            only: d2pak.sh folder_name
#            otherwise the string generated will not be of consistency

# output :   folder_name.d2p

# interal:   mkdir ./tmp
#            keep it so we can check



##############################################################################################
# global const
TMP_FOLDER_PATH="./tmp"
INPUT_FOLDER_PATH=$1
FILE_COUNT_IN_FOLDER=0
COLLISION_PERC=1


OPTIMAL_SEED=1
OPTIMAL_CAPACITY=1
##############################################################################################



##############################################################################################
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
##############################################################################################



##############################################################################################
# test_capacity $seed $file_capacity
test_capacity(){
    local coll=`count_collision $1 $2`
    printf "%u" $(($coll*100/$FILE_COUNT_IN_FOLDER))
}
##############################################################################################




##############################################################################################
# count_collision $seed $file_capacity
count_collision(){

    local tmp_input=$TMP_FOLDER_PATH/hashv_$1
    local tmp_outpt=$TMP_FOLDER_PATH/outpout

    awk '{printf("%u\n", $0 % '"$2"')}' $tmp_input > $tmp_outpt
    local count=`cat $tmp_outpt | sort | uniq | awk '{print NR}' | tail -1`
    echo $(($FILE_COUNT_IN_FOLDER - $count))

}
##############################################################################################




##############################################################################################
# init_tmp res_folder
init_tmp(){
    rm -rf $TMP_FOLDER_PATH
    mkdir $TMP_FOLDER_PATH
    find $1 -type f > $TMP_FOLDER_PATH/list
    FILE_COUNT_IN_FOLDER=`awk '{print NR}' $TMP_FOLDER_PATH/list | tail -1`
}
##############################################################################################



##############################################################################################
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
##############################################################################################





##############################################################################################
calc_all_hash(){
    for i in 1 2 3 4 5
    do
        calc_hashv $i
    done
}
##############################################################################################


##############################################################################################
# calc_avg_step $seed1 $seed2 $seed3 $file_count $capacity
calc_avg_step(){
    local f1=$TMP_FOLDER_PATH/hashv_$1
    local f2=$TMP_FOLDER_PATH/hashv_$2
    local f3=$TMP_FOLDER_PATH/hashv_$3
    cat $f1 $f2 $f3 > $TMP_FOLDER_PATH/hashv
    ./avgstep $4 $5 $TMP_FOLDER_PATH/hashv
}
##############################################################################################
##############################################################################################
get_opts(){

    local tmp_capacity=$(($FILE_COUNT_IN_FOLDER * 1))
    ./permint > $TMP_FOLDER_PATH/perm
    # sed -i "s@.*@calc_avg_step & $FILE_COUNT_IN_FOLDER $tmp_capacity@" $TMP_FOLDER_PATH/perm

    cat $TMP_FOLDER_PATH/perm | while read line; do
    calc_avg_step $line $FILE_COUNT_IN_FOLDER $tmp_capacity;
    echo; done > $TMP_FOLDER_PATH/avgstep
    paste $TMP_FOLDER_PATH/perm $TMP_FOLDER_PATH/avgstep > $TMP_FOLDER_PATH/perm_avgstep
    awk 'BEGIN {min = 999999} {if($4<min){min=$4}}END{print "Min=", min}' $TMP_FOLDER_PATH/perm_avgstep


    # while true
    # do

    #     local tmp_min=`calc_avg_step $FILE_COUNT_IN_FOLDER 1 $tmp_capacity`
    #     local tmp_seed=1
    #     for seed in 2 3 4 5
    #     do
    #         local tmp_minc=`calc_avg_step $FILE_COUNT_IN_FOLDER $seed $tmp_capacity`
    #         if (( $tmp_minc < $tmp_min ))
    #         then
    #             tmp_min=$tmp_minc
    #             tmp_seed=$seed
    #         fi
    #     done

    #     echo $FILE_COUNT_IN_FOLDER $tmp_seed $tmp_capacity $tmp_min

    #     if (( $tmp_min < "110" ))
    #     then
    #         # echo $seed $tmp_capacity
    #         OPTIMAL_SEED=$tmp_seed
    #         OPTIMAL_CAPACITY=$tmp_capacity
    #         return
    #     fi


    #     tmp_capacity=$(($tmp_capacity + 1))
    # done
}
##############################################################################################




##############################################################################################
gen_file_size(){
    cp $TMP_FOLDER_PATH/list $TMP_FOLDER_PATH/calc_size
    sed -i "s@.*@ls -al &@" $TMP_FOLDER_PATH/calc_size
    chmod +x $TMP_FOLDER_PATH/calc_size
    $TMP_FOLDER_PATH/calc_size | awk '{print $5}' - > $TMP_FOLDER_PATH/fsize
}
##############################################################################################


##############################################################################################
# gen_d2pk(){
# }
##############################################################################################




##############################################################################################
# main entry
check_para $# $1
init_tmp $1
calc_all_hash
get_opts
# gen_file_size
# gen_d2pk

# echo $OPTIMAL_SEED $OPTIMAL_CAPACITY
