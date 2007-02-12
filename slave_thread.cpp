#include "slave_thread.h"
#include "udsrf.h"
#include <signal.h>
#include "plugin_architecture_api.h"
#include "plugin_architecture_interface.h"
#include <pthread.h>
#include <memory>
#include "file_logger_plugin.h"
#include <iostream>
#include <vector>
#include "mysql_plugin.h"
#include "pgsql_plugin.h"
#include "tunables.h"
#include "slave_thread_auto.h"

pthread_t thread_slow;
pthread_t thread_fast;

pthread_cond_t wait_for_plugins = PTHREAD_COND_INITIALIZER;
pthread_mutex_t wait_for_plugins_mutex = PTHREAD_MUTEX_INITIALIZER;
bool plugin_created = false;

static volatile bool slow_runs = true;
static volatile bool fast_runs = true;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile int slow_threads = 0;
static volatile int fast_threads = 0;

static std::unexpected_handler old_unexpected_handler;
static std::terminate_handler old_terminate_handler;

VSFPluginArchitectureSingleton pluginArchitecture;

static void * slow_event_loop(void *);
static void * fast_event_loop(void *);
static void *slow_listener(void * v_un);
static void *fast_listener(void * v_un);
extern "C" void handle_slave_signal();
void handle_signal(int);
static void handle_unexpected_exception();
static void handle_terminate();
void slave_thread_start(struct sockaddr_un server_address_slow, struct sockaddr_un server_address_fast);

extern "C" void handle_slave_signal() {
  slow_runs = false;
  fast_runs = false;

  struct Event_ServQuit sigend;
  try {
    send_event_servQuit(&sigend);
  } catch (...) {} //can happen you know..
}

void handle_signal(int) {
  handle_slave_signal();
}

static void handle_unexpected_exception() {
  try {
    throw;
  } catch (std::exception & e) {
    cerr << "Caught an exception (" << typeid(e).name() << ")\n";
  }
  catch (...) {
    cerr << "Caught an unknown exception\n";
  }  
  printf("unexpected exception!!\n");
  handle_slave_signal();
  old_unexpected_handler();
}

static void handle_terminate() {
  try {
    throw;
  } catch (std::exception & e) {
    cerr << "Caught an exception (" << typeid(e).name() << ")\n";
  }
  catch (...) {
    cerr << "Caught an unknown exception\n";
  }  
  printf("unexpected terminate!!\n");
  handle_slave_signal();
  old_terminate_handler();
}

void slave_thread_start(struct sockaddr_un server_address_slow, struct sockaddr_un server_address_fast) {
  std::auto_ptr<VSFPlugin> currentPlugin;

  signal(SIGHUP, handle_signal);
  signal(SIGINT, handle_signal);
  signal(SIGQUIT, handle_signal);  

  old_unexpected_handler = std::set_unexpected(handle_unexpected_exception);
  old_terminate_handler = std::set_terminate(handle_terminate);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_mutex_init(&count_mutex, 0);

  if (pthread_create(&thread_slow,&attr,slow_listener,&server_address_slow))
  {
    printf("pthread_create(&thread_slow,&attr,slow_listener,&server_address_slow) failed!!\n");
    exit(0);
  }
  if (pthread_create(&thread_fast,&attr,fast_listener,&server_address_fast))
  {
    printf("pthread_create(&thread_fast,&attr,fast_listener,&server_address_fast) failed!!\n");
    exit(0);
  }

  currentPlugin = createCurrentSQLPlugin(pluginArchitecture);

  pthread_mutex_lock(&wait_for_plugins_mutex);
  plugin_created = true;
  pthread_cond_broadcast(&wait_for_plugins);
  pthread_mutex_unlock(&wait_for_plugins_mutex);

  while(slow_runs || fast_runs || slow_threads > 0 || fast_threads > 0) {
    sleep(1);
  }

  pthread_mutex_destroy(&count_mutex);
}

static void *slow_listener(void * v_un) {
    struct sockaddr_un * slow_un = ((struct sockaddr_un*)v_un);
    int socket_to_main_slow;

    printf("trying to create a PF_UNIX, SOCK_SEQPACKET socket\n");

    socket_to_main_slow = socket(PF_UNIX, SOCK_SEQPACKET, 0);

    printf("slow listener at work\n");

    if (bind(socket_to_main_slow, (struct sockaddr*)slow_un, sizeof(struct sockaddr_un)))
    {
      printf("bind error\n");
      perror(0);
      pthread_exit(0);
    } else {
      printf("bind ok\n");
    }
    if (listen(socket_to_main_slow,20))
    {
      printf("listen error\n");
      perror(0);
      pthread_exit(0);
    } else {
      printf("listen ok\n");
    }

    pthread_mutex_lock(&wait_for_plugins_mutex);
    if (!plugin_created)
      pthread_cond_wait(&wait_for_plugins, &wait_for_plugins_mutex);
    pthread_mutex_unlock(&wait_for_plugins_mutex);

    printf("<slow listener> entering accept loop\n");

    while(slow_runs)
    {
      int * newSock = new int;
      socklen_t sockaddr_len = 0;
      struct sockaddr_un addr;
      *newSock = accept(socket_to_main_slow, (struct sockaddr*)&addr, &sockaddr_len);      
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      struct sched_param sparam;
      sparam.sched_priority = 0;
      pthread_attr_setschedparam(&attr, &sparam);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

      if (*newSock == -1)
      {
        printf("accept error\n");
        perror(0);
        pthread_exit(0);
      }

      pthread_t thread;      
      if(pthread_create(&thread, &attr, slow_event_loop, newSock) == 0)
      {         
        pthread_mutex_lock(&count_mutex);
        slow_threads++;
        pthread_mutex_unlock(&count_mutex);
      }      
    }
    pthread_exit(0);
}

