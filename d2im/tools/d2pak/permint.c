#include <stdint.h>
#include <stdio.h>




int main(){

    int i;
    int j;
    int k;


    for(i=1;i<=5;i++){
        for(j=1;j<i;j++){
            for(k=1;k<j;k++){

                printf("%d %d %d\n", i, j, k);
                printf("%d %d %d\n", j, k, i);
                printf("%d %d %d\n", k, i, j);

            }
        }
    }

    return 0;
}


