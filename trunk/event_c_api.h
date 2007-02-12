#ifndef H_EVENT_C_API
#define H_EVENT_C_API

#include "cpp_to_c_helpers.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "session.h"

static const int Event_Identifier = 0x01000000;

enum Event_Number_Enum {
  Event_Number_Identifier = 0x01000000,
  Event_Number_Login,
  Event_Number_FailedLogin,
  Event_Number_Download,
  Event_Number_Upload,
  Event_Number_Mkdir,
  Event_Number_Connection,
  Event_Number_Delete,
  Event_Number_Rename,
  Event_Number_Rmdir,
  Event_Number_Chmod,
  Event_Number_CliEnd,
  Event_Number_ServQuit,
  Event_Number_Shared,
  Event_Number_MaxNumber //do not remove, this **must** be the last enum
};

/**
 * shared part of all events
 */
struct Event_Shared {
  struct timeval timestamp;  // time of the event
  struct sockaddr_in remote_ip; // the ip of the remote host
  const char * user_name; // the user name the remote host has logged in as. If the user is not logged in this is set to "<unconnected>"
};

/**
 * shared part of all file-related events
 */
struct Event_FileData_Shared {
  const char * path; // the directory the file resides in
  const char * name; // file name
  unsigned int size; // file size in bytes
};

/**
 * sent when a successful login occurs
 */
struct Event_Login {  
  struct Event_Shared sharedInfo;
  int is_anonymous; // is the login anonymous
};

/**
 * sent when a failed login occurs
 */
struct Event_FailedLogin {
  struct Event_Shared sharedInfo;
  const char * failed_password; // password the user used and failed
};

/**
 * sent when a download occurs, complete or not
 */
struct Event_Download {
  struct Event_Shared sharedInfo;  
  struct Event_FileData_Shared fileInfo;
  int is_complete; // was the download complete, -1 when permission was denied, 0 for success non-zero for failure
};

/**
 * sent when an upload occurs
 */
struct Event_Upload {
  struct Event_Shared sharedInfo;
  struct Event_FileData_Shared fileInfo;
  int is_complete; // was the upload complete, -1 when permission was denied
  int is_append; // was the data appended to an existing file
  int is_overwrite; // did the upload cause a file to be overwritten
};

/**
 * sent when the user tries to create a directory
 */
struct Event_Mkdir {
  struct Event_Shared sharedInfo;
  const char * path; //path of the directory the user wanted to create
  int succeeded; //did the operation succeed (-1 if because of request failure), 0 for success non-zero for failure
};

/**
 * sent when a connection request was rejected
 */
struct Event_Connection {
  struct Event_Shared sharedInfo;
  const char * reason;
};

/**
 * sent when the user tries to delete a file
 */
struct Event_Delete {
  struct Event_Shared sharedInfo;
  const char * path; //path of the file the user wanted to delete
  int succeeded; //did the operation succeed (-1 if because of request failure), 0 for success non-zero for failure
};

/**
 * sent when the user tries to rename a file
 */
struct Event_Rename {
  struct Event_Shared sharedInfo;
  const char * old_name; // the old file name (path prepended)
  const char * new_name; // the new file name (path prepended)
  int succeeded; //did the operation succeed (-1 if because of request failure), 0 for success non-zero for failure
};

/**
 * sent when the user tries to remove a directory
 */
struct Event_Rmdir {
  struct Event_Shared sharedInfo;
  const char * path; //path of the directory the user wanted to remove
  int succeeded; //did the operation succeed (-1 if because of request failure), 0 for success non-zero for failure
};

/**
 * sent when the user tries to change the mode of a file
 */
struct Event_Chmod {
  struct Event_Shared sharedInfo;
  const char * path;
  const char * name;
  const char * new_attrib;
  int succeeded; //did the operation succeed (-1 if because of request failure), 0 for success non-zero for failure
};

/**
 * sent when a client is going down
 */
struct Event_CliEnd {
  struct timeval timestamp;  // time of the event
};

/**
 * sent when the whole server is going down, internal purposes only
 * __WARNING__ the event handler for this event is not virtual,
 * if you do want to do anything when the server is going down
 * put that into the VSFPlugin destructor.
 */

struct Event_ServQuit {
};

/** send_event_login()
 * PURPOSE
 *  Notifies plugins that a login has occured
 * PARAMETERS
 *  event     - the login parameters
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_login(struct Event_Login * event);

/** send_event_download()
 * PURPOSE
 *  Notifies plugins that a download has started
 * PARAMETERS
 *  event     - the login parameters
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_download(struct Event_Download * event);

/** send_event_sigEnd()
 * PURPOSE
 *  Notifies plugins that the main process is about to quit (after this it will still request a quit)
 * PARAMETERS
 *  event     - the quit params
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_cliEnd(struct Event_CliEnd * event);

/** send_event_failedLogin()
 * PURPOSE
 *  Notifies plugins that there was a failed login attempt
 * PARAMETERS
 *  event     - the failed login information
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_failedLogin(struct Event_FailedLogin * event);

/** send_event_upload()
 * PURPOSE
 *  Notifies plugins that an upload has completed
 * PARAMETERS
 *  event     - the completed upload stats
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_upload(struct Event_Upload * event);

/** send_event_mkdir()
 * PURPOSE
 *  Notifies plugins that a directory has been created
 * PARAMETERS
 *  event     - the created dir name and stats
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_mkdir(struct Event_Mkdir * event);

/** send_event_connection()
 * PURPOSE
 *  Notifies plugins that a connection has been refused
 * PARAMETERS
 *  event     - the refused connection info (reason, ip etc.)
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_connection(struct Event_Connection * event);

/** send_event_delete()
 * PURPOSE
 *  Notifies plugins that a file has been deleted
 * PARAMETERS
 *  event     - the deleted files info and user info
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_delete(struct Event_Delete * event);

/** send_event_rename()
 * PURPOSE
 *  Notifies plugins that a file or dir has been renamed
 * PARAMETERS
 *  event     - the old and new names
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_rename(struct Event_Rename * event);

/** send_event_rmdir()
 * PURPOSE
 *  Notifies plugins that a directory has been removed
 * PARAMETERS
 *  event     - the removed dirs path and stats
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_rmdir(struct Event_Rmdir * event);

/** send_event_chmod()
 * PURPOSE
 *  Notifies plugins that a file/dir attribute has been changed
 * PARAMETERS
 *  event     - the file/dir old an new stats
 *  p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void send_event_chmod(struct Event_Chmod * event);

/** send_event_servQuit()
 * PURPOSE
 *  __internal__
 *  On server shutdown progresses internal loops
 */
C_COMPATIBLE_FUNCTION void send_event_servQuit(struct Event_ServQuit * event);

/** populate_shared_event
 *  PURPOSE
 *    Copies appropriate values of the session to the Event_Shared structure
 *  PARAMETERS
 *   event - shared structure
 *   p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void populate_shared_event(struct Event_Shared * event, struct vsf_session* p_sess);

/** populate_shared_event_for_connection
 *  PURPOSE
 *    Copies appropriate values of the session to the Event_Shared structure, but leaves name as "<unconnected>"
 *  PARAMETERS
 *   event - shared structure
 *   p_sess    - current session object
 */
C_COMPATIBLE_FUNCTION void populate_shared_event_for_connection(struct Event_Shared * event, struct vsf_session* p_sess);
#endif //H_EVENT_C_API
