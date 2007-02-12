#include "file_logger_plugin.h"

VSFFileLoggerPlugin::VSFFileLoggerPlugin(VSFPluginArchitecture & arch, std::string filePath) 
  : VSFPlugin(arch), logfile(filePath.c_str(), std::ios_base::out | std::ios_base::app), PlArch(arch) {  
  printf("VSFFileLoggerPlugin starting :)\n");
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

  arch.registerRequestHandler(*this, Request_Number_Login);
  arch.registerRequestHandler(*this, Request_Number_ReadFile);
  arch.registerRequestHandler(*this, Request_Number_ReadDirectory);
  arch.registerRequestHandler(*this, Request_Number_WriteFile);
  arch.registerRequestHandler(*this, Request_Number_WriteDirectory);  

  printf("VSFFileLoggerPlugin requests all done :)\n");

  printf("VSFFileLoggerPlugin constructor all done :)\n");
}

VSFFileLoggerPlugin::~VSFFileLoggerPlugin()
{
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

  logfile << "[destructor] server going down, bye\n";
}

void VSFFileLoggerPlugin::handleEvent(const EventChmod & e) {

}

void VSFFileLoggerPlugin::handleEvent(const EventConnection & e) {

}

void VSFFileLoggerPlugin::handleEvent(const EventDelete & e) {

}

void VSFFileLoggerPlugin::handleEvent(const EventDownload & e) {
  logfile << "A download occured: ";
  logfile << "File: " << e.getData()->fileInfo.path << "/" << e.getData()->fileInfo.name;
  logfile << " (size : " << e.getData()->fileInfo.size << " )";
  logfile << " user name : " << e.getData()->sharedInfo.user_name;
  logfile << " timestamp : " << e.getData()->sharedInfo.timestamp.tv_sec << " sec " << e.getData()->sharedInfo.timestamp.tv_usec << " usec";
  logfile << " ip: " << e.getData()->sharedInfo.remote_ip.sin_addr.s_addr << std::endl;
}

void VSFFileLoggerPlugin::handleEvent(const EventFailedLogin & e) {
  logfile << "Failed login occured";
  logfile << " password used: " << e.getData()->failed_password;
  logfile << " user name : " << e.getData()->sharedInfo.user_name;
  logfile << " timestamp : " << e.getData()->sharedInfo.timestamp.tv_sec << " sec " << e.getData()->sharedInfo.timestamp.tv_usec << " usec";
  logfile << " ip: " << e.getData()->sharedInfo.remote_ip.sin_addr.s_addr << std::endl;
}

void VSFFileLoggerPlugin::handleEvent(const EventLogin & e) { 
  logfile << "Successful login occured: " << ( (e.getData()->is_anonymous) ? "(anonymous)" : "(non-anonymous)");
  logfile << " user name : " << e.getData()->sharedInfo.user_name;
  logfile << " timestamp : " << e.getData()->sharedInfo.timestamp.tv_sec << " sec " << e.getData()->sharedInfo.timestamp.tv_usec << " usec";
  logfile << " ip: " << e.getData()->sharedInfo.remote_ip.sin_addr.s_addr << std::endl;
}

void VSFFileLoggerPlugin::handleEvent(const EventMkdir & e) {
  logfile << "Wow ! a directory has been created, oh joy! ";
  logfile << " path : " << e.getData()->path;
  logfile << " user name : " << e.getData()->sharedInfo.user_name;
  logfile << " timestamp : " << e.getData()->sharedInfo.timestamp.tv_sec << " sec " << e.getData()->sharedInfo.timestamp.tv_usec << " usec";
  logfile << " ip: " << e.getData()->sharedInfo.remote_ip.sin_addr.s_addr << std::endl;
}

void VSFFileLoggerPlugin::handleEvent(const EventRename & e) {

}

void VSFFileLoggerPlugin::handleEvent(const EventRmdir & e) {

}

void VSFFileLoggerPlugin::handleEvent(const EventCliEnd & e) {
  logfile << "A client parted";
  logfile << " timestamp : " << e.getData()->timestamp.tv_sec << " sec " << e.getData()->timestamp.tv_usec << " usec";
  logfile << std::endl;
}

void VSFFileLoggerPlugin::handleEvent(const EventUpload & e) {
  logfile << "Upload occured " << std::endl;
}

struct Request_Result VSFFileLoggerPlugin::handleRequest(const RequestLogin & r) {
  struct Request_Result retStruct = Request_Result_Init;

  logfile << "request login, name: " << r.getData()->sharedInfo.user_name << " login: " << r.getData()->login << std::endl;

  return retStruct;
}

struct Request_Result_And_Reason VSFFileLoggerPlugin::handleRequest(const RequestReadFile & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;

  printf("RequestReadFile: name: %s, path: %s, fullName: %s\n", r.getData()->name, r.getData()->path, r.getData()->fullName);

  if (std::string(r.getData()->name).find("nogo") != std::string::npos)
  {
    retStruct.result = -1;
  }

  return retStruct;
}

struct Request_Result_And_Reason VSFFileLoggerPlugin::handleRequest(const RequestReadDirectory & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;

  printf("RequestReadDirectory: name: %s, parentPath: %s\n", r.getData()->name, r.getData()->parentPath);

  if (std::string(r.getData()->name).find("nogo") != std::string::npos)
  {
    retStruct.result = -1;
  }

  return retStruct;
}

struct Request_Result_And_Reason VSFFileLoggerPlugin::handleRequest(const RequestWriteDirectory & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;

  printf("RequestWriteDirectory: name: %s, parentPath: %s\n", r.getData()->name, r.getData()->parentPath);

  if (std::string(r.getData()->name).find("nogo") != std::string::npos)
  {
    retStruct.result = -1;
  }

  return retStruct;
}

struct Request_Result_And_Reason VSFFileLoggerPlugin::handleRequest(const RequestWriteFile & r) {
  struct Request_Result_And_Reason retStruct = Request_Result_And_Reason_Init;

  printf("RequestWriteFile: name: %s, path: %s, fullName: %s, doesExist:%d\n", r.getData()->name, r.getData()->path, r.getData()->fullName, r.getData()->does_exist);

  if (std::string(r.getData()->name).find("nogo") != std::string::npos)
  {
    retStruct.result = -1;
  }

  return retStruct;
}

struct Request_Result_Homedir VSFFileLoggerPlugin::handleRequest(const RequestHomedir & r) {
  struct Request_Result_Homedir toRet = Request_Result_Homedir_Init;
  toRet.homedir = "/";
  return toRet;
}

