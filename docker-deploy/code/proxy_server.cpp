#include "proxy_server.h"

#include "cache.h"
#include "client_handler.h"
#include "http_message.h"

#define CACHE_CAPACITY 800

void request_handler(int request_fd, string request_ip, Cache * c) {
  ClientHandler rh(request_fd, request_ip, c);
  rh.run();
}

int main() {
  D("TEST DEBUG PRINT\n---------------------");
  cout << "Welcome to HTTP proxy!" << endl;
  cout << "------------------------" << endl;
  Cache cache(CACHE_CAPACITY);

  ProxyServer ps = ProxyServer();
  ps.startListen();
  while (1) {
    int request_fd;
    string request_ip;
    ps.acceptConnect(request_fd, request_ip);
    thread t(request_handler, request_fd, request_ip, &cache);
    t.detach();
  }

  return EXIT_SUCCESS;
}
