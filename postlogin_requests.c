#include "postlogin_requests.h"
#include "sysstr.h"
#include "sysutil.h"
#include "vsfutils_c.h"
#include "request_c_api.h"

static void
prepend_path_to_filename_2(struct mystr* p_str, const struct mystr* path_str)
{
  static struct mystr s_tmp_str;
  /* Only prepend current working directory if the incoming filename is
   * relative
   */
  str_empty(&s_tmp_str);
  if (str_isempty(p_str) || str_get_char_at(p_str, 0) != '/')
  {
    str_copy(&s_tmp_str, path_str);
    /* Careful to not emit // if we are in directory / (common with chroot) */
    if (str_isempty(&s_tmp_str) ||
        str_get_char_at(&s_tmp_str, str_getlen(&s_tmp_str) - 1) != '/')
    {
      str_append_char(&s_tmp_str, '/');
    }
  }
  str_append_str(&s_tmp_str, p_str);
  str_copy(p_str, &s_tmp_str);
}

static void
prepend_path_to_filename(struct mystr* p_str)
{
  static struct mystr s_tmp_str;
  /* Only prepend current working directory if the incoming filename is
   * relative
   */
  str_empty(&s_tmp_str);
  if (str_isempty(p_str) || str_get_char_at(p_str, 0) != '/')
  {
    str_getcwd(&s_tmp_str);
    /* Careful to not emit // if we are in directory / (common with chroot) */
    if (str_isempty(&s_tmp_str) ||
        str_get_char_at(&s_tmp_str, str_getlen(&s_tmp_str) - 1) != '/')
    {
      str_append_char(&s_tmp_str, '/');
    }
  }
  str_append_str(&s_tmp_str, p_str);
  str_copy(p_str, &s_tmp_str);
}

const char * 
getParentDirectoryName(const struct mystr * fullFilePath) {
  struct mystr tmp = getParentDirectoryNameStr(fullFilePath);
  const char * ret = str_strdup(&tmp);
  str_free(&tmp);
  return ret;
}

const char * 
getParentDirectoryPath(const struct mystr * fullFilePath) {
  struct mystr tmp = getParentDirectoryPathStr(fullFilePath);
  const char * ret = str_strdup(&tmp);
  str_free(&tmp);
  return ret;
}

struct mystr getParentDirectoryNameStr(const struct mystr * fullFilePath) {
  struct mystr tmp = INIT_MYSTR;
  struct mystr name = INIT_MYSTR;
  str_copy(&tmp, fullFilePath);
  str_split_char_reverse(&tmp, &name, '/');
  if (str_isempty(&tmp))
  {
    str_alloc_text(&name, "/");
  }
  else
  {
    str_split_char_reverse(&tmp, &name, '/');
  }  
  str_free(&tmp);
  return name;
}

struct mystr getParentDirectoryPathStr(const struct mystr * fullFilePath) {
  struct mystr tmp = INIT_MYSTR;
  struct mystr name = INIT_MYSTR;
  str_copy(&tmp, fullFilePath);
  str_split_char_reverse(&tmp, &name, '/');
  if (str_isempty(&tmp))
  {
    str_alloc_text(&name, "/");
  }
  else
  {
    str_split_char_reverse(&tmp, &name, '/');
  }
  if (str_isempty(&tmp))
  {
    str_alloc_text(&tmp, "/");
  }
  str_free(&name);
  return tmp;
}

struct Request_Result_And_Reason 
checkFileWriteAccess(struct vsf_session * p_sess, struct mystr * filename) {
  int tmpFD = str_open(filename, kVSFSysUtilOpenReadOnly);      
  if (tmpFD != -1)
  {
    vsf_sysutil_close(tmpFD);

    {//requestWriteFile
      struct Request_Write_File request;
      struct Request_Result_And_Reason request_result;
      struct mystr dirName = INIT_MYSTR;
      populate_shared_request(&request.sharedInfo, p_sess);
      str_getcwd(&dirName);
      request.does_exist = 1;
      request.path = str_strdup(&dirName);
      request.name = str_strdup(filename);
      str_copy(&dirName, filename);
      prepend_path_to_filename(&dirName);
      request.fullName = str_strdup(&dirName);
      sanitize_path_and_name(&request.path, &request.name);
      sanitize_path(&request.fullName);
      request_result = send_request_write_File(&request);
      vsf_sysutil_free((char*)request.sharedInfo.user_name);
      vsf_sysutil_free((char*)request.fullName);
      vsf_sysutil_free((char*)request.name);
      vsf_sysutil_free((char*)request.path);
      str_free(&dirName);
      if (request_result.result)
      {
        return request_result;
      }
      else
      {
        doneWith_Result_And_Reason(&request_result);
      }
    }//requestWriteFile
  }
  {//requestWriteDir
    struct Request_Write_Directory request;
    struct Request_Result_And_Reason request_result;
    struct mystr fullFileName = INIT_MYSTR;
    const char * toSanitize;
    populate_shared_request(&request.sharedInfo, p_sess);
    str_copy(&fullFileName, filename);
    prepend_path_to_filename(&fullFileName);
    toSanitize = str_strdup(&fullFileName);
    sanitize_path(&toSanitize);
    str_alloc_text(&fullFileName, toSanitize);    
    request.name = getParentDirectoryName(&fullFileName);
    request.parentPath = getParentDirectoryPath(&fullFileName);
    request.does_exist = 1;
    sanitize_path_and_name(&request.parentPath, &request.name);
    request_result = send_request_write_Directory(&request);
    vsf_sysutil_free((char*)request.sharedInfo.user_name);
    vsf_sysutil_free((char*)request.name);
    vsf_sysutil_free((char*)request.parentPath);
    str_free(&fullFileName);
    vsf_sysutil_free((char*)toSanitize);
    return request_result;
  }//requestWriteDir
  
}

