#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "test-utils.h"

int main(int argc, char **argv)
{
    init(argc, argv);
    char buf[20];
    if (get_rank() == 0)
    {

        TCP_Send("Hello World!", 13, get_succ(), 1);
        TCP_Recv(buf, 13, get_pred(), 1);
    }
    else if (get_rank() == 1)
    {
        TCP_Recv(buf, 13, get_pred(), 1);
        TCP_Send(buf, 13, get_succ(), 1);
    }
    else
    {
        TCP_Recv(buf, 13, get_pred(), 1);
        TCP_Send(buf, 13, get_succ(), 1);
    }

    return 0;
}