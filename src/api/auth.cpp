#import "auth.h"

#include <QDebug>
//#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client features
using namespace concurrency::streams; // Asynchronous streams
using namespace web::json;            // JSON library

Auth::Auth(const std::string& url)
{
  this->url = url;
}

Auth::~Auth() {}

std::string Auth::signIn(const std::string& email, const std::string& password)
{
  const std::string url = this->url;
  auto postJson =
      pplx::create_task([url, email, password]() {
        json::value jsonObject;
        jsonObject[U("email")] = json::value::string(U(email));
        jsonObject[U("password")] = json::value::string(U(password));

        return http_client(U(url)).request(
            methods::POST, uri_builder(U("login")).to_string(),
            jsonObject.serialize(), U("application/json"));
      })
          .then([](http_response response) {
            // Check the status code.
            if(response.status_code() != 200) {
              throw std::invalid_argument(
                  "Returned " + std::to_string(response.status_code()));
            }
            // Convert the response body to JSON object.
            return response.extract_json();
          })
          // Parse the user details.
          .then([](json::value jsonObject) { return jsonObject.as_string(); });

  try {
    postJson.wait();
    const std::string token = postJson.get();
    return token;
  }
  catch(const std::exception& e) {
    qDebug() << "Failed to sign in" << e.what();
  }
  return "";
}

bool Auth::verifyToken(const std::string& token)
{
  const std::string url = this->url;
  auto postJson =
      pplx::create_task([url, token]() {
        http_client client(U(url + "/profile"));
        http_request request(methods::POST);
        request.headers().add(U("Content-Type"), U("application/json"));
        request.headers().add(U("Authorization"), U("Bearer " + token));
        return client.request(request);
      }).then([](http_response response) {
        // Check the status code.
        if(response.status_code() != 200) {
          return false;
        }
        // Convert the response body to JSON object.
        return true;
      });

  postJson.wait();
  return postJson.get();
}

bool Auth::signOut(const std::string& token)
{
  qDebug() << "Signing out";
  const std::string url = this->url;
  auto postJson =
      pplx::create_task([url, token]() {
        http_client client(U(url + "/logout"));
        http_request request(methods::POST);
        request.headers().add(U("Content-Type"), U("application/json"));
        request.headers().add(U("Authorization"), U("Bearer " + token));
        return client.request(request);
      }).then([](http_response response) {
        // Check the status code.
        if(response.status_code() != 200) {
          return false;
        }
        // Convert the response body to JSON object.
        return true;
      });

  postJson.wait();
  return postJson.get();
}
