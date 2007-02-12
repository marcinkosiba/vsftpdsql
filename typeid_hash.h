#ifndef TYPEID_HASH_H
#define TYPEID_HASH_H

#include <typeinfo>

class TypeIDHashtable {
public:
  static int getHash(const std::type_info & typenfo);
};

#endif //TYPEID_HASH_H
