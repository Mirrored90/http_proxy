#include "cache.h"
mutex mtx_cache;
/**
 * Check if current cache is full or not. 
 * If full return ture.
 */
bool Cache::isCacheFull() {
  return dq_resp_.size() >= capacity_;
}

/**
 * Check if cache contain such key.
 */
bool Cache::isInCache(string cache_key) {
  auto it = mp_resp_.find(cache_key);
  if (it == mp_resp_.end()) {
    return false;
  }
  return true;
}

/**
 * Update my cache according to LRU
 */
void Cache::touch(Map::iterator it) {
  string key = it->first;
  dq_resp_.erase(it->second.first);
  dq_resp_.push_front(key);
  it->second.first = dq_resp_.begin();
}

/**
 * Put response into cache based on its cache key.
 */
void Cache::put(string cache_key, HttpResponse resp) {
  lock_guard<mutex> lock(mtx_cache);
  auto it = mp_resp_.find(cache_key);
  // if already in cache, update cache
  if (it != mp_resp_.end()) {
    touch(it);
  }
  // else put into cache
  else {
    if (isCacheFull()) {
      mp_resp_.erase(dq_resp_.back());
      dq_resp_.pop_back();
    }
    dq_resp_.push_front(cache_key);
  }
  mp_resp_[cache_key] = {dq_resp_.begin(), resp};
}

/**
 * Get content from cache based on key.
 * If not found, return empty str ""
 */
HttpResponse Cache::get(string cache_key) {
  lock_guard<mutex> lock(mtx_cache);
  auto it = mp_resp_.find(cache_key);
  HttpResponse res = it->second.second;
  touch(it);

  return res;
}
