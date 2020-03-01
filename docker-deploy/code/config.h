#ifndef CONFIG_H
#define CONFIG_H
#ifdef DEBUG
#define D(x)                                   \
  do {                                         \
    std::cerr << "[DEBUG] " << x << std::endl; \
  } while (0)
#else
#define D(x) \
  do {       \
  } while (0)
#endif

#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
enum validation_type { MUST_REVALIDATE, VALID, NO_VALIDATION_INFO };
using namespace std;
#endif
