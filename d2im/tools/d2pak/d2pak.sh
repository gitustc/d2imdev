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
# check_unique $s1 $s2 $s3 $avgstep
check_unique(){

    # echo $1
    # echo $2
    # echo $3
    # echo $4

    local tmp_len1=0 
    local tmp_len2=0 
    tmp_len1=`paste $TMP_FOLDER_PATH/hashv_$2 $TMP_FOLDER_PATH/hashv_$3 | uniq | awk 'END{print NR}' -`
    tmp_len2=`paste $TMP_FOLDER_PATH/hashv_$2 $TMP_FOLDER_PATH/hashv_$3 | awk 'END{print NR}' -`

    # echo $tmp_len1
    # echo $tmp_len2
    if (( $tmp_len1 != $tmp_len2 ))
    then
        return 1
    fi
    return 0

}

##############################################################################################
get_opts(){

    local tmp_capacity=$(($FILE_COUNT_IN_FOLDER * 1))
    local tmp_min=999999
    local tmp_res=999999
    ./permint > $TMP_FOLDER_PATH/perm

    while true
    do
        cat $TMP_FOLDER_PATH/perm | while read line; do
        calc_avg_step $line $FILE_COUNT_IN_FOLDER $tmp_capacity;
        echo; done > $TMP_FOLDER_PATH/avgstep
        paste $TMP_FOLDER_PATH/perm $TMP_FOLDER_PATH/avgstep > $TMP_FOLDER_PATH/perm_avgstep
        tmp_res=`awk 'BEGIN{min = 999999}{if($4<min){min=$4; info=$0}}END{print info}' $TMP_FOLDER_PATH/perm_avgstep`
        tmp_min=`echo $tmp_res | awk '{print $4}' -`

        echo $tmp_capacity $tmp_res


        if (($tmp_min < 110))
        then
            if check_unique $tmp_res
            then
                echo unique
                OPTIMAL_MIN="$tmp_capacity $tmp_res"
                return
            fi
        fi
        tmp_capacity=$(($tmp_capacity+1))
    done

}
##############################################################################################




##############################################################################################
gen_file_info(){
    cat $TMP_FOLDER_PATH/list | while read line; do
    ls -al $line | awk '{print $5}'; done > $TMP_FOLDER_PATH/file_size
    awk 'BEGIN{start=0; print start}{start=$1+start; print start}' $TMP_FOLDER_PATH/file_size > $TMP_FOLDER_PATH/file_start
    paste $TMP_FOLDER_PATH/file_size $TMP_FOLDER_PATH/file_start | sed '$d' - > $TMP_FOLDER_PATH/file_info
}
##############################################################################################


##############################################################################################
gen_file_head(){
    local target=$TMP_FOLDER_PATH/stream
    local opt_capacity=`echo $OPTIMAL_MIN | awk '{print $1}' -`
    local opt_seed0=`echo $OPTIMAL_MIN | awk '{print $2}' -`
    local opt_seed1=`echo $OPTIMAL_MIN | awk '{print $3}' -`
    local opt_seed2=`echo $OPTIMAL_MIN | awk '{print $4}' -`

    cat $TMP_FOLDER_PATH/hashv_$opt_seed0 $TMP_FOLDER_PATH/hashv_$opt_seed1 $TMP_FOLDER_PATH/hashv_$opt_seed2 >$TMP_FOLDER_PATH/hashvseeds
    ./genhead $FILE_COUNT_IN_FOLDER $opt_capacity $TMP_FOLDER_PATH/hashvseeds > $TMP_FOLDER_PATH/file_head

    rm -f $target

    echo $FILE_COUNT_IN_FOLDER >> $target
    echo $opt_capacity >> $target
    echo $opt_seed0 >> $target
    echo $opt_seed1 >> $target
    echo $opt_seed2 >> $target
    awk '{printf("%u\n%u\n%u\n",$1,$2,$3)}' $TMP_FOLDER_PATH/file_head >> $target
    awk '{printf("%u\n%u\n",$1,$2)}' $TMP_FOLDER_PATH/file_info >> $target

}
##############################################################################################
gen_d2pk(){
    local target=$TMP_FOLDER_PATH/$INPUT_FOLDER_PATH.d2p
    rm -f $target

    ./tad -u8 68 $target
    ./tad -u8 50 $target
    ./tad -u8 80 $target
    cat $TMP_FOLDER_PATH/stream | while read line; do
    ./tad -u32 $line $target; done

    cat $TMP_FOLDER_PATH/list | while read line; do
    cat $line >> $target; done

    mv $target $INPUT_FOLDER_PATH.d2p
}
##############################################################################################

##############################################################################################
# main entry
check_para $# $1
init_tmp $1
calc_all_hash
get_opts
gen_file_info
gen_file_head
gen_d2pk
