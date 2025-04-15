#include <sys/socket.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "server.h"

int
conn(struct pollfd *fds,
     int    *nfds,
     int    *sockfd,
     int    i)
{
        int new_fd;

        if (fds[i].fd != *sockfd)
        {
                return -1;
        }
        
        new_fd = accept(*sockfd, NULL, NULL);
        if (new_fd == -1)
        {
                return -1;
        }

        if (!(*sockfd >= 0 && *nfds < MAX_FDS))
        {
                (void)close(new_fd);
                return -1;
        }

        fds[*nfds].fd = new_fd;
        fds[*nfds].events = POLLIN;
        (*nfds)++;

        return 0;
}

void
disconn(struct pollfd *fds,
        int    *nfds,
        int    i)
{
        (void)printf("fd %d disconnected\n", fds[i].fd);
        (void)close(fds[i].fd);
        fds[i] = fds[*nfds - 1];
        (*nfds)--;
}

void
broadcast(struct     pollfd *fds,
          int        *nfds,
          int        senderfd,
          const char *msg,
          int        len)
{
        int i;

        for (i = 0; i < *nfds; i++)
        {
                if (fds[i].fd == senderfd)
                {
                        continue;
                }

                if (fds[i].fd == fds[0].fd)
                {
                        continue;
                }

                send(fds[i].fd, msg, len, 0);
        }
}

int
poll_events(struct pollfd *fds,
            int    *nfds,
            int    *sockfd)
{
        int  i, bytes;
        char buf[BUF_S];

        for (i = *nfds - 1; i >= 0; i--)
        {
                if (!(fds[i].revents & POLLIN))
                {
                        continue;
                }

                /* a new client connected */
                if (i == 0)
                {
                        conn(fds, nfds, sockfd, i);
                        continue;
                }

                
                /* message received from client */
                bytes = recv(fds[i].fd, buf, BUF_S - 1, 0);
                if (bytes > 0)
                {
                        /* send the message to all clients except
                         * the sender */
                        broadcast(fds, nfds, fds[i].fd, buf, bytes);
                        continue;
                }

                disconn(fds, nfds, i);
        }

        return 0;
}
