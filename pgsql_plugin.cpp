#include "pgsql_plugin.h"

using namespace std;

void VSFPgSQLPlugin::connectDB(PGconn * & connection)
{
  char *conninfo;
  conninfo = new char[120];
  sprintf(conninfo,"dbname = %s host = %s port = %d user = %s password = %s",
  tunable_pgsql_database_dbname,tunable_pgsql_database_host,
  tunable_pgsql_database_port,tunable_pgsql_database_username,
  tunable_pgsql_database_password);
  printf("VSPostgreSQL: connectDB: connecting to database...\n");
  connection = PQconnectdb(conninfo);
  if (PQstatus(connection) != CONNECTION_OK)
  {
    fprintf(stderr, "VSPostgreSQL: connection to database failed: %s",PQerrorMessage(connection));
    PQfinish(connection);
  }
  else 
  {
      fprintf(stderr,"VSPostgreSQL: successfully connected...\n");
  }
}

PGresult * VSFPgSQLPlugin::queryDB(const string query)
{
  return queryDB(query,_dbconn);
}

PGresult * VSFPgSQLPlugin::queryDB(const string query,PGconn * connection)
{
  printf("%s\n",query.c_str());
  PGresult *result;
 result = PQexec(connection,query.c_str());
  if( (PQresultStatus(result) != PGRES_TUPLES_OK) && (PQresultStatus(result) != PGRES_COMMAND_OK))
  {
    printf("Database error: %s", PQerrorMessage(connection));
    PQclear(result);
    return 0;
  }
  return result;
}

string VSFPgSQLPlugin::getTimeStamp()
{
  stringstream timestamp;
  struct tm tstamp;
  time_t rawtime;
  time ( &rawtime );
  gmtime_r(&rawtime,&tstamp);
  timestamp << setw(4) << right << setfill('0') << 1900+tstamp.tm_year << "-"
            << setw(2) << right << setfill('0') << tstamp.tm_mon + 1 << "-"
            << setw(2) << right << setfill('0') << tstamp.tm_mday << " "
            << setw(2) << right << setfill('0') << tstamp.tm_hour << ":"
            << setw(2) << right << setfill('0') << tstamp.tm_min << ":"
            << setw(2) << right << setfill('0') << tstamp.tm_sec;
  return timestamp.str();
}

string VSFPgSQLPlugin::getTimeStamp(struct timeval tval)
{
  stringstream timestamp;
  struct tm tstamp;
  gmtime_r(&(tval.tv_sec),&tstamp);
  timestamp << setw(4) << right << setfill('0') << 1900+tstamp.tm_year << "-"
            << setw(2) << right << setfill('0') << tstamp.tm_mon + 1<< "-"
            << setw(2) << right << setfill('0') << tstamp.tm_mday << " "
            << setw(2) << right << setfill('0') << tstamp.tm_hour << ":"
            << setw(2) << right << setfill('0') << tstamp.tm_min << ":"
            << setw(2) << right << setfill('0') << tstamp.tm_sec;
  return timestamp.str();
}

string VSFPgSQLPlugin::getIP(const struct sockaddr_in & sa_in )
{
  char * ipaddr;
  ipaddr=new char[16];
  if(sa_in.sin_family==AF_INET)
  {
    inet_ntop(sa_in.sin_family,&(sa_in.sin_addr),ipaddr,16);
  }
  else
  {
    printf("VSFPostgreSQL: vsftp-database does not support IPv6\n");
  }
  string ipstring;
  ipstring.assign(ipaddr);
  return ipstring;
}

long long int VSFPgSQLPlugin::getUid(string username)
{
  stringstream query;
  long long int uid;
  PGresult * result;
  query << "SELECT uid FROM users WHERE name = '" << sanitize(username) << "';";
  result = queryDB(query.str());
  if((result==NULL)||(PQntuples(result)!=1))
  {
    PQclear(result);
    printf("VSPostgreSQL:NO SUCH USER!\n");
    return -1;
  }
  else
  {
    sscanf(PQgetvalue(result,0,0),"%lld",&uid);
    PQclear(result);
    return uid;
  }
}

