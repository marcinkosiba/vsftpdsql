#ifndef PGSQL_PLUGIN_H
#define PGSQL_PLUGIN_H
#include <time.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "plugin_architecture_api.h"
#include "libpq-fe.h"
#include "exceptions.h"
#include "tunables.h"
#include "vsfutils.h"

using namespace std;

class VSFPgSQLPlugin : public VSFPlugin
{
private:
  PGconn* _dbconn;
  void connectDB(PGconn* &);
  PGresult * queryDB(const string);
  PGresult * queryDB(const string, PGconn *);
  string getTimeStamp();
  string getTimeStamp(struct timeval);
  string getIP(const struct sockaddr_in &);
  long long int getUid(string username);
  long long int getEid(const struct Event_Shared sharedInfo);
  string sanitize(const string &);
  VSFPluginArchitecture & PlugArch;
    string unslash(const char * original);
  string unslash(const string & original);

public:
  VSFPgSQLPlugin(VSFPluginArchitecture &);
  ~VSFPgSQLPlugin();

  void handleEvent(const EventChmod &);
  void handleEvent(const EventConnection &);
  void handleEvent(const EventDelete &);
  void handleEvent(const EventDownload &);
  void handleEvent(const EventFailedLogin &);
  void handleEvent(const EventLogin &);
  void handleEvent(const EventMkdir &);
  void handleEvent(const EventRename &);
  void handleEvent(const EventRmdir &);
  void handleEvent(const EventCliEnd &);
  void handleEvent(const EventUpload &);

  Request_Result handleRequest(const RequestLogin &);
  Request_Result_And_Reason handleRequest(const RequestReadFile &);
  Request_Result_And_Reason handleRequest(const RequestReadDirectory &);
  Request_Result_And_Reason handleRequest(const RequestWriteDirectory &);
  Request_Result_And_Reason handleRequest(const RequestWriteFile &);
  Request_Result_Homedir handleRequest(const RequestHomedir &);
};
#endif
