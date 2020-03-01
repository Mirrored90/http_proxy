#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H
#include "config.h"

/**
 * This is base class: HttpMessage
 */
class HttpMessage {
 private:
  string removeOws(string str);
  void parseRawMsg();
  void parseHeader();
  size_t isPositiveDigit(string str);
  size_t checkContentLengthValue(string str);

 protected:
  string raw_msg_;
  string start_line_;
  string header_field_;
  vector<vector<char> > entire_msg_;

 public:
  map<string, string> header_kv_;
  HttpMessage() {}
  explicit HttpMessage(string raw_msg) : raw_msg_(raw_msg) {
    parseRawMsg();
    parseHeader();
    /*
    cout << "--------------------" << endl;
    cout << start_line_ + header_field_ << endl;
    cout << "--------------------" << endl;
    */
  }
  bool isChunkValid();
  size_t isContentLengthValid();
  int getHeaderLength();
  void setEntireMsg(vector<vector<char> > entire_msg);
  vector<vector<char> > getEntireMsg() { return entire_msg_; }
  string getStartLine();
};

/**
 * This is child class: HttpRequest
 */
class HttpRequest : public HttpMessage {
 private:
  static int request_id;
  string host_;
  string port_;
  string method_;
  string target_;
  string req_date_;
  int log_id_;
  void parseRequestLine();
  void setReqDate();

 public:
  HttpRequest() {}
  explicit HttpRequest(string raw_msg) : HttpMessage(raw_msg) {
    log_id_ = request_id;
    request_id++;
    parseRequestLine();
    setReqDate();
    host_ = getHost();
  }

  vector<char> getNewRequest(string e_tag, string last_modified);
  bool isReqStoreable();
  bool isReqCacheable();
  string getHost();
  string getPort();
  string getMethod() { return method_; }
  int getLogId() { return log_id_; }
  string getCacheKey() { return start_line_ + host_; }
  string getReqDate() { return req_date_; }
};

/**
 * This is child class: HttpResponse
 */
class HttpResponse : public HttpMessage {
 private:
  string status_code_;
  void parseResponseLine();
  int isFresh();
  int isExpire();
  string expire_date_;
  void setExpireDate(string expire_date);

 public:
  HttpResponse() {}
  explicit HttpResponse(string raw_msg) : HttpMessage(raw_msg) { parseResponseLine(); }
  bool isRespStoreable();
  int isRespCacheable();
  string getStatusCode() { return status_code_; }
  string getEtag();
  string getLastModified();
  string getExpireDate() { return expire_date_; }
};

#endif
