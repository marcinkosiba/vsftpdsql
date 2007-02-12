#include "request_results.h"

C_COMPATIBLE_FUNCTION void doneWith_Result_And_Reason(struct Request_Result_And_Reason * r) {
  delete [] r->reason;
}

C_COMPATIBLE_FUNCTION void doneWith_Result(struct Request_Result *) {
  ;
}

C_COMPATIBLE_FUNCTION void doneWith_Result_Homedir(struct Request_Result_Homedir * r) {
  delete [] r->homedir;
}
