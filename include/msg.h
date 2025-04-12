#ifndef MSG_H
#define MSG_H

void broadcast(int sender, struct pollfd *fds, int nfds, char *msg, int len);

#endif