static void *fast_listener(void * v_un) {
  struct sockaddr_un * fast_un = ((struct sockaddr_un*)v_un);
  int socket_to_main_fast;

  printf("trying to create a PF_UNIX, SOCK_SEQPACKET socket\n");

  socket_to_main_fast = socket(PF_UNIX, SOCK_SEQPACKET, 0);

  printf("fast listener at work\n");

  if (bind(socket_to_main_fast, (struct sockaddr*)fast_un, sizeof(struct sockaddr_un)))
  {
    printf("bind error\n");
    perror(0);
    pthread_exit(0);
  }
  if (listen(socket_to_main_fast,20))
  {
    printf("listen error\n");
    perror(0);
    pthread_exit(0);
  }

  pthread_mutex_lock(&wait_for_plugins_mutex);
  if (!plugin_created)
    pthread_cond_wait(&wait_for_plugins, &wait_for_plugins_mutex);
  pthread_mutex_unlock(&wait_for_plugins_mutex);

  printf("<fast listener> entering accept loop\n");

  while (fast_runs)
  {
    int * newSock = new int;
    socklen_t sockaddr_len = 0;
    struct sockaddr_un addr;
    *newSock = accept(socket_to_main_fast, (struct sockaddr*)&addr, &sockaddr_len);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    struct sched_param sparam;
    sparam.sched_priority = -15;
    pthread_attr_setschedparam(&attr, &sparam);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (*newSock == -1)
    {
      printf("accept error\n");
      perror(0);
      pthread_exit(0);
    }

    pthread_t thread;    
    if (pthread_create(&thread, &attr, fast_event_loop, newSock) == 0)
    {      
      pthread_mutex_lock(&count_mutex);
      fast_threads++;
      pthread_mutex_unlock(&count_mutex);
    }    
  }
  pthread_exit(0);
}

static void *slow_event_loop(void * v_sock) {
  int socket = *((int*)v_sock);
  delete (int*)v_sock;
  bool running = true;
  //proces iteratively  

  while (running)
  {
    int number;

    try {
      recvData(socket, number);
    }
    catch(SendRecvException & e) {
      printf("<slow eloop> error in recv, bye\n");
      goto end_thread;
    }

    switch (number & 0x7F000000)
    {
    case Event_Identifier: //event
      {
        std::auto_ptr<Event> event;

        event = Event::readFromSocket(socket);

        if (event->getNumber() == Event_Number_CliEnd)
        {
          running = false;
        }

        if (event->getNumber() == Event_Number_ServQuit)
        {
          running = false;
          slow_runs = false;
          fast_runs = false;
        }

        //printf("event number: %xd\n", event->getNumber());
        //printf("doing: pluginArchitecture.getVSFPluginArchitecture().handleEvent(*event); \n");
        pluginArchitecture.getVSFPluginArchitecture().handleEvent(*event);
        //printf("phew, done :)\n");
      }
      break;
    case Request_Identifier: //request
      {
        std::auto_ptr<Request> request;

        request = Request::readFromSocket(socket);

        printf("slow: request number: %xd\n", request->getNumber());
        printf("doing: pluginArchitecture.getVSFPluginArchitecture().handleRequest(*request); \n");
        pluginArchitecture.getVSFPluginArchitecture().handleRequest(*request).writeToSocket(socket);
        printf("phew, done :)\n");
      }
      break;
    default:      
      printf("unknown magic number in event/request\n");
      handle_slave_signal();
      goto end_thread;
      break;
    }//switch
  }//while

  end_thread:
    pthread_mutex_lock(&count_mutex);
    slow_threads--;
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(0);
}

static void *fast_event_loop(void * v_sock) {
  int socket = *((int*)v_sock);
  delete (int*)v_sock;
  bool running = true;

  while (running)
  {
    int number;
    try {
    recvData(socket, number);
    } catch(SendRecvException & e) {
      printf("<fast eloop> error in recv, bye\n");
      goto end_thread;
    }

    switch (number & 0x7F000000)
    {
    case Event_Identifier: //event
      {
        std::auto_ptr<Event> event;

        event = Event::readFromSocket(socket);

        if (event->getNumber() == Event_Number_CliEnd)
        {
          running = false;
          break;
        }

        if (event->getNumber() == Event_Number_ServQuit)
        {
          running = false;
          slow_runs = false;
          fast_runs = false;
          break;
        }

        //printf("event number: %xd\n", event->getNumber());
        //printf("doing: pluginArchitecture.getVSFPluginArchitecture().handleEvent(*event); \n");
        pluginArchitecture.getVSFPluginArchitecture().handleEvent(*event);
        //printf("phew, done :)\n");
      }
      break;
    case Request_Identifier: //request
      {
        std::auto_ptr<Request> request;

        request = Request::readFromSocket(socket);

        printf("fast: request number: %xd\n", request->getNumber());
        printf("doing: pluginArchitecture.getVSFPluginArchitecture().handleRequest(*request); \n");
        pluginArchitecture.getVSFPluginArchitecture().handleRequest(*request).writeToSocket(socket);
        printf("phew, done :)\n");
      }
      break;
    default:      
      printf("unknown magic number in event/request\n");
      handle_slave_signal();
      goto end_thread;
      break;
    }//switch

  }//while

  end_thread:
    pthread_mutex_lock(&count_mutex);
    fast_threads--;
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(0);
}
