#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
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
setup_sock(int port,
           int *sockfd)
{
        struct sockaddr_in6 addr;
	int rv;
	int yes = 1;
        int no = 0;

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(port);
	addr.sin6_addr = in6addr_any;

	*sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (*sockfd == -1)
        {
		(void)printf("socket(2) failed: %d\n", errno);
		return -1;
	}

	rv = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (rv == -1)
        {
		(void)printf("setsockopt(2) failed: %d\n", errno);
		return -1;
	}
        rv = setsockopt(*sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));
        if (rv == -1)
        {
                (void)printf("setsockopt(2) failed 2: %d\n", errno);
                return -1;
        }

	rv = bind(*sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if (rv == -1)
        {
		(void)printf("bind(2) failed: %d\n", errno);
                (void)close(*sockfd);
		return -1;
	}

	rv = listen(*sockfd, BACKLOG);
	if (rv == -1)
        {
		(void)printf("listen(2) failed: %d\n", errno);
                (void)close(*sockfd);
		return -1;
	}

	rv = setnonblocking(*sockfd);
	if (rv == -1)
        {
		(void)printf("fcntl(2) failed: %d\n", errno);
		return -1;
	}

	return 0;
}
