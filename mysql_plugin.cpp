#include "mysql_plugin.h"
#warning the following code is readable only by compilers, trying to understand it may cause permanent brain damage
using namespace std;

MyDatabaseConnection::MyDatabaseConnection()
{
_result=0;
int error_num;
_dbconn=mysql_init(NULL);
mysql_options(_dbconn,MYSQL_READ_DEFAULT_GROUP,"vsftp");
printf("MYSQL: connektink \n");
  if(!mysql_real_connect(_dbconn,tunable_mysql_database_host, tunable_mysql_database_username,
                     tunable_mysql_database_password, tunable_mysql_database_dbname,
                     tunable_mysql_database_port, tunable_mysql_database_socketname,
                     tunable_mysql_database_connectionflags))
  {
    error_num=mysql_errno(_dbconn);
    switch(error_num)
    {
      case CR_CONN_HOST_ERROR:
        throw MySQLConnHostError();
        break;
      case CR_CONNECTION_ERROR:
        throw MySQLConnectionError();
        break;
      case CR_IPSOCK_ERROR:
        throw MySQLIpsockError();
        break;
      case CR_OUT_OF_MEMORY:
        throw MySQLOutOfMemory();
        break;
      case CR_SOCKET_CREATE_ERROR:
        throw MySQLSocketCreateError();
        break;
      case CR_UNKNOWN_HOST:
        throw MySQLUnknownHost();
        break;
      case CR_VERSION_ERROR:
        throw MySQLVersionError();
        break;
      case CR_NAMEDPIPEOPEN_ERROR:
        throw MySQLNamedpipeopenError();
        break;
      case CR_NAMEDPIPEWAIT_ERROR:
        throw MySQLNamedpipewaitError();
        break;
      case CR_NAMEDPIPESETSTATE_ERROR:
        throw MySQLNamedpipesetstateError();
        break;
      case CR_SERVER_LOST:
        throw MySQLServerLost();
        break;
      default:
        throw MySQLUnknown();
        break;
    }
  }
}

MyDatabaseConnection::~MyDatabaseConnection()
{
	mysql_close(_dbconn);
}

MYSQL_RES * MyDatabaseConnection::query(const string query)
{
	int error_num;
  printf("MYSQL query: %s\n",query.c_str());
  if(mysql_real_query(_dbconn,query.c_str(),query.length())!=0)
  {
    error_num=mysql_errno(_dbconn);
    switch(error_num)
    {
      case CR_COMMANDS_OUT_OF_SYNC:
        throw MySQLCommandsOutOfSync();
        break;
      case CR_SERVER_GONE_ERROR:
        throw MySQLServerGoneError();
        break;
      case CR_SERVER_LOST:
        throw MySQLServerLost();
        break;
      case CR_UNKNOWN_ERROR:
        throw MySQLUnknown();
        break;
      default:
        printf("MYSQL Unhandled Error %d : %s\n", mysql_errno(_dbconn), mysql_error(_dbconn));
    }
    printf("MYSQL Error %d : %s\n", mysql_errno(_dbconn), mysql_error(_dbconn));
    return 0;
  }
  _result = mysql_store_result(_dbconn);
  return _result;
}

string VSFMySQLPlugin::sanitize(const string & original)
{
  stringstream retval;
  char * escaped;
  escaped = new char[2*original.length() + 1];
  mysql_real_escape_string(dbcon->getConnection(), escaped,original.c_str(),original.length());
  retval << escaped;
  delete [] escaped;
  return retval.str();
}
/*
void VSFMySQLPlugin::connectDB(MYSQL * & connection)
{
int error_num;
connection=mysql_init(NULL);
mysql_options(connection,MYSQL_READ_DEFAULT_GROUP,"vsftp");
printf("VSMYSQL: connektink %s\n", getTimeStamp().c_str());
  if(!mysql_real_connect(connection,tunable_mysql_database_host, tunable_mysql_database_username,
                     tunable_mysql_database_password, tunable_mysql_database_dbname,
                     tunable_mysql_database_port, tunable_mysql_database_socketname,
                     tunable_mysql_database_connectionflags))
  {
    error_num=mysql_errno(connection);
    switch(error_num)
    {
      case CR_CONN_HOST_ERROR:
        throw MySQLConnHostError();
        break;
      case CR_CONNECTION_ERROR:
        throw MySQLConnectionError();
        break;
      case CR_IPSOCK_ERROR:
        throw MySQLIpsockError();
        break;
      case CR_OUT_OF_MEMORY:
        throw MySQLOutOfMemory();
        break;
      case CR_SOCKET_CREATE_ERROR:
        throw MySQLSocketCreateError();
        break;
      case CR_UNKNOWN_HOST:
        throw MySQLUnknownHost();
        break;
      case CR_VERSION_ERROR:
        throw MySQLVersionError();
        break;
      case CR_NAMEDPIPEOPEN_ERROR:
        throw MySQLNamedpipeopenError();
        break;
      case CR_NAMEDPIPEWAIT_ERROR:
        throw MySQLNamedpipewaitError();
        break;
      case CR_NAMEDPIPESETSTATE_ERROR:
        throw MySQLNamedpipesetstateError();
        break;
      case CR_SERVER_LOST:
        throw MySQLServerLost();
        break;
      default:
        throw MySQLUnknown();
        break;
    }
  }
}
//*/
string VSFMySQLPlugin::sanitize(const char * original)
{
  stringstream retval;
  char * escaped;
  escaped = new char[2*strlen(original) + 1];
  mysql_real_escape_string(dbcon->getConnection(), escaped,original,strlen(original));
  retval << escaped;
  delete [] escaped;
  return retval.str();
}
string VSFMySQLPlugin::unslash(const string & original)
{
	return unslash(original.c_str());
}
string VSFMySQLPlugin::unslash(const char * original)
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


