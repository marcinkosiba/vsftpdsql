#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class SendRecvException : public std::exception {
public:
  virtual const char * what() const throw()
  {
    return "send or recv failed";
  }
};

class SendException : public std::exception {
public:
  virtual const char * what() const throw()
  {
    return "send failed";
  }
};

class RecvException : public std::exception {
public:
  virtual const char * what() const throw()
  {
    return "recv failed";
  }
};


class InconsistentTypeException : public std::exception {
public:
  virtual const char * what() const throw()
  {
    return "type sent differs from type to receive to";
  }
};

class TooManyPluginArchitectures : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "too many plugin architectures created for the same thread\n";
  }
};

class TooManyRequestHandlers : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "trying to assign more than one request handler to a request\n";
  }
};

class UnregisteringNotRegisterd : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "trying to unregiser an unregistered handler\n";
  }
};

class NoEventHandler : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "trying to handle an event for which there is no handler\n";
  }
};

class NoRequestHandler : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "trying to handle a request for which there is no handler\n";
  }
};

class TooLongString : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "oh, come on - nobody sends strings as big as that!\n";
  }
};

class UnknownEventNumber : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "unknown event number, are you sending the event by hand?!\n";
  }
};

class UnknownRequestNumber : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "unknown request number, are you sending the request by hand?!\n";
  }
};

class NullPointerException : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "null pointer, duh\n";
  }
};

class UnimplementedPluginMethod : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "looks like you registered a method which you did not override\n";
  }
};

class DeserializationMemoryInconsistencyException : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "the block of memory is too short or corrupt\n";
  }
};

class UnknownPluginException : public std::exception { 
public:
  virtual const char * what() const throw()
  {
    return "the plugin in the configure file is unknown\n";
  }
};

class VSFPluginException : public std::exception {
public:
  virtual const char * what() const throw() = 0;
};

class MySQLConnHostError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Failed connecting to the MySQL server\n";
  }
};

class MySQLConnectionError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Failed connecting to the local MySQL server\n";
  }
};

class MySQLCommandsOutOfSync : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: MySQL commands executed in an improper order\n";
  }
};

class MySQLIpsockError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Failed to create an IP socket\n";
  }
};

class MySQLOutOfMemory : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Out of ... I don't remember what... oh yes, memory\n";
  }
};

class MySQLSocketCreateError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Failed to create a Unix socket\n";
  }
};

class MySQLUnknownHost : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Unknown MySQL host, check your DNS settings\n";
  }
};

class MySQLVersionError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Wrong MySQL version! Either the client is too old or the server is too new\n";
  }
};

class MySQLNamedpipeopenError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Failed to create a named pipe on Windows\n";
  }
};

class MySQLNamedpipewaitError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Failed to wait for a named pipe on Windows\n";
  }
};

class MySQLNamedpipesetstateError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Failed to get pipe handler on Windows\n";
  }
};

class MySQLServerLost : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Server Lost, connection to the server timed out\n";
  }
};

class MySQLServerGoneError : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: MySQL server has gone... away\n";
  }
};

class MySQLUnknown : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: Unknown Error code!\n";
  }
};

class MySQLFileEntryNotFound : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFMySQL: File or directory not found in database! (or multiple instances of the same file/directory found)\n";
  }
};

class PgSQLFileEntryNotFound : public VSFPluginException { 
public:
  virtual const char * what() const throw()
  {
    return "VSFPgSQL: File or directory not found in database! (or multiple instances of the same file/directory found)\n";
  }
};
#endif //EXCEPTIONS_H
