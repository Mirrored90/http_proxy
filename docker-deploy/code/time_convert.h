#ifndef TIME_CONVERT_H
#define TIME_CONVERT_H
#include "config.h"

/**
 * Convert date format to long int in sec in GMT
 * Format: Date: Sun, 23 Feb 2020 20:39:53 GMT
 */
long int convertDateToSec(string date) {
  struct tm * tmp_time = new struct tm;
  memset(tmp_time, 0, sizeof(*tmp_time));
  date = date.substr(0, date.size() - sizeof(" GMT"));
  strptime(date.c_str(), "%a, %d %b %Y %H:%M:%S", tmp_time);

  long int res = mktime(tmp_time);
  delete tmp_time;
  return res;
}

/**
 * Convert current time to date format in GMT
 * Format: Date: Sun, 23 Feb 2020 20:39:53 GMT
 */
string convertSecToDate() {
  char tm_buf[30] = {0};
  time_t raw_time;
  struct tm * time_info = new struct tm;
  time(&raw_time);  // in sec
  gmtime_r(&raw_time, time_info);
  strftime(tm_buf, sizeof(tm_buf), "%a, %d %b %Y %H:%M:%S GMT", time_info);

  string res = tm_buf;
  delete time_info;
  return res;
}

/**                                                                                    
 * Convert input sec to date format in GMT                                         
 * Format: Date: Sun, 23 Feb 2020 20:39:53 GMT                                         
 */
string convertInputSecToDate(long int sec) {
  char tm_buf[30] = {0};
  time_t raw_time = sec;
  struct tm * time_info = new struct tm;

  gmtime_r(&raw_time, time_info);
  strftime(tm_buf, sizeof(tm_buf), "%a, %d %b %Y %H:%M:%S GMT", time_info);

  string res = tm_buf;
  delete time_info;
  return res;
}
/**
 * Get current time in sec in GMT
 */
long int getCurrentTime() {
  string now = convertSecToDate();
  long int res = convertDateToSec(now);
  return res;
}

#endif