long long int VSFMySQLPlugin::getEid(const struct Event_Shared sharedInfo)
{
	MyDatabaseConnection mycon;
  long long int eid;
  stringstream query;
  MYSQL_RES * result;
  long long int uid;
  uid =getUid(sharedInfo.user_name);
  if(uid==-1) {
  query << "SELECT `eid` FROM `event` WHERE `uid` IS NULL AND `timestamp` = '" << getTimeStamp(sharedInfo.timestamp) << "' AND `ip_address` = '" << getIP(sharedInfo.remote_ip) << "'";
  } else {
    query << "SELECT `eid` FROM `event` WHERE `uid` = " << uid << " AND `timestamp` = '" << getTimeStamp(sharedInfo.timestamp) << "' AND `ip_address` = '" << getIP(sharedInfo.remote_ip) << "'";
  }

  result = mycon.query(query.str());
  if((result==0)||(mysql_num_rows(result)!=1))
  {
    mysql_free_result(result);
    printf("MYSQL:TOO MANY SIMILAR EVENTS!\n");
    return -1;
  }
  else
  {
  MYSQL_ROW row;
  row = mysql_fetch_row(result);
  sscanf(row[0],"%lld",&eid);
  mysql_free_result(result);
  return eid;
  }
}

string VSFMySQLPlugin::getTimeStamp()
{
  struct tm tstamp;
  time_t rawtime;
  stringstream tsstream;
  
  time ( &rawtime );
  gmtime_r(&rawtime,&tstamp);
  
  
  tsstream << setw(4) << right << setfill('0') << 1900+tstamp.tm_year << "-" <<
      setw(2) << right << setfill('0') << tstamp.tm_mon+1 << "-" 
      << setw(2) << right << setfill('0') << tstamp.tm_mday << " " << 
      setw(2) << right << setfill('0') << tstamp.tm_hour << ":" << 
      setw(2) << right << setfill('0') << tstamp.tm_min << ":" << 
      setw(2) << right << setfill('0') << tstamp.tm_sec;
      
  return tsstream.str();
}

long long int VSFMySQLPlugin::getUid(string username)
{
	MyDatabaseConnection mycon;
  long long int uid;
  MYSQL_RES * result;
  stringstream querystream;
  
  querystream << "SELECT `uid` FROM `users` WHERE `name` = '" << sanitize(username) << "'";
  
  
  result = mycon.query(querystream.str());
  if((result==0)||(mysql_num_rows(result)!=1))
  {
    mysql_free_result(result);
    printf("VSMYSQL:NO SUCH USER!\n");
    return -1;
  }
  else
  {
  MYSQL_ROW row;
  row = mysql_fetch_row(result);
  sscanf(row[0],"%lld",&uid);
  mysql_free_result(result);
  return uid;
  }
}

string VSFMySQLPlugin::getIP(const struct sockaddr_in& sa_in )
{
  char * ipaddr;
  ipaddr=new char[16];
  if(sa_in.sin_family==AF_INET)
  {
    inet_ntop(sa_in.sin_family,&(sa_in.sin_addr),ipaddr,16);
  }
  else
  {
    printf("VSFMYSQL: vsftp-database does not support IPv6, but it's an easy feature to add yourself :)\n");
  }
  string ipstring;
  ipstring.assign(ipaddr);
  return ipstring;
}

