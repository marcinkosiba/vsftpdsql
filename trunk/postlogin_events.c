#include "postlogin_events.h"
#include "event_c_api.h"
#include "sysstr.h"
#include "sysutil.h"
#include "vsfutils_c.h"

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

void send_log_event_download(struct vsf_session * p_sess, struct vsf_transfer_ret * trans_ret) {
  struct Event_Download event;
  struct mystr dirPath = INIT_MYSTR;
  populate_shared_event(&event.sharedInfo, p_sess);
  event.fileInfo.name = str_strdup(&p_sess->ftp_arg_str);
  str_getcwd(&dirPath);
  event.fileInfo.path = str_strdup(&dirPath);
  event.fileInfo.size = p_sess->transfer_size;
  event.is_complete = ((trans_ret->retval == 0) ? 1 : 0);
  sanitize_path_and_name(&event.fileInfo.path, &event.fileInfo.name);
  send_event_download(&event);
  vsf_sysutil_free((char*)event.fileInfo.name);
  vsf_sysutil_free((char*)event.fileInfo.path);
  vsf_sysutil_free((char*)event.sharedInfo.user_name);
}

void send_log_event_download_failed(struct vsf_session * p_sess) {
  struct Event_Download event;
  struct mystr dirPath = INIT_MYSTR;
  populate_shared_event(&event.sharedInfo, p_sess);
  event.fileInfo.name = str_strdup(&p_sess->ftp_arg_str);
  str_getcwd(&dirPath);
  event.fileInfo.path = str_strdup(&dirPath);
  event.fileInfo.size = 0;
  event.is_complete = -1;
  sanitize_path_and_name(&event.fileInfo.path, &event.fileInfo.name);
  send_event_download(&event);
  vsf_sysutil_free((char*)event.fileInfo.name);
  vsf_sysutil_free((char*)event.fileInfo.path);
  vsf_sysutil_free((char*)event.sharedInfo.user_name);
}

void send_log_event_upload(struct vsf_session * p_sess, struct vsf_transfer_ret * trans_ret, struct mystr * p_filename, int is_append, int doesExist) {
  struct Event_Upload event;
  struct mystr dirPath = INIT_MYSTR;
  populate_shared_event(&event.sharedInfo, p_sess);
  event.fileInfo.name = str_strdup(p_filename);
  str_getcwd(&dirPath);
  event.fileInfo.path = str_strdup(&dirPath);
  event.fileInfo.size = p_sess->transfer_size;
  event.is_complete = ((trans_ret->retval == 0) ? 1 : 0);
  event.is_append = is_append;
  event.is_overwrite = doesExist;
  sanitize_path_and_name(&event.fileInfo.path, &event.fileInfo.name);
  send_event_upload(&event);
  vsf_sysutil_free((char*)event.fileInfo.name);
  vsf_sysutil_free((char*)event.fileInfo.path);
  vsf_sysutil_free((char*)event.sharedInfo.user_name);
  str_free(&dirPath);
}

void send_log_event_upload_failed(struct vsf_session * p_sess, struct mystr * p_filename, int is_append, int doesExist) {
  struct Event_Upload event;
  struct mystr dirPath = INIT_MYSTR;
  populate_shared_event(&event.sharedInfo, p_sess);
  event.fileInfo.name = str_strdup(p_filename);
  str_getcwd(&dirPath);
  event.fileInfo.path = str_strdup(&dirPath);
  event.fileInfo.size = p_sess->transfer_size;
  event.is_complete = -1;
  event.is_append = is_append;
  event.is_overwrite = doesExist;
  sanitize_path_and_name(&event.fileInfo.path, &event.fileInfo.name);
  send_event_upload(&event);
  vsf_sysutil_free((char*)event.fileInfo.name);
  vsf_sysutil_free((char*)event.fileInfo.path);
  vsf_sysutil_free((char*)event.sharedInfo.user_name);
  str_free(&dirPath);
}

void send_log_event_mkdir(struct vsf_session * p_sess, int succeeded) {
  struct Event_Mkdir event;
  struct mystr dirPath = INIT_MYSTR;
  populate_shared_event(&event.sharedInfo, p_sess);
  str_copy(&dirPath, &p_sess->ftp_arg_str);
  prepend_path_to_filename(&dirPath);
  event.path = str_strdup(&dirPath);
  event.succeeded = succeeded;
  sanitize_path(&event.path);
  send_event_mkdir(&event);
  vsf_sysutil_free((char*)event.path);
  vsf_sysutil_free((char*)event.sharedInfo.user_name);
  str_free(&dirPath);
}

