#ifndef EVENTS_H
#define EVENTS_H

int conn(struct pollfd *fds, int *nfds, int *sockfd, int i);
void disconn(struct pollfd *fds, int *nfds, int i);
void broadcast(struct pollfd *fds, int *nfds, int senderfd,
               const char *msg, int len);
int poll_events(struct pollfd *fds, int *nfds, int *sockfd);

#endif