string VSFMySQLPlugin::getTimeStamp(struct timeval tval)
{
  struct tm tstamp;
  stringstream tsstream;

  gmtime_r(&(tval.tv_sec),&tstamp);

  tsstream << setw(4) << right << setfill('0') << 1900+tstamp.tm_year << "-" <<
      setw(2) << right << setfill('0') << tstamp.tm_mon+1 << "-" 
      << setw(2) << right << setfill('0') << tstamp.tm_mday << " " << 
      setw(2) << right << setfill('0') << tstamp.tm_hour << ":" << 
      setw(2) << right << setfill('0') << tstamp.tm_min << ":" << 
      setw(2) << right << setfill('0') << tstamp.tm_sec;
  return tsstream.str();
}
/*
MYSQL_RES * VSFMySQLPlugin::queryDB(const string query)
{
  return queryDB(query,_dbconn);
}

MYSQL_RES * VSFMySQLPlugin::queryDB(const string query, MYSQL * connection)
{
  int error_num;
  printf("%s\n",query.c_str());
  if(mysql_real_query(connection,query.c_str(),query.length())!=0)
  {
    error_num=mysql_errno(connection);
    switch(error_num)
    {
      case CR_COMMANDS_OUT_OF_SYNC:
        throw MySQLCommandsOutOfSync();
        break;
      case CR_SERVER_GONE_ERROR:
        throw MySQLServerGoneError();
        break;
      case CR_SERVER_LOST:
        throw MySQLServerLost();
        break;
      case CR_UNKNOWN_ERROR:
        throw MySQLUnknown();
        break;
      default:
        cout << "QS:" << query << ":QE" << endl;
        printf("EVEN WORSE %d\n!",error_num);
    }
    printf("Error %d : %s\n", mysql_errno(connection), mysql_error(connection));
    return 0;
  }
  return mysql_store_result(connection);
}
//*/
VSFMySQLPlugin::VSFMySQLPlugin(VSFPluginArchitecture & arch) : VSFPlugin(arch), PlArch(arch)
{
  stringstream querystream;
  //init variables;
  commit.assign("COMMIT");
//init connection to the database and check if the tables exist

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
  //init connection handler
/*
  //VS _dbconn=mysql_init(NULL);
  
  //connect to the DB
  //VS mysql_options(_dbconn,MYSQL_READ_DEFAULT_GROUP,"vsftp");
printf("VSMYSQL: connektink %s\n", getTimeStamp().c_str());
  if(!mysql_real_connect(_dbconn,tunable_mysql_database_host, tunable_mysql_database_username,
                     tunable_mysql_database_password, tunable_mysql_database_dbname,
                     tunable_mysql_database_port, tunable_mysql_database_socketname,
                     tunable_mysql_database_connectionflags))
  {
    error_num=mysql_errno(_dbconn);
    switch(error_num)
    {
      case CR_CONN_HOST_ERROR:
        throw MySQLConnHostError();
        break;
      case CR_CONNECTION_ERROR:
        throw MySQLConnectionError();
        break;
      case CR_IPSOCK_ERROR:
        throw MySQLIpsockError();
        break;
      case CR_OUT_OF_MEMORY:
        throw MySQLOutOfMemory();
        break;
      case CR_SOCKET_CREATE_ERROR:
        throw MySQLSocketCreateError();
        break;
      case CR_UNKNOWN_HOST:
        throw MySQLUnknownHost();
        break;
      case CR_VERSION_ERROR:
        throw MySQLVersionError();
        break;
      case CR_NAMEDPIPEOPEN_ERROR:
        throw MySQLNamedpipeopenError();
        break;
      case CR_NAMEDPIPEWAIT_ERROR:
        throw MySQLNamedpipewaitError();
        break;
      case CR_NAMEDPIPESETSTATE_ERROR:
        throw MySQLNamedpipesetstateError();
        break;
      case CR_SERVER_LOST:
        throw MySQLServerLost();
        break;
      default:
        throw MySQLUnknown();
        break;
    }
  }
//*/
	dbcon=new MyDatabaseConnection;
//connected to the database or exceptioned out

//add server startup to logs
  querystream << "INSERT INTO `event` (`eid`, `uid`, `timestamp`, `ip_address`, `event_type`) VALUES (DEFAULT, NULL, '" << getTimeStamp() << "', '', 'Server Startup')";
  dbcon->query(querystream.str()); 
  dbcon->query(commit);
//check if folders exist on HDD
}

VSFMySQLPlugin::~VSFMySQLPlugin()
{
stringstream querystream;

querystream << "INSERT INTO `event` (`eid`, `uid`, `timestamp`, `ip_address`, `event_type`) VALUES (DEFAULT, NULL, '" << getTimeStamp() << "','','Server Shutdown')";  

  dbcon->query(querystream.str()); 
  dbcon->query(commit);
  //disconnect from database
  delete dbcon;
  //free all allocated space

  PlArch.unregisterEventHandler(*this, Event_Number_Chmod);
  PlArch.unregisterEventHandler(*this, Event_Number_Connection);
  PlArch.unregisterEventHandler(*this, Event_Number_Delete);
  PlArch.unregisterEventHandler(*this, Event_Number_Download);
  PlArch.unregisterEventHandler(*this, Event_Number_FailedLogin);
  PlArch.unregisterEventHandler(*this, Event_Number_Login);
  PlArch.unregisterEventHandler(*this, Event_Number_Mkdir);
  PlArch.unregisterEventHandler(*this, Event_Number_Rename);
  PlArch.unregisterEventHandler(*this, Event_Number_Rmdir);
  PlArch.unregisterEventHandler(*this, Event_Number_Upload);
  PlArch.unregisterEventHandler(*this, Event_Number_CliEnd);

  PlArch.unregisterRequestHandler(*this, Request_Number_Login);
  PlArch.unregisterRequestHandler(*this, Request_Number_ReadFile);
  PlArch.unregisterRequestHandler(*this, Request_Number_ReadDirectory);
  PlArch.unregisterRequestHandler(*this, Request_Number_WriteFile);
  PlArch.unregisterRequestHandler(*this, Request_Number_WriteDirectory);
  PlArch.unregisterRequestHandler(*this, Request_Number_Homedir);
}

