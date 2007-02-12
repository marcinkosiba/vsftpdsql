#ifndef VSF_EVENT
#define VSF_EVENT

#include "event_c_api.h"
#include <memory>

class VSFPlugin;

/**
 * Just a set of wrappers for the struct's. Mainly to provide polymorphism
 * for the plugin architecture.
 */
class Event {
public:
  virtual int getNumber() = 0;
  virtual void getHandled(VSFPlugin &) = 0;
  static std::auto_ptr<Event> readFromSocket(int sock);
  virtual ~Event() {}
private:  
  virtual void readFromSock(int sock) = 0;
};

class EventLogin : public Event {
public:
  virtual int getNumber() { return  Event_Number_Login; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Login * getData() const {return &i_struct;}
  virtual ~EventLogin();
private:
  EventLogin() {}  
  virtual void readFromSock(int sock);
  struct Event_Login i_struct;
  friend class Event;
};

class EventFailedLogin : public Event {
public:
  virtual int getNumber() { return  Event_Number_FailedLogin; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_FailedLogin * getData() const {return &i_struct;}
  virtual ~EventFailedLogin();
private:
  EventFailedLogin(){}  
  virtual void readFromSock(int sock);
  struct Event_FailedLogin i_struct;
  friend class Event;
};

class EventDownload : public Event {
public:
  virtual int getNumber() { return  Event_Number_Download; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Download * getData() const {return &i_struct;}
  virtual ~EventDownload();
private:
  EventDownload() {}  
  virtual void readFromSock(int sock);
  struct Event_Download i_struct;
  friend class Event;
};

class EventUpload : public Event {
public:
  virtual int getNumber() { return  Event_Number_Upload; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Upload * getData() const {return &i_struct;}
  virtual ~EventUpload();
private:
  EventUpload() {}  
  virtual void readFromSock(int sock);
  struct Event_Upload i_struct;
  friend class Event;
};

class EventMkdir : public Event {
public:
  virtual int getNumber() { return  Event_Number_Mkdir; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Mkdir * getData() const {return &i_struct;}
  virtual ~EventMkdir();
private:
  EventMkdir() {}  
  virtual void readFromSock(int sock);
  struct Event_Mkdir i_struct;
  friend class Event;
};

class EventConnection : public Event {
public:
  virtual int getNumber() { return  Event_Number_Connection; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Connection * getData() const {return &i_struct;}
  virtual ~EventConnection();
private:
  EventConnection() {}  
  virtual void readFromSock(int sock);
  struct Event_Connection i_struct;
  friend class Event;
};

class EventDelete : public Event {
public:
  virtual int getNumber() { return  Event_Number_Delete; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Delete * getData() const {return &i_struct;}
  virtual ~EventDelete();
private:
  EventDelete() {}  
  virtual void readFromSock(int sock);
  struct Event_Delete i_struct;
  friend class Event;
};

class EventRename : public Event {
public:
  virtual int getNumber() { return  Event_Number_Rename; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Rename * getData() const {return &i_struct;}
  virtual ~EventRename();
private:
  EventRename() {}
  virtual void readFromSock(int sock);
  struct Event_Rename i_struct;
  friend class Event;
};

class EventRmdir : public Event {
public:
  virtual int getNumber() { return  Event_Number_Rmdir; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Rmdir * getData() const {return &i_struct;}
  virtual ~EventRmdir();
private:
  EventRmdir() {}  
  virtual void readFromSock(int sock);
  struct Event_Rmdir i_struct;
  friend class Event;
};

class EventChmod : public Event {
public:
  virtual int getNumber() { return  Event_Number_Chmod; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_Chmod * getData() const {return &i_struct;}
  virtual ~EventChmod();
private:
  EventChmod() {}  
  virtual void readFromSock(int sock);
  struct Event_Chmod i_struct;
  friend class Event;
};

class EventCliEnd : public Event {
public:
  virtual int getNumber() { return  Event_Number_CliEnd; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_CliEnd * getData() const {return &i_struct;}
  virtual ~EventCliEnd();
private:
  EventCliEnd() {}
  virtual void readFromSock(int sock);
  struct Event_CliEnd i_struct;
  friend class Event;
};

class EventServQuit : public Event {
public:
  virtual int getNumber() { return  Event_Number_ServQuit; }
  virtual void getHandled(VSFPlugin &);
  const struct Event_ServQuit * getData() const {return &i_struct;}
  virtual ~EventServQuit();
private:
  EventServQuit() {}  
  virtual void readFromSock(int sock);
  struct Event_ServQuit i_struct;
  friend class Event;
};


#endif //VSF_EVENT
