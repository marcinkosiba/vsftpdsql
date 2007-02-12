#include "request.h"
#include "udsrf.h"
#include "exceptions.h"
#include "plugin_architecture_api.h"
#include "plugin_architecture_interface.h"
#include <stdio.h>
#include <sys/time.h>

#define request_handler_header(retype, fun_name, struct_name) C_COMPATIBLE_FUNCTION retype send_request_##fun_name (struct Request_##struct_name * request)

#define gen_read_member(membername) void Request##membername ::readFromSock(int sock) { recvData(sock, i_struct); }

#define getHandled(requestname) UnknownObject Request##requestname ::getHandled(VSFPlugin & plugin) { \
                                  return UnknownObject(plugin.handleRequest(*this)); \
                                }

#define gen_funcs(fname, sname) getHandled(sname) \
                                gen_read_member(sname)

#define send_request_struct(sname) sendData(plugin_architecture_get_slave_socket_fast(), (int)Request_Number_##sname); \
                                   sendData(plugin_architecture_get_slave_socket_fast(), Request_Number_##sname); \
                                   sendData(plugin_architecture_get_slave_socket_fast(), *request);


#define return_result(stype)       stype retval; \
                                   std::auto_ptr<UnknownObject> uData = UnknownObject::readFromSocket(plugin_architecture_get_slave_socket_fast()); \
                                   deserialize(std::pair<char*, int>((char*)uData->getData(),uData->getDataSize()), retval); \
                                   return retval;

template <typename T> void deleteStruct(T & to_delete) {}

#include "request_auto.h"

request_handler_header(struct Request_Result, login, Login) {
  send_request_struct(Login)  
  return_result(struct Request_Result)
}

request_handler_header(struct Request_Result_Homedir, homedir, Homedir) {
  send_request_struct(Homedir)
  return_result(struct Request_Result_Homedir)
}

request_handler_header(struct Request_Result_And_Reason, read_File, Read_File) {
  send_request_struct(ReadFile)
  return_result(struct Request_Result_And_Reason)
}

request_handler_header(struct Request_Result_And_Reason, read_Directory, Read_Directory) {
  send_request_struct(ReadDirectory)
  return_result(struct Request_Result_And_Reason)
}

request_handler_header(struct Request_Result_And_Reason, write_File, Write_File) {
  send_request_struct(WriteFile)
  return_result(struct Request_Result_And_Reason)
}

request_handler_header(struct Request_Result_And_Reason, write_Directory, Write_Directory) {
  send_request_struct(WriteDirectory)
  return_result(struct Request_Result_And_Reason)
}

struct vsf_sysutil_sockaddr
{
  union
  {
    struct sockaddr u_sockaddr;
    struct sockaddr_in u_sockaddr_in;
    struct sockaddr_in6 u_sockaddr_in6;
  } u;
};

C_COMPATIBLE_FUNCTION void populate_shared_request(struct Request_Shared * request, struct vsf_session* p_sess) {
  if (request == 0 || p_sess == 0)
  {
    throw NullPointerException();
  }
  memcpy(&request->remote_ip, &p_sess->p_remote_addr->u.u_sockaddr_in, sizeof(request->remote_ip));
  request->user_name = const_cast<char*>(str_strdup(&p_sess->plugin_arch_user_str));
  gettimeofday(&request->timestamp,0);
}

C_COMPATIBLE_FUNCTION void populate_shared_request_for_connection(struct Request_Shared * request, struct vsf_session* p_sess) {
  if (request == 0 || p_sess == 0)
  {
    throw NullPointerException();
  }
  memcpy(&request->remote_ip, &p_sess->p_remote_addr->u.u_sockaddr_in, sizeof(request->remote_ip));
  request->user_name = "<unconnected>";
  gettimeofday(&request->timestamp,0);
}

RequestLogin::~RequestLogin() {
        deleteStruct(i_struct);
}
RequestHomedir::~RequestHomedir() {
        deleteStruct(i_struct);
}
RequestReadFile::~RequestReadFile() {
        deleteStruct(i_struct);
}
RequestReadDirectory::~RequestReadDirectory() {
        deleteStruct(i_struct);
}
RequestWriteFile::~RequestWriteFile() {
        deleteStruct(i_struct);
}
RequestWriteDirectory::~RequestWriteDirectory() {
        deleteStruct(i_struct);
}