void VSFMySQLPlugin::handleEvent(const EventChmod & event)
{
  int file_true;
  int improvise;
  MyDatabaseConnection mycon;
MYSQL_RES * result;
MYSQL_ROW row;
long long int ourfid;

	stringstream querystream;
	int old_owner_write,old_owner_read,new_owner_write,new_owner_read;
	int old_group_write,old_group_read,new_group_write,new_group_read;
	int old_others_write,old_others_read,new_others_write,new_others_read;
	long long int uid;

	file_true=0;
	improvise=0;
  querystream.str("");
  querystream << "SELECT `owner_read`, `owner_write`, `group_read`, `group_write`, `others_read`, `others_write` FROM `directories` WHERE `path` = '" << sanitize(event.getData()->path)  << sanitize(event.getData()->name) << "'";
  result = mycon.query(querystream.str());
  
  if((result==0)||(mysql_num_rows(result)!=1))
  {
    querystream.str("");
    querystream << "SELECT f.`owner_read`, f.`owner_write`, f.`group_read`, f.`group_write`, f.`others_read`, f.`others_write` FROM `files` f JOIN `directories` d ON d.`did` = f.`directory` WHERE f.`name` = '" << sanitize(event.getData()->name) << "' AND d.`path` = '" << unslash(sanitize(event.getData()->path)) << "'";
    result = mycon.query(querystream.str());

    file_true=1;
    if((result==0)||(mysql_num_rows(result)!=1))
    {
      improvise=1;
    }
  }
  

	if(improvise==0)
  {
  row = mysql_fetch_row(result);
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
  if(improvise==0)
  {
		old_owner_read=(strcmp(row[0],"0")==0)?0:1;
		old_owner_write=(strcmp(row[1],"0")==0)?0:1;
		old_group_read=(strcmp(row[2],"0")==0)?0:1;
		old_group_write=(strcmp(row[3],"0")==0)?0:1;
		old_others_read=(strcmp(row[4],"0")==0)?0:1;
		old_others_write=(strcmp(row[5],"0")==0)?0:1;
  }
  else
  {
  	old_owner_read=0;
		old_owner_write=0;
		old_group_read=0;
		old_group_write=0;
		old_others_read=0;
		old_others_write=0;
  }
  //basic inserting
  querystream.str("");
  uid=getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Chmod')";
  } else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Chmod')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);
  
  querystream.str("");
  querystream << "INSERT INTO `event_chmod` (`eid`,`path`,`old_owner_write`,`old_owner_read`,`old_group_write`,`old_group_read`,`old_others_write`,`old_others_read`,`new_owner_write`,`new_owner_read`,`new_group_write`,`new_group_read`,`new_others_write`,`new_others_read`,`succeeded`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->path) << sanitize(event.getData()->name) << "', " << old_owner_write << ", " << old_owner_read << ", " << old_group_write << ", " << old_group_read << ", " << old_others_write << ", " << old_others_read << ", " << new_owner_write << ", " << new_owner_read << ", " << new_group_write << ", " << new_group_read << ", " << new_others_write << ", " << new_others_read << ", " << event.getData()->succeeded << ")";
  
  mycon.query(querystream.str());
  mycon.query(commit);
  
  //insert to database
  if(improvise==1)
  {
  	return;
  }
	querystream.str("");
	querystream << "SELECT `fid` FROM `files` f JOIN `directories` d ON f.`directory` = d.`did` WHERE f.`name` = '" 
				<< sanitize(event.getData()->name) << "' AND d.`path` = '" << unslash(sanitize(event.getData()->path)) << 
				"' ";
	result=mycon.query(querystream.str());
	if((result==0)||(mysql_num_rows(result)!=1))
	{
		//throw MySQLFileEntryNotFound;
		querystream.str("");
		querystream << "SELECT `did` FROM `directories` WHERE `path` = '" << sanitize(event.getData()->path) << sanitize(event.getData()->name) << "' ";
		result=mycon.query(querystream.str());
		if((result==0)||(mysql_num_rows(result)!=1))
		{
			printf("MYSQL: file or dir for chmod not found in database\n");
			mysql_free_result(result);
			return;
		}
		row = mysql_fetch_row(result);
		sscanf(row[0],"%lld",&ourfid);
		querystream.str("");
		//modifying dir
		querystream << "UPDATE `directories` SET `owner_read` = "<< new_owner_read << " , `owner_write` = " <<
          new_owner_write << " , `group_read` = "<< new_group_read << " , `group_write` = "<< new_group_write <<
          " , `others_read` = " <<  new_others_read << " , `others_write` = "<< new_others_write << 
          " WHERE `did` = " << ourfid << " ";
		mycon.query(querystream.str());
		mycon.query(commit);
		return;
	}
	row = mysql_fetch_row(result);
	sscanf(row[0],"%lld",&ourfid);
	querystream.str("");
		//modifying file
		querystream << "UPDATE `files` SET `owner_read` = "<< new_owner_read << " , `owner_write` = " <<
          new_owner_write << " , `group_read` = " << new_group_read << " , `group_write` = " << new_group_write <<
          " , `others_read` = " <<  new_others_read << " , `others_write` = " << new_others_write << 
          " WHERE `fid` = " << ourfid << " ";
	mycon.query(querystream.str());
	mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventConnection & event)
{
printf("MYSQL**** : Event Connection!\n");
MyDatabaseConnection mycon;
stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Connection')";
  } else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Connection')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);

  querystream.str("");
  querystream << "INSERT INTO `event_connection` (`eid`,`reason`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->reason) << "')";
  
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventDelete & event)
{
  MyDatabaseConnection mycon;
stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Delete')";
  } else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Delete')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);
  //deleting the entry from the database (if succeded)
  if(event.getData()->succeeded == 0)
  {
    std::pair<std::string,std::string> filnam;
    filnam = VSFUtils::sanitizePath(event.getData()->path,"" );
    querystream.str("");
    querystream << "DELETE FROM `files` WHERE `fid`=(SELECT `fid` FROM `files` f JOIN `directories` d ON f.`directory` = d.`did` WHERE f.`name` = '" << sanitize(filnam.second) << "' AND d.`path` = '" 
    << unslash(sanitize(filnam.first)) << "' LIMIT 1 )";
    mycon.query(querystream.str());
    mycon.query(commit);
  }
  querystream.str("");
  querystream << "INSERT INTO `event_delete` (`eid`,`path`,`succeeded`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->path) << "', '"<< event.getData()->succeeded << "')";
  
  mycon.query(querystream.str());
  mycon.query(commit);

}