struct Request_Result_And_Reason
vsf_request_read_directory(struct vsf_session * p_sess, const struct mystr* parentPath, const struct mystr* name) {
  struct Request_Read_Directory request;
  struct Request_Result_And_Reason request_result;
  populate_shared_request(&request.sharedInfo, p_sess);
  request.name = str_strdup(name);
  request.parentPath = str_strdup(parentPath);
  sanitize_path_and_name(&request.parentPath, &request.name);
  request_result = send_request_read_Directory(&request);
  vsf_sysutil_free((char*)request.parentPath);
  vsf_sysutil_free((char*)request.name);
  vsf_sysutil_free((char*)request.sharedInfo.user_name);
  return request_result;
}

struct Request_Result_And_Reason
vsf_request_write_directory(struct vsf_session * p_sess, const struct mystr* parentPath, const struct mystr* name) {
  struct Request_Write_Directory request;
  struct Request_Result_And_Reason request_result;
  populate_shared_request(&request.sharedInfo, p_sess);
  {
    struct mystr dirFull = INIT_MYSTR;
    str_copy(&dirFull, name);
    prepend_path_to_filename_2(&dirFull, parentPath);
    struct vsf_sysutil_dir * pDir = str_opendir(&dirFull);
    if (pDir)
    {    
      request.does_exist = 1;
      vsf_sysutil_free(pDir);
    }
    else
    {
      request.does_exist = 0;
    }
    str_free(&dirFull);
  }  
  request.name = str_strdup(name);
  request.parentPath = str_strdup(parentPath);
  sanitize_path_and_name(&request.parentPath, &request.name);
  request_result = send_request_write_Directory(&request);
  vsf_sysutil_free((char*)request.name);
  vsf_sysutil_free((char*)request.parentPath);
  vsf_sysutil_free((char*)request.sharedInfo.user_name);
  return request_result;
}

struct Request_Result_And_Reason
vsf_request_read_file(struct vsf_session * p_sess, const struct mystr* path, const struct mystr* name) {
  struct Request_Read_File request;
  struct vsf_sysutil_statbuf * statbuf = 0;
  struct mystr fullFilePath = INIT_MYSTR;
  struct Request_Result_And_Reason request_result;
  int opened_file = str_open(name, kVSFSysStrOpenReadOnly);
  populate_shared_request(&request.sharedInfo, p_sess);
  vsf_sysutil_fstat(opened_file, &statbuf);
  request.size = vsf_sysutil_statbuf_get_size(statbuf);
  request.name = str_strdup(name);
  request.path = str_strdup(path);
  str_copy(&fullFilePath, name);
  prepend_path_to_filename_2(&fullFilePath, path);
  request.fullName = str_strdup(&fullFilePath);
  sanitize_path_and_name(&request.path, &request.name);
  sanitize_path(&request.fullName);
  request_result = send_request_read_File(&request);
  vsf_sysutil_free(statbuf);
  vsf_sysutil_free((char*)request.fullName);
  vsf_sysutil_free((char*)request.name);
  vsf_sysutil_free((char*)request.path);
  vsf_sysutil_free((char*)request.sharedInfo.user_name);
  str_free(&fullFilePath);
  vsf_sysutil_close(opened_file);
  return request_result;
}

struct Request_Result_And_Reason
vsf_request_write_file(struct vsf_session * p_sess, const struct mystr* path, const struct mystr* name) {
  struct Request_Write_File request;
  struct Request_Result_And_Reason request_result;
  struct mystr fullFilePath = INIT_MYSTR;
  str_copy(&fullFilePath, name);
  prepend_path_to_filename_2(&fullFilePath, path);
  populate_shared_request(&request.sharedInfo, p_sess);
  {
    int tmpFD = str_open(&fullFilePath,kVSFSysUtilOpenReadOnly);      
    if (tmpFD == -1)
    {
      request.does_exist = 0;
    }
    else 
    {
      request.does_exist = 1;
      vsf_sysutil_close(tmpFD);
    }
  }
  request.name = str_strdup(name);
  request.path = str_strdup(path);
  request.fullName = str_strdup(&fullFilePath);
  sanitize_path_and_name(&request.path, &request.name);
  sanitize_path(&request.fullName);
  request_result = send_request_write_File(&request);
  vsf_sysutil_free((char*)request.fullName);
  vsf_sysutil_free((char*)request.name);
  vsf_sysutil_free((char*)request.path);
  vsf_sysutil_free((char*)request.sharedInfo.user_name);
  str_free(&fullFilePath);
  return request_result;
}

struct Request_Result_Homedir vsf_request_user_homedir(struct vsf_session * p_sess) {
  struct Request_Homedir request;
  struct Request_Result_Homedir request_result = Request_Result_Homedir_Init;
  populate_shared_request(&request.sharedInfo, p_sess);
  request_result = send_request_homedir(&request);
  vsf_sysutil_free((char*)request.sharedInfo.user_name);
  return request_result;
}

