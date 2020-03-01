#ifndef CACHE_H
#define CACHE_H
#include "config.h"
#include "http_message.h"

class Cache {
 private:
  typedef list<string> List;  // store cache key
  typedef pair<List::iterator, HttpResponse>
      Pair;  // store the actuall data of HttpResponse
  typedef map<string, Pair> Map;
  const size_t capacity_;
  List dq_resp_;
  Map mp_resp_;

  bool isCacheFull();
  void touch(Map::iterator it);

 public:
  explicit Cache(size_t capacity) : capacity_(capacity) {}
  bool isInCache(string cache_key);
  void put(string cache_key, HttpResponse resp);
  HttpResponse get(string cache_key);
};

#endif
