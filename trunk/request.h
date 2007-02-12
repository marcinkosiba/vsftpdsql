#ifndef VSF_REQUEST_H
#define VSF_REQUEST_H

#include "request_c_api.h"
#include "unknown_object.h"

class VSFPlugin;

class Request {
public:
  virtual int getNumber() const = 0;
  virtual UnknownObject getHandled(VSFPlugin &) = 0;
  static std::auto_ptr<Request> readFromSocket(int sock);
  virtual ~Request() {}
private:  
  virtual void readFromSock(int sock) = 0;
};

class RequestLogin : public Request {
public:
  int getNumber() const { return Request_Number_Login ; }
  virtual UnknownObject getHandled(VSFPlugin &);
  const struct Request_Login * getData() const {return &i_struct;}
  virtual ~RequestLogin();
private:  
  RequestLogin() {}
  friend class Request;
  virtual void readFromSock(int sock);
  struct Request_Login i_struct;
};

class RequestHomedir : public Request {
public:
  int getNumber() const { return Request_Number_Login ; }
  virtual UnknownObject getHandled(VSFPlugin &);
  const struct Request_Homedir * getData() const {return &i_struct;}
  virtual ~RequestHomedir();
private:  
  RequestHomedir() {}
  friend class Request;
  virtual void readFromSock(int sock);
  struct Request_Homedir i_struct;
};

class RequestReadFile : public Request {
public:
  int getNumber() const { return Request_Number_ReadFile ; }
  virtual UnknownObject getHandled(VSFPlugin &);
  const struct Request_Read_File * getData() const {return &i_struct;}
  virtual ~RequestReadFile();
private: 
  RequestReadFile() {}
  friend class Request;
  virtual void readFromSock(int sock);
  struct Request_Read_File i_struct;
};

class RequestReadDirectory : public Request {
public:
  int getNumber() const { return Request_Number_ReadDirectory ; }
  virtual UnknownObject getHandled(VSFPlugin &);
  const struct Request_Read_Directory * getData() const {return &i_struct;}
  virtual ~RequestReadDirectory();
private:
  RequestReadDirectory() {}
  friend class Request;
  virtual void readFromSock(int sock);
  struct Request_Read_Directory i_struct;
};

class RequestWriteFile : public Request {
public:
  int getNumber() const { return Request_Number_WriteFile ; }
  virtual UnknownObject getHandled(VSFPlugin &);
  const struct Request_Write_File * getData() const {return &i_struct;}
  virtual ~RequestWriteFile();
private:  
  RequestWriteFile() {}
  friend class Request;
  virtual void readFromSock(int sock);
  struct Request_Write_File i_struct;
};

class RequestWriteDirectory : public Request {
public:
  int getNumber() const { return Request_Number_WriteDirectory ; }
  virtual UnknownObject getHandled(VSFPlugin &);
  const struct Request_Write_Directory * getData() const {return &i_struct;}
  virtual ~RequestWriteDirectory();
private:  
  RequestWriteDirectory() {}
  friend class Request;
  virtual void readFromSock(int sock);
  struct Request_Write_Directory i_struct;
};

#endif //VSF_REQUEST_H
