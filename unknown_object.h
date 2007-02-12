#ifndef UNKNOWN_OBJECT_H
#define UNKNOWN_OBJECT_H

#include <memory>
#include <string>
#include <typeinfo>
#include <sys/types.h>
#include <sys/socket.h>
#include "exceptions.h"
#include "typeid_hash.h"
#include "request_c_api.h"
#include "countedPtr.h"

/**
 * converts the object to data whic can be sent via sockets
 */

template<typename T> inline std::pair<char *,int> serialize(const T & object) {
  char * memory = new char [sizeof(object) + sizeof(int)];
  int * xorsum = (int*)memory;
  std::pair<char *, int> toRet;
  *xorsum = TypeIDHashtable::getHash(typeid(object));
  memcpy(memory + sizeof(int), &object, sizeof(object));

  toRet.first = memory;
  toRet.second = sizeof(object) + sizeof(int);
  return toRet;
}

/**
 * writes the serialized data to an object
 */
template<typename T> inline void deserialize(const std::pair<char *, int> & data, T & object) {
  int * xorsum = (int*)data.first;

  if (sizeof(object) != data.second)
  {
    throw DeserializationMemoryInconsistencyException();
  }
  if (*xorsum != TypeIDHashtable::getHash(typeid(object)))
  {
    throw InconsistentTypeException();
  }

  memcpy(&object, data.first + sizeof(int), sizeof(T));
}

#include "serialization_auto.h"

/**
 * Provides means of returning different data with different types.
 * This must be so, because we use one handler method in the pluginArchitecure...
 * Anyway - it's a lot better than (void *) :)
 */
class UnknownObject {
public:
  template<typename T> UnknownObject(const T & object) {
    std::pair<char *, int> serialized = serialize(object);
    size = serialized.second;
    data = std::auto_ptr<char>(serialized.first);
    name = typeid(object).name();
  }

  UnknownObject(const UnknownObject & object) { //copy constructor
    size = object.size;
    char * d = new char[size];
    memcpy(d, object.data.get(), size);
    data = std::auto_ptr<char>(d);
    name = object.name;
  }

  UnknownObject & operator=(const UnknownObject & object) {
    size = object.size;
    char * d = new char[size];
    memcpy(d, object.data.get(), size);
    data = std::auto_ptr<char>(d);
    name = object.name;
    return *this;
  }

  unsigned int getDataSize() const {
    return size;
  }

  void * getData() {
    return data.get();    
  }

  std::string getTypeName() const{
    return name;
  }

  static std::auto_ptr<UnknownObject> readFromSocket(int socket_number) {
    std::auto_ptr<UnknownObject> toRet(new UnknownObject());
    int namelen;    
    if(recv(socket_number, &namelen, sizeof(namelen), 0) != sizeof(namelen))
      throw SendRecvException();
    if (recv(socket_number, &toRet->size, sizeof(toRet->size), 0) != sizeof(toRet->size))
      throw RecvException();

    auto_arr<char> str(new char[namelen]);
    auto_arr<char> dta(new char[toRet->size]);
    if (recv(socket_number, str.get(), namelen, 0) != namelen)
      throw RecvException();
    if (recv(socket_number, dta.get(), toRet->size, 0) != toRet->size)
      throw RecvException();

    toRet->data = dta;
    toRet->name = std::string(str.get());

    return toRet;
  }

  void writeToSocket(int socket_number) const {
    int len = name.length()+1;
    printf("writing to sock\n");
    if (send(socket_number, &len, sizeof(len), 0) != sizeof(len))
      throw SendException();
    if(send(socket_number, &size, sizeof(size), 0) != sizeof(size))
      throw SendException();
    if (send(socket_number, name.c_str(), len, 0) != len)
      throw SendException();
    if (send(socket_number, data.get(), size, 0) != size)
      throw SendException();
    printf("writing to sock done\n");
  }
private:
  UnknownObject() : size(0), data(0) {}
  int size;
  auto_arr<char> data;  
  std::string name;
};

#endif //UNKNOWN_OBJECT_H
