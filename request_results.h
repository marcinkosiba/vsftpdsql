#ifndef REQUEST_RESULTS_H
#define REQUEST_RESULTS_H

#include "cpp_to_c_helpers.h"

struct Request_Result_And_Reason {
  int result; //0 for success, -1 for failure, any other value - failure
  const char * reason; //on success set to "No failure", on failure set to the reason of the failure
};

#define Request_Result_And_Reason_Init {0, "No failure"}

C_COMPATIBLE_FUNCTION void doneWith_Result_And_Reason(struct Request_Result_And_Reason *);

struct Request_Result {
  int result; //0 for success, -1 for failure, any other value - failure
};

#define Request_Result_Init {0}

C_COMPATIBLE_FUNCTION void doneWith_Result(struct Request_Result *);

struct Request_Result_Homedir {
  const char * homedir;
};

#define Request_Result_Homedir_Init {"/"}

C_COMPATIBLE_FUNCTION void doneWith_Result_Homedir(struct Request_Result_Homedir *);

#endif //REQUEST_RESULTS_H
