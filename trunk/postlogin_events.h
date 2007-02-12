#ifndef POSTLOGIN_EVENTS_H
#define POSTLOGIN_EVENTS_H

#include "session.h"
#include "ftpdataio.h"
#include "str.h"

void send_log_event_download(struct vsf_session * p_sess, struct vsf_transfer_ret * trans_ret);
void send_log_event_download_failed(struct vsf_session * p_sess);
void send_log_event_upload(struct vsf_session * p_sess, struct vsf_transfer_ret * trans_ret, struct mystr* p_filename, int is_append, int doesExist);
void send_log_event_upload_failed(struct vsf_session * p_sess, struct mystr* p_filename, int is_append, int doesExist);
void send_log_event_mkdir(struct vsf_session * p_sess, int succeeded);
void send_log_event_mkdir_failed(struct vsf_session * p_sess);
void send_log_event_rmdir(struct vsf_session * p_sess, int succeeded);
void send_log_event_rmdir_failed(struct vsf_session * p_sess);
void send_log_event_delete(struct vsf_session * p_sess, int succeeded);
void send_log_event_delete_failed(struct vsf_session * p_sess);
void send_log_event_chmod(struct vsf_session * p_sess, struct mystr * p_arg_str, struct mystr * s_chmod_file_str, int retval);
void send_log_event_chmod_failed(struct vsf_session * p_sess, struct mystr * p_arg_str, struct mystr * s_chmod_file_str);
void send_log_event_rename(struct vsf_session * p_sess, int succeeded);
void send_log_event_rename_failed(struct vsf_session * p_sess);

#endif //POSTLOGIN_EVENTS_H