long long int VSFPgSQLPlugin::getEid(const struct Event_Shared sharedInfo)
{
  long long int eid;
  stringstream query;
  PGresult * result;

  long long int uid;
  uid =getUid(sharedInfo.user_name);
  if(uid==-1) {
  query << "SELECT eid FROM event WHERE uid IS NULL AND timestamp = '" << getTimeStamp(sharedInfo.timestamp) << "' AND ip_address = '" << getIP(sharedInfo.remote_ip) << "';";
  } else {
  query << "SELECT eid FROM event WHERE uid = " << uid << " AND timestamp = '" << getTimeStamp(sharedInfo.timestamp) << "' AND ip_address = '" << getIP(sharedInfo.remote_ip) << "';";
  }
  result = queryDB(query.str());
  if((result==NULL)||(PQntuples(result)!=1))
  {
    PQclear(result);
    printf("VSPgSQL: TOO MANY SIMILAR EVENTS!\n");
    return -1;
  }
  else
  {
    sscanf(PQgetvalue(result,0,0),"%lld",&eid);
    PQclear(result);
    return eid;
  }
}

string VSFPgSQLPlugin::sanitize(const string & original)
{
  stringstream sanitized;
  char * escaped;
  escaped = new char[2*original.length() + 1];
  PQescapeStringConn(_dbconn,escaped,original.c_str(),original.length(),NULL);
  sanitized << escaped;
  delete [] escaped;
  return sanitized.str();
}

VSFPgSQLPlugin::VSFPgSQLPlugin(VSFPluginArchitecture & arch) : VSFPlugin(arch), PlugArch(arch)
{
  fprintf(stderr,"Welcome to PgSQLPlugin...\n");
  stringstream query;
  char *conninfo;

  arch.registerRequestHandler(*this, Request_Number_Login);
  arch.registerRequestHandler(*this, Request_Number_ReadFile);
  arch.registerRequestHandler(*this, Request_Number_ReadDirectory);
  arch.registerRequestHandler(*this, Request_Number_WriteFile);
  arch.registerRequestHandler(*this, Request_Number_WriteDirectory);
  arch.registerRequestHandler(*this, Request_Number_Homedir);

  arch.registerEventHandler(*this, Event_Number_Chmod);
  arch.registerEventHandler(*this, Event_Number_Connection);
  arch.registerEventHandler(*this, Event_Number_Delete);
  arch.registerEventHandler(*this, Event_Number_Download);
  arch.registerEventHandler(*this, Event_Number_FailedLogin);
  arch.registerEventHandler(*this, Event_Number_Login);
  arch.registerEventHandler(*this, Event_Number_Mkdir);
  arch.registerEventHandler(*this, Event_Number_Rename);
  arch.registerEventHandler(*this, Event_Number_Rmdir);
  arch.registerEventHandler(*this, Event_Number_Upload);
  arch.registerEventHandler(*this, Event_Number_CliEnd);
  fprintf(stderr,"VSPostgreSQL: all handlers registered...\n");
  //init connection to the database and check if the tables exist
  //connect to the DB
  conninfo = new char[120];
  sprintf(conninfo,"dbname = %s host = %s port = %d user = %s password = %s",
  tunable_pgsql_database_dbname,tunable_pgsql_database_host,tunable_pgsql_database_port,
  tunable_pgsql_database_username,tunable_pgsql_database_password);
  printf("VSPostgreSQL: connecting to database...\n");
  _dbconn = PQconnectdb(conninfo);
  if (PQstatus(_dbconn) != CONNECTION_OK)
  {
    fprintf(stderr, "VSPostgreSQL: connection to database failed: %s",PQerrorMessage(_dbconn));
    PQfinish(_dbconn);
    exit(-1);
  }
  else
  {
      fprintf(stderr,"VSPostgreSQL: connection established...\n");
  }

  //connected to the database
  //checking if tables exist

  //TODO
  //#warning TODO:check if tables exist
  //add server startup to logs
  query << "INSERT INTO event (eid, uid, timestamp, ip_address, event_type) VALUES (DEFAULT, NULL, '" << getTimeStamp() << "', '', 'Server Startup');";
  queryDB(query.str());
  //queryDB("COMMIT;");
}

