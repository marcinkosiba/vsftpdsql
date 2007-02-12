#ifndef FILE_LOGGER_PLUGIN_H
#define FILE_LOGGER_PLUGIN_H

#include <fstream>
#include <string>
#include "plugin_architecture_api.h"

/**
 * Test class, logs all operations to a file.
 * Normaly it wouldn't work because of a chroot() jail
 */
class VSFFileLoggerPlugin : public VSFPlugin {
public:
  VSFFileLoggerPlugin(VSFPluginArchitecture & arch, std::string = std::string("/tmp/vsftp_testlog"));
  ~VSFFileLoggerPlugin();

  virtual void handleEvent(const EventChmod &);
  virtual void handleEvent(const EventConnection &);
  virtual void handleEvent(const EventDelete &);
  virtual void handleEvent(const EventDownload &);
  virtual void handleEvent(const EventFailedLogin &);
  virtual void handleEvent(const EventLogin &);
  virtual void handleEvent(const EventMkdir &);
  virtual void handleEvent(const EventRename &);
  virtual void handleEvent(const EventRmdir &);
  virtual void handleEvent(const EventCliEnd &);
  virtual void handleEvent(const EventUpload &);

  virtual struct Request_Result handleRequest(const RequestLogin &);
  virtual struct Request_Result_And_Reason handleRequest(const RequestReadFile &);
  virtual struct Request_Result_And_Reason handleRequest(const RequestReadDirectory &);
  virtual struct Request_Result_And_Reason handleRequest(const RequestWriteDirectory &);
  virtual struct Request_Result_And_Reason handleRequest(const RequestWriteFile &);
  virtual struct Request_Result_Homedir handleRequest(const RequestHomedir &);

private:
  std::fstream logfile;
  VSFPluginArchitecture & PlArch;
};


#endif //FILE_LOGGER_PLUGIN_H

