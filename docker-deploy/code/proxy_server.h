#ifndef PROXY_SERVER
#define PROXY_SERVER
#include "config.h"
const char * PROXY_PORT = "12345";
class ProxyServer {
 private:
  int listen_fd;
  struct addrinfo host_info;
  struct addrinfo * host_info_list;

 public:
  ~ProxyServer() {
    sleep(1);
    close(listen_fd);
    freeaddrinfo(host_info_list);
  }
  void startListen();
  void acceptConnect(int & request_fd, string & request_ip);
};

void ProxyServer::acceptConnect(int & request_fd, string & request_ip) {
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  int client_connection_fd;
  client_connection_fd =
      accept(listen_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if (client_connection_fd == -1) {
    cerr << "Error: cannot accept connection on socket" << endl;
    exit(EXIT_FAILURE);
  }  //if
  struct sockaddr_in * temp = (struct sockaddr_in *)&socket_addr;
  request_fd = client_connection_fd;
  request_ip = inet_ntoa(temp->sin_addr);
}

void ProxyServer::startListen() {
  int status;
  //const char * hostname = NULL;
  memset(&host_info, 0, sizeof(host_info));
  const char * hostname = NULL;
  const char * port = PROXY_PORT;
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags = AI_PASSIVE;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    exit(EXIT_FAILURE);
  }  //if

  listen_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
  if (listen_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    exit(EXIT_FAILURE);
  }  //if

  int yes = 1;
  status = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(listen_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);

  if (status == -1) {
    cerr << "Error: cannot bind socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    exit(EXIT_FAILURE);
  }  //if
  status = listen(listen_fd, 100);
  if (status == -1) {
    cerr << "Error: cannot listen on socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    exit(EXIT_FAILURE);
  }  //if
}

#endif
