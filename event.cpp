#include "event.h"
#include "udsrf.h"
#include "plugin_architecture_interface.h"
#include "plugin_architecture_api.h"
#include <sys/time.h>

#define send_event_function(fun_name, struct_name) C_COMPATIBLE_FUNCTION void send_event_##fun_name (struct Event_##struct_name * event) { \
  sendData(plugin_architecture_get_slave_socket_slow(), (int)Event_Number_##struct_name); \
  sendData(plugin_architecture_get_slave_socket_slow(), Event_Number_##struct_name); \
  sendData(plugin_architecture_get_slave_socket_slow(), *event); \
}

#define send_event_double_function(fun_name, struct_name) C_COMPATIBLE_FUNCTION void send_event_##fun_name (struct Event_##struct_name * event) { \
  sendData(plugin_architecture_get_slave_socket_fast(), (int)Event_Number_##struct_name); \
  sendData(plugin_architecture_get_slave_socket_fast(), Event_Number_##struct_name); \
  sendData(plugin_architecture_get_slave_socket_fast(), *event); \
  sendData(plugin_architecture_get_slave_socket_slow(), (int)Event_Number_##struct_name); \
  sendData(plugin_architecture_get_slave_socket_slow(), Event_Number_##struct_name); \
  sendData(plugin_architecture_get_slave_socket_slow(), *event); \
}

#define getHandled(eventname) void Event##eventname ::getHandled(VSFPlugin & plugin) { \
                                plugin.handleEvent(*this); \
                              }

#define create_read_member(membername) void Event##membername ::readFromSock(int sock) { recvData(sock, i_struct); }

#define gen_funcs(fname, sname) send_event_function(fname, sname) \
                                getHandled(sname) \
                                create_read_member(sname)

#define gen_funcs_double(fname, sname) send_event_double_function(fname, sname) \
                                getHandled(sname) \
                                create_read_member(sname)

template <typename T> void deleteStruct(T & to_delete) {}

#include "event_auto.h"

struct vsf_sysutil_sockaddr
{
  union
  {
    struct sockaddr u_sockaddr;
    struct sockaddr_in u_sockaddr_in;
    struct sockaddr_in6 u_sockaddr_in6;
  } u;
};

C_COMPATIBLE_FUNCTION void populate_shared_event(struct Event_Shared * event, struct vsf_session* p_sess) {
  if (event == 0 || p_sess == 0)
  {
    throw NullPointerException();
  }
  memcpy(&event->remote_ip, &p_sess->p_remote_addr->u.u_sockaddr_in, sizeof(event->remote_ip));
  event->user_name = const_cast<char*>(str_strdup(&p_sess->plugin_arch_user_str));
  gettimeofday(&event->timestamp,0);
}

C_COMPATIBLE_FUNCTION void populate_shared_event_for_connection(struct Event_Shared * event, struct vsf_session* p_sess) {
  if (event == 0 || p_sess == 0)
  {
    throw NullPointerException();
  }
  memcpy(&event->remote_ip, &p_sess->p_remote_addr->u.u_sockaddr_in, sizeof(event->remote_ip));
  event->user_name = "<unconnected>";
  gettimeofday(&event->timestamp,0);
}

EventLogin::~EventLogin() {
	deleteStruct(i_struct);
}
EventFailedLogin::~EventFailedLogin() {
	deleteStruct(i_struct);
}
EventDownload::~EventDownload() {
	deleteStruct(i_struct);
}
EventUpload::~EventUpload() {
	deleteStruct(i_struct);
}
EventMkdir::~EventMkdir() {
	deleteStruct(i_struct);
}
EventConnection::~EventConnection() {
	deleteStruct(i_struct);
}
EventDelete::~EventDelete() {
	deleteStruct(i_struct);
}
EventRename::~EventRename() {
	deleteStruct(i_struct);
}
EventRmdir::~EventRmdir() {
	deleteStruct(i_struct);
}
EventChmod::~EventChmod() {
	deleteStruct(i_struct);
}
EventCliEnd::~EventCliEnd() {
	deleteStruct(i_struct);
}
EventServQuit::~EventServQuit() {
	deleteStruct(i_struct);
}

