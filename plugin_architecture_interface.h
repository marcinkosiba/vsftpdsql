#ifndef VSF_PLUGIN_ARCHITECTURE_INTERFACE
#define VSF_PLUGIN_ARCHITECTURE_INTERFACE

#include "session.h"
#include "cpp_to_c_helpers.h"

/* vsf_plugin_architecture_start()
 * PURPOSE
 * Creates and initializes the threads (high and low prio) used for processing events
 * and sets up the ipc
 * PARAMETERS
 * p_sess       - the current session object
 */
C_COMPATIBLE_FUNCTION void vsf_plugin_architecture_start(struct vsf_session* p_sess);

/** plugin_architecture_get_slave_socket_slow()
 *  PURPOSE
 *   Returns a communication socket to the slave proces (for events mostly)
 */
C_COMPATIBLE_FUNCTION int plugin_architecture_get_slave_socket_slow();

/** plugin_architecture_get_slave_socket_fast()
 *  PURPOSE
 *   Returns a communication socket to the slave proces (for requests mostly)
 */
C_COMPATIBLE_FUNCTION int plugin_architecture_get_slave_socket_fast();

/** reconnect_after_fork()
 *  PURPOSE
 *   Reconnects to the plugin architecture process after a fork so that the child can communicate
 *   normally.
 */
C_COMPATIBLE_FUNCTION int reconnect_after_fork();

#endif //PLUGIN_ARCHITECTURE_INTERFACE
