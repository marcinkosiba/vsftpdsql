#ifndef POSTLOGIN_REQUESTS_H
#define POSTLOGIN_REQUESTS_H

#include "session.h"
#include "ftpdataio.h"
#include "str.h"
#include "request_results.h"

const char * getParentDirectoryName(const struct mystr * fullFilePath);
const char * getParentDirectoryPath(const struct mystr * fullFilePath);
struct mystr getParentDirectoryNameStr(const struct mystr * fullFilePath);
struct mystr getParentDirectoryPathStr(const struct mystr * fullFilePath);
struct Request_Result_And_Reason checkFileWriteAccess(struct vsf_session * p_sess, struct mystr * filename);
struct Request_Result_And_Reason vsf_request_read_directory(struct vsf_session * p_sess, const struct mystr* parentPath, const struct mystr* name);
struct Request_Result_And_Reason vsf_request_read_file(struct vsf_session * p_sess, const struct mystr* path, const struct mystr* name);
struct Request_Result_And_Reason vsf_request_write_directory(struct vsf_session * p_sess, const struct mystr* parentPath, const struct mystr* name);
struct Request_Result_And_Reason vsf_request_write_file(struct vsf_session * p_sess, const struct mystr* path, const struct mystr* name);
struct Request_Result_Homedir vsf_request_user_homedir(struct vsf_session * p_sess);

#endif //POSTLOGIN_REQUESTS_H

