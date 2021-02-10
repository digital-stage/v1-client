#import "auth.h"
#import <QDebug>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;                  // JSON library

Auth::Auth(const std::string& url) {
    this->url = url;
}

Auth::~Auth() {

}

std::string Auth::signIn(const std::string& email, const std::string& password) {
    qDebug() << "Try siging in";

    auto postJson = pplx::create_task([this]() {
        json::value jsonObject;
        jsonObject[U("email")] = json::value::string(U("atakan"));
        jsonObject[U("password")] = json::value::string(U("sarioglu"));

        return http_client(U(url))
            .request(methods::POST,
                uri_builder(U("api")).append_path(U("users")).to_string(),
                jsonObject.serialize(), U("application/json"));
    });


    if( email == "bla" && password == "bla" ) {
        qDebug() << "Sucessfully signed in";
        return "mytoken";
    }
    qDebug() << "Failed to sign in";
    return "";
}

bool Auth::verifyToken(const std::string& token) {
    if( token == "mytoken" ) {
        qDebug() << "Token is valid";
        return true;
    }
    qDebug() << "Invalid token";
    return false;
}

bool Auth::signOut() {
    qDebug() << "Signing out";
    return true;
}

