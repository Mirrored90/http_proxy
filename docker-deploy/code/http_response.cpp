#include "http_message.h"
#include "time_convert.h"
void HttpResponse::parseResponseLine() {
  size_t fisrt_sp = start_line_.find(" ");
  if (fisrt_sp == string::npos) {
    //exception
  }
  size_t second_sp = start_line_.find(" ", fisrt_sp + 1);
  if (fisrt_sp == string::npos) {
    //exception
  }
  size_t last_pos = start_line_.find("\r\n", second_sp + 1);
  if (last_pos == string::npos) {
    //exception
  }

  status_code_ = start_line_.substr(fisrt_sp + 1, second_sp - (fisrt_sp + 1));
}

/**                                                                                    
 * Check if we can store the response into cache
 * based on response. Only both requst and response return true,
 * we can store response in cache.                                                    
 */
bool HttpResponse::isRespStoreable() {
  // store only when 200
  if (status_code_ != "200") {
    return false;
  }

  // no store if have Cache-Control = no-store or private
  if (header_kv_.find("Cache-Control") != header_kv_.end()) {
    if (header_kv_["Cache-Control"].find("no-store") != string::npos)
      return false;
    if (header_kv_["Cache-Control"].find("private") != string::npos)
      return false;
  }
  // if contain one of following, then we can store
  bool flag = false;
  if (header_kv_.find("Expires") != header_kv_.end())
    flag = true;

  if (header_kv_.find("Cache-Control") == header_kv_.end())
    return false;
  if (header_kv_["Cache-Control"].find("max-age") != string::npos)
    flag = true;

  if (header_kv_["Cache-Control"].find("s-maxage") != string::npos)
    flag = true;

  if (header_kv_["Cache-Control"].find("public") != string::npos)
    flag = true;

  return flag;
}

/**
 * Set Expire Date
 */
void HttpResponse::setExpireDate(string expire_date) {
  expire_date_ = expire_date;
}

/**
 * Check the freshness of resonse.
 * current resp_date + max-age 
 */
int HttpResponse::isFresh() {
  string value = header_kv_["Cache-Control"];
  size_t pos = value.find("max-age=");
  string max_age = value.substr(pos + sizeof("max-age=") - 1);
  string resp_date = header_kv_["Date"];
  long int age = convertDateToSec(resp_date) + atoi(max_age.c_str());
  long int now = getCurrentTime();
  string expire_date = convertInputSecToDate(age);

  setExpireDate(expire_date);
  if (age > now)
    return VALID;
  else
    return MUST_REVALIDATE;
}

/**
 * Compare current time and expire field. Return true if the response is expired.
 */
int HttpResponse::isExpire() {
  long int expire = convertDateToSec(header_kv_["Expires"]);
  long int now = getCurrentTime();
  setExpireDate(header_kv_["Expires"]);
  if (now > expire)
    return MUST_REVALIDATE;
  else
    return VALID;
}

/**
 * If this function return true, we can just use this response;    
 * else we need to do revalidation.
 */
int HttpResponse::isRespCacheable() {
  // if there is no cache-control
  if (header_kv_.find("Cache-Control") == header_kv_.end()) {
    if (header_kv_.find("Expires") == header_kv_.end()) {
      return NO_VALIDATION_INFO;
    }
    else {
      return isExpire();
    }
  }
  // if contain no-cache
  if (header_kv_["Cache-Control"].find("no-cache") != string::npos) {
    return NO_VALIDATION_INFO;
  }

  // If is no date
  if (header_kv_.find("Date") == header_kv_.end()) {
    return NO_VALIDATION_INFO;
  }
  // if no max-age
  if (header_kv_["Cache-Control"].find("max-age") == string::npos) {
    return NO_VALIDATION_INFO;
  }

  // if it is fresh
  if (isFresh()) {
    return VALID;
  }
  // not fresh
  else {
    // no revalidation information, cannot revalidate
    if (header_kv_.find("Last-Modified") == header_kv_.end() &&
        header_kv_.find("E-Tag") == header_kv_.end()) {
      return NO_VALIDATION_INFO;
    }
    // can validate, need to do validation
    else {
      return MUST_REVALIDATE;
    }
  }
}

/**
 * This function returns the value of E-Tag; If E-Tag does not exist;
 * return an empty string.
 */
string HttpResponse::getEtag() {
  string res = "";
  if (header_kv_.find("E-Tag") != header_kv_.end()) {
    res = header_kv_["E-Tag"];
  }
  return res;
}

/**                                                                                    
 * This function returns the value of Last-Modified; 
 * If Last-Modified does not exist, return an empty string. 
 */
string HttpResponse::getLastModified() {
  string res = "";
  if (header_kv_.find("Last-Modified") != header_kv_.end()) {
    res = header_kv_["Last-Modified"];
  }
  return res;
}
