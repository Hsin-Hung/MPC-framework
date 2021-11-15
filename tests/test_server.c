#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <errno.h>

#define PORT 8080
#define TRUE 1
#define FALSE 0

struct TCP_Request
{

    int socket_fd;
    struct sockaddr_in socket_addr;
    int count;
    struct pollfd fds[200];
    void *buf;
    int flag; /* send: 0, receive: 1 */
};

int TCP_Irecv(void *buf, int count, int source, int tag, struct TCP_Request *r)
{

    int server_fd, new_socket, valread, on = 1;
    struct sockaddr_in *socket_addr = &(r->socket_addr);
    int opt = 1;
    int addrlen = sizeof(*socket_addr);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (ioctl(server_fd, FIONBIO, (char *)&on) < 0)
    {
        perror("ioctl() failed");
        exit(-1);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    socket_addr->sin_family = AF_INET;
    socket_addr->sin_addr.s_addr = INADDR_ANY;
    socket_addr->sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)socket_addr,
             sizeof(*socket_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    memset(r->fds, 0, sizeof(r->fds));
    r->fds[0].fd = server_fd;
    r->fds[0].events = POLLIN;
    r->count = count;
    r->socket_fd = server_fd;
    r->buf = buf;
    r->flag = 1;

    return 0;
}

int TCP_Wait(struct TCP_Request *r)
{

    struct sockaddr_in address;
    int addrlen = sizeof(address), new_socket;
    int timeout = (3 * 60 * 1000), rc, nfds = 1, current_size = 0, i, j, new_sd = -1, nbuf = 0;
    int close_conn, len, compress_array = FALSE, end_server = FALSE;
    char buffer[80];

    if (r->flag == 0)
    {
        ssize_t n;
        const char *p = r->buf;
        while (r->count > 0)
        {
            n = send(r->socket_fd, r->buf, r->count, 0);
            if (n <= 0)
                return -1;
            p += n;
            r->count -= n;
        }
        return 0;
    }

    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    /*************************************************************/
    do
    {
        /***********************************************************/
        /* Call poll() and wait 3 minutes for it to complete.      */
        /***********************************************************/
        printf("Waiting on poll()...\n");
        rc = poll(r->fds, nfds, timeout);

        /***********************************************************/
        /* Check to see if the poll call failed.                   */
        /***********************************************************/
        if (rc < 0)
        {
            perror("  poll() failed");
            break;
        }

        /***********************************************************/
        /* Check to see if the 3 minute time out expired.          */
        /***********************************************************/
        if (rc == 0)
        {
            printf("  poll() timed out.  End program.\n");
            break;
        }

        /***********************************************************/
        /* One or more descriptors are readable.  Need to          */
        /* determine which ones they are.                          */
        /***********************************************************/
        current_size = nfds;
        for (i = 0; i < current_size; i++)
        {
            /*********************************************************/
            /* Loop through to find the descriptors that returned    */
            /* POLLIN and determine whether it's the listening       */
            /* or the active connection.                             */
            /*********************************************************/
            if (r->fds[i].revents == 0)
                continue;

            /*********************************************************/
            /* If revents is not POLLIN, it's an unexpected result,  */
            /* log and end the server.                               */
            /*********************************************************/
            if (r->fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", r->fds[i].revents);
                end_server = TRUE;
                break;
            }
            if (r->fds[i].fd == r->socket_fd)
            {
                /*******************************************************/
                /* Listening descriptor is readable.                   */
                /*******************************************************/
                printf("  Listening socket is readable\n");

                /*******************************************************/
                /* Accept all incoming connections that are            */
                /* queued up on the listening socket before we         */
                /* loop back and call poll again.                      */
                /*******************************************************/
                do
                {
                    /*****************************************************/
                    /* Accept each incoming connection. If               */
                    /* accept fails with EWOULDBLOCK, then we            */
                    /* have accepted all of them. Any other              */
                    /* failure on accept will cause us to end the        */
                    /* server.                                           */
                    /*****************************************************/

                    new_sd = accept(r->socket_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                    if (new_sd < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            perror("  accept() failed");
                            end_server = TRUE;
                        }
                        break;
                    }

                    /*****************************************************/
                    /* Add the new incoming connection to the            */
                    /* pollfd structure                                  */
                    /*****************************************************/
                    printf("  New incoming connection - %d\n", new_sd);
                    r->fds[nfds].fd = new_sd;
                    r->fds[nfds].events = POLLIN;
                    nfds++;

                    /*****************************************************/
                    /* Loop back up and accept another incoming          */
                    /* connection                                        */
                    /*****************************************************/

                } while (new_sd != -1);
            }

            /*********************************************************/
            /* This is not the listening socket, therefore an        */
            /* existing connection must be readable                  */
            /*********************************************************/

            else
            {
                printf("  Descriptor %d is readable\n", r->fds[i].fd);
                close_conn = FALSE;
                /*******************************************************/
                /* Receive all incoming data on this socket            */
                /* before we loop back and call poll again.            */
                /*******************************************************/

                do
                {
                    /*****************************************************/
                    /* Receive data on this connection until the         */
                    /* recv fails with EWOULDBLOCK. If any other         */
                    /* failure occurs, we will close the                 */
                    /* connection.                                       */
                    /*****************************************************/
                    size_t st;
                    if (r->count <= 0)
                        return 1;
                    rc = read(r->fds[i].fd, &r->buf[nbuf], r->count);
                    if (rc < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            perror("  recv() failed");
                            close_conn = TRUE;
                        }
                        break;
                    }
                    r->count -= rc;
                    nbuf += rc;

                    /*****************************************************/
                    /* Check to see if the connection has been           */
                    /* closed by the client                              */
                    /*****************************************************/
                    if (rc == 0)
                    {
                        printf("  Connection closed\n");
                        close_conn = TRUE;
                        break;
                    }

                    /*****************************************************/
                    /* Data was received                                 */
                    /*****************************************************/

                } while (TRUE);

                /*******************************************************/
                /* If the close_conn flag was turned on, we need       */
                /* to clean up this active connection. This            */
                /* clean up process includes removing the              */
                /* descriptor.                                         */
                /*******************************************************/
                if (close_conn)
                {
                    close(r->fds[i].fd);
                    r->fds[i].fd = -1;
                    compress_array = TRUE;
                }

            } /* End of existing connection is readable             */
        }     /* End of loop through pollable descriptors              */

        /***********************************************************/
        /* If the compress_array flag was turned on, we need       */
        /* to squeeze together the array and decrement the number  */
        /* of file descriptors. We do not need to move back the    */
        /* events and revents fields because the events will always*/
        /* be POLLIN in this case, and revents is output.          */
        /***********************************************************/
        if (compress_array)
        {
            compress_array = FALSE;
            for (i = 0; i < nfds; i++)
            {
                if (r->fds[i].fd == -1)
                {
                    for (j = i; j < nfds; j++)
                    {
                        r->fds[j].fd = r->fds[j + 1].fd;
                    }
                    i--;
                    nfds--;
                }
            }
        }

    } while (end_server == FALSE); /* End of serving running.    */

    /*************************************************************/
    /* Clean up all of the sockets that are open                 */
    /*************************************************************/
    for (i = 0; i < nfds; i++)
    {
        if (r->fds[i].fd >= 0)
            close(r->fds[i].fd);
    }
}

int main(int argc, char **argv)
{

    struct TCP_Request r;
    char buf[1024];
    struct pollfd fds[200];
    TCP_Irecv(buf, 12, 1, 1, &r);
    TCP_Wait(&r);
    printf("buf: %s\n", buf);
}