VSFPgSQLPlugin::~VSFPgSQLPlugin()
{
  stringstream query;
  fprintf(stderr,"Shutting down PgSQLPlugin...\n");
  //update event log
  query << "INSERT INTO event (eid, uid, timestamp, ip_address, event_type) VALUES (DEFAULT, NULL, '" << getTimeStamp() << "','','Server Shutdown');";
  queryDB(query.str());
  //queryDB("COMMIT;");

  //disconnect from database
  PQfinish(_dbconn);

  //unregister from handling all events and request
  PlugArch.unregisterEventHandler(*this, Event_Number_Chmod);
  PlugArch.unregisterEventHandler(*this, Event_Number_Connection);
  PlugArch.unregisterEventHandler(*this, Event_Number_Delete);
  PlugArch.unregisterEventHandler(*this, Event_Number_Download);
  PlugArch.unregisterEventHandler(*this, Event_Number_FailedLogin);
  PlugArch.unregisterEventHandler(*this, Event_Number_Login);
  PlugArch.unregisterEventHandler(*this, Event_Number_Mkdir);
  PlugArch.unregisterEventHandler(*this, Event_Number_Rename);
  PlugArch.unregisterEventHandler(*this, Event_Number_Rmdir);
  PlugArch.unregisterEventHandler(*this, Event_Number_Upload);
  PlugArch.unregisterEventHandler(*this, Event_Number_CliEnd);

  PlugArch.unregisterRequestHandler(*this, Request_Number_Login);
  PlugArch.unregisterRequestHandler(*this, Request_Number_ReadFile);
  PlugArch.unregisterRequestHandler(*this, Request_Number_ReadDirectory);
  PlugArch.unregisterRequestHandler(*this, Request_Number_WriteFile);
  PlugArch.unregisterRequestHandler(*this, Request_Number_WriteDirectory);
  PlugArch.unregisterRequestHandler(*this, Request_Number_Homedir); 
  fprintf(stderr,"VSPostgreSQL: all handlers unregistered...\n");
}

void VSFPgSQLPlugin::handleEvent(const EventChmod & event)
{
  int file_true;

  PGresult * result;
  stringstream query;
  long long int uid;
  int old_owner_write,old_owner_read,new_owner_write,new_owner_read;
  int old_group_write,old_group_read,new_group_write,new_group_read;
  int old_others_write,old_others_read,new_others_write,new_others_read;

  file_true=0;
  query.str("");
  query << "SELECT owner_read, owner_write, group_read, group_write, others_read, others_write FROM directories WHERE path = '" << sanitize(event.getData()->path)  << sanitize(event.getData()->name) << "';";
  result = queryDB(query.str());
  if((result==NULL)||(PQntuples(result)!=1))
  {
    query.str("");
    query << "SELECT f.owner_read, f.owner_write, f.group_read, f.group_write, f.others_read, f.others_write FROM files f JOIN directories d ON d.did = f.directory WHERE f.name = '" << sanitize(event.getData()->name) << "' AND d.path = '" << unslash(sanitize(event.getData()->path)) << "';";
    result = queryDB(query.str());
    file_true=1;
    if((result==NULL)||(PQntuples(result)!=1))
    {
      PQclear(result);
      throw PgSQLFileEntryNotFound();
    }
  }
  //changing from the octal representation to the separate bit's representation
  new_owner_write=0;
  new_owner_read=0;
  new_group_write=0;
  new_group_read=0;
  new_others_write=0;
  new_others_read=0;

  if(((event.getData()->new_attrib[0]) & 0x2)>0 )
  {
    new_owner_write=1;
  }
  if(((event.getData()->new_attrib[0]) & 0x4)>0 )
  {
    new_owner_read=1;
  }
  if(((event.getData()->new_attrib[1]) & 0x2)>0 )
  {
    new_group_write=1;
  }
  if(((event.getData()->new_attrib[1]) & 0x4)>0 )
  {
    new_group_read=1;
  }
  if(((event.getData()->new_attrib[2]) & 0x2)>0 )
  {
    new_others_write=1;
  }
  if(((event.getData()->new_attrib[2]) & 0x4)>0 )
  {
    new_others_read=1;
  }
  //interpreting selected data
  old_owner_read=(strcmp(PQgetvalue(result,0,0),"0")==0)?0:1;
  old_owner_write=(strcmp(PQgetvalue(result,0,1),"0")==0)?0:1;
  old_group_read=(strcmp(PQgetvalue(result,0,2),"0")==0)?0:1;
  old_group_write=(strcmp(PQgetvalue(result,0,3),"0")==0)?0:1;
  old_others_read=(strcmp(PQgetvalue(result,0,4),"0")==0)?0:1;
  old_others_write=(strcmp(PQgetvalue(result,0,5),"0")==0)?0:1;

  // doing the CHMOD
  if (event.getData()->succeeded==0) {
      query.str("");
      if(file_true!=0) {
          //modifying file
          query << "UPDATE files SET owner_read = "<< new_owner_read << " , owner_write = " << 
              new_owner_write << " , group_read = " << new_group_read << " , group_write = " << 
              new_group_write << " , others_read = " <<  new_others_read << " , others_write = " << 
              new_others_write << " WHERE fid = (SELECT fid FROM files f JOIN directories d ON f.directory = d.did" <<
              " WHERE f.name = '" << sanitize(event.getData()->name) << "' AND d.path = '" <<
              unslash(sanitize(event.getData()->path)) << "';";
      }
      else {
          //modifying directories
          query << "UPDATE directories SET owner_read = "<< new_owner_read << " , owner_write = " << 
              new_owner_write << " , group_read = " << new_group_read << " , group_write = " << 
              new_group_write << " , others_read = " <<  new_others_read << " , others_write = " << 
              new_others_write << " WHERE did = (SELECT did FROM directories" <<
              " WHERE path = '" << sanitize(event.getData()->path) <<  sanitize(event.getData()->path) 
          << "';";
      }
      queryDB(query.str());
      //queryDB("COMMIT;");
  }

  PQclear(result);

  query.str("");
  uid=getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Chmod');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Chmod');";
  }
  queryDB(query.str());
  //queryDB("COMMIT;");

  query.str("");
  query << "INSERT INTO event_chmod (eid,path,old_owner_write,old_owner_read,old_group_write,old_group_read,old_others_write,old_others_read,new_owner_write,new_owner_read,new_group_write,new_group_read,new_others_write,new_others_read,succeeded) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->path) << sanitize(event.getData()->name) << "', " << old_owner_write << ", " << old_owner_read << ", " << old_group_write << ", " << old_group_read << ", " << old_others_write << ", " << old_others_read << ", " << new_owner_write << ", " << new_owner_read << ", " << new_group_write << ", " << new_group_read << ", " << new_others_write << ", " << new_others_read << ", " << event.getData()->succeeded << ");";
  queryDB(query.str());
  //queryDB("COMMIT;");
}

