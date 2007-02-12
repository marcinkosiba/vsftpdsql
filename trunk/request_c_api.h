#ifndef H_REQUEST_C_API
#define H_REQUEST_C_API

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "cpp_to_c_helpers.h"
#include "request_results.h"

static const int Request_Identifier = 0x02000000;

enum Request_Number_Enum {
  Request_Number_Identifier = 0x02000000,
  Request_Number_Login,
  Request_Number_Homedir,
  Request_Number_ReadFile,
  Request_Number_ReadDirectory,
  Request_Number_WriteFile,
  Request_Number_WriteDirectory,
  Request_Number_MaxNumber //do not remove, this **must** be the last enum
};

struct Request_Shared {
  struct timeval timestamp;  // time of the request
  struct sockaddr_in remote_ip; // the ip of the remote host
  const char * user_name; // the user name the remote host has logged in as. If the user is not logged in this is set to "<unconnected>"
};

/**
 * sent when a foreign host wants to log in as a user
 * EXPECTED RETURNVALUE
 * Request_Result_And_Reason
 * 0 or -1 in type int
 */
struct Request_Login {
  struct Request_Shared sharedInfo;
  const char * login;
  const char * password;
};

/**
 * sent when we have succesfully logged in and want to chdir
 * there
 * EXPECTED RETURNVALUE
 * Request_Result_Homedir
 */
struct Request_Homedir {
  struct Request_Shared sharedInfo;
};

/**
 * sent when a user wants to read data from a file
 * 
 * EXPECTED RETURNVALUE
 * 0 or -1 in type int
 * reason of failure in type const char *
 */
struct Request_Read_File {
  struct Request_Shared sharedInfo;
  const char * path; // directory in which the file resides
  const char * name; // the file name
  const char * fullName; // the full path - dirname prepended
  unsigned int size; // file size
};

/**
 * sent when a user wants to read data from a directory
 * 
 * EXPECTED RETURNVALUE
 * 0 or -1 in type int
 * reason of failure in type const char *
 */
struct Request_Read_Directory {
  struct Request_Shared sharedInfo;
  const char * parentPath; //path to the parent directory
  const char * name;  //name of the directory
};

/**
 * sent when a user wants to create a directory
 * 
 * EXPECTED RETURNVALUE
 * 0 or -1 in type int
 * reason of failure in type const char *
 */
struct Request_Write_Directory {
  struct Request_Shared sharedInfo;
  const char * parentPath; //path to the parent directory
  const char * name;  //name of the directory
  int does_exist;
};

/**
 * sent when a user wants to write data to a file
 * 
 * EXPECTED RETURNVALUE
 * 0 or -1 in type int
 * reason of failure in type const char *
 */
struct Request_Write_File {
  struct Request_Shared sharedInfo;
  const char * path; // directory in which the file resides
  const char * name; // the file name
  const char * fullName; // the full path - dirname prepended
  int does_exist;
};

C_COMPATIBLE_FUNCTION struct Request_Result send_request_login(struct Request_Login * request);
C_COMPATIBLE_FUNCTION struct Request_Result_Homedir send_request_homedir(struct Request_Homedir * request);
C_COMPATIBLE_FUNCTION struct Request_Result_And_Reason send_request_read_File(struct Request_Read_File * request);
C_COMPATIBLE_FUNCTION struct Request_Result_And_Reason send_request_read_Directory(struct Request_Read_Directory * request);
C_COMPATIBLE_FUNCTION struct Request_Result_And_Reason send_request_write_Directory(struct Request_Write_Directory * request);
C_COMPATIBLE_FUNCTION struct Request_Result_And_Reason send_request_write_File(struct Request_Write_File * request);
C_COMPATIBLE_FUNCTION void populate_shared_request(struct Request_Shared * request, struct vsf_session* p_sess);
C_COMPATIBLE_FUNCTION void populate_shared_request_for_connection(struct Request_Shared * request, struct vsf_session* p_sess);

#endif //H_REQUEST_C_API
