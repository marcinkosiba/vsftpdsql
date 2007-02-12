#include "tunables.h"
#include "plugin_architecture_api.h"
#include <strings.h>
#include <memory>
std::auto_ptr<VSFPlugin> createCurrentSQLPlugin(VSFPluginArchitectureSingleton & pluginArchitecture) {
  if (strcasecmp(tunable_sql_plugin, "VSFFileLoggerPlugin") == 0) {
    return std::auto_ptr<VSFPlugin>(new VSFFileLoggerPlugin(pluginArchitecture.getVSFPluginArchitecture()));
  } else 
  if (strcasecmp(tunable_sql_plugin, "VSFMySQLPlugin") == 0) {
    return std::auto_ptr<VSFPlugin>(new VSFMySQLPlugin(pluginArchitecture.getVSFPluginArchitecture()));
  } else 
  if (strcasecmp(tunable_sql_plugin, "VSFPgSQLPlugin") == 0) {
    return std::auto_ptr<VSFPlugin>(new VSFPgSQLPlugin(pluginArchitecture.getVSFPluginArchitecture()));
  } else 
   { throw UnknownPluginException(); }
}
