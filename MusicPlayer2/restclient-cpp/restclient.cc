/**
 * @file restclient.cpp
 * @brief implementation of the restclient class
 *
 * This just provides static wrappers around the Connection class REST
 * methods. However since I didn't want to have to pull in a whole URI parsing
 * library just now, the Connection constructor is passed an empty string and
 * the full URL is passed to the REST methods. All those methods to is
 * concatenate them anyways. So this should do for now.
 *
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */

#include "restclient.h"

#include <curl/curl.h>

#include "version.h"
#include "connection.h"

/**
 * @brief global init function. Call this before you start any threads.
 */
int RestClient::init() {
  CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
  if (res == CURLE_OK) {
    return 0;
  } else {
    return 1;
  }
}

/**
 * @brief global disable function. Call this before you terminate your
 * program.
 */
void RestClient::disable() {
  curl_global_cleanup();
}

/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::get(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  conn->AppendHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36");
  conn->AppendHeader("Accept", "text/html, application/xml;q=0.9, application/xhtml+xml, */*");
  conn->AppendHeader("Accept-Charset","GB2312,utf-8;q=0.7,*;q=0.7");
  conn->AppendHeader("Accept-Language","zh-cn,zh;q=0.5");
  conn->AppendHeader("Content-Type", "application/json;charset=UTF-8");
  conn->AppendHeader("Connection","Keep-Alive");
  ret = conn->get(url);
  delete conn;
  return ret;
}

/**
 * @brief HTTP POST method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP POST body
 *
 * @return response struct
 */
RestClient::Response RestClient::post(const std::string& url,
                                      const std::string& ctype,
                                      const std::string& data) {
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  conn->AppendHeader("Content-Type", ctype);
  ret = conn->post(url, data);
  delete conn;
  return ret;
}

/**
 * @brief HTTP PUT method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP PUT body
 *
 * @return response struct
 */
RestClient::Response RestClient::put(const std::string& url,
                                     const std::string& ctype,
                                     const std::string& data) {
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  conn->AppendHeader("Content-Type", ctype);
  ret = conn->put(url, data);
  delete conn;
  return ret;
}

/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::del(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  ret = conn->del(url);
  delete conn;
  return ret;
}

/**
 * @brief HTTP HEAD method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::head(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  ret = conn->head(url);
  delete conn;
  return ret;
}
