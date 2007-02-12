#ifndef VSF_C_T_CP_HELPERS
#define VSF_C_T_CP_HELPERS

#ifdef __cplusplus
  #define C_COMPATIBLE_FUNCTION extern "C"
#else
  #define C_COMPATIBLE_FUNCTION
#endif

#ifdef __cplusplus
  #define HIDE_CPP_FROM_C(strings) strings
#else
  #define HIDE_CPP_FROM_C(strings)
#endif

#endif
