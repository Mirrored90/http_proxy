#include "http_message.h"

string HttpMessage::getStartLine() {
  // exception
  return start_line_;
}

/**
 * Store entire msg body for sake of sending back.
 * Escape the entire_msg[0] and use onw first_part_.
 */
void HttpMessage::setEntireMsg(vector<vector<char> > entire_msg) {
  for (size_t i = 0; i < entire_msg.size(); i++) {
    entire_msg_.push_back(entire_msg[i]);
  }
}

void HttpMessage::parseRawMsg() {
  // parse start line
  size_t end_start_line = raw_msg_.find("\r\n");
  if (end_start_line == string::npos) {
    // throw exception
    cout << "Fail to parse start line!" << endl;
  }
  start_line_ = raw_msg_.substr(0, end_start_line);

  // parse header filed
  size_t end_header_field = raw_msg_.find("\r\n\r\n");
  if (end_header_field == string::npos) {
    // throw exception
    cout << "Fail to parse header!" << endl;
  }
  header_field_ = raw_msg_.substr(end_start_line, end_header_field - end_start_line);
}

void HttpMessage::parseHeader() {
  size_t start_pos = 0;

  // keep parsing until there is no kv pair indicated by :
  while (header_field_.find(":", start_pos) != string::npos) {
    start_pos = header_field_.find("\r\n", start_pos);
    size_t key_end = header_field_.find(":", start_pos);
    string key = header_field_.substr(start_pos + 2, key_end - (start_pos + 2));
    start_pos = header_field_.find("\r\n", start_pos + 2);
    string value = header_field_.substr(key_end + 2, start_pos - (key_end + 2));

    header_kv_[key] = removeOws(value);

    //D(key + ":" + header_kv_[key]);
  }
}

string HttpMessage::removeOws(string str) {
  string res(str);
  int end_pos = -1;
  int start_pos = -1;
  // search tail OWS
  for (int i = str.size() - 1; i >= 0; i--) {
    if (str[i] != ' ') {
      end_pos = i;
      break;
    }
  }
  if (end_pos != -1) {
    res = str.substr(0, end_pos + 1);
  }

  // search head OWS
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] != ' ') {
      start_pos = i;
      break;
    }
  }
  if (start_pos != -1) {
    res = res.substr(start_pos);
  }

  return res;
}

bool HttpMessage::isChunkValid() {
  // if Transfer-Encoding
  if (header_kv_.find("Transfer-Encoding") != header_kv_.end()) {
    string value = header_kv_["Transfer-Encoding"];
    // if there is chunked
    if (value.find("chunked") != string::npos) {
      size_t pos = value.find("chunked");
      string temp = value.substr(pos + 7);
      // if chunked is the last one
      if (temp == "") {
        return true;
      }
    }
  }
  return false;
}

size_t HttpMessage::isPositiveDigit(string str) {
  bool has_only_digits = (str.find_first_not_of("0123456789") == std::string::npos);
  if (has_only_digits) {
    return atoi(str.c_str());
  }
  return 0;
}

size_t HttpMessage::checkContentLengthValue(string str) {
  vector<string> values;
  string delim = ", ";

  size_t start = 0;
  size_t end = str.find(delim);
  while (end != std::string::npos) {
    string temp = str.substr(start, end - start);
    if (!isPositiveDigit(temp)) {
      return 0;
    }
    values.push_back(temp);
    start = end + delim.length();
    end = str.find(delim, start);
  }  // end while
  values.push_back(str.substr(start));

  if (values.size() < 2) {
    return 0;
  }

  // check if all values are same
  for (size_t i = 1; i < values.size(); i++) {
    if (values[i] != values[i - 1]) {
      return 0;
    }
  }

  return atoi(values[0].c_str());
}

size_t HttpMessage::isContentLengthValid() {
  if (header_kv_.find("Content-Length") != header_kv_.end()) {
    string value = header_kv_["Content-Length"];
    // if only one value
    int res = isPositiveDigit(value);
    if (res) {
      return res;
    }
    // more than one
    else {
      return checkContentLengthValue(value);
    }
  }
  return 0;
}

/**
 * Return length of (start line + header fields) 
 */
int HttpMessage::getHeaderLength() {
  return raw_msg_.find("\r\n\r\n") + 4;
}