void VSFMySQLPlugin::handleEvent(const EventDownload & event)
{
MyDatabaseConnection mycon;	
stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Download')";
  } else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " <<  uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Download')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);
  querystream.str("");
  querystream << "INSERT INTO `event_download` (`eid`,`path`,`filename`,`size`,`is_complete`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << unslash(sanitize(event.getData()->fileInfo.path)) << "', '" << sanitize(event.getData()->fileInfo.name) << "', " << event.getData()->fileInfo.size << ", " << event.getData()->is_complete << ")";
  
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventFailedLogin & event)
{
MyDatabaseConnection mycon;
stringstream querystream;
long long int uid;
  uid =getUid(event.getData()->sharedInfo.user_name);
      if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'FailedLogin')";
      }else {
        querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'FailedLogin')";
      }
  mycon.query(querystream.str());
  mycon.query(commit);
  querystream.str("");
  querystream << "INSERT INTO `event_failedlogin` (`eid`,`failed_password`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->failed_password) << "')";
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventLogin & event)
{
MyDatabaseConnection mycon;
stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
      if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Login')";
      } else {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Login')";
      }
  mycon.query(querystream.str());
  mycon.query(commit);
  querystream.str("");
  querystream << "INSERT INTO `event_login` (`eid`,`is_anonymous`) VALUES (" << getEid(event.getData()->sharedInfo) << ", " << event.getData()->is_anonymous << ")";
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventMkdir & event)
{
MyDatabaseConnection mycon;
stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Mkdir')";
  } else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT," << uid <<",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Mkdir')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);
  
  //adding dir to the database (if succeded)
  if(event.getData()->succeeded == 0)
  {
    querystream.str("");
    querystream << "INSERT INTO `directories` (`did`, `path`, `owner`, `dir_group`, `owner_read`, `owner_write`, `group_read`, `group_write`, `others_read` , `others_write`) VALUES (DEFAULT, '"<<sanitize(event.getData()->path) << "', "<< ((uid!=-1)?(uid):0) <<" , 0 , " << ((tunable_local_umask & 0400)?0:1) << " , " << ((tunable_local_umask & 0200)?0:1) << " , " << ((tunable_local_umask & 040)?0:1) << " , " << ((tunable_local_umask & 020)?0:1) << " , " << ((tunable_local_umask & 04)?0:1) << " , " << ((tunable_local_umask & 02)?0:1) << " ) "; 
    mycon.query(querystream.str());
    mycon.query(commit);
  }
  
  querystream.str("");
  querystream << "INSERT INTO `event_mkdir` (`eid`,`path`,`succeeded`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->path) << "', " << event.getData()->succeeded << ")";
  
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventRename & event)
{
MyDatabaseConnection mycon;
MYSQL_RES * result;
MYSQL_ROW row;
long long int ourfid;
stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Rename')";
  } else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, "<< uid <<",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Rename')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);
  //changing the file name in the database (if succeded)

  
    if(event.getData()->succeeded==0)
  {
    std::pair<std::string,std::string> oldnam;
    std::pair<std::string,std::string> newnam;
    oldnam=VSFUtils::sanitizePath(event.getData()->old_name,"");
    newnam=VSFUtils::sanitizePath(event.getData()->new_name,"");
    querystream.str("");
    querystream << "SELECT `fid` FROM `files` f JOIN `directories` d ON f.`directory` = d.`did` WHERE f.`name` = '" 
          << sanitize(oldnam.second) << "' AND d.`path` = '" << unslash(sanitize(oldnam.first )) << 
          "' ";
    result=mycon.query(querystream.str());
  	if((result==0)||(mysql_num_rows(result)!=1))
  	{
			//throw MySQLFileEntryNotFound;
			//maybe dir?
			querystream.str("");
			querystream << "SELECT `did` FROM `directories` WHERE `path` = '" << sanitize(oldnam.first) << sanitize(oldnam.second) << "' ";
			result=mycon.query(querystream.str());
			if((result==0)||(mysql_num_rows(result)!=1))
			{
				printf("MYSQL: file or dir for renaming not found in database\n");
				mysql_free_result(result);
				return;
			}
	  	row = mysql_fetch_row(result);
			sscanf(row[0],"%lld",&ourfid);
			querystream.str("");
      //modifying dir
      querystream << "UPDATE `directories` SET `path`= '" << sanitize(oldnam.first) << sanitize(newnam.second) << "'  WHERE `did` = " << ourfid << " ";
			mycon.query(querystream.str());
			mycon.query(commit);
			return;
		}
  	row = mysql_fetch_row(result);
  	sscanf(row[0],"%lld",&ourfid);
    querystream.str("");
      //modifying file
      querystream << "UPDATE `files` SET `name`= '" << sanitize(newnam.second) << "' , `directory` = (SELECT `did` FROM `directories` WHERE `path`='" << unslash(sanitize(newnam.first)) << "' LIMIT 1) WHERE `fid` = " << ourfid << " ";
    printf("OLDNAME: %s\n",event.getData()->old_name); 
    mycon.query(querystream.str());
    mycon.query(commit);
  }
  
  
  querystream.str("");
  querystream << "INSERT INTO `event_rename` (`eid`,`oldname`,`newname`,`succeeded`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->old_name) << "', '"<< sanitize(event.getData()->new_name) << "', " << event.getData()->succeeded << ")";
  
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventRmdir & event)
{
MyDatabaseConnection mycon;
MYSQL_RES * result;
MYSQL_ROW row;
long long int ourdid;
  stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Rmdir')";
  }else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Rmdir')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);

  //removing dir from the database (if succeded)
  
    if(event.getData()->succeeded == 0)
  {
  	querystream.str("");
  	querystream << "SELECT `did` FROM `directories` WHERE `path` = '" << sanitize(event.getData()->path) << "' ";
    result=mycon.query(querystream.str());
  	if((result==0)||(mysql_num_rows(result)!=1))
  	{
			//throw MySQLFileEntryNotFound;
			printf("MYSQL: dir for deleting not found - database integrity breached\n");
			mysql_free_result(result);
		  return;
		}
  	row = mysql_fetch_row(result);
  	sscanf(row[0],"%lld",&ourdid);
  	
    querystream.str("");
    querystream << "DELETE FROM `directories` WHERE `did`=" << ourdid << " ";
    mycon.query(querystream.str());
    mycon.query(commit);
  }

  
  
  
  querystream.str("");
  querystream << "INSERT INTO `event_rmdir` (`eid`,`path`,`succeeded`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << sanitize(event.getData()->path) << "', " << event.getData()->succeeded << ")";
  
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventCliEnd & event)
{
MyDatabaseConnection mycon;
stringstream querystream;
  
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL, '" << getTimeStamp(event.getData()->timestamp) << "','', 'CliEnd')";
  mycon.query(querystream.str());
  mycon.query(commit);
}

