#ifndef CONN_H
#define CONN_H

int conn(struct pollfd *fds, int nfds, int i, int sockfd);
void disconn(struct pollfd *fds, int nfds, int i);

#endif
