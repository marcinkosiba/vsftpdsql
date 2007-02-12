template<> inline void sendData(int socket, const Event_Shared & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.timestamp);
  sendData(socket, to_send.remote_ip);
  {
    int len = strlen(to_send.user_name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.user_name, len, 0) != len) {
      throw SendRecvException();
    }
  }
}
template<> inline void sendData(int socket, const Event_FileData_Shared & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  {
    int len = strlen(to_send.path) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.path, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.name, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.size, sizeof(unsigned int), 0) != sizeof(unsigned int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_Login & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  if(send(socket, &to_send.is_anonymous, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_FailedLogin & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.failed_password) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.failed_password, len, 0) != len) {
      throw SendRecvException();
    }
  }
}
template<> inline void sendData(int socket, const Event_Download & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  sendData(socket, to_send.fileInfo);
  if(send(socket, &to_send.is_complete, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_Upload & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  sendData(socket, to_send.fileInfo);
  if(send(socket, &to_send.is_complete, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
  if(send(socket, &to_send.is_append, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
  if(send(socket, &to_send.is_overwrite, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_Mkdir & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.path) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.path, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_Connection & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.reason) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.reason, len, 0) != len) {
      throw SendRecvException();
    }
  }
}
template<> inline void sendData(int socket, const Event_Delete & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.path) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.path, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_Rename & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.old_name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.old_name, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.new_name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.new_name, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_Rmdir & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.path) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.path, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_Chmod & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.path) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.path, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.name, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.new_attrib) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.new_attrib, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Event_CliEnd & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.timestamp);
}
template<> inline void sendData(int socket, const Event_ServQuit & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Shared & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.timestamp);
  recvData(socket, to_recv_to.remote_ip);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.user_name = strnk;
  }
}
template<> inline void recvData(int socket, Event_FileData_Shared & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.path = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.name = strnk;
  }
  if(recv(socket, &to_recv_to.size, sizeof(unsigned int), 0) != sizeof(unsigned int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Login & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  if(recv(socket, &to_recv_to.is_anonymous, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_FailedLogin & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.failed_password = strnk;
  }
}
template<> inline void recvData(int socket, Event_Download & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  recvData(socket, to_recv_to.fileInfo);
  if(recv(socket, &to_recv_to.is_complete, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Upload & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  recvData(socket, to_recv_to.fileInfo);
  if(recv(socket, &to_recv_to.is_complete, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
  if(recv(socket, &to_recv_to.is_append, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
  if(recv(socket, &to_recv_to.is_overwrite, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Mkdir & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.path = strnk;
  }
  if(recv(socket, &to_recv_to.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Connection & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.reason = strnk;
  }
}
template<> inline void recvData(int socket, Event_Delete & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.path = strnk;
  }
  if(recv(socket, &to_recv_to.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Rename & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.old_name = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.new_name = strnk;
  }
  if(recv(socket, &to_recv_to.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Rmdir & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.path = strnk;
  }
  if(recv(socket, &to_recv_to.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_Chmod & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.path = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.name = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.new_attrib = strnk;
  }
  if(recv(socket, &to_recv_to.succeeded, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Event_CliEnd & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.timestamp);
}
template<> inline void recvData(int socket, Event_ServQuit & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
}
template<> inline void sendData(int socket, const Request_Shared & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.timestamp);
  sendData(socket, to_send.remote_ip);
  {
    int len = strlen(to_send.user_name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.user_name, len, 0) != len) {
      throw SendRecvException();
    }
  }
}
template<> inline void sendData(int socket, const Request_Login & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.login) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.login, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.password) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.password, len, 0) != len) {
      throw SendRecvException();
    }
  }
}
template<> inline void sendData(int socket, const Request_Homedir & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
}
template<> inline void sendData(int socket, const Request_Read_File & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.path) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.path, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.name, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.fullName) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.fullName, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.size, sizeof(unsigned int), 0) != sizeof(unsigned int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Request_Read_Directory & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.parentPath) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.parentPath, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.name, len, 0) != len) {
      throw SendRecvException();
    }
  }
}
template<> inline void sendData(int socket, const Request_Write_Directory & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.parentPath) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.parentPath, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.name, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.does_exist, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void sendData(int socket, const Request_Write_File & to_send) throw(std::exception) {
  int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
  if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
  { 
    throw SendRecvException();
  }
  sendData(socket, to_send.sharedInfo);
  {
    int len = strlen(to_send.path) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.path, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.name) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.name, len, 0) != len) {
      throw SendRecvException();
    }
  }
  {
    int len = strlen(to_send.fullName) + 1;
    if (len > 1024)
      throw TooLongString();
    if(send(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if(send(socket, to_send.fullName, len, 0) != len) {
      throw SendRecvException();
    }
  }
  if(send(socket, &to_send.does_exist, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Request_Shared & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.timestamp);
  recvData(socket, to_recv_to.remote_ip);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.user_name = strnk;
  }
}
template<> inline void recvData(int socket, Request_Login & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.login = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.password = strnk;
  }
}
template<> inline void recvData(int socket, Request_Homedir & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
}
template<> inline void recvData(int socket, Request_Read_File & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.path = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.name = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.fullName = strnk;
  }
  if(recv(socket, &to_recv_to.size, sizeof(unsigned int), 0) != sizeof(unsigned int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Request_Read_Directory & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.parentPath = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.name = strnk;
  }
}
template<> inline void recvData(int socket, Request_Write_Directory & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.parentPath = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.name = strnk;
  }
  if(recv(socket, &to_recv_to.does_exist, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
template<> inline void recvData(int socket, Request_Write_File & to_recv_to) throw(std::exception) {
  int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
  int xorsum_recvd;
  if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int))
  {
    throw SendRecvException();
  }
  if (xorsum_recvd != my_xorsum)
  {
    throw InconsistentTypeException();
  }
  recvData(socket, to_recv_to.sharedInfo);
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.path = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.name = strnk;
  }
  {
    int len;
    char * strnk;
    if(recv(socket, &len, sizeof(len), 0) != sizeof(len)) {
      throw SendRecvException();
    }
    if (len > 1024)
      throw TooLongString();
    strnk = new char[len];
    if(recv(socket, strnk, len, 0) != len) {
      throw SendRecvException();
    }
    to_recv_to.fullName = strnk;
  }
  if(recv(socket, &to_recv_to.does_exist, sizeof(int), 0) != sizeof(int)) {
    throw SendRecvException();
  }
}
