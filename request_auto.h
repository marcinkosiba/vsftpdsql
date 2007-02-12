gen_funcs( login ,  Login )
gen_funcs( homedir ,  Homedir )
gen_funcs( readFile ,  ReadFile )
gen_funcs( readDirectory ,  ReadDirectory )
gen_funcs( writeDirectory ,  WriteDirectory )
gen_funcs( writeFile ,  WriteFile )
std::auto_ptr<Request> Request::readFromSocket(int sock) {
  Request_Number_Enum eventNumber;
  recvData(sock, eventNumber);
  if (!(eventNumber & Request_Identifier))
  {
    throw InconsistentTypeException();
  }
  std::auto_ptr<Request> toRet;
  switch (eventNumber)
  {
  case Request_Number_Login:
    toRet = std::auto_ptr<Request>(new RequestLogin);
    toRet->readFromSock(sock);
  break;
  case Request_Number_Homedir:
    toRet = std::auto_ptr<Request>(new RequestHomedir);
    toRet->readFromSock(sock);
  break;
  case Request_Number_ReadFile:
    toRet = std::auto_ptr<Request>(new RequestReadFile);
    toRet->readFromSock(sock);
  break;
  case Request_Number_ReadDirectory:
    toRet = std::auto_ptr<Request>(new RequestReadDirectory);
    toRet->readFromSock(sock);
  break;
  case Request_Number_WriteDirectory:
    toRet = std::auto_ptr<Request>(new RequestWriteDirectory);
    toRet->readFromSock(sock);
  break;
  case Request_Number_WriteFile:
    toRet = std::auto_ptr<Request>(new RequestWriteFile);
    toRet->readFromSock(sock);
  break;
  default:
    throw UnknownRequestNumber();
  }
return toRet;
}
template<> void deleteStruct(Request_Shared & to_delete) {
  deleteStruct(to_delete.timestamp);
  deleteStruct(to_delete.remote_ip);
  delete [] to_delete.user_name;
}
template<> void deleteStruct(Request_Login & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.login;
  delete [] to_delete.password;
}
template<> void deleteStruct(Request_Homedir & to_delete) {
  deleteStruct(to_delete.sharedInfo);
}
template<> void deleteStruct(Request_Read_File & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.path;
  delete [] to_delete.name;
  delete [] to_delete.fullName;
}
template<> void deleteStruct(Request_Read_Directory & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.parentPath;
  delete [] to_delete.name;
}
template<> void deleteStruct(Request_Write_Directory & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.parentPath;
  delete [] to_delete.name;
}
template<> void deleteStruct(Request_Write_File & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.path;
  delete [] to_delete.name;
  delete [] to_delete.fullName;
}