void send_log_event_mkdir_failed(struct vsf_session * p_sess) {
  send_log_event_mkdir(p_sess, -1);
}

void send_log_event_rmdir(struct vsf_session * p_sess, int succeeded) {
    struct Event_Rmdir event;
    struct mystr dirPath = INIT_MYSTR;
    populate_shared_event(&event.sharedInfo, p_sess);
    str_copy(&dirPath, &p_sess->ftp_arg_str);
    prepend_path_to_filename(&dirPath);
    event.path = str_strdup(&dirPath);
    event.succeeded = succeeded;
    sanitize_path(&event.path);
    send_event_rmdir(&event);
    vsf_sysutil_free((char*)event.path);
    vsf_sysutil_free((char*)event.sharedInfo.user_name);
    str_free(&dirPath);
}

void send_log_event_rmdir_failed(struct vsf_session * p_sess) {
  send_log_event_rmdir(p_sess, -1);
}

void send_log_event_delete(struct vsf_session * p_sess, int succeeded) {
    struct Event_Delete event;
    struct mystr dirPath = INIT_MYSTR;
    populate_shared_event(&event.sharedInfo, p_sess);
    str_copy(&dirPath, &p_sess->ftp_arg_str);
    prepend_path_to_filename(&dirPath);
    event.path = str_strdup(&dirPath);
    event.succeeded = succeeded;
    sanitize_path(&event.path);
    send_event_delete(&event);
    vsf_sysutil_free((char*)event.path);
    vsf_sysutil_free((char*)event.sharedInfo.user_name);
    str_free(&dirPath);
}

void send_log_event_delete_failed(struct vsf_session * p_sess) {
  send_log_event_delete(p_sess, 0);
}

void send_log_event_rename(struct vsf_session * p_sess, int succeeded) {
    struct Event_Rename event;
    struct mystr newname = INIT_MYSTR;
    struct mystr oldname = INIT_MYSTR;
    populate_shared_event(&event.sharedInfo, p_sess);
    str_copy(&oldname, &p_sess->rnfr_filename_str);
    prepend_path_to_filename(&oldname);
    event.old_name = str_strdup(&oldname);
    str_copy(&newname, &p_sess->ftp_arg_str);
    prepend_path_to_filename(&newname);
    event.new_name = str_strdup(&newname);
    event.succeeded = succeeded;
    sanitize_path(&event.old_name);
    sanitize_path(&event.new_name);
    send_event_rename(&event);
    vsf_sysutil_free((char*)event.old_name);
    vsf_sysutil_free((char*)event.new_name);
    vsf_sysutil_free((char*)event.sharedInfo.user_name);
    str_free(&newname);
    str_free(&oldname);
}

void send_log_event_rename_failed(struct vsf_session * p_sess) {
  send_log_event_rename(p_sess, -1);
}

void send_log_event_chmod(struct vsf_session * p_sess, struct mystr * p_arg_str, struct mystr * s_chmod_file_str, int retval) {
    struct Event_Chmod event;
    struct mystr path_str = INIT_MYSTR;
    populate_shared_event(&event.sharedInfo, p_sess);
    event.new_attrib = str_strdup(p_arg_str);
    str_getcwd(&path_str);
    event.name = str_strdup(s_chmod_file_str);
    event.path = str_strdup(&path_str);
    event.succeeded = ((vsf_sysutil_retval_is_error(retval)) ? 0 : 1);
    sanitize_path_and_name(&event.path, &event.name);
    send_event_chmod(&event);
    vsf_sysutil_free((char*)event.new_attrib);
    vsf_sysutil_free((char*)event.name);
    vsf_sysutil_free((char*)event.path);
    vsf_sysutil_free((char*)event.sharedInfo.user_name);
    str_free(&path_str);
}

void send_log_event_chmod_failed(struct vsf_session * p_sess, struct mystr * p_arg_str, struct mystr * s_chmod_file_str) {
    struct Event_Chmod event;
    struct mystr path_str = INIT_MYSTR;
    populate_shared_event(&event.sharedInfo, p_sess);
    event.new_attrib = str_strdup(p_arg_str);
    str_getcwd(&path_str);
    event.name = str_strdup(s_chmod_file_str);
    event.path = str_strdup(&path_str);
    event.succeeded = -1;
    sanitize_path_and_name(&event.path, &event.name);
    send_event_chmod(&event);
    vsf_sysutil_free((char*)event.new_attrib);
    vsf_sysutil_free((char*)event.name);
    vsf_sysutil_free((char*)event.path);
    vsf_sysutil_free((char*)event.sharedInfo.user_name);
    str_free(&path_str);
}
