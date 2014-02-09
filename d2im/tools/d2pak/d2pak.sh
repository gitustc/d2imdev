#!/bin/bash


# input  :   folder_name, folder_name must be in this folder
#            do not use ./folder_name for /home/..../folder_name
#            only: d2pak.sh folder_name
#            otherwise the string generated will not be of consistency

# output :   folder_name.d2p

# interal:   mkdir ./tmp
#            keep it so we can check



TMP_FOLDER_PATH="./tmp"
INPUT_FOLDER_PATH=$1
FILE_COUNT_IN_FOLDER=0
COLLISION_PERC=1
OPTIMAL_MIN=999999




#######################################################################################
# input: seed
#      : tmp/list
# outpt: tmp/hashv_seed
#
# calc_hashv $seed
#
calc_hashv(){
    rm -f $TMP_FOLDER_PATH/hashv_$1
    cat $TMP_FOLDER_PATH/list | while read line; do
    ./mpqhash $1 $line >> $TMP_FOLDER_PATH/hashv_$1; done
}



#######################################################################################
# init_tmp res_folder
init_tmp(){
    rm -rf $TMP_FOLDER_PATH
    mkdir $TMP_FOLDER_PATH
    find $1 -type f > $TMP_FOLDER_PATH/list
    FILE_COUNT_IN_FOLDER=`awk 'END{print NR}' $TMP_FOLDER_PATH/list`
}



#######################################################################################
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



#######################################################################################
calc_all_hash(){
    for i in 1 2 3 4 5
    do
        calc_hashv $i
    done
}


#######################################################################################
# calc_avg_step $seed1 $seed2 $seed3 $file_count $capacity
calc_avg_step(){
    local f1=$TMP_FOLDER_PATH/hashv_$1
    local f2=$TMP_FOLDER_PATH/hashv_$2
    local f3=$TMP_FOLDER_PATH/hashv_$3
    cat $f1 $f2 $f3 > $TMP_FOLDER_PATH/hashv
    ./avgstep $4 $5 $TMP_FOLDER_PATH/hashv
}




#######################################################################################
# check_unique $s1 $s2 $s3 $avgstep
check_unique(){

    local tmp_len1=0 
    local tmp_len2=0 
    local tmp_f1=$TMP_FOLDER_PATH/hashv_$2
    local tmp_f2=$TMP_FOLDER_PATH/hashv_$3

    tmp_len1=`paste $tmp_f1 $tmp_f2 | uniq | awk 'END{print NR}' -`
    tmp_len2=`paste $tmp_f1 $tmp_f2 | awk 'END{print NR}' -`

    if (( $tmp_len1 != $tmp_len2 ))
    then
        return 1
    fi
    return 0

}

#######################################################################################
get_opts(){

    local tmp_capacity=$(($FILE_COUNT_IN_FOLDER * 1))
    local tmp_min=999999
    local tmp_res=999999

    local tmp_perm=$TMP_FOLDER_PATH/perm
    local tmp_avgs=$TMP_FOLDER_PATH/avgstep
    local tmp_perm_avgs=$TMP_FOLDER_PATH/perm_avgs

    ./permint > $TMP_FOLDER_PATH/perm

    while true
    do
        cat $TMP_FOLDER_PATH/perm | while read line; do
        calc_avg_step $line $FILE_COUNT_IN_FOLDER $tmp_capacity;
        echo; done > $TMP_FOLDER_PATH/avgstep

        paste $tmp_perm $tmp_avgs > $tmp_perm_avgs
        tmp_res=`awk 'BEGIN{ min = 999999}
        {
            if($4<min){
                min=$4; 
                info=$0
            }
        }END{print info}' $tmp_perm_avgs`
        tmp_min=`echo $tmp_res | awk '{print $4}' -`
        echo $FILE_COUNT_IN_FOLDER $tmp_capacity $tmp_res

        if (($tmp_min < 110))
        then
            if check_unique $tmp_res
            then
                echo uniqueness check succeed !!!
                OPTIMAL_MIN="$tmp_capacity $tmp_res"
                return
            fi
        fi
        tmp_capacity=$(($tmp_capacity+1))
    done
}


#######################################################################################
gen_file_info(){
    local tmp_file_size=$TMP_FOLDER_PATH/file_size
    local tmp_file_list=$TMP_FOLDER_PATH/list
    local tmp_file_start=$TMP_FOLDER_PATH/file_start
    local tmp_file_info=$TMP_FOLDER_PATH/file_info

    cat $tmp_file_list | while read line; do
    ls -al $line | awk '{print $5}'; done > $tmp_file_size
    awk 'BEGIN{start=0; print start}
    {
        start=$1+start;
        print start
    }' $tmp_file_size > $tmp_file_start
    paste $tmp_file_size $tmp_file_start | sed '$d' - > $tmp_file_info
}


#######################################################################################
gen_file_head(){

    local target=$TMP_FOLDER_PATH/stream

    local opt_capacity=`echo $OPTIMAL_MIN | awk '{print $1}' -`
    local opt_seed0=`echo $OPTIMAL_MIN | awk '{print $2}' -`
    local opt_seed1=`echo $OPTIMAL_MIN | awk '{print $3}' -`
    local opt_seed2=`echo $OPTIMAL_MIN | awk '{print $4}' -`

    # echo $opt_capacity
    # echo $opt_seed0
    # echo $opt_seed1
    # echo $opt_seed2

    local opt_hash0=$TMP_FOLDER_PATH/hashv_$opt_seed0
    local opt_hash1=$TMP_FOLDER_PATH/hashv_$opt_seed1
    local opt_hash2=$TMP_FOLDER_PATH/hashv_$opt_seed2
    local opt_hashs=$TMP_FOLDER_PATH/hashvseeds

    local file_head=$TMP_FOLDER_PATH/file_head
    local file_info=$TMP_FOLDER_PATH/file_info

    cat $opt_hash0 $opt_hash1 $opt_hash2 > $opt_hashs
    ./genhead $FILE_COUNT_IN_FOLDER $opt_capacity $opt_hashs > $file_head

    rm -f $target

    echo $FILE_COUNT_IN_FOLDER >> $target
    echo $opt_capacity >> $target
    echo $opt_seed0 >> $target
    echo $opt_seed1 >> $target
    echo $opt_seed2 >> $target
    awk '{printf("%u\n%u\n%u\n",$1,$2,$3)}' $file_head >> $target
    awk '{printf("%u\n%u\n",$1,$2)}' $file_info >> $target
}


#######################################################################################
gen_d2pk(){
    local target=$TMP_FOLDER_PATH/$INPUT_FOLDER_PATH.d2p
    rm -f $target

    
    printf "%s" "D2PK" >> $target
    cat $TMP_FOLDER_PATH/stream | while read line; do
    ./tad -u32 $line $target; done

    cat $TMP_FOLDER_PATH/list | while read line; do
    cat $line >> $target; done

    mv $target $INPUT_FOLDER_PATH.d2p
}

#######################################################################################
# main entry
check_para $# $1
init_tmp $1
calc_all_hash
get_opts
gen_file_info
gen_file_head
gen_d2pk