void VSFPgSQLPlugin::handleEvent(const EventConnection & event)
{
  stringstream query;
printf("shit happens\n");
  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Connection');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Connection');";
  }
  queryDB(query.str());
  //queryDB("COMMIT;");

  query.str("");
  query << "INSERT INTO event_connection (eid,reason) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->reason) << "');";

  queryDB(query.str());
  //queryDB("COMMIT;");
}

void VSFPgSQLPlugin::handleEvent(const EventDelete & event)
{
  stringstream query;

  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Delete');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Delete');";
  }
  queryDB(query.str());
  //queryDB("COMMIT;");

  if(event.getData()->succeeded == 0) 
  { 
      std::pair<std::string,std::string> filnam; 
      filnam = VSFUtils::sanitizePath(event.getData()->path,"" ); 
      query.str(""); 
      query << "DELETE FROM files WHERE fid=(SELECT fid FROM files f JOIN directories d ON f.directory = d.did WHERE f.name = '" 
          << sanitize(filnam.second) << "' AND d.path = '"  << unslash(sanitize(filnam.first)) << "' );"; 
      queryDB(query.str()); 
//      queryDB("COMMIT;"); 
  } 


  query.str("");
  query << "INSERT INTO event_delete (eid,path,succeeded) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->path) << "', '"<< event.getData()->succeeded << "');";

  queryDB(query.str());
  //queryDB("COMMIT;");

}

void VSFPgSQLPlugin::handleEvent(const EventDownload & event)
{
  stringstream query;

  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Download');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Download');";
  }
  queryDB(query.str());
  //queryDB("COMMIT;");
  query.str("");
  query << "INSERT INTO event_download (eid,path,filename,size,is_complete,) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << unslash(sanitize(event.getData()->fileInfo.path)) << "', '" << sanitize(event.getData()->fileInfo.name) << "', " << event.getData()->fileInfo.size << ", " << event.getData()->is_complete << ");";

  queryDB(query.str());
  //queryDB("COMMIT;");
}

