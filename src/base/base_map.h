#include "base_inc.h"

#define Decl_StringMap(T) \
  struct _StringMap##T \
  { \
    String *keys; \
    T *values; \
    u64 cap; \
  }; \

#define StringMap(T) struct _StringMap##T

Decl_StringMap(u64)

#define create_stringmap(T, cap) \
  \
  \
  \
  \