void VSFMySQLPlugin::handleEvent(const EventUpload & event)
{
MyDatabaseConnection mycon;
stringstream querystream;
long long int uid;
  uid = getUid(event.getData()->sharedInfo.user_name);
  if(uid==-1) {
  querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, NULL,'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Upload')";
  } else {
    querystream << "INSERT INTO `event` (`eid`,`uid`,`timestamp`,`ip_address`,`event_type`) VALUES (DEFAULT, " << uid << ",'" << getTimeStamp(event.getData()->sharedInfo.timestamp) << "','" << getIP(event.getData()->sharedInfo.remote_ip) << "', 'Upload')";
  }
  mycon.query(querystream.str());
  mycon.query(commit);

  //creating database entry (if succeded)
  
    if((event.getData()->is_complete==1)&&(event.getData()->is_append==0)&&(event.getData()->is_overwrite==0)) //complete and not append or overwrite -> insert a file
  {
    querystream.str("");
    querystream << "INSERT INTO `files` (`fid`, `name`, `directory`, `owner`, `file_group`, `owner_read`, `owner_write`, `group_read`, `group_write`, `others_read` , `others_write`) VALUES (DEFAULT, '"<< sanitize(event.getData()->fileInfo.name)  << "' , (SELECT `did` FROM `directories` WHERE `path` = '" << unslash(sanitize(event.getData()->fileInfo.path)) << "' LIMIT 1) , "<< ((uid!=-1)?(uid):0) <<" , 0 , " << ((tunable_local_umask & 0400)?0:1) << " , " << ((tunable_local_umask & 0200)?0:1) << " , " << ((tunable_local_umask & 040)?0:1) << " , " << ((tunable_local_umask & 020)?0:1) << " , " << ((tunable_local_umask & 04)?0:1) << " , " << ((tunable_local_umask & 02)?0:1) << " ) "; 
    mycon.query(querystream.str());
    mycon.query(commit);
  }
  
  querystream.str("");
  querystream << "INSERT INTO `event_upload` (`eid`,`path`,`filename`,`size`,`is_complete`,`is_append`,`is_overwrite`) VALUES (" << getEid(event.getData()->sharedInfo) << ", '" << unslash(sanitize(event.getData()->fileInfo.path)) << "', '" << sanitize(event.getData()->fileInfo.name) << "', " << event.getData()->fileInfo.size << ", " << event.getData()->is_complete << ", " << event.getData()->is_append << ", " << event.getData()->is_overwrite << ")";
  
  mycon.query(querystream.str());
  mycon.query(commit);
}

Request_Result VSFMySQLPlugin::handleRequest(const RequestLogin & r)
{
	MyDatabaseConnection mycon;
  struct Request_Result retStruct = Request_Result_Init;
  stringstream query;
  MYSQL_RES * result;
  retStruct.result=-1;
  query << "SELECT * FROM `users` WHERE name='" << sanitize(r.getData()->login) << "' AND passwd=SHA1('" << sanitize(r.getData()->password) << "')";


  result=mycon.query(query.str());
  if((result!=0)&&(mysql_num_rows(result)==1))
  {
    retStruct.result=0;
    query.str("");
    query << "UPDATE `users` SET `last_login`= '" << sanitize(getTimeStamp()) << "' WHERE name='" << sanitize(r.getData()->login) << "' ";
    mycon.query(query.str());
    mycon.query(commit);
  }
  return retStruct;
}



