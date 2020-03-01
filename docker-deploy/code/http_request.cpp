#include "http_message.h"
int HttpRequest::request_id = 0;

string HttpRequest::getHost() {
  // exception
  string hostname = header_kv_["Host"];
  size_t colon = hostname.find(':');
  if (colon == string::npos) {
    return hostname;
  }
  else {
    header_kv_["Host"] = hostname.substr(0, colon);
    // port_ = hostname.substr(colon + 1);
  }
  return header_kv_["Host"];
}

string HttpRequest::getPort() {
  // exception

  return port_;
}

void HttpRequest::parseRequestLine() {
  size_t method_end = start_line_.find(" ");
  method_ = start_line_.substr(0, method_end);
  size_t target_end = start_line_.find(" ", method_end + 1);
  target_ = start_line_.substr(method_end + 1, target_end - (method_end + 1));

  if (method_ == "CONNECT") {
    port_ = "443";
  }
  else {
    port_ = "80";
  }
}

/**                                                                                    
 * Check if we can store the response into cache                        
 * based on request. Only both requst and response return true,                    
 * we can store response in cache.                                              
 */
bool HttpRequest::isReqStoreable() {
  // store only for GET
  if (method_ != "GET") {
    return false;
  }

  // no store if have Cache-Control = no-store
  if (header_kv_.find("Cache-Control") != header_kv_.end()) {
    if (header_kv_["Cache-Control"].find("no-store") != string::npos)
      return false;
  }

  // no store if have Authorization
  if (header_kv_.find("Authorization") != header_kv_.end())
    return false;

  return true;
}

/**                                                                                    
 * isReqCacheable returns false, we can not extract response from cache.       
 */
bool HttpRequest::isReqCacheable() {
  if (method_ != "GET") {
    return false;
  }
  // if have Pragma header field
  if (header_kv_.find("Pragma") != header_kv_.end()) {
    if (header_kv_["Pragma"] == "no-cache") {
      return false;
    }
  }
  // if have Cache-Control
  if (header_kv_.find("Cache-Control") != header_kv_.end()) {
    if (header_kv_["Cache-Control"].find("no-cache") != string ::npos) {
      return false;
    }
  }

  return true;
}

/**
 * This function modify vector<char> first_part_ add e_tag and last_modified kv pairs.
 */
vector<char> HttpRequest::getNewRequest(string e_tag, string last_modified) {
  size_t pos = raw_msg_.find("\r\n\r\n");
  string temp = raw_msg_.substr(0, pos + 2);
  string toadd = "";

  size_t start_et = temp.find("If-None-Match");
  if (e_tag != "" && start_et != string::npos) {
    string kv = "If-None-Match: " + e_tag + " \r\n";
    toadd += kv;
    size_t end_et = temp.find("\r\n", start_et);
    temp.erase(start_et, end_et + 2 - start_et);
  }
  size_t start_lm = temp.find("Last-Modified");
  if (last_modified != "" && start_lm != string::npos) {
    string kv = "Last-Modified: " + last_modified + " \r\n";
    toadd += kv;
    size_t end_lm = temp.find("\r\n", start_lm);
    temp.erase(start_lm, end_lm + 2 - start_lm);
  }

  temp += toadd + "\r\n";
  vector<char> res(temp.begin(), temp.end());

  return res;
}

void HttpRequest::setReqDate() {
  char tm_buf[30] = {0};
  time_t raw_time;
  struct tm * time_info = new struct tm;
  time(&raw_time);  // in sec
  gmtime_r(&raw_time, time_info);
  strftime(tm_buf, sizeof(tm_buf), "%a, %d %b %Y %H:%M:%S GMT", time_info);

  req_date_ = tm_buf;
  delete time_info;
}
