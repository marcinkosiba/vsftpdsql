#ifndef VSF_UDSRF_H
#define VSF_UDSRF_H

/*
  The Universal Debuggable Sender - Receiver Functions :)
*/

#include "exceptions.h"
#include "unknown_object.h"
#include "typeid_hash.h"

/** sendData()
 *  PURPOSE
 *    Provides a shorthand for sending data, also enables limited typechecking based on typeid(T).name()
 *  PARAMETERS
 *    socket - the socket which we use for sending data
 *    to_send - the data we wish to send
 */
template<typename T> void sendData(int socket, const T & to_send) throw(std::exception);

/** recvData()
 *  PURPOSE
 *   Provides a shorthand for receiving data, also enables limited typechecking based on typeid(T).name()
 *  PARAMETERS
 *   socket - socket used fot receiving data
 *   to_recv_to - an existing object, may be not initialised, which will be populated with 
 *                the received data
 */
template<typename T> void recvData(int socket, const T & to_recv_to) throw(std::exception);


/******************************* implementation **********************************************/

#include <typeinfo>
#include <exception>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

template<typename T> inline void sendData(int socket, const T & to_send) throw(std::exception) {
    int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
    if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
    {
      throw SendRecvException();
    }

  if (send(socket, &to_send, sizeof(T),0) != sizeof(T))
  {
    perror(0);
    throw SendRecvException();
  }
}

template<> inline void sendData(int socket, const UnknownObject & to_send) throw(std::exception) {
    int xoredsum = TypeIDHashtable::getHash(typeid(to_send));
    if (send(socket, &xoredsum, sizeof(int), 0) != sizeof(int))
    {
      throw SendRecvException();
    }
    to_send.writeToSocket(socket);
}

template<typename T> void recvData(int socket, T & to_recv_to) throw(std::exception) {
    int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
    int xorsum_recvd;
    if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int)) 
    {
      throw SendRecvException();
    }
    if (xorsum_recvd != my_xorsum)
    {
      throw InconsistentTypeException();
    }
  
  if (recv(socket, &to_recv_to, sizeof(T),0) != sizeof(T))
  {
    perror(0);
    throw SendRecvException();
  }
}

template<> inline void recvData(int socket, UnknownObject & to_recv_to) throw(std::exception) {
    int my_xorsum = TypeIDHashtable::getHash(typeid(to_recv_to));
    int xorsum_recvd;
    if(recv(socket, &xorsum_recvd, sizeof(int), 0) != sizeof(int)) 
    {
      throw SendRecvException();
    }
    if (xorsum_recvd != my_xorsum)
    {
      throw InconsistentTypeException();
    }

    to_recv_to = *UnknownObject::readFromSocket(socket);
}

#include "event_c_api.h"
#include "request_c_api.h"
#include "udsrf_auto.h"

#endif