Request_Result_And_Reason VSFMySQLPlugin::handleRequest(const RequestWriteFile & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
	MyDatabaseConnection mycon;
  MYSQL_RES * result1;
  MYSQL_RES * result2;
  MYSQL_ROW permissions_row;
  MYSQL_ROW row;
  stringstream query;
  long long int uid;
  long long int ownerid;
  
  //get owner and permissions
  query << "SELECT f.`owner`, f.`file_group`, f.`owner_write`, f.`group_write`, f.`others_write` FROM `files` f JOIN `directories` d ON d.`did` = f.`directory` WHERE d.`path`='" << unslash(sanitize(r.getData()->path)) << "' AND f.`name`='" << sanitize(r.getData()->name) << "'";
  
  result1=mycon.query(query.str());
  if((result1==0)||(mysql_num_rows(result1)!=1))
  {
    retStruct.result=0;
    retStruct.reason=strdup("File could not be found in the database\n");
    if(result1!=0)
    {
    	mysql_free_result(result1);
    }
    return retStruct;
  }
  permissions_row = mysql_fetch_row(result1);

  //get uid an compare
  query.str("");
  query << "SELECT `uid` FROM `users` WHERE `name` = '" << sanitize(r.getData()->sharedInfo.user_name) << "'";
  
  result2 = mycon.query(query.str());
  if((result2==0)||(mysql_num_rows(result2)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    if(result2!=0)
    {
    	mysql_free_result(result2);
    }
    return retStruct;
  }

  row = mysql_fetch_row(result2);
  sscanf(row[0],"%lld",&uid);
  mysql_free_result(result2);

  sscanf(permissions_row[0],"%lld",&ownerid);
  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(permissions_row[2],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this file\n");
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result1);
      return retStruct;
    }
  }
  mysql_free_result(result2);
  //checking if uid is in the group
  query.str("");
  query << "SELECT `uid` FROM `users_groups` WHERE `gid` = " << sanitize(permissions_row[1]) << " AND `uid` = " << uid << " ";
  
  result2 = mycon.query(query.str());
  if((result2!=0)&&(mysql_num_rows(result2)==1))
  {
  //login is in the same group as the file
    if(strcmp(permissions_row[3],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this file\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
  }
  mysql_free_result(result2);

  //else login is somebody else
  if(strcmp(permissions_row[4],"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to write this file\n");
    mysql_free_result(result1);
    return retStruct;
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
    mysql_free_result(result1);
    return retStruct;
  }
  //unreachable point in code
}

Request_Result_And_Reason VSFMySQLPlugin::handleRequest(const RequestWriteDirectory & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
	MyDatabaseConnection mycon;
  MYSQL_RES * result1;
  MYSQL_RES * result2;
  MYSQL_ROW permissions_row;
  MYSQL_ROW row;
  stringstream query;
  long long int uid=0;
  long long int ownerid=0;
  
  //get owner and permissions
  query << "SELECT `owner`, `dir_group`, `owner_write`, `group_write`, `others_write` FROM `directories` WHERE `path`='" << sanitize(r.getData()->parentPath) << sanitize(r.getData()->name) << "'";
  
  result1=mycon.query(query.str());
  if((result1==0)||(mysql_num_rows(result1)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("Directory could not be found in the database\n");
    if(result1!=0)
    {
    	mysql_free_result(result1);
    }
    return retStruct;
  }

  permissions_row = mysql_fetch_row(result1);
  sscanf(permissions_row[0],"%lld",&ownerid);
  

  
  //get uid an compare
  query.str("");
  query << "SELECT `uid` FROM `users` WHERE `name` = '" << sanitize(r.getData()->sharedInfo.user_name) << "' ";
  
  result2 = mycon.query(query.str());
  if((result2==0)||(mysql_num_rows(result2)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    mysql_free_result(result2);
    mysql_free_result(result1);
    return retStruct;
  }
  row = mysql_fetch_row(result2);
  sscanf(row[0],"%lld",&uid);
  
  mysql_free_result(result2);

  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(permissions_row[2],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this directory\n");
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result1);
      return retStruct;
    }
  }
  //checking if uid is in the group
  query.str("");
  query << "SELECT `uid` FROM `users_groups` WHERE `gid` = " << sanitize(permissions_row[1]) << " AND `uid` = " << uid << " ";
  
  result2 = mycon.query(query.str());
  if((result2!=0)&&(mysql_num_rows(result2)==1))
  {
  //login is in the same group as the file
    if(strcmp(permissions_row[3],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to write this directory\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
  }
  if(result2!=0)
  {
  	mysql_free_result(result2);
  }

  //else login is somebody else
  if(strcmp(permissions_row[4],"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to write this directory\n");
    mysql_free_result(result1);
    return retStruct;
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
    mysql_free_result(result1);
    return retStruct;
  }
  //unreachable point in code
}


Request_Result_And_Reason VSFMySQLPlugin::handleRequest(const RequestReadFile & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
	MyDatabaseConnection mycon;
  MYSQL_RES * result1;
  MYSQL_RES * result2;
  MYSQL_ROW permissions_row;
  MYSQL_ROW row;
  stringstream query;
  long long int uid;
  long long int ownerid;
  
  //get owner and permissions
  query << "SELECT f.`owner`, f.`file_group`, f.`owner_read`, f.`group_read`, f.`others_read` FROM `files` f JOIN `directories` d ON d.`did` = f.`directory` WHERE d.`path`='" << unslash(sanitize(r.getData()->path)) << "' AND f.`name`='" << sanitize(r.getData()->name) << "'";
  
  result1=mycon.query(query.str());
  if((result1==0)||(mysql_num_rows(result1)!=1))
  {
    retStruct.result=0;
    retStruct.reason=strdup("File could not be found in the database\n");
    if(result1!=0)
    {
    	mysql_free_result(result1);
    }
    return retStruct;
  }
  permissions_row = mysql_fetch_row(result1);

  //get uid an compare
  query.str("");
  query << "SELECT `uid` FROM `users` WHERE `name` = '" << sanitize(r.getData()->sharedInfo.user_name) << "'";
  
  result2 = mycon.query(query.str());
  if((result2==0)||(mysql_num_rows(result2)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    if(result2!=0)
    {
    	mysql_free_result(result2);
    }
    return retStruct;
  }

  row = mysql_fetch_row(result2);
  sscanf(row[0],"%lld",&uid);
  mysql_free_result(result2);

  sscanf(permissions_row[0],"%lld",&ownerid);
  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(permissions_row[2],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this file\n");
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result1);
      return retStruct;
    }
  }
  mysql_free_result(result2);
  //checking if uid is in the group
  query.str("");
  query << "SELECT `uid` FROM `users_groups` WHERE `gid` = " << sanitize(permissions_row[1]) << " AND `uid` = " << uid << " ";
  
  result2 = mycon.query(query.str());
  if((result2!=0)&&(mysql_num_rows(result2)==1))
  {
  //login is in the same group as the file
    if(strcmp(permissions_row[3],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this file\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
  }
  mysql_free_result(result2);

  //else login is somebody else
  if(strcmp(permissions_row[4],"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to read this file\n");
    mysql_free_result(result1);
    return retStruct;
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
    mysql_free_result(result1);
    return retStruct;
  }
  //unreachable point in code
}

Request_Result_And_Reason VSFMySQLPlugin::handleRequest(const RequestReadDirectory & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;
	MyDatabaseConnection mycon;
  MYSQL_RES * result1;
  MYSQL_RES * result2;
  MYSQL_ROW permissions_row;
  MYSQL_ROW row;
  stringstream query;
  long long int uid=0;
  long long int ownerid=0;
  
  //get owner and permissions
  query << "SELECT `owner`, `dir_group`, `owner_read`, `group_read`, `others_read` FROM `directories` WHERE `path`='" << sanitize(r.getData()->parentPath) << sanitize(r.getData()->name) << "'";
  
  result1=mycon.query(query.str());
  if((result1==0)||(mysql_num_rows(result1)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("Directory could not be found in the database\n");
    if(result1!=0)
    {
    	mysql_free_result(result1);
    }
    return retStruct;
  }

  permissions_row = mysql_fetch_row(result1);
  sscanf(permissions_row[0],"%lld",&ownerid);
  

  
  //get uid an compare
  query.str("");
  query << "SELECT `uid` FROM `users` WHERE `name` = '" << sanitize(r.getData()->sharedInfo.user_name) << "' ";
  
  result2 = mycon.query(query.str());
  if((result2==0)||(mysql_num_rows(result2)!=1))
  {
    retStruct.result=-1;
    retStruct.reason=strdup("The user could not be found in the database\n");
    mysql_free_result(result2);
    mysql_free_result(result1);
    return retStruct;
  }
  row = mysql_fetch_row(result2);
  sscanf(row[0],"%lld",&uid);
  
  mysql_free_result(result2);

  if(uid==ownerid)
  {
  //login is owner
    if(strcmp(permissions_row[2],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this directory\n");
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result1);
      return retStruct;
    }
  }
  //checking if uid is in the group
  query.str("");
  query << "SELECT `uid` FROM `users_groups` WHERE `gid` = " << sanitize(permissions_row[1]) << " AND `uid` = " << uid << " ";
  
  result2 = mycon.query(query.str());
  if((result2!=0)&&(mysql_num_rows(result2)==1))
  {
  //login is in the same group as the file
    if(strcmp(permissions_row[3],"0")==0)
    {
      retStruct.result=-1;
      retStruct.reason=strdup("You don't have permissions to read this directory\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
    else
    {
      retStruct.result=0;
      retStruct.reason=strdup("OK\n");
      mysql_free_result(result2);
      mysql_free_result(result1);
      return retStruct;
    }
  }
  if(result2!=0)
  {
  	mysql_free_result(result2);
  }

  //else login is somebody else
  if(strcmp(permissions_row[4],"0")==0)
  {
    retStruct.result=-1;
    retStruct.reason=strdup("You don't have permissions to read this directory\n");
    mysql_free_result(result1);
    return retStruct;
  }
  else
  {
    retStruct.result=0;
    retStruct.reason=strdup("OK\n");
    mysql_free_result(result1);
    return retStruct;
  }
  //unreachable point in code
}



struct Request_Result_Homedir VSFMySQLPlugin::handleRequest(const RequestHomedir & r)
{
	MyDatabaseConnection mycon;
  struct Request_Result_Homedir retStruct = Request_Result_Homedir_Init;
  stringstream query;
  MYSQL_RES * result;
  MYSQL_ROW row;

  query.str("");
  query << "SELECT `homedir` FROM `users` WHERE `name` = '" << sanitize(r.getData()->sharedInfo.user_name) << "' ";
  result = mycon.query(query.str());
  if((result!=0)&&(mysql_num_rows(result)==1))
  {
  	row = mysql_fetch_row(result);
  	retStruct.homedir=strdup(row[0]);
  }
  return retStruct;
}
