#ifndef PLUGIN_ARCHITECTURE_API_H
#define PLUGIN_ARCHITECTURE_API_H

#include "event.h"
#include "request.h"
#include <vector>
#include "unknown_object.h"

class VSFPlugin;
class VSFPluginArchitectureSingleton;

/**
 * A handler for all events and requests.
 * provides support for multiple event listeners
 * and just one request listener
 */
class VSFPluginArchitecture {  
public:
  /**
   * assigns the plugin to the given event
   */
  void registerEventHandler(VSFPlugin & handler, Event_Number_Enum eventNumber);

  /**
   * assigns the plugin to the given requsest, if there is already a plugin registered,
   * throws an exception
   */
  void registerRequestHandler(VSFPlugin & handler, Request_Number_Enum requestNumber);

  /**
   * removes the plugin from the listeners list for the given event
   */
  void unregisterEventHandler(VSFPlugin & handler, Event_Number_Enum eventNumber);

  /**
   * removes the plugin from the request handler position
   */
  void unregisterRequestHandler(VSFPlugin & handler, Request_Number_Enum requestNumber);
  /**
   * makes the given request slot free for registration
   */
  void unregisterRequestHandler(Request_Number_Enum requestNumber);

  /**
   * handles an event, if there is no handler assigned - throws an exception
   */
  void handleEvent(Event &);

  /**
   * handles a request, if there is no handler assigned - throws an exception
   */
  UnknownObject handleRequest(Request &);  
  friend class VSFPluginArchitectureSingleton;
protected:
  VSFPluginArchitecture();
  static bool architectureExists;
private:
  std::vector<VSFPlugin*> pluginEventHandlers[Event_Number_MaxNumber - Event_Identifier];
  VSFPlugin* pluginRequestHandlers[Request_Number_MaxNumber - Request_Identifier];
};

/**
 * ensures there is only one plugin architecure :)
 */
class VSFPluginArchitectureSingleton {
public:
  VSFPluginArchitecture & getVSFPluginArchitecture();
private:
  static VSFPluginArchitecture * architecture;
};

/**
 * a plugin, essentialy a set of event and request handlers
 * Do note, that the constructor must notify the architecture
 * which events and request the plugin wants to handle
 */
class VSFPlugin {
public:
  VSFPlugin(VSFPluginArchitecture & arch) {arch.registerEventHandler(*this, Event_Number_ServQuit);}
  virtual ~VSFPlugin() {}
  
  virtual void handleEvent(const EventChmod &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventConnection &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventDelete &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventDownload &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventFailedLogin &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventLogin &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventMkdir &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventRename &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventRmdir &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventCliEnd &) {throw UnimplementedPluginMethod();}
  virtual void handleEvent(const EventUpload &) {throw UnimplementedPluginMethod();}
  void handleEvent(const EventServQuit &) {}

  virtual struct Request_Result handleRequest(const RequestLogin &) {throw UnimplementedPluginMethod();}
  virtual struct Request_Result_Homedir handleRequest(const RequestHomedir &) {throw UnimplementedPluginMethod();}
  virtual struct Request_Result_And_Reason handleRequest(const RequestReadFile &) {throw UnimplementedPluginMethod();}
  virtual struct Request_Result_And_Reason handleRequest(const RequestReadDirectory &) {throw UnimplementedPluginMethod();}
  virtual struct Request_Result_And_Reason handleRequest(const RequestWriteDirectory &) {throw UnimplementedPluginMethod();}
  virtual struct Request_Result_And_Reason handleRequest(const RequestWriteFile &) {throw UnimplementedPluginMethod();}
};

#endif
