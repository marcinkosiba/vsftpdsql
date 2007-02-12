#include "plugin_architecture_interface.h"
#include "event_c_api.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <exception>
#include <unistd.h>
#include "slave_thread.h"
#include "udsrf.h"
#include <signal.h>
#include <iostream>
#include <set>
#include "request.h"
#include <fstream>
#include <linux/capability.h>

extern "C" void die(const char* p_text);
extern "C" int vsf_sysutil_fork();
extern "C" void handle_slave_signal();

static int slave_socket_fast = 0;
static int slave_socket_slow = 0;
int slave_pid = -1;

struct sockaddr_un fast_un;
struct sockaddr_un slow_un;

void handle_unexpected_exception() {  
  handle_slave_signal();

  die("Unexpected exception\n");
}

void handleSignal(int a) {
  handle_slave_signal();

  char erc[100];
  sprintf(erc,"got a signal (%s) :(\n", strsignal(a));  
  die(erc);
}

C_COMPATIBLE_FUNCTION void vsf_plugin_architecture_start(struct vsf_session* p_sess) {
  static bool started = false;
  if (started)
  {
    return;
  }
  std::set_unexpected(handle_unexpected_exception);

  fast_un.sun_family = PF_UNIX;
  fast_un.sun_path[0] = 0;
  slow_un.sun_family = PF_UNIX;
  slow_un.sun_path[0] = 0;

  srandom(time(0));
  sprintf(fast_un.sun_path + 1, "vsftp_comm_fast%ld", random());
  sprintf(slow_un.sun_path + 1, "vsftp_comm_slow%ld", random());

  int slaveno;
  // create slave

  switch(slaveno = vsf_sysutil_fork()) {
  case -1:
    die("fork did not succeed");
    break;
  case 0:
    {        
      pid_t parent_pid = getppid();

      slave_thread_start(slow_un, fast_un); //should never return
      printf("slave thread died\n");
      if (parent_pid != 1)
      {
        kill(parent_pid, SIGUSR2);
      }
      printf("killing parent\n");
      exit(0);
    }
    break;
  default:    
    slave_pid = slaveno;

    sleep(1);

    for(int i = 0; i < 3; ++i) {
      if(reconnect_after_fork()) {
        printf("trying to connect to plugin arch thread\n");
        sleep(1);
      }
      else {
        printf("connect ok\n");
        break;
      }
    }

    if (slave_socket_fast == 0 || slave_socket_slow == 0)
    {
      die("connection to plugin architecture thread failed");
    }

    signal(SIGINT, handleSignal);
    signal(SIGUSR2, handleSignal);

    break;
  }
}

C_COMPATIBLE_FUNCTION int plugin_architecture_get_slave_socket_fast() {
  return slave_socket_fast;
}

C_COMPATIBLE_FUNCTION int plugin_architecture_get_slave_socket_slow() {
  return slave_socket_slow;
}

C_COMPATIBLE_FUNCTION int reconnect_after_fork() {
  slave_socket_fast = socket(PF_UNIX, SOCK_SEQPACKET, 0);
  slave_socket_slow = socket(PF_UNIX, SOCK_SEQPACKET, 0);

  if(connect(slave_socket_fast, (struct sockaddr*)&fast_un, sizeof(fast_un))){
    goto failed;
  }
  if(connect(slave_socket_slow, (struct sockaddr*)&slow_un, sizeof(slow_un))){
    goto failed;
  }
  return 0;
failed:
  close(slave_socket_fast);
  close(slave_socket_slow);
  slave_socket_fast = 0;
  slave_socket_slow = 0;
  return -1;
}