void VSFPgSQLPlugin::handleEvent(const EventFailedLogin & event)
{
  stringstream query;

  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'FailedLogin');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'FailedLogin');";
  }
  queryDB(query.str());
  //queryDB("COMMIT;");
  query.str("");
  query << "INSERT INTO event_failedlogin (eid,failed_password) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->failed_password) << "');";
  queryDB(query.str());
  //queryDB("COMMIT;");
}

void VSFPgSQLPlugin::handleEvent(const EventLogin & event)
{
  fprintf(stderr,"Handling event: EventLogin...\n");
  stringstream query;

  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Login');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Login');";
  }
  queryDB(query.str());
  //queryDB("COMMIT;");
  query.str("");
  query << "INSERT INTO event_login (eid,is_anonymous) VALUES (" << getEid(event.getData()->sharedInfo) << ", " << event.getData()->is_anonymous << ");";
  queryDB(query.str());
  //queryDB("COMMIT;");
}

void VSFPgSQLPlugin::handleEvent(const EventMkdir & event)
{
  stringstream query;

  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'MkDir');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'MkDir');";
  }

  queryDB(query.str());
  //queryDB("COMMIT;");
  query.str("");
  query << "INSERT INTO event_mkdir (eid,path,succeeded) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << unslash(sanitize(event.getData()->path)) << "', " << event.getData()->succeeded << ");";

  queryDB(query.str());
//  queryDB("COMMIT;");

//adding dir to the database (if succeded) 

  if(event.getData()->succeeded == 0) 
  { 
      query.str(""); 
      query << "INSERT INTO directories (did, path,owner,dir_group, owner_read, owner_write, group_read, group_write, others_read , others_write) VALUES (DEFAULT, '"<<unslash(sanitize(event.getData()->path)) << "', "<< ((uid!=-1)?(uid):0) <<" , 0 , " << ((tunable_local_umask & 0400)?0:1) << " , " << ((tunable_local_umask & 0200)?0:1) << " , " << ((tunable_local_umask & 040)?0:1) << " , " << ((tunable_local_umask & 020)?0:1) << " , " << ((tunable_local_umask & 04)?0:1) << " , " << ((tunable_local_umask & 02)?0:1) << " ); ";  
      queryDB(query.str()); 
     // queryDB("COMMIT;");
 } 

}

void VSFPgSQLPlugin::handleEvent(const EventRename & event)
{
  stringstream query;

  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Rename');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Rename');";
  }

  queryDB(query.str());
  //queryDB("COMMIT;");
  query.str("");
  query << "INSERT INTO event_rename (eid,oldname,newname,succeeded) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->old_name) << "', '"<< sanitize(event.getData()->new_name) << "', " << event.getData()->succeeded << "');";

  queryDB(query.str());
//  queryDB("COMMIT;");

//changing the file name in the database (if succeded) 
//TODO add separate dbconn 

  if(event.getData()->succeeded==0) 
  { 
      std::pair<std::string,std::string> oldnam; 
      std::pair<std::string,std::string> newnam; 
      oldnam=VSFUtils::sanitizePath(event.getData()->old_name,""); 
      newnam=VSFUtils::sanitizePath(event.getData()->new_name,""); 
      query.str(""); 
      //modifying file 
      query << "UPDATE files SET name= '" << sanitize(newnam.second) << "' , directory = (SELECT did FROM directories WHERE path='" << sanitize(newnam.first) << "') WHERE fid = (SELECT fid FROM files f JOIN directories d ON f.directory = d.did WHERE f.name = '"  
      << sanitize(oldnam.second) << "' AND d.path = '" << sanitize(oldnam.first ) <<  
          "');"; 
 	queryDB(query.str()); 
// 	queryDB("COMMIT;");
 } 

}

void VSFPgSQLPlugin::handleEvent(const EventRmdir & event)
{
  stringstream query;

    long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'RmDir');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'RmDir');";
  }

  queryDB(query.str());
  //queryDB("COMMIT;");

  query.str("");
  query << "INSERT INTO event_rmdir (eid,path,succeeded) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << unslash(sanitize(event.getData()->path)) << "', " << event.getData()->succeeded << ");";

  queryDB(query.str());
//  queryDB("COMMIT;");

  //removing dir from the database (if succeded) 
  if(event.getData()->succeeded == 0) 
  { 
      query.str(""); 
      query << "DELETE FROM directories WHERE did=(SELECT did FROM directories WHERE path = '"  
          << unslash(sanitize(event.getData()->path)) << "' );"; 
      queryDB(query.str()); 
//      queryDB("COMMIT;"); 

  } 

}

