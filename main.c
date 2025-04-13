#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "conn.h"
#include "msg.h"
#include "sock.h"
#include "server.h"

int
main(void)
{
	int sockfd;
	int nfds = 2;
	struct pollfd fds[MAX_FDS] = {0};
	char buf[BUF_S];

	sockfd = setup_sock(PORT);
	if (sockfd == -1)
		return -1;

	(void)printf("listening on port %d\n", PORT);

	fds[0].fd = sockfd;
	fds[0].events = POLLIN;

	for (;;) {
		int i;
		int rv = poll(fds, nfds, -1);
		if (rv < 0) {
			perror("poll failed");
			continue;
		}

		for (i = 0; i < nfds; ++i) {
			int bytes;
			if (!(fds[i].revents & POLLIN))
				continue;

			if (i == 0) {
				conn(fds, nfds, i, sockfd);
				++nfds;
				continue;
			}

			bytes = recv(fds[i].fd, buf, BUF_S - 1, 0);
			if (bytes > 0) {
				broadcast(fds[i].fd, fds, nfds, buf, bytes);
			} else if (bytes <= 0) {
				disconn(fds, nfds, i);
				--nfds;
				--i;
			}
		}
	}

	(void)close(sockfd);
	return 0;
}
