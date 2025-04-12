#include <sys/socket.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include "server.h"

int
conn(struct pollfd *fds, int nfds, int i, int sockfd)
{
	if (fds[i].fd == sockfd)
		sockfd = accept(sockfd, NULL, NULL);
	
	if (!(sockfd >= 0 && nfds < MAX_FDS))
		return -1;
	
	fds[nfds].fd = sockfd;
	fds[nfds].events = POLLIN;

	return 0;
}

void
disconn(struct pollfd *fds, int nfds, int i)
{
	(void)printf("fd %d disconnected\n", fds[i].fd);
	close(fds[i].fd);
	fds[i] = fds[nfds - 1];
}
