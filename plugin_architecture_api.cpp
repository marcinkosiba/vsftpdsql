#include "plugin_architecture_api.h"
#include "exceptions.h"
#include <stdexcept>

bool VSFPluginArchitecture::architectureExists = false;

VSFPluginArchitecture::VSFPluginArchitecture() {
  if (!architectureExists)
  {
    architectureExists = true;
  }
  else
  {
    throw TooManyPluginArchitectures();
  }  
  memset(pluginRequestHandlers,0, sizeof(VSFPlugin*)*(Request_Number_MaxNumber - Request_Identifier));
}

void VSFPluginArchitecture::registerEventHandler(VSFPlugin & handler, Event_Number_Enum eventNumber) 
{
  if (eventNumber >= Event_Number_MaxNumber || eventNumber <= Event_Identifier)
  {
    throw std::out_of_range("eventNumber not in enum bounds");
  } 
  else 
  {
    eventNumber = (Event_Number_Enum)(eventNumber - Event_Number_Identifier);
  }

  pluginEventHandlers[eventNumber].push_back(&handler);  
}

void VSFPluginArchitecture::registerRequestHandler(VSFPlugin & handler, Request_Number_Enum requestNumber) 
{
  if (requestNumber >= Request_Number_MaxNumber || requestNumber < Request_Identifier)
  {
    throw std::out_of_range("eventNumber not in enum bounds");
  }
  else
  {
    requestNumber = (Request_Number_Enum)(requestNumber - Request_Number_Identifier);
  }

  if (pluginRequestHandlers[requestNumber])
  {
    throw TooManyRequestHandlers();
  }
  else
  {
    pluginRequestHandlers[requestNumber] = &handler;
  }
}

void VSFPluginArchitecture::unregisterEventHandler(VSFPlugin & handler, Event_Number_Enum eventNumber) 
{
  if (eventNumber >= Event_Number_MaxNumber || eventNumber <= Event_Identifier)
  {
    throw std::out_of_range("eventNumber not in enum bounds");
  } 
  else 
  {
    eventNumber = (Event_Number_Enum)(eventNumber - Event_Number_Identifier);
  }

  bool found = false;

  for (std::vector<VSFPlugin*>::iterator i = pluginEventHandlers[eventNumber].begin(); 
        i != pluginEventHandlers[eventNumber].end(); ++i)
  {
    if (*i == &handler)
    {
      pluginEventHandlers[eventNumber].erase(i);
      found = true;
      break;
    }
  }  
  if (!found)
  {
    throw UnregisteringNotRegisterd();
  }
}

void VSFPluginArchitecture::unregisterRequestHandler(VSFPlugin & handler, Request_Number_Enum requestNumber) 
{
  if (requestNumber >= Request_Number_MaxNumber || requestNumber < Request_Identifier)
  {
    throw std::out_of_range("requestNumber not in enum bounds");
  }
  else
  {
    requestNumber = (Request_Number_Enum)(requestNumber - Request_Number_Identifier);
  }
  if (pluginRequestHandlers[requestNumber] == &handler)
  {
    pluginRequestHandlers[requestNumber] = 0;
  }
  else 
  {
    throw UnregisteringNotRegisterd();
  }
}

void VSFPluginArchitecture::unregisterRequestHandler(Request_Number_Enum requestNumber) 
{
  if (requestNumber >= Request_Number_MaxNumber || requestNumber < Request_Identifier)
  {
    throw std::out_of_range("requestNumber not in enum bounds");
  }
  else
  {
    requestNumber = (Request_Number_Enum)(requestNumber - Request_Number_Identifier);
  }

  pluginRequestHandlers[requestNumber] = 0;
}

void VSFPluginArchitecture::handleEvent(Event & e) {
  int eventNumber = e.getNumber();

  if (eventNumber >= Event_Number_MaxNumber || eventNumber <= Event_Identifier)
  {
    throw std::out_of_range("eventNumber not in enum bounds");   
  } 
  else 
  {
    eventNumber = (Event_Number_Enum)(eventNumber - Event_Number_Identifier);
  }

  if (pluginEventHandlers[eventNumber].empty())
  {
    throw NoEventHandler();
  }

  for (std::vector<VSFPlugin*>::iterator i = pluginEventHandlers[eventNumber].begin(); 
        i != pluginEventHandlers[eventNumber].end(); ++i)
  {
    e.getHandled(*(*i));
  }
}

UnknownObject VSFPluginArchitecture::handleRequest(Request & r) {
  int requestNumber = r.getNumber();

  if (requestNumber >= Request_Number_MaxNumber || requestNumber < Request_Identifier)
  {
    throw std::out_of_range("requestNumber not in enum bounds");
  }
  else
  {
    requestNumber = (Request_Number_Enum)(requestNumber - Request_Number_Identifier);
  }

  if (pluginRequestHandlers[requestNumber] == 0)
  {
    throw NoRequestHandler();
  }

  return r.getHandled(*pluginRequestHandlers[requestNumber]);
}

VSFPluginArchitecture * VSFPluginArchitectureSingleton::architecture = 0;

VSFPluginArchitecture & VSFPluginArchitectureSingleton::getVSFPluginArchitecture() {
  if (architecture == 0)
  {
    architecture = new VSFPluginArchitecture();
  }
  return *architecture;
}
