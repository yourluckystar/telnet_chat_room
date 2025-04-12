#include <sys/socket.h>
#include <poll.h>

void
broadcast(int sender, struct pollfd *fds, int nfds, char *msg, int len)
{
	int i;
	for (i = 0; i < nfds; ++i) {
		/* skip the sender and listening sockets
		 * but send to everyone else */
		if (fds[i].fd == sender)
			continue;

		if (fds[i].fd == fds[0].fd)
			continue;

		send(fds[i].fd, msg, len, 0);
	}
}
