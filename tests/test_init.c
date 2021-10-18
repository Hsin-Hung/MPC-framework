#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test-utils.h"

int main(int argc, char **argv)
{
    if(argc < 3){
        printf("Not enough arguments!");
        exit(0);
    }

    uint host_ip = atoi(argv[1]), conn_ip = atoi(argv[2]);

    char *ip = "127.0.0.1";
    init(ip, host_ip, conn_ip);

    return 0;
}