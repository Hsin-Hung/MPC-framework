#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "test-utils.h"

int main(int argc, char **argv)
{
    init(argc, argv);
    char buf1[20], buf2[20], buf3[20];
    /*if (get_rank() == 0)
    {

        TCP_Send("Hello 1!", 9, 1, 1);
        TCP_Send("Hello 2!", 9, 1, 1);
        TCP_Send("Hello 3!", 9, 1, 1);

        TCP_Send("Hello 1!", 9, 2, 1);
        TCP_Send("Hello 2!", 9, 2, 1);
        TCP_Send("Hello 3!", 9, 2, 1);
    }
    else if (get_rank() == 1)
    {
        TCP_Recv(buf1, 9, 0, 1);
        TCP_Recv(buf2, 9, 0, 1);
        TCP_Recv(buf3, 9, 0, 1);
    }
    else
    {
        TCP_Recv(buf1, 9, 0, 1);
        TCP_Recv(buf2, 9, 0, 1);
        TCP_Recv(buf3, 9, 0, 1);
    }

    printf("buf1: %s", buf1);
    printf("buf2: %s", buf2);
    printf("buf3: %s", buf3);

    */
    return 0;
}