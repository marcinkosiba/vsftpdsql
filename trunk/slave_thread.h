#ifndef HI_PRIO_THREAD_H
#define HI_PRIO_THREAD_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

void slave_thread_start(struct sockaddr_un server_address_slow, struct sockaddr_un server_address_fast);

#endif //HI_PRIO_THREAD_H
