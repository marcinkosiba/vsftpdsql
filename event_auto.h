gen_funcs( login ,  Login )
gen_funcs( failedLogin ,  FailedLogin )
gen_funcs( download ,  Download )
gen_funcs( upload ,  Upload )
gen_funcs( mkdir ,  Mkdir )
gen_funcs( connection ,  Connection )
gen_funcs( delete ,  Delete )
gen_funcs( rename ,  Rename )
gen_funcs( rmdir ,  Rmdir )
gen_funcs( chmod ,  Chmod )
gen_funcs_double( cliEnd ,  CliEnd )
gen_funcs_double( servQuit ,  ServQuit )
std::auto_ptr<Event> Event::readFromSocket(int sock) {
  Event_Number_Enum eventNumber;
  recvData(sock, eventNumber);
  if (!(eventNumber & Event_Identifier))
  {
    throw InconsistentTypeException();
  }
  std::auto_ptr<Event> toRet;
  switch (eventNumber)
  {
  case Event_Number_Login:
    toRet = std::auto_ptr<Event>(new EventLogin);
    toRet->readFromSock(sock);
  break;
  case Event_Number_FailedLogin:
    toRet = std::auto_ptr<Event>(new EventFailedLogin);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Download:
    toRet = std::auto_ptr<Event>(new EventDownload);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Upload:
    toRet = std::auto_ptr<Event>(new EventUpload);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Mkdir:
    toRet = std::auto_ptr<Event>(new EventMkdir);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Connection:
    toRet = std::auto_ptr<Event>(new EventConnection);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Delete:
    toRet = std::auto_ptr<Event>(new EventDelete);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Rename:
    toRet = std::auto_ptr<Event>(new EventRename);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Rmdir:
    toRet = std::auto_ptr<Event>(new EventRmdir);
    toRet->readFromSock(sock);
  break;
  case Event_Number_Chmod:
    toRet = std::auto_ptr<Event>(new EventChmod);
    toRet->readFromSock(sock);
  break;
  case Event_Number_CliEnd:
    toRet = std::auto_ptr<Event>(new EventCliEnd);
    toRet->readFromSock(sock);
  break;
  case Event_Number_ServQuit:
    toRet = std::auto_ptr<Event>(new EventServQuit);
    toRet->readFromSock(sock);
  break;
  default:
    throw UnknownEventNumber();
  }
return toRet;
}
template<> void deleteStruct(Event_Shared & to_delete) {
  deleteStruct(to_delete.timestamp);
  deleteStruct(to_delete.remote_ip);
  delete [] to_delete.user_name;
}
template<> void deleteStruct(Event_FileData_Shared & to_delete) {
  delete [] to_delete.path;
  delete [] to_delete.name;
}
template<> void deleteStruct(Event_Login & to_delete) {
  deleteStruct(to_delete.sharedInfo);
}
template<> void deleteStruct(Event_FailedLogin & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.failed_password;
}
template<> void deleteStruct(Event_Download & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  deleteStruct(to_delete.fileInfo);
}
template<> void deleteStruct(Event_Upload & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  deleteStruct(to_delete.fileInfo);
}
template<> void deleteStruct(Event_Mkdir & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.path;
}
template<> void deleteStruct(Event_Connection & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.reason;
}
template<> void deleteStruct(Event_Delete & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.path;
}
template<> void deleteStruct(Event_Rename & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.old_name;
  delete [] to_delete.new_name;
}
template<> void deleteStruct(Event_Rmdir & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.path;
}
template<> void deleteStruct(Event_Chmod & to_delete) {
  deleteStruct(to_delete.sharedInfo);
  delete [] to_delete.path;
  delete [] to_delete.name;
  delete [] to_delete.new_attrib;
}
template<> void deleteStruct(Event_CliEnd & to_delete) {
  deleteStruct(to_delete.timestamp);
}
template<> void deleteStruct(Event_ServQuit & to_delete) {
}
