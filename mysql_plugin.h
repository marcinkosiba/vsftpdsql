#ifndef MYSQL_PLUGIN_H
#define MYSQL_PLUGIN_H
#include "plugin_architecture_api.h"
//#include <linux/types.h>
//#include <asm-generic/bitops/fls.h>
//#include <asm-generic/bitops/hweight.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "exceptions.h"
#include "tunables.h"
#include <errmsg.h>
#include <time.h>
#include <arpa/inet.h>
#include <my_global.h>
#include <mysql.h>
#include "vsfutils.h"

using namespace std;

class MyDatabaseConnection
{
private:
	MYSQL* _dbconn;
	MYSQL_RES * _result;
public:
	MyDatabaseConnection();
	~MyDatabaseConnection();
	MYSQL_RES * query(const string);
	MYSQL_RES * getResult() { return _result; }
	MYSQL* getConnection() { return _dbconn; }
};

class VSFMySQLPlugin : public VSFPlugin
{
private:
//protected stuff
//  MYSQL* _dbconn;
MyDatabaseConnection * dbcon;
  string commit;
  
//  MYSQL_RES * queryDB(const string);
//  MYSQL_RES * queryDB(const string, MYSQL *);
  string getTimeStamp();
  string getTimeStamp(struct timeval);
  long long int getUid(string username);
  long long int getEid(const struct Event_Shared sharedInfo);
  string getIP(const struct sockaddr_in &);
  string sanitize(const string &);
  string sanitize(const char * original);
//  void connectDB(MYSQL * & connection);
  VSFPluginArchitecture & PlArch;
  string unslash(const char * original);
  string unslash(const string & original);
public:
//public stuff
  VSFMySQLPlugin(VSFPluginArchitecture &);
  ~VSFMySQLPlugin();
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
