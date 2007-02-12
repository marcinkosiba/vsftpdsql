template<> inline std::pair<char *,int> serialize(const Request_Result_And_Reason & object) {
  int struct_size = sizeof(int);
  std::pair<char *,int> toRet;
  char * data;
  char * cdata;
  struct_size += sizeof(object.result);
  struct_size += sizeof(int); 
  struct_size += strlen(object.reason) + 1; 
  data = new char[struct_size];
  cdata = data;
  *((int*)data) = TypeIDHashtable::getHash(typeid(object));
  data += sizeof(int);
  memcpy(data, &object.result, sizeof(int));
  data += sizeof(int);
  *((int*)data) = strlen(object.reason); 
  data += sizeof(int); 
  memcpy(data,object.reason, strlen(object.reason) + 1); 
  data += strlen(object.reason) + 1; 
  toRet.first = cdata;
  toRet.second = struct_size;
  return toRet;
}
template<> inline std::pair<char *,int> serialize(const Request_Result & object) {
  int struct_size = sizeof(int);
  std::pair<char *,int> toRet;
  char * data;
  char * cdata;
  struct_size += sizeof(object.result);
  data = new char[struct_size];
  cdata = data;
  *((int*)data) = TypeIDHashtable::getHash(typeid(object));
  data += sizeof(int);
  memcpy(data, &object.result, sizeof(int));
  data += sizeof(int);
  toRet.first = cdata;
  toRet.second = struct_size;
  return toRet;
}
template<> inline std::pair<char *,int> serialize(const Request_Result_Homedir & object) {
  int struct_size = sizeof(int);
  std::pair<char *,int> toRet;
  char * data;
  char * cdata;
  struct_size += sizeof(int); 
  struct_size += strlen(object.homedir) + 1; 
  data = new char[struct_size];
  cdata = data;
  *((int*)data) = TypeIDHashtable::getHash(typeid(object));
  data += sizeof(int);
  *((int*)data) = strlen(object.homedir); 
  data += sizeof(int); 
  memcpy(data,object.homedir, strlen(object.homedir) + 1); 
  data += strlen(object.homedir) + 1; 
  toRet.first = cdata;
  toRet.second = struct_size;
  return toRet;
}
template<> inline void deserialize(const std::pair<char *, int> & data, Request_Result_And_Reason & object) {
  int * xorsum = (int*)data.first;
  char * ddata = data.first; 
  ddata += sizeof(int);
  int struct_size = sizeof(int);
  if (*xorsum != TypeIDHashtable::getHash(typeid(object))) {
  throw InconsistentTypeException();
  }
  struct_size += sizeof(object.result);
  ddata += sizeof(object.result);
  {
    int strlen = *(int*)ddata;
    struct_size += strlen + 1 + sizeof(int);
    ddata += strlen + 1 + sizeof(int);
  }
  if(data.second != struct_size) { 
    throw DeserializationMemoryInconsistencyException();
  }
  ddata = data.first + sizeof(int);
  memcpy(&object.result, ddata, sizeof(int));
  ddata += sizeof(int);
  {
    int strlen = *(int*)ddata;
    ddata += sizeof(int);
    char * str = new char[strlen + 1];
    memcpy(str, ddata, strlen + 1);
    ddata += strlen + 1;
    object.reason = str;
  }
}
template<> inline void deserialize(const std::pair<char *, int> & data, Request_Result & object) {
  int * xorsum = (int*)data.first;
  char * ddata = data.first; 
  ddata += sizeof(int);
  int struct_size = sizeof(int);
  if (*xorsum != TypeIDHashtable::getHash(typeid(object))) {
  throw InconsistentTypeException();
  }
  struct_size += sizeof(object.result);
  ddata += sizeof(object.result);
  if(data.second != struct_size) { 
    throw DeserializationMemoryInconsistencyException();
  }
  ddata = data.first + sizeof(int);
  memcpy(&object.result, ddata, sizeof(int));
  ddata += sizeof(int);
}
template<> inline void deserialize(const std::pair<char *, int> & data, Request_Result_Homedir & object) {
  int * xorsum = (int*)data.first;
  char * ddata = data.first; 
  ddata += sizeof(int);
  int struct_size = sizeof(int);
  if (*xorsum != TypeIDHashtable::getHash(typeid(object))) {
  throw InconsistentTypeException();
  }
  {
    int strlen = *(int*)ddata;
    struct_size += strlen + 1 + sizeof(int);
    ddata += strlen + 1 + sizeof(int);
  }
  if(data.second != struct_size) { 
    throw DeserializationMemoryInconsistencyException();
  }
  ddata = data.first + sizeof(int);
  {
    int strlen = *(int*)ddata;
    ddata += sizeof(int);
    char * str = new char[strlen + 1];
    memcpy(str, ddata, strlen + 1);
    ddata += strlen + 1;
    object.homedir = str;
  }
}