void VSFPgSQLPlugin::handleEvent(const EventCliEnd & event)
{
  stringstream query;

  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL, '" << getTimeStamp(event.getData()->timestamp) << "','', 'CliEnd');";
  queryDB(query.str());
//  queryDB("COMMIT;");
}

void VSFPgSQLPlugin::handleEvent(const EventUpload & event)
{
  stringstream query;

  long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Upload');";
  } else {
  query << "INSERT INTO event (eid,uid,timestamp,ip_address,event_type) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Upload');";
  }
  queryDB(query.str());
  //queryDB("COMMIT;");
  query.str("");
  query << "INSERT INTO event_upload (eid,path,filename,size,is_complete,is_append,is_overwrite) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << unslash(sanitize(event.getData()->fileInfo.path)) << "', '" << sanitize(event.getData()->fileInfo.name) << "', " << event.getData()->fileInfo.size << ", " << event.getData()->is_complete << ", " << event.getData()->is_append << ", " << event.getData()->is_overwrite << ");";

  queryDB(query.str());
//  queryDB("COMMIT;");

//creating database entry (if succeded) 

  if((event.getData()->is_complete==1)&&(event.getData()->is_append==0)&&(event.getData()->is_overwrite==0)) //complete and not append or overwrite -> insert a file 
  { 
      query.str(""); 
      query << "INSERT INTO files (fid,name,directory,owner,file_group,owner_read,owner_write,group_read,group_write,others_read,others_write) VALUES (DEFAULT, '"<< sanitize(event.getData()->fileInfo.name)  << "' , (SELECT did FROM directories WHERE path = '" << unslash(sanitize(event.getData()->fileInfo.path)) << "') , "<< ((uid!=-1)?(uid):0) <<" , 0 , " << ((tunable_local_umask & 0400)?0:1) << " , " << ((tunable_local_umask & 0200)?0:1) << " , " << ((tunable_local_umask & 040)?0:1) << " , " << ((tunable_local_umask & 020)?0:1) << " , " << ((tunable_local_umask & 04)?0:1) << " , " << ((tunable_local_umask & 02)?0:1) << " ) ;";  
      queryDB(query.str()); 
//      queryDB("COMMIT;");
  } 

}

string VSFPgSQLPlugin::unslash(const string & original)
{
	return unslash(original.c_str());
}
string VSFPgSQLPlugin::unslash(const char * original)
{
	char * escaped;
  stringstream retval;
  if(strlen(original) <= 1)
  {
  	retval << original;
  	return retval.str();
  }
  escaped = strdup(original);
	if(strlen(escaped)>1 && escaped[strlen(escaped)-1]=='/')
	{
		escaped[strlen(escaped)-1]=0;
	}
  retval << escaped;
  if(escaped)
  {
  	free(escaped);
  }
  return retval.str();
}


Request_Result VSFPgSQLPlugin::handleRequest(const RequestLogin & r)
{
  fprintf(stderr,"Handling request: RequestLogin");
  struct Request_Result retStruct = Request_Result_Init;
  stringstream query;
  PGresult * result;

  retStruct.result=-1;
  query << "SELECT * FROM users WHERE name='" << sanitize(r.getData()->login) << "' AND passwd=MD5('" << sanitize(r.getData()->password) << "');";

//  checkFolders() TODO
//#warning TODO: check if folders exists

  //connect to database
  PGconn * _reqconn;
  connectDB(_reqconn);
  //execute query - check if user and password match
  result=queryDB(query.str(), _reqconn);
  if((result==NULL)||(PQntuples(result)==1))
  {
    retStruct.result=0;
    query.str("");
    //update last login info
    query << "UPDATE users SET last_login=timestamp '" << sanitize(getTimeStamp()) << "' WHERE name='" << sanitize(r.getData()->login) << "'; ";
    queryDB(query.str(), _reqconn);
    //queryDB("COMMIT;", _reqconn);
  }
  //disconnect from DB
  PQfinish(_reqconn);
  return retStruct;
}


