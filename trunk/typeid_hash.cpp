#include "typeid_hash.h"
#include <map>

static std::map<const char *, int> hashmap;

inline int xorsum_string(const char * str) {
  int retval = 0;
  for (unsigned int i = 0; i < strlen(str); ++i)
  {
    retval ^= ((int)str[i]) << ((i % 4)*4);
  }
  return retval;
}

int TypeIDHashtable::getHash(const std::type_info & typenfo) {
  if ( hashmap.find(typenfo.name()) != hashmap.end() )
    return hashmap[typenfo.name()];
  else {
    int xoredsum = xorsum_string(typenfo.name());
    hashmap[typenfo.name()] = xoredsum;    
    return xoredsum;
  }
}
