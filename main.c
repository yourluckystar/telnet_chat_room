#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include "sock.h"
#include "events.h"
#include "server.h"

int
main(void)
{
        int    rv;                  /* return value */
	int    sockfd;              /* server socket file descriptor */
	int    nfds = 1;            /* set number of fds to 1 */
	struct pollfd fds[MAX_FDS]; /* poll array of all structs */

	rv = setup_sock(PORT, &sockfd);
	if (rv == -1)
        {
		exit(EXIT_FAILURE);
        }

	(void)printf("listening on port %d\n", PORT);

        memset(fds, 0, sizeof(fds));
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;

	for (;;)
        {
                /* -1 disables the poll timeout */
		rv = poll(fds, nfds, -1);
		if (rv < 0)
                {
			perror("poll(2) failed");
			continue;
		}

                /* if a file descriptor is ready we can
                 * loop through each fd to find it */
                rv = poll_events(fds, &nfds, &sockfd);
                if (rv == -1)
                {
                        continue;
                }
        }

	(void)close(sockfd);
	return 0;
}