Request_Result_And_Reason VSFPgSQLPlugin::handleRequest(const RequestReadFile & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
  PGconn* _reqconn;
  PGresult * result;
  PGresult * perm_result;
  stringstream query;
  long long int uid;
  long long int ownerid;

  connectDB(_reqconn);
  //get uid
  query << "SELECT uid FROM users WHERE name = '" << sanitize(r.getData()->sharedInfo.user_name) << "';";
  result = queryDB(query.str(),_reqconn);
  if((result==NULL)||(PQntuples(result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    PQclear(result);
    PQfinish(_reqconn);
    return retStruct;
  }
  sscanf(PQgetvalue(result,0,0),"%lld",&uid);
  PQclear(result);
  query.str("");

  //get owner and permissions
  query << "SELECT f.owner, f.file_group, f.owner_read, f.group_read, f.others_read FROM files f JOIN directories d ON d.did = f.directory WHERE d.path='" << unslash(sanitize(r.getData()->path)) << "' AND f.name='" << sanitize(r.getData()->name) << "';";
  perm_result=queryDB(query.str(), _reqconn);
  if((perm_result==NULL)||(PQntuples(perm_result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("File could not be found in the database\n");
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  sscanf(PQgetvalue(perm_result,0,0),"%lld",&ownerid);
  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(PQgetvalue(perm_result,0,2),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this file\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }
  //checking if uid is in the group
  query.str("");
  query << "SELECT uid FROM users_groups WHERE gid = " << sanitize(PQgetvalue(perm_result,0,1)) << " AND uid = " << uid << " ;";
  result = queryDB(query.str(),_reqconn);
  if((result==NULL)||(PQntuples(result)==1))
  {
  //login is in the same group as the file
    if(strcmp(PQgetvalue(perm_result,0,3),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this file\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(result);
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }
  PQclear(result);

  //else login is somebody else
  if(strcmp(PQgetvalue(perm_result,0,4),"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to read this file\n");
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
  }
  PQclear(perm_result);
  PQfinish(_reqconn);
  return retStruct;
}


Request_Result_And_Reason VSFPgSQLPlugin::handleRequest(const RequestReadDirectory & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
  PGconn* _reqconn;
  PGresult * result;
  PGresult * perm_result;
  stringstream query;
  long long int uid;
  long long int ownerid;

  //get owner and permissions
  query << "SELECT owner, dir_group, owner_read, group_read, others_read FROM directories WHERE path='" << sanitize(r.getData()->parentPath) <<  sanitize(r.getData()->name) << "';";

  connectDB(_reqconn);
  perm_result=queryDB(query.str(), _reqconn);
  if((perm_result==NULL)||(PQntuples(perm_result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("Directory could not be found in the database\n");
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  //get uid an compare
  query.str("");
  query << "SELECT uid FROM users WHERE name = '" << sanitize(r.getData()->sharedInfo.user_name) << "';";
  result = queryDB(query.str(),_reqconn);
  if((result==NULL)||(PQntuples(result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    PQclear(perm_result);
    PQclear(result);
    PQfinish(_reqconn);
    return retStruct;
  }
  sscanf(PQgetvalue(result,0,0),"%lld",&uid);
  PQclear(result);

  sscanf(PQgetvalue(perm_result,0,0),"%lld",&ownerid);
  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(PQgetvalue(perm_result,0,2),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this directory\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  //checking if uid is in the group
  query.str("");
  query << "SELECT uid FROM users_groups WHERE gid = " << sanitize(PQgetvalue(perm_result,0,1)) << " AND uid = " << uid << " ;";
  result = queryDB(query.str(),_reqconn);
  if((result==NULL)||(PQntuples(result)==1))
  {
  //login is in the same group as the file
    if(strcmp(PQgetvalue(perm_result,0,3),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this directory\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(result);
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;

  }
  PQclear(result);

  //else login is somebody else
  if(strcmp(PQgetvalue(perm_result,0,4),"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to read this directory\n");
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
  }
  PQclear(perm_result);
  PQfinish(_reqconn);
  return retStruct;
}


Request_Result_And_Reason VSFPgSQLPlugin::handleRequest(const RequestWriteFile & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
  PGconn* _reqconn;
  PGresult * result;
  PGresult * perm_result;
  stringstream query;
  long long int uid;
  long long int ownerid;

  //get owner and permissions
  query << "SELECT f.owner, f.file_group, f.owner_write, f.group_write, f.others_write FROM files f JOIN directories d ON d.did = f.directory WHERE d.path='" << unslash(sanitize(r.getData()->path)) << "' AND f.name='" << sanitize(r.getData()->name) << "';";

  connectDB(_reqconn);
  perm_result=queryDB(query.str(), _reqconn);
  if((perm_result==NULL)||(PQntuples(perm_result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("File could not be found in the database\n");
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  //get uid an compare
  query.str("");
  query << "SELECT uid FROM users WHERE name = '" << sanitize(r.getData()->sharedInfo.user_name) << "';";
  result = queryDB(query.str(),_reqconn);
  if((result==NULL)||(PQntuples(result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    PQclear(result);
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }
  sscanf(PQgetvalue(result,0,0),"%lld",&uid);
  PQclear(result);

  sscanf(PQgetvalue(perm_result,0,0),"%lld",&ownerid);
  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(PQgetvalue(perm_result,0,2),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this file\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  //checking if uid is in the group
  query.str("");
  query << "SELECT uid FROM users_groups WHERE gid = " << sanitize(PQgetvalue(perm_result,0,1)) << " AND uid = " << uid << " ;";
  result = queryDB(query.str(),_reqconn);
  if((result==NULL)||(PQntuples(result)==1))
  {
  //login is in the same group as the file
    if(strcmp(PQgetvalue(perm_result,0,3),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this file\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(result);
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }
  PQclear(result);

  //else login is somebody else
  if(strcmp(PQgetvalue(perm_result,0,4),"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to write this file\n");
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
  }
  PQclear(perm_result);
  PQfinish(_reqconn);
  return retStruct;
}


Request_Result_And_Reason VSFPgSQLPlugin::handleRequest(const RequestWriteDirectory & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
  PGconn* _reqconn;
  PGresult * result;
  PGresult * perm_result;
  stringstream query;
  long long int uid;
  long long int ownerid;

  //get owner and permissions
  query << "SELECT owner, dir_group, owner_write, group_write, others_write FROM directories WHERE path='" << sanitize(r.getData()->parentPath) <<  sanitize(r.getData()->name) << "';";

  connectDB(_reqconn);
  perm_result=queryDB(query.str(), _reqconn);
  if((perm_result==NULL)||(PQntuples(perm_result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("Directory could not be found in the database\n");
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  //get uid an compare
  query.str("");
  query << "SELECT uid FROM users WHERE name = '" << sanitize(r.getData()->sharedInfo.user_name) << "';";
  result = queryDB(query.str(),_reqconn);
  if((result==NULL)||(PQntuples(result)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    PQclear(result);
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  sscanf(PQgetvalue(result,0,0),"%lld",&uid);
  PQclear(result);

  sscanf(PQgetvalue(perm_result,0,0),"%lld",&ownerid);
  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(PQgetvalue(perm_result,0,2),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this directory\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }

  //checking if uid is in the group
  query.str("");
  query << "SELECT uid FROM users_groups WHERE gid = " << sanitize(PQgetvalue(perm_result,0,1)) << " AND uid = " << uid << " ;";
  result = queryDB(query.str(),_reqconn);
  if((result!=0)||(PQntuples(result)==1))
  {
  //login is in the same group as the file
    if(strcmp(PQgetvalue(perm_result,0,3),"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this directory\n");
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
    }
    PQclear(result);
    PQclear(perm_result);
    PQfinish(_reqconn);
    return retStruct;
  }
  PQclear(result);

  //else login is somebody else
  if(strcmp(PQgetvalue(perm_result,0,4),"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to write this directory\n");
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
  }
  PQclear(perm_result);
  PQfinish(_reqconn);
  return retStruct;
}

Request_Result_Homedir VSFPgSQLPlugin::handleRequest(const RequestHomedir & r) 
{ 
    struct Request_Result_Homedir retStruct = Request_Result_Homedir_Init; 
    stringstream query; 
  PGconn* _reqconn;
  PGresult * result; 
 	query.str(""); 
 	query << "SELECT homedir FROM users WHERE name = '" << sanitize(r.getData()->sharedInfo.user_name) << "'; "; 
  connectDB(_reqconn);
    result = queryDB(query.str(),_reqconn); 
 	if((result!=0)||(PQntuples(result)==1)) 
 	{ 
        retStruct.homedir=PQgetvalue(result,0,0);
 	} 
      PQclear(result); 
      PQfinish(_reqconn);
 	return retStruct; 
} 
