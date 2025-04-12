#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "server.h"

int
setnonblocking(int sockfd)
{
	int result, flags;
	flags = fcntl(sockfd, F_GETFL, 0);
	result = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
	return result;
}

int
setup_sock(int port)
{
	int sockfd, rv;
	struct sockaddr_in addr;
	int yes = 1;

	(void)memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		(void)printf("socket(2) failed: %d\n", errno);
		return -1;
	}

	rv = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (rv == -1) {
		(void)printf("setsockopt(2) failed: %d\n", errno);
		return -1;
	}

	rv = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if (rv == -1) {
		(void)printf("bind(2) failed: %d\n", errno);
		return -1;
	}

	rv = listen(sockfd, BACKLOG);
	if (rv == -1) {
		(void)printf("listen(2) failed: %d\n", errno);
		return -1;
	}

	rv = setnonblocking(sockfd);
	if (rv == -1) {
		(void)printf("fcntl(2) failed: %d\n", errno);
		return -1;
	}
	return sockfd;
}
